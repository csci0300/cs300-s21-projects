#include <fcntl.h>
#include <google/protobuf/empty.pb.h>
#include <sstream>
#include <cstdio>

#include "../shardkv/shardkv.h"
#include "../shardmaster/shardmaster.h"
#include "../simple_shardkv/simpleshardkv.h"
#include "test_utils.h"

using Empty = google::protobuf::Empty;

std::vector<std::string> split(const std::string& s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

void start_simple_shardkvs(const Addrs& addrs) {
  for (const std::string& addr : addrs) {
    start_simple_shardkv(addr);
  }
}

void start_simple_shardkv(const std::string& addr) {
  spawn_service_in_thread<SimpleShardkvServer>(addr);
}

void start_shardkvs(const Addrs& addrs, const std::string& shardmaster_addr) {
  for (const std::string& addr : addrs) {
    start_shardkv(addr, shardmaster_addr);
  }
}

void start_shardkv(const std::string& addr,
                   const std::string& shardmaster_addr) {
  // pass addr param twice because {addr, shardmaster_addr} are the args to the
  // ShardkvServer constructor
  spawn_service_in_thread<ShardkvServer, const std::string&,
                          const std::string&>(addr, addr, shardmaster_addr);
}

std::vector<pid_t> start_shardkvs_proc(const Addrs& addrs,
                                       const std::string& shardmaster_addr) {
  std::vector<pid_t> pids;
  for (const std::string& addr : addrs) {
    pids.push_back(start_shardkv_proc(addr, shardmaster_addr));
  }
  return pids;
}

pid_t start_shardkv_proc(const std::string& addr,
                         const std::string& shardmaster_addr) {
  pid_t pid = fork();
  assert(pid != -1);
  if (!pid) {
    // redirect stdout and stderr
    //        int fd = open("/dev/null", O_WRONLY | O_CREAT, 0666);
    //        assert(fd >= 0);
    //        dup2(fd, 1);
    //        dup2(fd, 2);

    std::vector<char*> args;
    auto tokens = split(addr, ':');
    auto sm_tokens = split(shardmaster_addr, ':');

    args.push_back(const_cast<char*>("./shardkv"));
    args.push_back(const_cast<char*>(tokens[1].c_str()));
    args.push_back(const_cast<char*>(sm_tokens[0].c_str()));
    args.push_back(const_cast<char*>(sm_tokens[1].c_str()));
    args.push_back(0);
    execv("./shardkv", args.data());
  }
  return pid;
}

void start_shardmaster(const std::string& addr) {
  spawn_service_in_thread<StaticShardmaster>(addr);
}

bool test_get_impl(const std::string& addr, std::string key,
                   const std::optional<std::string>& value) {
  auto channel = grpc::CreateChannel(addr, grpc::InsecureChannelCredentials());
  auto stub = Shardkv::NewStub(channel);

  ::grpc::ClientContext cc;
  GetRequest req;
  GetResponse res;
  req.set_key(key);

  auto status = stub->Get(&cc, req, &res);
  // if we pass a non-nullopt optional, we expect success - otherwise we expect
  // an error
  if (value.has_value()) {
    return value.value() == res.data();
  }
  return !status.ok();
}

bool test_get(const std::string& addr, std::string key,
              const std::optional<std::string>& value) {
  int i = 0;
  // loop RETRIES times _unless_ we don't expect a success, in which case we
  // just try once
  auto timeout = std::chrono::milliseconds(1000);
  do {
    bool res = test_get_impl(addr, key, value);
    if (res) {
      // if we succeeded return true (passed)
      return true;
    } else if (value == std::nullopt) {
      // res is false, so we failed
      // if we "failed" while expecting a failure (an unexpected success) save
      // retries and report the failure
      return false;
    }
    std::this_thread::sleep_for(timeout);
    ++i;
  } while (i < RETRIES);
  // didn't succeed in the end
  return false;
}

bool test_put(const std::string& addr, std::string key,
              const std::string& value, std::string user, bool success) {
  auto channel = grpc::CreateChannel(addr, grpc::InsecureChannelCredentials());
  auto stub = Shardkv::NewStub(channel);

  ::grpc::ClientContext cc;
  PutRequest req;
  Empty res;
  req.set_key(key);
  req.set_data(value);
  req.set_user(user);

  auto status = stub->Put(&cc, req, &res);
  return status.ok() == success;
}

bool test_append(const std::string& addr, std::string key,
                 const std::string& value, bool success) {
  auto channel = grpc::CreateChannel(addr, grpc::InsecureChannelCredentials());
  auto stub = Shardkv::NewStub(channel);

  ::grpc::ClientContext cc;
  AppendRequest req;
  Empty res;
  req.set_key(key);
  req.set_data(value);

  auto status = stub->Append(&cc, req, &res);
  return status.ok() == success;
}

bool test_delete(const std::string& addr, std::string key,
                 bool success) {
  auto channel = grpc::CreateChannel(addr, grpc::InsecureChannelCredentials());
  auto stub = Shardkv::NewStub(channel);

  ::grpc::ClientContext cc;
  DeleteRequest req;
  Empty res;
  req.set_key(key);

  auto status = stub->Delete(&cc, req, &res);
  return status.ok() == success;
}

// testing functions for shardmaster - for join/leave/move we will have to call
// query anyway so maybe bundle them?
bool test_join(const std::string& shardmaster_addr, const std::string& addr,
               bool success) {
  auto channel =
      grpc::CreateChannel(shardmaster_addr, grpc::InsecureChannelCredentials());
  auto stub = Shardmaster::NewStub(channel);

  ::grpc::ClientContext cc;
  JoinRequest req;
  Empty response;

  req.set_server(addr);

  auto status = stub->Join(&cc, req, &response);
  return status.ok() == success;
}

bool test_leave(const std::string& shardmaster_addr, const Addrs& addrs,
                bool success) {
  auto channel =
      grpc::CreateChannel(shardmaster_addr, grpc::InsecureChannelCredentials());
  auto stub = Shardmaster::NewStub(channel);

  ::grpc::ClientContext cc;
  LeaveRequest req;
  Empty response;
  for (const auto& addr : addrs) {
    req.add_servers(addr);
  }

  auto status = stub->Leave(&cc, req, &response);
  return status.ok() == success;
}

bool test_move(const std::string& shardmaster_addr, const std::string& addr,
               const shard_t& shard, bool success) {
  auto channel =
      grpc::CreateChannel(shardmaster_addr, grpc::InsecureChannelCredentials());
  auto stub = Shardmaster::NewStub(channel);

  ::grpc::ClientContext cc;
  MoveRequest req;
  Empty response;

  req.set_server(addr);
  req.mutable_shard()->set_lower(shard.lower);
  req.mutable_shard()->set_upper(shard.upper);

  auto status = stub->Move(&cc, req, &response);
  return status.ok() == success;
}

bool test_query(const std::string& shardmaster_addr,
                const std::map<std::string, std::vector<shard_t>>& m) {
  auto channel =
      grpc::CreateChannel(shardmaster_addr, grpc::InsecureChannelCredentials());
  auto stub = Shardmaster::NewStub(channel);

  ::grpc::ClientContext cc;
  Empty req;
  QueryResponse response;

  auto status = stub->Query(&cc, req, &response);
  if (!status.ok()) {
    return false;
  }

  // read response into a map then check if it's equal to what we expect (m)
  std::map<std::string, std::vector<shard_t>> res_map;

  for (const auto& e : response.config()) {
    std::vector<shard_t> shards;
    for (const auto& s : e.shards()) {
      shards.push_back({s.lower(), s.upper()});
    }
    // we only want to store non-empty vectors here to simplify the maps passed
    // in (don't have to explicitly set empty vectors for servers with no
    // shards).
    if (!shards.empty()) {
      res_map[e.server()] = shards;
    }
  }

  if (res_map.size() != m.size()) {
    return false;
  }
  // now that we know sizes are equal, we can try each key
  for (const auto& [server, shards] : res_map) {
    auto it = m.find(server);
    if (it == m.end()) {
      // missing a server
      return false;
    }

    const std::vector<shard_t>& m_shards = it->second;
    if (shards.size() != m_shards.size()) {
      // missing/extra shards
      return false;
    }

    for (const shard_t& s : shards) {
      if (std::find(m_shards.begin(), m_shards.end(), s) == m_shards.end()) {
        // missing shards
        return false;
      }
    }
  }
  return true;
}

bool test_gdpr_delete(const std::string& shardmaster_addr, std::string user, bool success){
    auto channel =
      grpc::CreateChannel(shardmaster_addr, grpc::InsecureChannelCredentials());
    auto stub = Shardmaster::NewStub(channel);

    ::grpc::ClientContext cc;
    GDPRDeleteRequest req;
    Empty response;

    req.set_key(user);

    auto status = stub->GDPRDelete(&cc, req, &response);
    return status.ok() == success;
}

/**
 * sends signal to all specified pids and then waitpids until they're all gone
 * @param pids list of pids to kill - must be children of current process
 */
void cleanup_children(const std::vector<pid_t>& pids) {
  fprintf(stdout, "cleaning up children\n");
  for (const pid_t& pid : pids) {
    kill(pid, SIGKILL);
  }

  for (const pid_t& pid : pids) {
    waitpid(pid, nullptr, 0);
  }
}

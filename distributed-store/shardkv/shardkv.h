#ifndef SHARDING_SHARDKV_H
#define SHARDING_SHARDKV_H

#include <grpcpp/grpcpp.h>
#include <thread>
#include "../common/common.h"

#include "../build/shardkv.grpc.pb.h"
#include "../build/shardmaster.grpc.pb.h"

class ShardkvServer : public Shardkv::Service {
  using Empty = google::protobuf::Empty;

 public:
  explicit ShardkvServer(std::string addr, const std::string& shardmaster_addr)
      : address(std::move(addr)) {
    // This thread will query the shardmaster every 100 milliseconds for updates
    std::thread query(
        [this](const std::string sm_addr) {
          std::chrono::milliseconds timespan(100);
          auto stub = Shardmaster::NewStub(
              grpc::CreateChannel(sm_addr, grpc::InsecureChannelCredentials()));
          while (true) {
            this->QueryShardmaster(stub.get());
            std::this_thread::sleep_for(timespan);
          }
        },
        shardmaster_addr);
    // we detach the thread so we don't have to wait for it to terminate later
    query.detach();
  };

  // TODO implement these three methods, should be fairly similar to your
  // simple_shardkv
  ::grpc::Status Get(::grpc::ServerContext* context,
                     const ::GetRequest* request,
                     ::GetResponse* response) override;
  ::grpc::Status Put(::grpc::ServerContext* context,
                     const ::PutRequest* request, Empty* response) override;
  ::grpc::Status Append(::grpc::ServerContext* context,
                        const ::AppendRequest* request,
                        Empty* response) override;
  ::grpc::Status Delete(::grpc::ServerContext* context,
                        const ::DeleteRequest* request,
                        Empty* response) override;

  // TODO this will be called in a separate thread, here is where you want to
  // query the shardmaster for configuration updates and respond to changes
  // appropriately (i.e. transferring keys, no longer serving keys, etc.)
  void QueryShardmaster(Shardmaster::Stub* stub);

 private:
  // address we're running on (hostname:port)
  const std::string address;
  // TODO add any fields you want here!
};

#endif  // SHARDING_SHARDKV_H

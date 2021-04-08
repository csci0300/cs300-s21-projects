#include <unistd.h>
#include <cstdio>
#include <thread>

#include "dynamic_shardmaster.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: ./dynamic_shardmaster <port>\n");
    return 1;
  }
  // shardmaster service
  ShardmasterServer shardmaster;
  // construct address
  char hostnamebuf[256];
  gethostname(hostnamebuf, 256);
  // construct addresses
  std::string hostname(hostnamebuf);
  std::string addr = hostname + ":" + std::string(argv[1]);
  ::grpc::ServerBuilder builder;
  builder.AddListeningPort(addr, ::grpc::InsecureServerCredentials());
  builder.RegisterService(&shardmaster);
  std::unique_ptr<::grpc::Server> server = builder.BuildAndStart();
  fprintf(stdout, "Listening on: %s\n", addr.c_str());

  // now start up rebalancing thread
  std::thread rebalance([&shardmaster]() {
    // rebalance every second
    std::chrono::seconds timespan(1);
    while (true) {
      shardmaster.Rebalance();
      std::this_thread::sleep_for(timespan);
    }
  });
  rebalance.detach();
  server->Wait();
}

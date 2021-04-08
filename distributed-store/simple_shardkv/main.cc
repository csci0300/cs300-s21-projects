#include <grpcpp/grpcpp.h>
#include <unistd.h>
#include <thread>
#include <cstdio>

#include "simpleshardkv.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: ./simple_shardkv <port>\n");
    return 1;
  }
  char hostnamebuf[256];
  gethostname(hostnamebuf, 256);
  // construct addresses
  std::string hostname(hostnamebuf);
  std::string port(argv[1]);
  std::string addr = hostname + ":" + port;
  ::grpc::ServerBuilder builder;
  builder.AddListeningPort(addr, ::grpc::InsecureServerCredentials());
  SimpleShardkvServer shardkv;
  builder.RegisterService(&shardkv);
  std::unique_ptr<::grpc::Server> server = builder.BuildAndStart();
  fprintf(stdout, "Listening on: %s\n", addr.c_str());
  server->Wait();
  return 0;
}

#include <unistd.h>
#include <cstdio>
#include "shardmaster.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: ./shardmaster <PORT>\n");
    return 1;
  }
  // shardmaster service
  StaticShardmaster shardmaster;
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
  server->Wait();
  return 0;
}

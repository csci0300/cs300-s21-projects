#include <grpcpp/grpcpp.h>
#include <unistd.h>
#include <cstdio>

#include "shardkv.h"

int main(int argc, char** argv) {
  if (argc != 4) {
    fprintf(stderr, "usage: ./shardkv <PORT> <SHARDMASTER HOSTNAME> " \
                    "<SHARDMASTER PORT>\n");
    return 1;
  }
  // get our hostname so we can construct address for shardkv. we need this
  // because the shardmaster will know us by our hostname and port, so we should
  // track that.
  char hostnamebuf[256];
  gethostname(hostnamebuf, 256);
  // construct addresses
  std::string hostname(hostnamebuf);
  std::string port(argv[1]);
  std::string addr = hostname + ":" + port;

  fprintf(stdout, "Listening on: %s\n", addr.c_str());
  std::string shardmaster_addr =
      std::string(argv[2]) + ":" + std::string(argv[3]);
  fprintf(stdout, "Shardmaster on: %s\n", shardmaster_addr.c_str());

  ::grpc::ServerBuilder builder;
  builder.AddListeningPort(addr, ::grpc::InsecureServerCredentials());
  ShardkvServer shardkv(addr, shardmaster_addr);
  builder.RegisterService(&shardkv);
  std::unique_ptr<::grpc::Server> server = builder.BuildAndStart();

  server->Wait();
  return 0;
}

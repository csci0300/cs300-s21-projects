#include "network_helpers.hh"
#include "vunmo-server.hh"

#define N_WORKERS 5

int main(int argc, char** argv) {
  int port;
  int num_workers = N_WORKERS;

  if (argc < 2) {
    fprintf(stderr, "Usage: ./server <PORT>\n");
    exit(1);
  }

  if (argc > 2) {
    num_workers = strtol(argv[2], nullptr, 0);
  }

  // parse command line arguments
  port = strtol(argv[1], nullptr, 0);

  assert(port > 0 && port <= 65535);

  // create and start server
  printf("Starting server on port %d with %d workers...\n", port, num_workers);
  Server s;
  s.start(port, num_workers);
  char* command = nullptr;
  size_t buf_size;
  getline(&command, &buf_size, stdin);
  s.stop();
}

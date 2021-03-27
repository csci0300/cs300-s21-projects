#include "network_helpers.hh"
#include "vunmo-client.hh"

int parse_command(char* command, request_t* req) {
  memset(req, 0, sizeof(request_t));

  uint64_t amount;
  uint64_t target_client;

  // pattern match command string
  if ((strcmp(command, "") == 0)) {
    return 1;
  } else if (strcmp(command, "balance") == 0) {
    req->type = BALANCE;
  } else if (sscanf(command, "deposit %" SCNu64, &amount) == 1) {
    req->type = DEPOSIT;
    req->amount = amount;
  } else if (sscanf(command, "withdraw %" SCNu64, &amount) == 1) {
    req->type = WITHDRAWAL;
    req->amount = amount;
  } else if (sscanf(command, "pay %" SCNu64 "%" SCNu64, &target_client,
                    &amount) == 2) {
    req->type = PAYMENT;
    req->amount = amount;
    req->target_client_id = target_client;
  } else if (sscanf(command, "charge %" SCNu64 "%" SCNu64, &target_client,
                    &amount) == 2) {
    req->type = CHARGE;
    req->amount = amount;
    req->target_client_id = target_client;
  } else {
    return -1;
  }
  return 0;
}

int handle_command(char* command, Client* c) {
  request_t req;
  response_t resp;
  // parse command
  int err = parse_command(command, &req);
  if (err < 0) {
    fprintf(stderr, "[ERROR] Invalid command: \"%s\"\n", command);
    return -2;
  } else if (err == 1)
    return 0;  // blank line
  // make request
  err = c->make_request(&req, &resp);
  if (err < 0) return err;

  // print response
  printf("%s", resp.message);
  return 0;
}

void run_repl(Client* c) {
  char* command = nullptr;
  size_t buf_size;
  // loop until break
  while (true) {
    // print prompt
    printf("> ");
    fflush(stdout);

    // read command
    ssize_t bytes_read = getline(&command, &buf_size, stdin);
    if (bytes_read <= 0) return;
    command[bytes_read - 1] = '\0';

    // handle command
    int err = handle_command(command, c);
    if (err == -1) break;

    // handle notifications
    err = c->display_notifications();
    if (err == -1) break;
  }
  free(command);
}

int main(int argc, char** argv) {
  srand(time(NULL));
  char* host;
  char* port;

  if (argc != 3 && argc != 4) {
    fprintf(stderr, "Usage: ./client <HOST> <PORT> [client ID]\n");
    exit(1);
  }

  // Parse arguments.
  host = argv[1];
  port = argv[2];

  // instantiate client
  Client c;
  if (argc == 3) {
    // if the user didn't supply an ID, randomly generate one
    c.id = rand() % MAX_CLIENT_ID;

  } else if (argc == 4) {
    // otherwise, convert the ID to an int and store it
    c.id = strtol(argv[3], nullptr, 0);
  }

  printf("Started client [ %ld ].\n", c.id);

  // connect to server
  printf("Connecting to server at %s:%s...\n", host, port);
  int err = c.connect(host, port);
  if (err != 0) {
    fprintf(stderr, "Failed to connect to server.\n");
    exit(1);
  };
  printf("Connected to server.\n");

  // start repl
  run_repl(&c);

  // close connections
  printf("Disconnecting client...");
  c.disconnect();

  return 0;
}

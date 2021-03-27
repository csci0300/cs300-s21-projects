#include "network_helpers.hh"

int create_connection(const char* host, const char* port) {
  struct addrinfo hints, *ai;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;      // use IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;  // use TCP
  hints.ai_flags = AI_NUMERICSERV;
  int r = getaddrinfo(host, port, &hints, &ai);
  if (r != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
    exit(1);
  }

  int fd = socket(ai->ai_family, ai->ai_socktype, 0);
  if (fd < 0) {
    perror("socket");
    freeaddrinfo(ai);
    return fd;
  }
  r = connect(fd, ai->ai_addr, ai->ai_addrlen);
  if (r < 0) {
    perror("connect");
    close(fd);
    freeaddrinfo(ai);
    return -1;
  }
  freeaddrinfo(ai);
  return fd;
}

int open_listen_socket(int port) {
  // Create socket
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    perror("socket");
    return -1;
  }

  // Useful options: close-on-exec, reuse-address
  int r = fcntl(fd, F_SETFD, FD_CLOEXEC);
  if (r < 0) {
    perror("fcntl");
    close(fd);
    return -1;
  }

  int yes = 1;
  r = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  if (r < 0) {
    perror("setsockopt");
    close(fd);
    return -1;
  }

  // Bind to port
  struct sockaddr_in address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  address.sin_addr.s_addr = INADDR_ANY;
  r = bind(fd, (struct sockaddr*)&address, sizeof(address));
  if (r < 0) {
    perror("bind");
    close(fd);
    return -1;
  }

  // Actually start listening
  r = listen(fd, 100);
  if (r < 0) {
    perror("listen");
    close(fd);
    return -1;
  }

  return fd;
}
#pragma once

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sched.h>
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cerrno>
#include <condition_variable>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <list>
#include <mutex>
#include <string>
#include <thread>

/**
 * Create a connection to a listener on the specified host and port.
 *
 * @param host: Host that listener is running on
 * @param port: Port that listener is running on
 * @return file descriptor of the new connection on success; -1 on failure
 */
int create_connection(const char* host, const char* port);

/**
 * Open a socket for listening on `port`. The socket will accept
 * connections from any host, and has a listen queue of 100 connections.
 * @param port: Port on which the listening socket will run
 * @return file descriptor of the listening socket on success; -1 on failure
 */
int open_listen_socket(int port);
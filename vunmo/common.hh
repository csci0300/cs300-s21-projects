#pragma once

#include <cstdint>

#define RESP_STR_LEN 100
#define MAX_CLIENT_ID 10000

// Enums for types of request
enum request_type { BALANCE, DEPOSIT, WITHDRAWAL, PAYMENT, CHARGE };

// Struct representing request from client
typedef struct request {
  request_type type;
  uint64_t origin_client_id;
  uint64_t target_client_id;
  uint64_t amount;
} request_t;

// Enums for types of notifications
enum notification_type { PAYMENT_NOTIFICATION, CHARGE_REQUEST_NOTIFICATION };

// Struct representing notification to client
typedef struct notification {
  notification_type type;
  uint64_t origin_client_id;
  uint64_t amount;
  char message[RESP_STR_LEN];
} notification_t;

// Struct representing response from server to client.
typedef struct response {
  uint64_t amount;
  int success;
  char message[RESP_STR_LEN];
} response_t;

// Enums for types of connections client can make to server
enum hello_type { REQUESTS_HELLO, NOTIFICATIONS_HELLO };

// Struct representing initial message client sends to server
typedef struct client_hello {
  uint64_t client_id;
  hello_type type;
} client_hello_t;

#pragma once

#include "common.hh"
#include "network_helpers.hh"
#include "synchronized_queue.cc"

#include <poll.h>
#include <atomic>
#include <unordered_map>
#include <vector>

// Error code for when client issuing request is not found.
#define ECLINOTFOUND 1
// Error code for when request's target client is not found.
#define ETARNOTFOUND 2
// Error code for when client is trying to perform a request on themselves
// (i.e., origin and target clients are the same).
#define ESELFACTION 3

// Struct containing information about client's connection.
typedef struct client_conn {
  uint64_t id;
  int request_fd;
  int notification_fd;
  bool is_connected;
  std::mutex mtx;
} client_conn_t;

// Struct representing client's account.
typedef struct account {
  uint64_t id;
  uint64_t balance;
  std::mutex mtx;
} account_t;

class Server {
 public:
  ~Server() {
    if (!this->is_stopped.exchange(true)) {
      this->stop();
    }
  }

  /* ================== */
  /* === Server API === */
  /* ================== */

  /**
   * Start server. This involves starting worker pool, client listener, and
   * request listener threads.
   *
   * @param port: Port to start the server on
   * @param n_workers: Number of worker threads to spawn
   * @return -1 on failure, 0 on success
   */
  int start(int port, int n_workers);

  /**
   * Stops the server. Cleans up any threads and resources the server allocated,
   * and returns the final balances of all clients.
   *
   * @return a map of client IDs and their final balances
   */
  std::unordered_map<uint64_t, uint64_t> stop();

 private:
  // Thread-safe work queue.
  synchronized_queue<request_t*> work_queue;

  // Maps unique ID to a pointer to the corresponding client_conns_t struct.
  std::unordered_map<uint64_t, client_conn_t*> client_conns;
  // Mutex to guard the above map.
  std::mutex client_conns_mtx;

  // Maps unique ID to a pointer to the corresponding account_t struct.
  std::unordered_map<uint64_t, account_t*> accounts;
  // Mutex to guard the above map.
  std::mutex accounts_mtx;

  // File descriptor to accept client connections.
  int listener_fd;
  // Map to keep track of clients who are in the middle of connecting.
  std::unordered_map<uint64_t, client_conn_t*> incomplete_conns;

  // Number of worker threads the server is running.
  int num_workers;
  // Vector of worker threads.
  std::vector<std::thread> workers;
  // Thread that listens for client connections and accepts them.
  std::thread client_listener;
  // Thread that receives requests from clients and puts them on the work queue.
  std::thread request_listener;
  // An atomic. thread-safe boolean for whether the server has been stopped or
  // not.
  std::atomic_bool is_stopped;

  /* =================== */
  /* === Concurrency === */
  /* =================== */

  /**
   * Find and return locked client. If fail, client is unlocked.
   *
   * @param id: Client's id.
   * @param client: Pointer we will set to client's account pointer when found.
   * @return 0, or -ECLINOTFOUND if client not found.
   */
  int get_account(uint64_t id, account_t** client);

  /**
   * Find and return locked clients. If fail, both clients are unlocked.
   *
   * @param first_id: Origin client's id.
   * @param first_account: Pointer we will set to origin client's account
   * pointer when found.
   * @param second_id: Target client's id.
   * @param second_account: Pointer we will set to target client's account
   * pointer when found.
   * @return 0 on success, appropriate error if failed (-ECLINOTFOUND if origin
   * client not found, -ETARNOTFOUND if target client not found, or -ESELFACTION
   * if origin and target clients are the same).
   */
  int get_two_accounts(uint64_t first_id, account_t** first_account,
                       uint64_t second_id, account_t** second_account);

  /* ========================== */
  /* === Work and Requests ===  */
  /* ========================== */

  /**
   * Process request. This will involve finding clients,
   * updating balances, and sending responses and notifications.
   *
   * @params req: Request to be handled.
   * @return 0 on success, appropriate error if failed (-ECLINOTFOUND if origin
   * client not found, -ETARNOTFOUND if target client not found, or -ESELFACTION
   * if origin and target clients are the same).
   */
  int process_request(request_t* req);

  /**
   * Sends a response to a client after processing a request from them.
   * @param client_id: ID of origin client.
   * @param resp: Pointer to a response_t struct to send to the client.
   * @return 0 on success, -ECLINOTFOUND if client not found.
   */
  int send_response(uint64_t client_id, response_t* resp);

  /**
   * Sends a notification to a target client.
   * @param client_id: ID of target client.
   * @param n: Pointer to a notification_t struct to send to the target client.
   * @return 0 on success, -ECLINOTFOUND if client not found.
   */
  int send_notification(uint64_t client_id, notification_t* n);

  /**
   * A helper function that specifically handles for error cases in which
   * client(s) are missing for the given request. Writes appropriate response to
   * the original client, if it's found.
   * @param err: Error code to handle
   * @param req: Request that couldn't be processed because client(s) are
   * missing
   */
  void handle_missing_clients(int err, request_t* req);

  /**
   * Return vector of requests just received from the network.
   * This call will hang until at least one request is received.
   */
  std::vector<request_t*> fetch_requests();

  /**
   * In a loop, call fetch_request to get requests from the network
   * and add them to the work queue. Exits when the server has been stopped.
   */
  void receive_requests_loop();

  /**
   * In a loop, pop a request from the work queue and process it.
   * Exits when the server has been stopped.
   */
  void work_loop();

  /* ========================= */
  /* === Accepting Clients === */
  /* ========================= */

  /**
   * In a loop, call accept_client and add that client to the
   * client map. Exits when the server has been stopped.
   */
  void accept_clients_loop();

  /**
   * Accepts client connections. This call will hang until a client properly
   * connects.
   *
   * @return a client_conn struct for a newly connected client
   */
  client_conn_t* accept_client();

  /**
   * Helper function that handles a client that is reconnecting. Merges the new
   * client info into existing client_conn_t that we already have in the
   * `client_conns` map.
   *
   * @param new_client: New client_conn_t for the client that is reconnecting.
   * @param existing_client: Existing client_conn_t that we already have in our
   * `client_conns` map for the same client.
   */
  void handle_reconnecting_client(client_conn_t* new_client,
                                  client_conn_t* existing_client);
};

/**
 * Prints the request as a string.
 */
void print_request(request_t* req);

/* ======================== */
/* === Request Handlers === */
/* ======================== */

/**
 * Query client's current BALANCE.
 *
 * @param req: Request to handle
 * @param account: Account of the client issuing the request
 * @param resp: Response struct to be filled with response
 * @return 0 on success (note that this function will never fail, so will not
 * return -1 like other handlers).
 */
int handle_balance(request_t* req, account_t* account, response_t* resp);

/**
 * Increment client's BALANCE by given amount.
 *
 * @param req: Request to handle
 * @param account: Account of the client issuing the request
 * @param resp: Response struct to be filled with response
 * @return 0 on success (note that this function will never fail, so will not
 * return -1 like other handlers).
 */
int handle_deposit(request_t* req, account_t* account, response_t* resp);

/**
 * Decrement client's BALANCE by given amount. If BALANCE is insufficient,
 * return error.
 *
 * @param req: Request to handle.
 * @param account: Account of the client issuing request
 * @param resp: Response struct to be filled with response
 * @return 0 on success, -1 on fail
 */
int handle_withdraw(request_t* req, account_t* account, response_t* resp);

/**
 * Transfer given amount from originating client to target client. If origin
 * client's BALANCE is insufficient, return error. Otherwise, create
 * notification for target client.
 *
 * @param req: Request to handle
 * @param first_account: Account of the client issuing request
 * @param second_account: Account of the target client
 * @param resp: Response struct to be filled with response
 * @param notif: Notification struct to be filled with notification
 * @return 0 on success, -1 on fail
 */
int handle_pay(request_t* req, account_t* first_account,
               account_t* second_account, response_t* resp,
               notification_t* notif);

/**
 * Generate charge request from origin client to target client.
 *
 * @param req: Request to handle
 * @param first_account: Account of the client issuing request
 * @param second_account: Account of the target client
 * @param resp: Response struct to be filled with response
 * @param notif: Notification struct to be filled with notification
 * @return 0 on success (note that this function will never fail, so will not
 * return -1 like other handlers).
 */
int handle_charge(request_t* req, account_t* first_account,
                  account_t* second_account, response_t* resp,
                  notification_t* notif);

/**
 * Handles an unknown request by filling response struct with generic error
 * message.
 *
 * @param req: Request to handle
 * @param resp: Response struct to be filled with response
 */
int handle_unknown_request(request_t* req, response_t* resp);

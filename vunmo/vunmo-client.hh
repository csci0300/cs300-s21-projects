#pragma once

#include "network_helpers.hh"
#include "common.hh"
#include "synchronized_queue.cc"
#include <atomic>

#include <cinttypes>

class Client {
public:
    // Client ID
    uint64_t id;

    /**
     * Connects to server and starts listening for notifications.
     *
     * @param host: Server's IP
     * @param port: Server's port
     * @return 0 on success, or -1 on failure.
     */
    int connect(const char* host, const char* port);

    /**
     * Disconnects from server and shuts down notification queue.
     *
     * @return 0 on success, or -1 on failure.
     */
    int disconnect();

    /**
     * Requests and prints client balance.
     *
     * @param resp: Response struct, which will contain server's response
     * @return 0 on success, or -1 on failure
     */
    int get_balance(response_t *resp);

    /**
     * Deposits (adds) amount to client's account.
     *
     * @param amount: Amount to deposit
     * @param resp: Response struct, which will contain server's response
     * @return 0 on success, or -1 on failure
     */
    int deposit(uint64_t amount, response_t *resp);

    /**
     * Withdraws (subtracts) amount from client's account.
     *
     * @param amount: Amount to withdraw
     * @param resp: Response struct, which will contain server's response
     * @return 0 on success, or -1 on failure
     */
    int withdraw(uint64_t amount, response_t *resp);

    /**
     * Transfers amount from client's account to target's account.
     *
     * @param target_id: ID of client we are paying
     * @param amount: Amount to pay
     * @param resp: Response struct, which will contain server's response
     * @return 0 on success, or -1 on failure
     */
    int pay(uint64_t target_id, uint64_t amount, response_t *resp);

    /**
     * Sends a charge request notification to target for amount.
     *
     * @param target_id: ID of client we are charging
     * @param amount: Amount to charge
     * @param resp: Response struct, which will contain server's response
     * @return 0 on success, or -1 on failure
     */
    int charge(uint64_t target_id, uint64_t amount, response_t *resp);

    /**
     * Sends the specified request to the server.
     *
     * @param req: Request we want to send
     * @param resp: Response struct, which will contain server's response
     * @return 0 on success, or -1 on failure
     */
    int make_request(request_t *req, response_t *resp);

    /**
     * Respond to charge notification.
     *
     * @param n: Charge notification
     * @param accept: True if we are accepting the charge, or false if we are not
     * @return 0 on success, or -1 on failure
     */
    int handle_charge_request(notification_t *n, bool accept);

    /**
     * Return all currently queued notifications (and clear queue).
     *
     * @return Vector of notifications
     */
    std::vector<notification_t> get_notifications();

    /**
     * Print all currently queued notifications.
     *
     * @return 0 on success, or -1 on failure
     */
    int display_notifications();

    /**
     * Print charge request and query user for how to handle it.
     *
     * @param n: Charge notification
     * @return 0 on success, or -1 on failure
     */
    int print_charge_request(notification_t *n);

private:
    // Request and notification file descriptors
    int request_fd, notification_fd;
    // Notification queue
    synchronized_queue<notification_t*> notification_queue;
    // Thread that listens for notifications from server
    std::thread notification_listener;
    // Atomic, thread-safe boolean for whether the client has been stopped
    std::atomic_bool is_stopped{false};
    // Atomic, thread-safe boolean for whether the client is connected to server.
    std::atomic_bool is_connected{false};

    /**
     * Continually listen for notifications from network and add them to queue.
     */
    void receive_notifications();

};

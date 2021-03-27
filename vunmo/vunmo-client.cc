#include "vunmo-client.hh"

int Client::get_balance(response_t *resp){
    request_t req;
    // prepare request
    req.type = BALANCE;
    // make request
    return this->make_request(&req, resp);
}


int Client::deposit(uint64_t amount, response_t *resp){
    request_t req;
    // prepare request
    req.type = DEPOSIT;
    req.amount = amount;
    // make request
    return this->make_request(&req, resp);
}


int Client::withdraw(uint64_t amount, response_t *resp){
    request_t req;
    // prepare request
    req.type = WITHDRAWAL;
    req.amount = amount;
    // make request
    return this->make_request(&req, resp);
}


int Client::pay(uint64_t target_id, uint64_t amount, response_t *resp){
    request_t req;
    // prepare request
    req.type = PAYMENT;
    req.amount = amount;
    req.target_client_id = target_id;
    // make request
    return this->make_request(&req, resp);
}


int Client::charge(uint64_t target_id, uint64_t amount, response_t *resp){
    request_t req;
    // prepare request
    req.type = CHARGE;
    req.amount = amount;
    req.target_client_id = target_id;
    // make request
    return this->make_request(&req, resp);
}


int Client::make_request(request_t *req, response_t *resp) {
    if (!this->is_connected.load()) {
        return -1;
    }
    // set request origin id
    req->origin_client_id = this->id;

    // send request
    int bytes_written = write(this->request_fd, req, sizeof(request_t));
    if (bytes_written < (int) sizeof(request_t)) {
        this->is_connected.store(false);
        return -1;
    }
    // wait for response
    int bytes_read = read(this->request_fd, resp, sizeof(response_t));
    if (bytes_read < (int) sizeof(response_t)) {
        this->is_connected.store(false);
        return -1;
    }
    return 0;
}


int Client::handle_charge_request(notification_t *n, bool accept) {
    request_t req;
    response_t dummy_resp; // won't be used anywhere

    // if decline, do nothing
    if (!accept) return 0;

    // prepare payment request
    req.type = PAYMENT;
    req.origin_client_id = this->id;
    req.target_client_id = n->origin_client_id;
    req.amount = n->amount;

    // make request
    return this->make_request(&req, &dummy_resp);
}


int Client::print_charge_request(notification_t *n) {
    int to_read = 3;
    char resp[to_read];
    bool charge_accepted;

    // get y/n input from user
    while (true) {
        printf(" - Do you accept this charge request? [y/n]: ");
        fflush(stdout);

        if (fgets(resp, to_read, stdin) == nullptr) {
            // EOF reached before receiving a proper response.
            return -1;
        }
        // parse response
        if(strcmp(resp, "y\n") == 0) {
            charge_accepted = true;
        } else if(strcmp(resp, "n\n") == 0) {
            charge_accepted = false;
        } else {
            continue;
        }
        break;
    }

    return this->handle_charge_request(n, charge_accepted);
}


int Client::display_notifications() {
    auto ns = this->get_notifications();
    notification_t n;

    for (size_t i = 0; i < ns.size(); i++) {
        // print
        n = ns[i];
        printf("[%lu/%lu] %s", i + 1, ns.size(), n.message);

        // handle charge request
        if (n.type == CHARGE_REQUEST_NOTIFICATION) {
            if (this->print_charge_request(&n) < 0) {
                printf("[ERROR] Unable to handle a charge request of %lu from %lu.\n",
                       n.amount, n.origin_client_id);
            }
        }
    }
    return 0;
}


std::vector<notification_t> Client::get_notifications() {
    std::vector<notification_t> ns;
    for (auto & n : this->notification_queue.flush()) {
        ns.push_back(*n);
        delete n;
    }
    return ns;
}


void Client::receive_notifications(){
    while (!this->is_stopped.load()) {
        // read notification
        notification_t n;
        int bytes_read = read(this->notification_fd, &n, sizeof(notification_t));
        if (bytes_read < (int) sizeof(notification_t)) {
            continue;
        }
        // add notification to queue
        auto new_n = new notification_t();
        *new_n = n;
        this->notification_queue.push(new_n);
    }
    close(this->notification_fd);
}

/**
 * Continually listen for notifications from network and add them to queue.
 */
int send_hello(const char* host, const char* port, uint64_t client_id, hello_type type) {
    // create connection
    int fd = create_connection(host, port);
    if (fd < 0) {
        return -1;
    }
    // send hello
    client_hello_t hello;
    hello.type = type;
    hello.client_id = client_id;
    int bytes_written = write(fd, &hello, sizeof(client_hello_t));
    if (bytes_written < (int) sizeof(client_hello_t)) {
        return -1;
    }
    return fd;
}

int Client::connect(const char* host, const char* port) {
    // create connection for requests
    this->request_fd = send_hello(host, port, this->id, REQUESTS_HELLO);
    if (this->request_fd < 0) return -1;

    // create connection for notifications
    this->notification_fd = send_hello(host, port, this->id, NOTIFICATIONS_HELLO);
    if (this->notification_fd < 0) return -1;

    this->is_connected.store(true);

    // start thread for receiving notifications
    this->notification_listener = std::thread(&Client::receive_notifications, this);

    return 0;
}

int Client::disconnect(){
    this->is_stopped.store(true);
    this->is_connected.store(false);

    // close connections
    close(this->request_fd);

    this->notification_listener.join();

    // stop notification queue
    this->notification_queue.stop();
    for (auto & n : this->notification_queue.flush()) delete n;

    return 0;

}
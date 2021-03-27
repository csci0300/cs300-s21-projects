#include "../../vunmo-server.hh"
#include "../../vunmo-client.hh"
#include "../../tests.hh"

int main() {
    int err;

    // Create and start server.
    Server s;
    err = s.start(port, N_WORKERS);
    if (err != 0) {
        fprintf(stderr, "Server failed to start.\n");
        return -1;
    }

    sleep(1); // give the server time to start

    Client c;
    c.id = CLIENT_ID;

    err = c.connect(host_str, port_str);
    if (err != 0) {
        fprintf(stderr, "Client failed to connect to server.\n");
        return -1;
    }

    response_t resp;
    err = c.get_balance(&resp);
    if (err != 0) {
        fprintf(stderr, "Client failed to make request.\n");
        return -1;
    }
    if (resp.amount != 0) {
        fprintf(stderr, "Initial balance was not 0.\n");
        return -1;
    }

    c.disconnect();
    s.stop();
}

#include "../../vunmo-server.hh"
#include "../../vunmo-client.hh"
#include "../../tests.hh"

/**
 * Deposit Request
 *
 * Tests that a client can deposit properly.
 */
int main() {
    // Create and start server.
    Server s;
    s.start(port, N_WORKERS);

    sleep(1);

    Client client;
    client.id = CLIENT_ID;
    if (client.connect(host_str, port_str) < 0) {
        fprintf(stderr, "Client failed to connect to server.\n");
        return 1;
    }

    response_t resp; // not used
    if (client.deposit(DEPOSIT_AMOUNT, &resp) < 0) {
        fprintf(stderr, "Client failed to deposit properly.\n");
        return 1;
    }

    if (!resp.success || resp.amount != DEPOSIT_AMOUNT) {
        fprintf(stderr, "Client's deposit was unsuccessful.\n");
        return 1;
    }


    client.disconnect();

    s.stop();
    return 0;
}

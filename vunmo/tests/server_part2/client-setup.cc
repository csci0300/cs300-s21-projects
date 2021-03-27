#include "../../network_helpers.hh"
#include "../../vunmo-server.hh"
#include "../../vunmo-client.hh"
#include "../../tests.hh"

/**
 * Client Setup
 *
 * Tests that a client is set up properly on the server when it connects.
 * Checks that the client exists in the server's client map with its ID
 * and initial amount of 0 in its account.
 */
int main() {
    // Create and start server.
    Server s;
    s.start(port, N_WORKERS);

    sleep(1); // give the server time to start

    Client client;
    client.id = CLIENT_ID;
    if (client.connect(host_str, port_str) < 0) {
        fprintf(stderr, "Client failed to connect to server.\n");
        return 1;
    }

    response_t resp;
    client.get_balance(&resp);
    if (!resp.success) {
        fprintf(stderr, "Client was unable to check its balance.\n");
        return 1;
    }
    if (resp.amount != 0) {
        fprintf(stderr, "Client is not setup with initial balance of 0.\n");
        return 1;
    }

    client.disconnect();
    s.stop();

    return 0;
}

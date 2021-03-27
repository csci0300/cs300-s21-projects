#include "../../vunmo-server.hh"
#include "../../vunmo-client.hh"
#include "../../tests.hh"

#define CHARGE_AMOUNT 500

/**
 * Charge Request Invalid Target
 *
 * Tests that a client cannot charge another client that doesn't exist.
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

    response_t resp;
    // Try charging a nonexistent target client CHARGE_AMOUNT amount of money. This should fail.
    if(client.charge(TARGET_CLIENT_ID, CHARGE_AMOUNT, &resp) < 0) {
        fprintf(stderr, "Client failed to send a charge request.\n");
        return 1;
    }
    if (resp.success) {
        fprintf(stderr, "Client's charge request succeeded when it should have failed.\n");
        return 1;
    }

    sleep(1);

    // Check that client's account is still the same.
    if (client.get_balance(&resp) < 0) {
        fprintf(stderr, "Client failed to send a balance request.\n");
        return 1;
    }
    if(!resp.success || resp.amount != 0) {
        fprintf(stderr, "Client's balance changed when it shouldn't have.\n");
        return 1;
    }

    client.disconnect();

    s.stop();
    return 0;
}

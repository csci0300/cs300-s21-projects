#include "../../vunmo-server.hh"
#include "../../vunmo-client.hh"
#include "../../tests.hh"

#define PAYMENT_AMOUNT 500

/**
 * Pay Request Invalid Target
 *
 * Tests that a client's pay request doesn't go through when the target client doesn't exist.
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
    // Deposit DEPOSIT_AMOUNT amount of money into client's account
    if (client.deposit(DEPOSIT_AMOUNT, &resp) < 0) {
        fprintf(stderr, "Client failed to send a deposit request.\n");
        return 1;
    }
    if (!resp.success || resp.amount != DEPOSIT_AMOUNT) {
        fprintf(stderr, "Client's deposit was unsuccessful.\n");
        return 1;
    }

    // Try payment the target client PAYMENT_AMOUNT amount of money.
    if(client.pay(TARGET_CLIENT_ID, PAYMENT_AMOUNT, &resp) < 0) {
        fprintf(stderr, "Client failed to send a pay request.\n");
        return 1;
    }
    if (resp.success || resp.amount == DEPOSIT_AMOUNT - PAYMENT_AMOUNT) {
        fprintf(stderr, "Client's payment went through when it should have failed.\n");
        return 1;
    }

    client.disconnect();

    s.stop();
    return 0;
}

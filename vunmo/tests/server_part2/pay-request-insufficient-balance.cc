#include "../../vunmo-server.hh"
#include "../../vunmo-client.hh"
#include "../../tests.hh"

#define PAYMENT_AMOUNT 500

/**
 * Pay Request Insufficient Balance
 *
 * Tests that a client's pay request doesn't go through when the client doesn't have enough money.
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

    Client target_client;
    target_client.id = TARGET_CLIENT_ID;
    if (target_client.connect(host_str, port_str) < 0) {
        fprintf(stderr, "Target client failed to connect to server.\n");
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

    // Try paying the target client more than client's balance. This should fail.
    if(client.pay(TARGET_CLIENT_ID, DEPOSIT_AMOUNT * 2, &resp) < 0) {
        fprintf(stderr, "Client failed to send a pay request.\n");
        return 1;
    }
    if (resp.success || resp.amount != DEPOSIT_AMOUNT) {
        fprintf(stderr, "Client's payment went through when it should have failed.\n");
        return 1;
    }

    // Check that target's balance hasn't changed.
    if(target_client.get_balance(&resp) < 0) {
        fprintf(stderr, "Target client failed to send a balance request.\n");
        return 1;
    }
    if(!resp.success || resp.amount != 0) {
        fprintf(stderr, "Target client's balance has changed when it shouldn't have.\n");
        return 1;
    }

    client.disconnect();
    target_client.disconnect();

    s.stop();
    return 0;
}

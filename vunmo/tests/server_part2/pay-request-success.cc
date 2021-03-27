#include "../../vunmo-server.hh"
#include "../../vunmo-client.hh"
#include "../../tests.hh"

#define PAYMENT_AMOUNT 500

/**
 * Pay Request Success
 *
 * Tests that a client can withdraw properly.
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

    // Try paying the target client PAYMENT_AMOUNT amount of money.
    if(client.pay(TARGET_CLIENT_ID, PAYMENT_AMOUNT, &resp) < 0) {
        fprintf(stderr, "Client failed to send a pay request.\n");
        return 1;
    }
    if (!resp.success || resp.amount != DEPOSIT_AMOUNT - PAYMENT_AMOUNT) {
        fprintf(stderr, "Client's payment failed.\n");
        return 1;
    }

    // Check that PAYMENT_AMOUNT amount of money is now in target's account.
    if(target_client.get_balance(&resp) < 0) {
        fprintf(stderr, "Target client failed to send a balance request.\n");
        return 1;
    }
    if(!resp.success || resp.amount != PAYMENT_AMOUNT) {
        fprintf(stderr, "Payment was not properly reflected in target's account.\n");
        return 1;
    }

    client.disconnect();
    target_client.disconnect();

    s.stop();
    return 0;
}

#include "../../vunmo-server.hh"
#include "../../vunmo-client.hh"
#include "../../tests.hh"

#define WITHDRAW_AMOUNT 500

/**
 * Withdraw Request
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

    response_t resp;
    if (client.deposit(DEPOSIT_AMOUNT, &resp) < 0) {
        fprintf(stderr, "Client failed to send a deposit request.\n");
        return 1;
    }
    if (!resp.success || resp.amount != DEPOSIT_AMOUNT) {
        fprintf(stderr, "Client's deposit was unsuccessful.\n");
        return 1;
    }

    // Try withdrawing more than what is in the account. This should fail.
    if(client.withdraw(DEPOSIT_AMOUNT * 2, &resp) < 0) {
        fprintf(stderr, "Client failed to send a withdraw request.\n");
        return 1;
    }
    if (resp.success || resp.amount != DEPOSIT_AMOUNT) {
        fprintf(stderr, "Client's withdrawal succeeded when it should have failed.\n");
        return 1;
    }

    // Try withdrawing some amount. This should go through.
    if(client.withdraw(WITHDRAW_AMOUNT, &resp) < 0) {
        fprintf(stderr, "Client failed to send a withdraw request.\n");
        return 1;
    }
    if (!resp.success || resp.amount != DEPOSIT_AMOUNT - WITHDRAW_AMOUNT) {
        fprintf(stderr, "Client's withdrawal failed.\n");
        return 1;
    }

    client.disconnect();

    s.stop();
    return 0;
}

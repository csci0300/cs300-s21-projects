#include "../../vunmo-server.hh"
#include "../../vunmo-client.hh"
#include "../../tests.hh"

#define CHARGE_AMOUNT 500

/**
 * Charge Request Success
 *
 * Tests that a client can charge another client properly.
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
    // Deposit DEPOSIT_AMOUNT amount of money into target client's account
    if (target_client.deposit(DEPOSIT_AMOUNT, &resp) < 0) {
        fprintf(stderr, "Target client failed to send a deposit request.\n");
        return 1;
    }
    if (!resp.success || resp.amount != DEPOSIT_AMOUNT) {
        fprintf(stderr, "Target client's deposit was unsuccessful.\n");
        return 1;
    }

    // Try charging the target client CHARGE_AMOUNT amount of money.
    if(client.charge(TARGET_CLIENT_ID, CHARGE_AMOUNT, &resp) < 0) {
        fprintf(stderr, "Client failed to send a charge request.\n");
        return 1;
    }
    if (!resp.success) {
        fprintf(stderr, "Client's charge request failed.\n");
        return 1;
    }

    sleep(1);

    // Get target's notifications.
    std::vector<notification_t> notifications = target_client.get_notifications();
    if (notifications.empty()) {
        fprintf(stderr, "Target did not receive any notifications.\n");
        return 1;
    }
    // Check that target received a correct notification.
    notification_t notif = notifications.front();
    if (notif.type != CHARGE_REQUEST_NOTIFICATION
        || notif.origin_client_id != CLIENT_ID
        || notif.amount != CHARGE_AMOUNT) {
        fprintf(stderr, "Target client received incorrect notification.\n");
        return 1;
    }
    // Accept the charge.
    if (target_client.handle_charge_request(&notif, true) < 0) {
        fprintf(stderr, "Target client failed to handle charge request.\n");
        return 1;
    }

    // Check that CHARGE_AMOUNT amount of money is added to client's account.
    if (client.get_balance(&resp) < 0) {
        fprintf(stderr, "Client failed to send a balance request.\n");
        return 1;
    }
    if(!resp.success || resp.amount != CHARGE_AMOUNT) {
        fprintf(stderr, "Charge was not properly reflected in client's account.\n");
        return 1;
    }

    // Check that CHARGE_AMOUNT amount of money is subtracted from target's account.
    if(target_client.get_balance(&resp) < 0) {
        fprintf(stderr, "Target client failed to send a balance request.\n");
        return 1;
    }
    if(!resp.success || resp.amount != DEPOSIT_AMOUNT - CHARGE_AMOUNT) {
        fprintf(stderr, "Charge was not properly reflected in target's account.\n");
        return 1;
    }

    client.disconnect();
    target_client.disconnect();

    s.stop();
    return 0;
}

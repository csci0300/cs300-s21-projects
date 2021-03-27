#include "../../tests.hh"
#include "../../network_helpers.hh"
#include "../../vunmo-server.hh"
#include "../../vunmo-client.hh"

#define NUM_CLIENTS 30
#define INITIAL_DEPOSIT 1000000

/**
 * A helper function that checks for any charge notifications and replies to them. Note that we
 * always accept charge requests in this test.
 */
int accept_charges(Client *c) {
    int err;
    auto ns = c->get_notifications();
    for (auto & n : ns) {
        if (n.type == CHARGE_REQUEST_NOTIFICATION) {
            err = c->handle_charge_request(&n, true);
            if (err < 0) return err;
        }
    }
    return 0;
}

/**
 * A helper function run by each client thread. Randomly pays or charges another client that is
 * also chosen randomly.
 */
int interactions(Client *c) {
    response_t resp;

    int err;
    for (int i = 0; i < 100; i++) {
        uint64_t target_id1 = rand() % NUM_CLIENTS;
        uint64_t target_id2 = rand() % NUM_CLIENTS;

        err = c->pay(target_id1, i, &resp);
        if (err < 0) return err;
        err = c->charge(target_id2, 1000-i, &resp);
        if (err < 0) return err;

        if (i % 10 == 0) {
            err = accept_charges(c);
            if (err < 0) return err;
        }
    }
    return 0;
}

/**
 * Basic Load Test
 *
 * Creates NUM_CLIENTS number of clients, and each client randomly pays or charges another client,
 * also chosen randomly. The test ensures that the initial total amount of money has not changed
 * at the end (no money lost, no hanging request). The test serves as a basic load test.
 */
int main() {
    srand(time(nullptr));
    // Create and start server.
    Server s;
    s.start(port, N_WORKERS);

    sleep(1);

    std::thread client_threads[NUM_CLIENTS];
    Client *clients[NUM_CLIENTS];

    // Create clients, each with INITIAL_DEPOSIT amount in its account.
    for (int i = 0; i < NUM_CLIENTS; i++) {
        auto *c = new Client();
        c->id = i;
        int err = c->connect(host_str, port_str);
        if (err < 0) {
            fprintf(stderr, "Failed to connect to server.\n");
            continue;
        }
        clients[i] = c;
        response_t resp;
        c->deposit(INITIAL_DEPOSIT, &resp);
    }

    // Spawn a thread per client, each running the interactions function.
    for (int i = 0; i < NUM_CLIENTS; i++) {
        client_threads[i] = std::thread(interactions, clients[i]);
    }

    int total_sum = 0;
    for(int i = 0; i < NUM_CLIENTS; i++) {
        // Attach all client threads.
        client_threads[i].join();
        clients[i]->disconnect();
        printf("Client %lu finished.\n", clients[i]->id);
        delete clients[i];
    }

    // freeze server and iterate over client snapshot
    printf("stopping server...\n");
    auto frozen_clients = s.stop();
    for (auto it = frozen_clients.begin(); it != frozen_clients.end(); ++it) {
        total_sum += it->second;
        printf("Client %lu finished with $%lu\n", it->first, it->second);
    }

    // Assert that we didn't lose any money; the total amount of money should be the same
    // at the end.
    if (total_sum != NUM_CLIENTS * INITIAL_DEPOSIT) {
        fprintf(stderr, "Total sum of money at the end doesn't match initial total sum");
        return 1;
    }

    // At this point, clients finished with internally consistent balances.
    // Stop the server.

    return 0;
}

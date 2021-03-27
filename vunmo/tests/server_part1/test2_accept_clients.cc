#include "../../vunmo-server.hh"
#include "../../vunmo-client.hh"
#include "../../tests.hh"

int main() {
    const int num_clients = 3;
    int err;

    // Create and start server.
    Server s;
    err = s.start(port, N_WORKERS);
    if (err != 0) {
        fprintf(stderr, "Server failed to start.\n");
        return -1;
    }

    sleep(1); // give the server time to start

    Client clients[3];

    for (int i = 0; i < num_clients; i++) {
        clients[i].id = i;
        err = clients[i].connect(host_str, port_str);
        if (err != 0) {
            fprintf(stderr, "Client failed to connect to server.\n");
            return -1;
        }
    }

    sleep(1); // give the server time to stop

    auto balances = s.stop();

    // from now on use exit instead of return to ignore the fact that clients
    // cannot disconnect correctly yet

    for (int i = 0; i < num_clients; i++) {
        auto entry = balances.find(i);
        if (entry == balances.end()) {
            fprintf(stderr, "Client accounts were missing.\n");
            exit(1);
        }
        if (entry->second != 0) {
            fprintf(stderr, "Starting client balances were incorrect.\n");
            exit(1);
        }
    }

    exit(0);

}

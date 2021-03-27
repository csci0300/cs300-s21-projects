#include "../../vunmo-server.hh"
#include "../../vunmo-client.hh"
#include "../../tests.hh"

int main() {
    // Create and start server.
    Server s;
    s.start(port, N_WORKERS);

    sleep(1); // give the server time to start

    int fd = create_connection(host_str, port_str);
    if (fd < 0) {
        fprintf(stderr, "Failed to open connection with server.\n");
        return -1;
    }

}

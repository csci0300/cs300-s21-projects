#include "../../synchronized_queue.cc"
#include "../../tests.hh"

/*
 * Serial Push All Flush
 *
 * Use a single thread to first push and then pop elements in order.
 */

int main(int argc, char** argv) {
    synchronized_queue<int> q;

    std::size_t s;
    // push all elements
    for (int i = 0; i < NUM_ELTS; i++) {
        q.push(i);
    }
    s = q.size();
    if (s != NUM_ELTS) {
        fprintf(stderr, "Pushed %d elements but queue has size %d.\n", NUM_ELTS, (int) s);
        return 1;
    }
    // flush elements
    std::vector<int> elts = q.flush();
    if (elts.size() != NUM_ELTS) {
        fprintf(stderr, "Expected %d flushed elements but got %d.\n", NUM_ELTS, (int) elts.size());
        return 1;
    }
    for (int i = 0; i < NUM_ELTS; i++) {
        if (elts[i] != i) {
            fprintf(stderr, "Expected elt %d but got %d.\n", i, elts[i]);
            return 1;
        }
    }
    q.stop();
    return 0;
}
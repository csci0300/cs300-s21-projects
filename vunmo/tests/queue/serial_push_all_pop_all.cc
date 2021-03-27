#include "../../synchronized_queue.cc"
#include "../../tests.hh"

/*
 * Serial Push All Pop All
 *
 * Use a single thread to first push all and then pop all elements in order.
 */

int main(int argc, char** argv) {
    synchronized_queue<int> q;

    // push all elements
    for (int i = 0; i < NUM_ELTS; i++) {
        q.push(i);
    }
    std::size_t s = q.size();
    if (s != NUM_ELTS) {
        fprintf(stderr, "Pushed %d elements but queue has size %d.\n", NUM_ELTS, (int) s);
        return 1;
    }
    // pop all elements
    int cur;
    bool stopped;
    for (int i = 0; i < NUM_ELTS; i++) {
        stopped = q.pop(&cur);
        if (stopped) {
            fprintf(stderr, "Queue spuriously stopped.\n");
            return 1;
        }
    }
    q.stop();
    return 0;
}
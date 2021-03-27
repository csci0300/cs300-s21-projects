#include "../../synchronized_queue.cc"
#include "../../tests.hh"

/*
 * Serial Push Pop
 *
 * Use a single thread to push and pop elements in order, one at a time.
 */

int main(int argc, char** argv) {
    synchronized_queue<int> q;

    std::size_t s;
    int cur;
    bool stopped;

    // push elements
    for (int i = 0; i < NUM_ELTS; i++) {
        // push, check size
        q.push(i);
        s = q.size();
        if (s != 1) {
            fprintf(stderr, "Queue should have size 1 but has size %d.\n", (int) s);
            return 1;
        }
        // pop, check stopped, elt, size
        stopped = q.pop(&cur);
        if (stopped) {
            fprintf(stderr, "Queue spuriously stopped.\n");
            return 1;
        }
        if (cur != i) {
            fprintf(stderr, "Expected to pop elt %d but got %d.\n", i, cur);
            return 1;
        }
        s = q.size();
        if (s != 0) {
            fprintf(stderr, "Queue should be empty but has size %d.\n", (int) s);
            return 1;
        }
    }
    q.stop();
    return 0;
}
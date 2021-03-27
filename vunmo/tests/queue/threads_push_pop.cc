#include <thread>
#include <vector>
#include <algorithm>
#include "../../synchronized_queue.cc"
#include "../../tests.hh"

/*
 * Threads Push Pop
 *
 * Use a single thread to push and pop elements in order, one at a time.
 */

void push_and_pop_elements(std::vector<int> *elts, synchronized_queue<int> *q,
        std::vector<int> *popped_elts) {

    int popped_elt;
    bool stopped;

    for (int &elt : (*elts)) {
        // push
        q->push(elt);
        // pop
        stopped = q->pop(&popped_elt);
        if (stopped) {
            fprintf(stderr, "Queue spuriously stopped.\n");
            exit(1);
        }
        // save
        popped_elts->push_back(popped_elt);
    }
}

int main() {
    synchronized_queue<int> q;

    // generate elts to push
    std::vector<int> elts_to_push[NUM_THREADS];
    std::vector<int> all_pushed_elts;
    for (int i = 0; i < NUM_ELTS; i++) {
        for (int j = 0; j < NUM_THREADS; j++) {
            // randomize each element
            int randint = rand() % MAX_ELT;
            elts_to_push[j].push_back(randint);
            all_pushed_elts.push_back(randint);
        }
    }

    // start threads
    std::thread threads[NUM_THREADS];
    std::vector<int> popped_elts[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("starting thread %d\n", i);
        threads[i] = std::thread(push_and_pop_elements, &elts_to_push[i], &q, &popped_elts[i]);
    }

    // join threads and accumulate popped elements
    std::vector<int> all_popped_elts;
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
        printf("joined thread %d\n", i);
        for (int &elt : popped_elts[i]) {
            all_popped_elts.push_back(elt);
        }
    }

    q.stop();

    // verify popped items length
    int expected_elts = NUM_THREADS * NUM_ELTS;
    if ((int) all_popped_elts.size() != expected_elts) {
        fprintf(stderr, "Expected %d elements popped, but got %d.\n",
                expected_elts, (int) all_popped_elts.size());
        exit(1);
    }
    // sort and compare
    std::sort(all_popped_elts.begin(), all_popped_elts.end());
    std::sort(all_pushed_elts.begin(), all_pushed_elts.end());
    for (int i = 0; i < expected_elts; i++) {
        if (all_popped_elts[i] != all_pushed_elts[i]) {
            fprintf(stderr, "Popped elements do not match pushed elements.\n");
            exit(1);
        }
    }
    return 0;
}
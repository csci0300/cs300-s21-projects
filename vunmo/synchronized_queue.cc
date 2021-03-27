#ifndef SYNCHRONIZED_QUEUE
#define SYNCHRONIZED_QUEUE

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

/**
 * Implementation of a thread-safe, generic queue.
 *
 * This class is a wrapper for a std::queue that handles all synchronization
 * automatically. Any number of threads can be using this queue concurrently,
 * without having to explicitly lock and unlock mutexes.
 *
 * The class is parametrized by <T>, which can be any object or struct. This
 * allows the queue to be used in a very similar way to the bare std::queue.
 *
 * Before termination of a program the queue needs to be stopped, to allow all
 * waiting threads to continue and to allow the std::condition_variable to be
 * cleaned up.
 */
template <typename T>
class synchronized_queue {
 public:
  /**
   * Get current size of queue.
   *
   * NOTE: By the time this call returns, another thread may have changed the
   * size of the queue. For this reason it is not advised to use this call for
   * anything other than logging.
   *
   * @return current size of the queue
   *
   */
  std::size_t size();

  /**
   * Pop and set the elt pointer element from the front of the queue. Returns
   * true if the queue has been stopped, or false otherwise.
   *
   * Unless the queue has been stopped, calling pop() will always set elt and
   * return false. If the queue is empty, pop will wait until an element is
   * pushed onto it.
   *
   * However, if the queue has been stopped, even if the queue is not empty, the
   * function will return true with no element set.
   *
   * @param elt pointer to be set to popped element
   * @return true if the queue has been stopped, or false otherwise
   */
  bool pop(T* elt);

  /**
   * Push an element onto the back of the queue.
   *
   * If the queue is empty, this call will signal any one thread that is waiting
   * to pop from the queue.
   *
   * @param elt element to be pushed on the queue
   */
  void push(T elt);

  /**
   * Flush will return a vector of all elements currently in the queue.
   *
   * After this function is called the queue will be empty. This call may
   * return an empty vector without blocking.
   *
   * @return vector of all elements in the queue
   */
  std::vector<T> flush();

  /**
   * Stop the queue.
   *
   * This function serves as a way to release any threads waiting for the
   * queue to become non-empty. Doing this also allows the condition variable
   * to be cleaned up when the program exists.
   */
  void stop();

 private:
  // Underlying queue data structure.
  std::queue<T> q;
  // Synchronization fields.
  std::mutex mtx;
  std::condition_variable cv;
  // Atomic, thread-safe boolean that indicates whether the queue has been
  // stopped; false until stop() is called, which sets it to true.
  std::atomic_bool is_stopped{false};
};

template <typename T>
std::size_t synchronized_queue<T>::size() {
  std::size_t s;

  this->mtx.lock();
  s = this->q.size();
  this->mtx.unlock();

  return s;
}

template <typename T>
bool synchronized_queue<T>::pop(T* elt) {
  // TODO: implement
  return false;
}

template <typename T>
void synchronized_queue<T>::push(T elt) {
  // TODO: implement
}

template <typename T>
std::vector<T> synchronized_queue<T>::flush() {
  std::vector<T> elts;
  // TODO: implement
  return elts;
}

template <typename T>
void synchronized_queue<T>::stop() {
  // set is_stopped to true, and wake up all threads waiting on the cond
  // variable
  // TODO: implement
}

#endif

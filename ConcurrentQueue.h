#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H

/**
 * This header-only template requires C++11 or newer
 */

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

namespace neu
{
namespace lustiglab
{
namespace framework
{
/**
 * A multiple-producers, multiple-consumers thread-safe queue,
 * designed to avoid deadlock. T must support move semantics.
 */
template <typename T> class ConcurrentQueue
{
  public:
    ConcurrentQueue();
    virtual ~ConcurrentQueue();
    // Push an item into the queue
    void push(const T &&item)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(std::move(item));
        lock.unlock();
        // notify a single waiting thread
        m_cond.notify_one();
    }
    // Pop an item from the queue; blocks if queue is empty
    T pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        // using while loop to guard against spurious wake-ups
        while(m_queue.empty()) {
            // unlock the mutex and join the queue of waiting threads
            m_cond.wait(lock);
        }
        T item = std::move(m_queue.front());
        m_queue.pop();
        return std::move(item);
    }
    T pop(int timeout)
    {
        // TODO: implement a version of pop() that unblocks after timeout
        return T();
    }

  private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cond;
};
} // namespace framework
} // namespace lustiglab
} // namespace neu

#endif // CONCURRENTQUEUE_H

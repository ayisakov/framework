#ifndef SIMPLEEVENTAPP_H
#define SIMPLEEVENTAPP_H

#include <atomic>
#include "ConcurrentQueue.h"
#include "IMessageReceiver.h"
#include "IRunnable.h"

namespace ayisakov
{
namespace framework
{
/**
 * TEvent must support move semantics.
 */
template <typename TEvent>
class SimpleEventApp : public IRunnable, public IMessageReceiver<TEvent>
{
  public:
    SimpleEventApp() : m_run(false), m_exitCode(0) {}
    virtual ~SimpleEventApp() override {}
    virtual int run() override
    {
        m_run = true;
        m_exitCode = 0;
        while(m_run) {
            TEvent event = m_queue.pop();
            process(event);
        }
        return m_exitCode;
    }
    /**
     * Send a message to this receiver
     * @param [in] message the message
     * @return 0 on success
     */
    virtual int send(TEvent &&message) override
    {
        m_queue.push(std::move(message));
        return 0;
    }

    virtual void terminate() override { m_run = false; }

  protected:
    virtual void process(const TEvent &event)
    {
        // TODO: implement
    }

  private:
    ConcurrentQueue<TEvent> m_queue;
    std::atomic<bool> m_run;
    std::atomic<int> m_exitCode;
};
} // namespace framework
} // namespace ayisakov

#endif // SIMPLEEVENTAPP_H

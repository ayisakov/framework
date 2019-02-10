#ifndef THREADRUNNER_H
#define THREADRUNNER_H

#include <atomic>
#include <thread>
#include "IAsyncRunner.h"

namespace ayisakov
{
namespace framework
{
class ThreadRunner : public IAsyncRunner
{
  public:
    ThreadRunner(IRunnable &task);
    virtual ~ThreadRunner(); // TODO: make sure to request termination and join the thread here
    /**
     * Launch asynchronously, and do not block once started.
     *
     * @return 0 if launched successfully; nonzero value otherwise.
     */
    virtual int launch();
    /**
     * Block here until asynchronous execution has finished (join the thread).
     * Returns immediately if no task is running at the time of the call.
     *
     * @return last exit code or 0 if no task was run
     */
    virtual int waitExit();
    /**
     * Returns true if the task is running.
     */
    virtual bool running();

  protected:
    virtual int run();

  private:
    std::thread m_thread;
    std::atomic<bool> m_running;
    std::atomic<int> m_retval;
};
} // namespace framework
} // namespace ayisakov


#endif // THREADRUNNER_H

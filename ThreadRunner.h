#ifndef THREADRUNNER_H
#define THREADRUNNER_H

#include <atomic>
#include <thread>
#include <functional>
#include "IAsyncRunner.h"

namespace ayisakov
{
namespace framework
{
class ThreadRunner : public IAsyncRunner
{
  public:
    using CompletionHandler = std::function<void(int)>;
    ThreadRunner(IRunnable &task);
    ThreadRunner(IRunnable &task, const CompletionHandler &handler);
    virtual ~ThreadRunner();
    /**
     * Launch asynchronously, and do not block once started.
     *
     * @return 0 if launched successfully; nonzero value otherwise.
     */
    virtual int launch() override;
    /**
     * Block here until asynchronous execution has finished (join the thread).
     * Returns immediately if no task is running at the time of the call.
     *
     * @return last exit code or 0 if no task was run
     */
    virtual int waitExit() override;
    /**
     * Returns true if the task is running.
     */
    virtual bool running() override;

  protected:
    virtual int run() override;

  private:
    std::thread m_thread;
    std::atomic<bool> m_running;
    std::atomic<int> m_retval;
    CompletionHandler m_userHandler;
};
} // namespace framework
} // namespace ayisakov


#endif // THREADRUNNER_H

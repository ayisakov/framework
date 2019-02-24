#ifndef IASYNCRUNNER_H
#define IASYNCRUNNER_H

#ifdef FRAMEWORK_TASK_DEBUG
#include <iostream>
#endif // FRAMEWORK_TASK_DEBUG
#include "IRunnable.h"

namespace ayisakov
{
namespace framework
{
/**
 * This is the interface for running a Runnable asynchronously.
 */
class IAsyncRunner
{
  public:
    IAsyncRunner(IRunnable &task) : m_task(task) {}
    virtual ~IAsyncRunner() {}
    /**
     * Launch asynchronously, and do not block once started.
     *
     * @return 0 if launched successfully or if task is already
     * running; nonzero value otherwise.
     */
    virtual int launch() = 0;
    /**
     * Block here until asynchronous execution has finished.
     * Returns immediately if no task is running at the time of the call.
     *
     * @return last exit code or 0 if no task was run
     */
    virtual int waitExit() { return 0; }
    /**
     * Returns true if the task is running.
     */
    virtual bool running() = 0;

  protected:
    IRunnable &task() { return m_task; }
    virtual int run()
    {
        int ret = m_task.initialize();
        if(ret) {
            return ret;
        }
        ret = m_task.run();
#ifdef FRAMEWORK_TASK_DEBUG
        std::cout << ret << " from m_task.run()" << std::endl;
#endif // FRAMEWORK_TASK_DEBUG
        m_task.postrun();
        return ret;
    }

  private:
    IRunnable &m_task;
}; // namespace framework
} // namespace framework
} // namespace ayisakov

#endif // IASYNCRUNNER_H

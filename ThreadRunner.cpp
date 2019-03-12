#ifdef FRAMEWORK_TASK_DEBUG
#include <iostream>
#endif // FRAMEWORK_TASK_DEBUG
#include "ThreadRunner.h"

using ayisakov::framework::IRunnable;
using ayisakov::framework::ThreadRunner;

ayisakov::framework::ThreadRunner::ThreadRunner(IRunnable &task)
: IAsyncRunner(task), m_running(false), m_retval(0)
{
}
ayisakov::framework::ThreadRunner::ThreadRunner(IRunnable &task,
                                                const CompletionHandler &handler)
: ThreadRunner(task), m_userHandler(handler)
{
}

ayisakov::framework::ThreadRunner::~ThreadRunner()
{
    waitExit();
}

int ayisakov::framework::ThreadRunner::launch()
{
    if(running()) return 0;

    m_thread = std::thread(&ThreadRunner::run, std::ref(*this));
    if(!m_thread.joinable()) return -1;

    return 0;
}

bool ayisakov::framework::ThreadRunner::running()
{
    if(!m_thread.joinable()) return false;
    return m_running;
}

int ayisakov::framework::ThreadRunner::run()
{
    m_running = true;
    int ret = IAsyncRunner::run();
#ifdef FRAMEWORK_TASK_DEBUG
    std::cout << ret << " from IAsyncRunner::run()" << std::endl;
#endif // FRAMEWORK_TASK_DEBUG
    m_running = false;
    m_retval = ret;

    // notify of completion, if using notification callback
    if(m_userHandler) {
        m_userHandler(ret);
    }

    return ret;
}

int ayisakov::framework::ThreadRunner::waitExit()
{
    if(!m_thread.joinable()) {
        if(m_running) {
            return -1; // running but not joinable is an error condition
        }
        return m_retval; // already ended
    }
    task().terminate();
    m_thread.join();
    return m_retval;
}

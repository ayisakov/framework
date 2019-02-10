#include "ThreadRunner.h"

using ayisakov::framework::IRunnable;
using ayisakov::framework::ThreadRunner;

ayisakov::framework::ThreadRunner::ThreadRunner(IRunnable &task)
: m_task(task), m_running(false), m_retval(0)
{
}

ayisakov::framework::ThreadRunner::~ThreadRunner()
{
    waitExit();
}

int ayisakov::framework::ThreadRunner::launch()
{
    if(running()) return 0;

    m_thread = std::thread(&IThreadRunner::run, std::ref(*this));
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
    m_running = false;
    m_retval = ret;
    return ret;
}

int ayisakov::framework::ThreadRunner::waitExit()
{
    task().terminate();
    m_thread.join();
    return m_retval;
}

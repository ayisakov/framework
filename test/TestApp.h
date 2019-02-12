#ifndef TESTAPP_H
#define TESTAPP_H

#include <atomic>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include "ITestApp.h"

namespace ayisakov
{
namespace framework
{
/**
 * This is a utility class used to check how many times a callback was called.
 */
class TestFunctor
{
  public:
    TestFunctor() : m_value(0) {}
    ~TestFunctor() {}
    void increment() {
        m_value++;
        m_cond.notify_all();
    } // the callback
    void reset() { m_value = 0; }
    int value() const { return m_value; }
    /**
     * @return 0 on success, 1 if wait timed out
     */
    int waitUntilCalled(int timeout_sec) {
        std::chrono::duration<int> timeout(timeout_sec);
        std::unique_lock<std::mutex> mlock(m_mutex);
        std::cv_status waitret = m_cond.wait_for(mlock, timeout);
        return waitret == std::cv_status::timeout ? 1 : 0;
    }

  private:
    std::atomic<int> m_value;
    std::mutex m_mutex; // used for condition variable wait
    std::condition_variable m_cond;
};

class TestApp : public ITestApp
{
  public:
    TestApp();
    TestApp(const std::function<void(void)> &onSampleMsg);
    virtual ~TestApp();
    // Message handlers
    void onMsg(const MsgSample &msg) override;
    void onMsg(const MsgQuit &msg) override;

  private:
    std::function<void(void)> m_onSampleMsg;

};
} // namespace framework
} // namespace ayisakov
#endif // TESTAPP_H

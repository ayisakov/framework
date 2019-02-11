#ifndef TESTAPP_H
#define TESTAPP_H

#include <atomic>
#include <functional>
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
    void increment() { m_value++; } // the callback
    void reset() { m_value = 0; }
    int value() const { return m_value; }

  private:
    std::atomic<int> m_value;
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

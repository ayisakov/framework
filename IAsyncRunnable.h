#ifndef IASYNCRUNNABLE_H
#define IASYNCRUNNABLE_H

#include "IRunnable.h"

namespace ayisakov
{
namespace framework
{
class IAsyncRunnable : public IRunnable
{
  public:
    IAsyncRunnable() {}
    /**
     * Optionally provide a callback to notify the
     * calling process of a successful startup or some other condition.
     * What defines a successful startup is up to the user of this class.
     */
    IAsyncRunnable(std::function<void()> &startupCallback)
    : m_startupCallback(startupCallback)
    {
    }
    virtual ~IAsyncRunnable() {}

  protected:
    void notify()
    {
        if(m_startupCallback) {
            m_startupCallback();
        }
    }

  private:
    std::function<void()> m_startupCallback;
};
} // namespace framework
} // namespace ayisakov

#endif // IASYNCRUNNABLE_H

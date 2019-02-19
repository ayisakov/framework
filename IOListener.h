#ifndef IOLISTENER_H
#define IOLISTENER_H

#include <atomic>
#include "IIOListener.h"

namespace ayisakov
{
namespace framework
{
class IIOProvider;
class IOListener : public IIOListener
{
  public:
    IOListener();
    IOListener(IIOProvider *pProvider);
    virtual ~IOListener();

    virtual int subscribe(IIOProvider *pProvider) override;

    /**
     * Returns true if the listener is running (usually in its own thread)
     */
    virtual bool isRunning() override;

    // run() should then call pProvider's dispatchEvents() function to begin
    // dispatching events for ports created by the registered provider
    virtual int run() override;

  private:
    IIOProvider *m_pProvider;
    std::atomic<bool> m_isRunning;

};
} // namespace framework
} // namespace ayisakov

#endif // IOLISTENER_H

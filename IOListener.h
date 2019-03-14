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
     * Remove link between the specified provider and this listener
     */
    virtual void unsubscribe(IIOProvider *pProvider) override;

    /**
     * Returns true if the listener is running (usually in its own thread)
     */
    virtual bool isRunning() override;

    // run() should then call pProvider's dispatchEvents() function to begin
    // dispatching events for ports created by the registered provider
    virtual int run() override;

    /**
     * Send a message to this receiver
     * @param [in] message the message
     * @return 0 on success
     */
//    virtual int send(IMessagePtr &message) override;

  private:
    IIOProvider *m_pProvider;
    std::atomic<bool> m_isRunning;
};
} // namespace framework
} // namespace ayisakov

#endif // IOLISTENER_H

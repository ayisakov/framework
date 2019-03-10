#ifndef IIOPROVIDER_H
#define IIOPROVIDER_H

#include <functional>
#include <memory>
#include "ITimer.h"

namespace ayisakov
{
namespace framework
{
class IIOListener;
class ISerialPort;
/**
 * The IOProvider is an I/O microservice.
 */
using Handler = std::function<void(void)>;
using ITimerPtr = std::shared_ptr<ITimer>;
class IIOProvider
{
  public:
    IIOProvider() {}
    virtual ~IIOProvider() {}

    /**
     * Obtain a pointer to an available serial port
     * This port is owned exclusively by the provider and
     * only leased to the caller.
     * NEVER attempt to delete the object to which a pointer
     * has been received using this function.
     */
    virtual ISerialPort *getSerialPort() = 0;

    // Indicate to the provider that the port is no longer being used
    virtual void release(const std::string &portId) = 0;

    // Set a timer
    virtual ITimerPtr
    setTimer(unsigned milliseconds, TimerHandler &callback) = 0;

    /**
     * Register a listener that will process events on ports
     * managed by this provider. In order to be notified about
     * events on a specific port, the listener must also register
     * with each ISerialPort it wishes to monitor.
     * Only one listener can be subscribed at a time. Attempting
     * to subscribe another will return an error code.
     *
     * @return 0 on success or same listener is already subscribed
     */
    virtual int setListener(IIOListener *pListener) = 0;

    /**
     * Unregister a previously subscribed listener
     */
    virtual int removeListener(IIOListener *pListener) = 0;

    /**
     * Wait for events on sockets managed by this provider.
     * This function should be called in the context of the
     * thread responsible for dispatching events. All registered
     * callbacks will run in the thread that calls this function.
     *
     * @param continuously: if true, the call will block here
     * until the I/O provider is stopped. If false, only
     * outstanding events at the time of the call will be
     * processed, after which the function will return
     */
    virtual int dispatchEvents(IIOListener *pListener,
                               bool continuously = true) = 0;

    /**
     * Queue up a custom event handler for deferred execution.
     * The handler will be invoked in during a call to
     * dispatchEvents.
     *
     */
    virtual int postEvent(const Handler &handler) = 0;

    /**
     * Stop dispatching events
     */
    virtual void stop() = 0;
};
} // namespace framework
} // namespace ayisakov

#endif // IIOPROVIDER_H

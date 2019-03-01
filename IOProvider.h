#ifndef IOPROVIDER_H
#define IOPROVIDER_H

#include <boost/asio.hpp>
#include <boost/functional/hash.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "IIOProvider.h"

namespace ayisakov
{
namespace framework
{
typedef boost::hash<boost::uuids::uuid> uuid_hash;
class IOProvider : public IIOProvider
{
  public:
    IOProvider();
    virtual ~IOProvider();

    // Create or obtain an available serial port and return
    // a pointer to it
    virtual ISerialPort *getSerialPort() override;

    // Indicate to the provider that the port is no longer being used
    virtual void release(const std::string &portId) override;

    // Set a timer
    virtual ITimerPtr setTimer(unsigned milliseconds,
                               TimerHandler &callback) override;

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
    virtual int setListener(IIOListener *pListener) override;

    /**
     * Unregister a previously subscribed listener
     */
    virtual int removeListener(IIOListener *pListener) override;

    /**
     * Queue up a custom event handler for deferred execution.
     * The handler will be invoked in during a call to
     * dispatchEvents.
     *
     */
    virtual int postEvent(const Handler &handler) override;

    /**
     * Wait for events on sockets managed by this provider.
     * This function should be called in the context of the
     * thread responsible for dispatching events. All registered
     * callbacks will run in the thread that calls this function.
     *
     */
    virtual int dispatchEvents(IIOListener *pListener,
                               bool continuously = true) override;

  private:
    // Ports, hashed by a unique id with which they are tagged
    std::unordered_map<boost::uuids::uuid, std::unique_ptr<ISerialPort>, uuid_hash> m_ports;

    // Unused ports
    std::unordered_set<boost::uuids::uuid, uuid_hash> m_unused;

    // A listener, if one is registered
    IIOListener *m_listener;

    // The I/O context
    boost::asio::io_service m_ioContext;

    // true if an invocation of dispatchEvents has not returned
    bool m_dispatching;
};
} // namespace framework
} // namespace ayisakov
#endif // IOPROVIDER_H

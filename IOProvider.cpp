#include "IOProvider.h"
#include "IOListener.h"
#include "RelTimerMs.h"
#include "ScopeSetter.h"
#include "SerialPort.h"

namespace ayif = ayisakov::framework;

ayisakov::framework::IOProvider::IOProvider(ILogger *pLogger)
: m_listener(nullptr), m_dispatching(false), m_pLogger(pLogger)
{
}

ayisakov::framework::IOProvider::~IOProvider()
{
    stop();
    // Remove reference to this in a listener, if one exists
    if(m_listener) {
        m_listener->unsubscribe(this);
    }
}

ayisakov::framework::ISerialPort *ayisakov::framework::IOProvider::getSerialPort()
{
    ISerialPort *pPort = nullptr;
    // first, check if there are any unused ports
    if(!m_unused.empty()) {
        auto it = m_unused.begin();
        pPort = m_ports[*it].get();
        m_unused.erase(it);
    } else {
        // if no unused ports, then create a new port
        std::unique_ptr<SerialPort> created_derived(
            new SerialPort(m_ioContext, this, m_pLogger));
        boost::uuids::uuid id = created_derived->uuid();
        // TODO: make this safer. Check for clashes before inserting;
        // otherwise this operation can destroy referenced objects.
        m_ports[id] = std::move(created_derived);
        pPort = m_ports[id].get();
    }
    return pPort;
}

void ayisakov::framework::IOProvider::release(const std::string &portId)
{
    // Convert string to uuid
    boost::uuids::string_generator gen;
    boost::uuids::uuid id(gen(portId));
    // Make sure that we actually own a port with that UUID
    try {
        std::unique_ptr<ISerialPort> &port_found = m_ports.at(id);
        // Reset the port
        port_found->reset(); // port_found should always be a valid pointer
        // Store the uuid in the unused set
        // TODO: add maximum number of unused ports to keep around
        m_unused.insert(id);
    } catch(const std::out_of_range &e) {
        return;
    }
}

int ayisakov::framework::IOProvider::setListener(IIOListener *pListener)
{
    if(m_listener)
        return -1; // Only one listener can be a subscriber

    if(!pListener) return -2; // Fed a null listener pointer

    // all is well
    m_listener = pListener;
    return 0;
}

int ayisakov::framework::IOProvider::removeListener(ayisakov::framework::IIOListener *pListener)
{
    if(!pListener) return -2;

    if(pListener != m_listener)
        return -1; // listeners must match

    // all is well
    m_listener = nullptr;
    return 0;
}

ayif::ITimerPtr
ayisakov::framework::IOProvider::setTimer(unsigned int ms, const TimerHandler &callback)
{
    ITimerPtr pTimer(new RelTimerMs(m_ioContext, ms, callback));
    return pTimer;
}

int ayif::IOProvider::postEvent(const Handler &handler)
{
    m_ioContext.post(handler);
    return 0;
}

int ayisakov::framework::IOProvider::dispatchEvents(ayisakov::framework::IIOListener *pListener,
                                                    bool continuously)
{
    if(pListener != m_listener) {
        return -1; // listeners must match if registered
    }

    if(m_dispatching) {
        return -2; // multiple simultaneous invocations are not allowed
    }

    // set dispatching to true and reset at scope exit
    ayif::ScopeSetter<bool> dispSetter(m_dispatching, true);

	// If the context was run non-continuously, it needs to be reset before the next invocation
    if(m_ioContext.stopped()) {
        m_ioContext.reset();
    }

    std::unique_ptr<boost::asio::io_service::work> pWork(nullptr);
    if(continuously) {
        // Needed to keep io_service::run() from returning when
        // there are no outstanding asynchronous operations
        pWork = std::unique_ptr<boost::asio::io_service::work>(
            new boost::asio::io_service::work(m_ioContext));
    }
    while(true) { // in a loop so that non-fatal exceptions can be handled without exiting
        try {
            // Block here until stopped if running continuously
            // or until all handlers have been called if not
            m_ioContext.run();
//            std::cout
//                << "Returned from boost::asio::io_service::run..."
//                << std::endl;
            break;                   // normal exit
        } catch(std::exception &e) { // TODO: only catch Boost-specific exceptions here; the rest should bubble up
            // TODO: deal with exception
//            std::cout << "Exception from "
//                         "boost::asio::io_service::run: "
//                      << e.what() << std::endl;
            return -1; // TODO: only do this for fatal exceptions
        }
    }
    return 0;
}

void ayif::IOProvider::stop()
{
    if(!m_ioContext.stopped()) {
        m_ioContext.stop();
    }
}

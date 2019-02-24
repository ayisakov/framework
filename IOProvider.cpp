#include "IOProvider.h"
#include "IOListener.h"
#include "SerialPort.h"


ayisakov::framework::IOProvider::IOProvider()
: m_listener(nullptr)
{
}

ayisakov::framework::IOProvider::~IOProvider()
{
    // Stop the io_service/context, dispatching all outstanding events
    m_pWork.reset();
    // Remove reference to this in a listener, if one exists
    if(m_listener) {
        m_listener->unsubscribe(this);
    }
}

ayisakov::framework::ISerialPort *ayisakov::framework::IOProvider::getPort()
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
            new SerialPort(m_ioContext, this));
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

int ayisakov::framework::IOProvider::dispatchEvents(ayisakov::framework::IIOListener *pListener)
{
    if(pListener != m_listener) {
        return -1; // listeners must match if registered
    }

    m_pWork = std::unique_ptr<boost::asio::io_service::work>(
        new boost::asio::io_service::work(m_ioContext));
    while(true) {
        try {
            // Block here until stopped
            m_ioContext.run();
            break; // normal exit
        } catch(std::exception &e) {
            // TODO: deal with exception
            return -1;
        }
    }
    return 0;
}

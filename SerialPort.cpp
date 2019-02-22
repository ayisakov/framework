#include "SerialPort.h"
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "IIOProvider.h"

ayisakov::framework::SerialPort::SerialPort(boost::asio::io_service &context,
                                            IIOProvider *pProvider)
: m_pProvider(pProvider), m_port(boost::asio::serial_port(context)),
  m_uuid(boost::uuids::random_generator()())
{
}

ayisakov::framework::SerialPort::~SerialPort()
{
    // TODO: see if any clean-up is needed that is not
    // performed by the underlying object's destructor
}

void ayisakov::framework::SerialPort::registerSink(IMessageSink *pSink)
{
    m_pSink = pSink;
}

const boost::uuids::uuid &ayisakov::framework::SerialPort::uuid()
{
    return m_uuid;
}

std::string ayisakov::framework::SerialPort::id()
{
    return boost::uuids::to_string(uuid());
}

int ayisakov::framework::SerialPort::open(const std::string &device)
{
    try {
        m_port.open(device);
    } catch(boost::system::system_error &e) {
        return -1;
    }
    return 0;
}

int ayisakov::framework::SerialPort::close()
{
    if(!m_port.is_open()) return 0;

    try {
        m_port.close();
    } catch(boost::system::system_error &e) {
        return -1;
    }
    return 0;
}

void ayisakov::framework::SerialPort::release()
{
    if(m_pProvider) {
        m_pProvider->release(boost::uuids::to_string(uuid()));
    }
}

int ayisakov::framework::SerialPort::writeAsync(const std::string &buffer)
{
    // TODO: implement
}

int ayisakov::framework::SerialPort::readAsync()
{
    // TODO: implement
}

void ayisakov::framework::SerialPort::reset() { m_port.close(); }

void ayisakov::framework::SerialPort::onWriteSuccess(int bytesWritten)
{
    // TODO: implement
}

void ayisakov::framework::SerialPort::onWriteFail(int errorCode)
{
    // TODO: implement
}

void ayisakov::framework::SerialPort::onReadSuccess(const char *buffer, size_t len)
{
    // TODO: implement
}

void ayisakov::framework::SerialPort::onReadFail(int errorCode)
{
    // TODO: implement
}

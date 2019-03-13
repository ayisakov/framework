#include "SerialPort.h"
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "IIOProvider.h"

namespace ayif = ayisakov::framework;

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
        // TODO: add setting baud rate to the intertace
        m_port.set_option(boost::asio::serial_port_base::baud_rate(9600));
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

int ayisakov::framework::SerialPort::writeAsync(IWriteBufferPtr &pWriteBuf,
                                                const WriteCallback &callback)
{
    // TODO: implement a version of this function that uses boost::asio::async_read_until()
    // and set a busy flag when the operation begins to make sure that no calls to any version
    // of the read function either fail or become queued until this operation completes
    if(!pWriteBuf) return -1; // empty container

    // register this buffer, making sure tag is unique
    BufferTag tag = pWriteBuf->tag();
    if(m_writeBuffers.find(tag) != m_writeBuffers.end()) {
        return -2; // tag not unique
    }
    // save raw pointer before moving buffer to container
    IWriteBuffer *pBuf = pWriteBuf.get();
    if(!pBuf) {
        // TODO: turn this and other debug messages into
        // debug-level log function calls using the ILogger
        // interface or use exceptions
        //        std::cout << "Could not retrieve plain pointer!" << std::endl;
        return -4;
    }
    m_writeBuffers[tag] = std::move(pWriteBuf);
    IWriteBuffer *pUnused = m_writeBuffers[tag].get();
    if(pUnused != pBuf) {
        //        std::cout << "Failed to store buffer pointer!" << std::endl;
        return -3;
    }

    if(pBuf != m_writeBuffers[tag].get()) {
        // std::cout << "Failed to store buffer pointer!" << std::endl;
        return -3;
    }

    auto handler = [=](const boost::system::error_code &error,
                       std::size_t bytesWritten) {
        //       std::cout
        //           << "Entered SerialPort's internal write handler."
        //           << std::endl;
        IWriteBuffer *buf = (m_writeBuffers[tag]).get();
        //        std::cout << "Wrote " << bytesWritten << " byte(s)" << std::endl;
        buf->bytesWritten(bytesWritten);
        buf->error(error.value());
        if(callback) {
            //            callback(buf);
            callback(m_writeBuffers[tag]);
        }
        m_writeBuffers.erase(tag);
    };
    m_port.async_write_some(boost::asio::buffer(pBuf->contents(),
                                                pBuf->length()),
                            handler);
    return 0;
}

int ayisakov::framework::SerialPort::writeSync(IWriteBuffer &writeBuf)
{
    // TODO: it's probably cleaner to let any exceptions bubble
    // up through this function instead
    try {
        std::size_t written =
            boost::asio::write(m_port,
                               boost::asio::buffer(writeBuf.contents(),
                                                   writeBuf.length()));
        if(written != writeBuf.length()) {
            return -1;
        }
    } catch(std::exception &e) {
        return -1;
    }
    return 0;
}

int ayisakov::framework::SerialPort::readAsync(IReadBufferPtr &pReadBuf,
                                               const ReadCallback &callback)
{
    IReadBuffer *pBuf = regBuffer(pReadBuf);
    if(!pBuf) {
        return -1;
    }
    m_port.async_read_some(
        boost::asio::buffer(pBuf->contents(), pBuf->length()),
        boost::bind(&SerialPort::onReadInternal, this, callback,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    return 0;
}

int ayif::SerialPort::readAsync(IReadBufferPtr &pReadBuf,
                                const std::string &regex,
                                const ReadCallback &callback)
{
    IReadBuffer *pBuf = regBuffer(pReadBuf);
    if(!pBuf) {
        return -1;
    }

    boost::asio::async_read_until(
        m_port, boost::asio::buffer(pBuf->contents(), pBuf->length()),
        boost::regex(regex),
        boost::bind(&SerialPort::onReadInternal, this, callback,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));

    return 0;
}

ayif::IReadBuffer *ayif::SerialPort::regBuffer(IReadBufferPtr &pReadBuf)
{
    if(!pReadBuf) {
        return nullptr; // empty container
    }

    // register this buffer, making sure tag is unique
    BufferTag tag = pReadBuf->tag();
    if(m_readBuffers.find(tag) != m_readBuffers.end()) {
        return nullptr; // tag not unique
    }
    // grab a plain pointer before moving
    IReadBuffer *pBuf = pReadBuf.get();
    m_readBuffers[tag] = std::move(pReadBuf);
    return pBuf; // success
}

void ayif::SerialPort::onReadInternal(ReadCallback callback,
                                      const boost::system::error_code &ec,
                                      std::size_t bytesRead)
{
    IReadBufferPtr &buf = m_readBuffers[tag];
    if(!buf) {
        return;
    }
    buf->bytesRead(bytesRead);
    buf->error(error.value());
    if(callback) {
        callback(buf);
    }
    m_readBuffers.erase(tag);
}

int ayisakov::framework::SerialPort::readSync(IReadBuffer &readBuf)
{
    try {
        boost::asio::read(m_port,
                          boost::asio::buffer(readBuf.contents(),
                                              readBuf.length()));
    } catch(std::exception &e) {
        return -1;
    }
    return 0;
}

void ayisakov::framework::SerialPort::reset() { m_port.close(); }

// void ayisakov::framework::SerialPort::onWriteSuccess(int bytesWritten)
//{
//    // TODO: implement (e.g. by posting a message to the registered sink)
//}
//
// void ayisakov::framework::SerialPort::onWriteFail(int errorCode)
//{
//    // TODO: implement
//}
//
// void ayisakov::framework::SerialPort::onReadSuccess(const char *buffer, size_t len)
//{
//    // TODO: implement
//}
//
// void ayisakov::framework::SerialPort::onReadFail(int errorCode)
//{
//    // TODO: implement
//}

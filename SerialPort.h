#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <unordered_map>
#include "ISerialPort.h"
#include "IReadBuffer.h"
#include "IWriteBuffer.h"

namespace ayisakov
{
namespace framework
{
class IIOProvider;

class SerialPort : public ISerialPort
{
  public:
    /**
     * NOTE: Boost has renamed io_service to io_context in
     * version >= 1.66 (io_service remains as a typedef)
     */
    SerialPort(boost::asio::io_service &context, IIOProvider *pProvider);
    virtual ~SerialPort();

    /**
     * Add a receiver to which events will be forwarded.
     */
    virtual void registerSink(IMessageSink *pSink) override;

    /**
     * Get the id of this port
     */
    virtual std::string id() override;

    /**
     * Get the native UUID of this port
     */
    virtual const boost::uuids::uuid &uuid();

    /**
     * Open the device
     *
     * @return 0 on success; error code on failure
     */
    virtual int open(const std::string &device) override;

    /**
     * Close the device
     *
     * @return 0 on success or if already closed
     */
    virtual int close() override;

    // indicate to the provider that the port is no longer being used
    virtual void release() override;

    /**
     * Begin an asynchronous write operation, transferring
     * ownership of the buffer to the port.
     * Upon completion of the write operation, the buffer
     * will be returned to the caller via the callback.
     */
    virtual int writeAsync(IWriteBufferPtr &pWriteBuf,
                           const WriteCallback &callback) override;

    /**
     * Begin an asynchronous read operation, transferring
     * ownership of the buffer to the port.
     * Upon completion of the read operation, the buffer
     * will be returned to the caller via the callback.
     */
    virtual int readAsync(IReadBufferPtr &pReadBuf,
                          const ReadCallback &callback) override;

    /**
     * Reset this port, i.e. disconnect and remove association
     * with any physical device.
     */
    virtual void reset() override;

    /**
     * Callbacks - these will generally be called by a different thread
     * than the one that called writeAsync() or readAsync().
     */

    //    virtual void onWriteSuccess(int bytesWritten) override;
    //    virtual void onWriteFail(int errorCode) override;
    //    virtual void onReadSuccess(const char *buffer, size_t len) override;
    //    virtual void onReadFail(int errorCode) override;

  private:
    IMessageSink *m_pSink;
    IIOProvider *m_pProvider;
    boost::asio::serial_port m_port;
    boost::uuids::uuid m_uuid;
    // Buffers with outstanding write operations
    std::unordered_map<BufferTag, IWriteBufferPtr> m_writeBuffers;
    // Buffers with outstanding read operations
    std::unordered_map<BufferTag, IReadBufferPtr> m_readBuffers;
};
} // namespace framework
} // namespace ayisakov

#endif // SERIALPORT_H

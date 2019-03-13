#ifndef ISERIALPORT_H
#define ISERIALPORT_H

#include <functional>
#include <string>
#include "IMessageSink.h"
#include "IReadBuffer.h"
#include "IWriteBuffer.h"

namespace ayisakov
{
namespace framework
{
using WriteCallback = std::function<void(IWriteBufferPtr &)>;
using ReadCallback = std::function<void(IReadBufferPtr &)>;

class ISerialPort
{
  public:
    ISerialPort() {}
    virtual ~ISerialPort() {}

    /**
     * Add a receiver to which events will be forwarded.
     */
    virtual void registerSink(IMessageSink *pSink) = 0;

    /**
     * Get the id of this port
     */
    virtual std::string id() = 0;

    /**
     * Open the device
     *
     * @return 0 on success; error code on failure
     */
    virtual int open(const std::string &device) = 0;

    /**
     * Close the device
     *
     * @return 0 on success or if already closed
     */
    virtual int close() = 0;

    // If this port is managed, indicate that it is no longer needed
    virtual void release() = 0;

    /**
     * Begin an asynchronous write operation, transferring
     * ownership of the buffer to the port.
     * Upon completion of the write operation, the buffer
     * will be returned to the caller via the callback.
     */
    virtual int writeAsync(IWriteBufferPtr &pWriteBuf,
                           const WriteCallback &callback) = 0;

    /**
     * Perform a blocking write operation
     *
     * @return 0 on success, nonzero error code on failure
     */
    virtual int writeSync(IWriteBuffer &writeBuf) = 0;

    /**
     * Begin an asynchronous read operation, transferring
     * ownership of the buffer to the port.
     * Upon completion of the read operation, the buffer
     * will be returned to the caller via the callback.
     */
    virtual int readAsync(IReadBufferPtr &pReadBuf,
                          const ReadCallback &callback) = 0;

    /**
     * Begin an asynchronous read operation that will be completed
     * when a substring of the data in the read buffer matches
     * the regular expression, transferring
     * ownership of the buffer to the port.
     * Upon completion of the read operation, the buffer
     * will be returned to the caller via the callback.
     */
    virtual int readAsync(IReadBufferPtr &pReadBuf,
                          const std::string &regex,
                          const ReadCallback &callback) = 0;

    /**
     * Perform a blocking read operation
     *
     * @return 0 on success, nonzero error code on failure
     */
    virtual int readSync(IReadBuffer &readBuf) = 0;

    /**
     * Reset this port, i.e. disconnect and remove association
     * with any physical device.
     */
    virtual void reset() = 0;

    /**
     * Callbacks - these will generally be called by a different thread
     * than the one that called writeAsync() or readAsync().
     */

    //    virtual void onWriteSuccess(int bytesWritten) = 0;
    //    virtual void onWriteFail(int errorCode) = 0;
    //    virtual void onReadSuccess(IReadBufferPtr &pReadbuf) =
    //    0; virtual void onReadFail(int errorCode) = 0;
};
} // namespace framework
} // namespace ayisakov

#endif // ISERIALPORT_H

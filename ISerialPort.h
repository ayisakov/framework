#ifndef ISERIALPORT_H
#define ISERIALPORT_H

#include <string>

namespace ayisakov
{
namespace framework
{
class IIOListener;
class ISerialPort
class IMessageReceiver;
{
  public:
    ISerialPort() {}
    virtual ~ISerialPort {}

    /**
     * Add a receiver to which events will be forwarded.
     */
    virtual void registerSink(IMessageReceiver *pSink) = 0;

    /**
     * Get the id of this port
     */
    const std::string &id() = 0;

    // If this port is managed, indicate that it is no longer needed
    virtual void release() = 0;

    // Begin an asynchronous write operation
    virtual int writeAsync(const std::string &buffer) = 0;

    // Begin an asynchronous read operation
    virtual int readAsync() = 0;

    /**
     * Reset this port, i.e. disconnect and remove association
     * with any physical device.
     */
    virtual void reset() = 0;

    /**
     * Callbacks - these will generally be called by a different thread
     * than the one that called writeAsync() or readAsync().
     */

    virtual void onWriteSuccess(int bytesWritten) = 0;
    virtual void onWriteFail(int errorCode) = 0;
    virtual void onReadSuccess(const char *buffer, size_t len) = 0;
    virtual void onReadFail(int errorCode) = 0;
};
} // namespace framework
} // namespace ayisakov

#endif // ISERIALPORT_H

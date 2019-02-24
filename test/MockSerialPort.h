#ifndef MOCKSERIALPORT_H
#define MOCKSERIALPORT_H

#include "../ISerialPort.h"

namespace ayisakov
{
namespace framework
{
class MockSerialPort : public ISerialPort
{
  public:
    MockSerialPort();
    virtual ~MockSerialPort();

    /**
     * Add a receiver to which events will be forwarded.
     */
    virtual void registerSink(IMessageSink *pSink) override;

    /**
     * Get the id of this port
     */
    virtual std::string id() override;

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

    // If this port is managed, indicate that it is no longer needed
    virtual void release() override;

    // Begin an asynchronous write operation
    virtual int writeAsync(const std::string &buffer) override;

    // Begin an asynchronous read operation
    virtual int readAsync() override;

    /**
     * Reset this port, i.e. disconnect and remove association
     * with any physical device.
     */
    virtual void reset() override;

    /**
     * Callbacks - these will generally be called by a different thread
     * than the one that called writeAsync() or readAsync().
     */

    virtual void onWriteSuccess(int bytesWritten) override;
    virtual void onWriteFail(int errorCode) override;
    virtual void onReadSuccess(const char *buffer, size_t len) override;
    virtual void onReadFail(int errorCode) override;

private:
    IMessageSink m_pSink;
};
} // namespace framework
} // namespace ayisakov

#endif // MOCKSERIALPORT_H

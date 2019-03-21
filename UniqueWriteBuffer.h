#ifndef UNIQUEWRITEBUFFER_H
#define UNIQUEWRITEBUFFER_H

#include <memory>
#include <utility>
#include <string>
#include "BufferProxy.h"
#include "IWriteBuffer.h"

namespace ayisakov
{
namespace framework
{
class UniqueWriteBuffer : public IWriteBuffer
{
  public:
    // construct using a null-terminated string
    UniqueWriteBuffer(const char *data);
    // construct using std::string
    UniqueWriteBuffer(const std::string &data);
    UniqueWriteBuffer(const UniqueWriteBuffer &original) = delete;
    UniqueWriteBuffer operator=(const UniqueWriteBuffer &orig) = delete;
    ~UniqueWriteBuffer();

    static IWriteBufferPtr create(const std::string &data);

    // total number of bytes
    std::size_t length() const override;
    // a unique tag
    BufferTag tag() const override;

    // get a pointer to the beginning of the buffer
    const std::uint8_t *contents() const override;
    // get a reference to the beginning of the unwritten portion
    BufferProxy unwrittenContents() override;
    // set number of bytes written from this buffer
    // throws std::out_of_range if written > length()
    void bytesWritten(std::size_t written) override;
    // get number of bytes written from this buffer
    std::size_t bytesWritten() override;
    // error code (e.g. from asynchronous buffer operation) (==0 if no error)
    virtual BufferErrorCode error() const override;
    virtual void error(BufferErrorCode code) override;
	// get buffer contents as string
    virtual std::string str() const override;

  private:
    std::string m_data;
    std::size_t m_bytesWritten;
    BufferErrorCode m_errorCode;
};
using UniqueWriteBufferPtr = std::unique_ptr<UniqueWriteBuffer>;

} // namespace framework
} // namespace ayisakov

#endif // UNIQUEWRITEBUFFER_H

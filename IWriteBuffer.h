#ifndef IWRITEBUFFER_H
#define IWRITEBUFFER_H

#include <memory>
#include "BufferProxy.h"
#include "IBuffer.h"

namespace ayisakov
{
namespace framework
{
class IWriteBuffer : public IBuffer
{
  public:
    IWriteBuffer() {}
    virtual ~IWriteBuffer() {}
    // get a pointer to the beginning of the buffer
    virtual const uint8_t *contents() = 0;
    // get a reference to the beginning of the unwritten portion
    virtual BufferProxy unwrittenContents() = 0;
    // set number of bytes written from this buffer
    virtual void bytesWritten(std::size_t written) = 0;
    // get number of bytes written from this buffer
    virtual std::size_t bytesWritten() = 0;
};
using IWriteBufferPtr = std::unique_ptr<IWriteBuffer>;
} // namespace framework
} // namespace ayisakov

#endif // IWRITEBUFFER_H

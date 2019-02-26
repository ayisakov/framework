#ifndef AYIF_BUFFER_H
#define AYIF_BUFFER_H

namespace ayisakov
{
namespace framework
{
using BufferTag = std::uintptr_t;
using BufferErrorCode = int;
class IBuffer
{
  public:
    IBuffer() {}
    virtual ~IBuffer() {}
    // total number of bytes
    virtual std::size_t length() = 0;
    // a unique tag
    virtual BufferTag tag() = 0;
    // error code (e.g. from asynchronous buffer operation) (==0 if no error)
    virtual BufferErrorCode error() = 0;
    virtual void error(BufferErrorCode code) = 0;
};
} // namespace framework
} // namespace ayisakov

#endif // AYIF_BUFFER_H

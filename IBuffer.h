#ifndef AYIF_BUFFER_H
#define AYIF_BUFFER_H

namespace ayisakov
{
namespace framework
{
using BufferTag = std::uintptr_t;
class IBuffer
{
    IBuffer() {}
    virtual ~IBuffer() {}
    // the data
    virtual const char *contents() = 0;
    // total number of bytes
    virtual std::size_t length() = 0;
    // a unique tag
    virtual BufferTag tag() = 0;
};
} // namespace framework
} // namespace ayisakov

#endif // AYIF_BUFFER_H

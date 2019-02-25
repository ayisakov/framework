#ifndef IREADBUFFER_H
#define IREADBUFFER_H

#include "IBuffer.h"

namespace ayisakov
{
namespace framework
{
/**
 * A modifiable buffer into which data can be read
 */
class IReadBuffer : public IBuffer
{
  public:
    IReadBuffer() {}
    virtual ~IReadBuffer() {}
    // get a pointer to the beginning of the buffer
    uint8_t *contents() = 0;
    // set number of bytes read into this buffer
    virtual void bytesRead(std::size_t read) = 0;
    // get number of bytes read into this buffer
    virtual std::size_t bytesRead() = 0;
};
} // namespace framework
} // namespace ayisakov

#endif // IREADBUFFER_H

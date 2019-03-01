#ifndef BUFFERPROXY_H
#define BUFFERPROXY_H

#include "IBuffer.h"

namespace ayisakov
{
namespace framework
{
/**
 * A buffer proxy does not own a buffer. It merely
 * serves to indicate the beginning and length of
 * a part of an existing buffer.
 * The user must guarantee the persistence of
 * the referenced buffer throughout the lifetime of
 * the proxy.
 */
class BufferProxy : public IBuffer
{
  public:
    BufferProxy(const uint8_t *begin, std::size_t length)
    : m_pBuffer(begin), m_length(length)
    {
    }
    ~BufferProxy() {}
    const uint8_t *contents() { return m_pBuffer; }
    std::size_t length() { return m_length; }
    BufferTag tag() { return reinterpret_cast<BufferTag>(this); }
    // error code (e.g. from asynchronous buffer operation) (==0 if no error)
    virtual BufferErrorCode error() override
    {
        return BufferErrorCode(0);
    }
    virtual void error(BufferErrorCode code) override {}

  private:
    const uint8_t *m_pBuffer;
    std::size_t m_length;
};
} // namespace framework
} // namespace ayisakov

#endif // BUFFERPROXY_H

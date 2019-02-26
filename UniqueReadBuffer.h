#ifndef UNIQUEREADBUFFER_H
#define UNIQUEREADBUFFER_H

#include <memory>
#include <vector>
#include "BufferProxy.h"
#include "IReadBuffer.h"

namespace ayisakov
{
namespace framework
{
class UniqueReadBuffer : public IReadBuffer
{
  public:
    UniqueReadBuffer(std::size_t length);
    UniqueReadBuffer(const UniqueReadBuffer &original) = delete;
    UniqueReadBuffer operator=(const UniqueReadBuffer &orig) = delete;
    ~UniqueReadBuffer();

    // total number of bytes
    std::size_t length() override;
    // a unique tag
    BufferTag tag() override;

    // get a pointer to the beginning of the buffer
    uint8_t *contents() override;
    // set number of bytes read into this buffer
    // throws std::out_of_range if read > length()
    void bytesRead(std::size_t read) override;
    // get number of bytes read into this buffer
    std::size_t bytesRead() override;
    // error code (e.g. from asynchronous buffer operation) (==0 if no error)
    virtual BufferErrorCode error() override;
    virtual void error(BufferErrorCode code) override;

  private:
    std::vector<std::uint8_t> m_data;
    std::size_t m_bytesRead;
    BufferErrorCode m_errorCode;
};

using UniqueReadBufferPtr = std::unique_ptr<UniqueReadBuffer>;
} // namespace framework
} // namespace ayisakov

#endif // UNIQUEREADBUFFER_H

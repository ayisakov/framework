#ifndef UNIQUEREADBUFFER_H
#define UNIQUEREADBUFFER_H

#include <memory>
#include "BufferCommon.h"
#include "IReadable.h"

namespace ayisakov
{
namespace framework
{
class UniqueReadBuffer : public BufferCommon, public IReadable
{
  public:
    UniqueReadBuffer(const char *data, size_t length);
    UniqueReadBuffer(const UniqueReadBuffer &original) = delete;
    UniqueReadBuffer operator=(const UniqueReadBuffer &orig) = delete;
    virtual ~UniqueReadBuffer();

    std::size_t read() override;

  private:
};

using UniqueReadBufferPtr = std::unique_ptr<UniqueReadBuffer>;
} // namespace framework
} // namespace ayisakov

#endif // UNIQUEREADBUFFER_H

#ifndef UNIQUEWRITEBUFFER_H
#define UNIQUEWRITEBUFFER_H

#include <memory>
#include "BufferCommon.h"
#include "IWritable.h"

namespace ayisakov
{
namespace framework
{
class UniqueWriteBuffer : public BufferCommon, public IWritable
{
  public:
    UniqueWriteBuffer(const char *data, size_t length);
    UniqueWriteBuffer(const UniqueWriteBuffer &original) = delete;
    UniqueWriteBuffer operator=(const UniqueWriteBuffer &orig) = delete;
    virtual ~UniqueWriteBuffer();

    std::size_t written() override;

  private:
};

using UniqueWriteBufferPtr = std::unique_ptr<UniqueWriteBuffer>;
} // namespace framework
} // namespace ayisakov

#endif // UNIQUEWRITEBUFFER_H

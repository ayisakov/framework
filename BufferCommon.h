#ifndef AYI_BUFFERCOMMON_H
#define AYI_BUFFERCOMMON_H

#include <vector>
#include "IBuffer.h"

namespace ayisakov
{
namespace framework
{
class BufferCommon : public IBuffer
{
  public:
    BufferCommon();
    virtual ~BufferCommon();

    const char *contents() override;
    std::size_t length() override;
    BufferTag tag() override;

  private:
    std::vector m_data;
};
} // namespace framework
} // namespace ayisakov

#endif // AYI_BUFFERCOMMON_H

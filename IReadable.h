#ifndef IREADABLE_H
#define IREADABLE_H

#include "IBuffer.h"

namespace ayisakov
{
namespace framework
{
class IReadBufer : public IBuffer
{
public:
    IReadable() {}
    virtual ~IReadable() {}
    // number of bytes read
    virtual std::size_t read() = 0;
};
} // namespace framework
} // namespace ayisakov

#endif // IREADABLE_H

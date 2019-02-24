#ifndef IWRITABLE_H
#define IWRITABLE_H

namespace ayisakov
{
namespace framework
{
class IWritable
{
  public:
    IWritable() {}
    virtual ~IWritable() {}
    // number of bytes written
    virtual std::size_t written() = 0;
};
} // namespace framework
} // namespace ayisakov

#endif // IWRITABLE_H

#ifndef IAPPMESSAGE_H
#define IAPPMESSAGE_H

#include "../IMessage.h"

namespace ayisakov
{
namespace framework
{
class ITestApp;
class IAppMessage : public IMessage
{
  public:
    IAppMessage() {}
    virtual ~IAppMessage() {}
    virtual void examine(ITestApp &reader) const = 0;
    virtual std::string toString() const override { return std::string(); }
};
} // namespace framework
} // namespace ayisakov
#endif // IAPPMESSAGE_H

#ifndef TESTMESSAGES_H
#define TESTMESSAGES_H

#include "IAppMessage.h"

namespace ayisakov
{
namespace framework
{
class MsgSample : public IAppMessage
{
  public:
    MsgSample() {}
    virtual ~MsgSample() {}
    virtual void examine(ITestApp &reader) const override;
    virtual std::string toString() const override
    {
        return "Sample message";
    }
};

class MsgQuit : public IAppMessage
{
  public:
    MsgQuit() {}
    virtual ~MsgQuit() {}
    virtual void examine(ITestApp &reader) const override;
    virtual std::string toString() const override
    {
        return "Quit message";
    }
};
} // namespace framework
} // namespace ayisakov


#endif // TESTMESSAGES_H

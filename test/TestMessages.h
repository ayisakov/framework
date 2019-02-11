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
};

class MsgQuit : public IAppMessage
{
  public:
    MsgQuit() {}
    virtual ~MsgQuit() {}
    virtual void examine(ITestApp &reader) const override;
};
} // namespace framework
} // namespace ayisakov


#endif // TESTMESSAGES_H

#ifndef ITESTAPP_H
#define ITESTAPP_H

#include <memory>
#include "IAppMessage.h"
#include "../SimpleEventApp.h"

namespace ayisakov
{
namespace framework
{
class MsgSample;
class MsgQuit;
typedef std::unique_ptr<ayisakov::framework::IAppMessage> TestMsgBase;

class ITestApp : public SimpleEventApp<TestMsgBase>
{
  public:
    ITestApp() {}
    virtual ~ITestApp() override {}

    // Message handler prototypes
    virtual void onMsg(const MsgSample &msg) = 0;
    virtual void onMsg(const MsgQuit &msg) = 0;

protected:
    virtual void process(const TestMsgBase &event) override
    {
        if(event) {
            event->examine(*this);
        }
    }
};

} // namespace framework
} // namespace ayisakov

#endif // ITESTAPP_H

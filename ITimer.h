#ifndef AYI_ITIMER_H
#define AYI_ITIMER_H

#include <functional>

namespace ayisakov
{
namespace framework
{
// using TimerRetCode = int;
// class ITimerHandler
//{
//  public:
//    ITimerHandler() {}
//    virtual ~ITimerHandler() {}
//    virtual void operator()(TimerRetCode code) = 0;
//};
using TimerRetCode = int;
using TimerHandler = std::function<void(TimerRetCode)>;
class ITimer
{
  public:
    ITimer() {}
    virtual ~ITimer() {}
    virtual void cancel() = 0;
    virtual void start() = 0;
    //    virtual void start(ITimerHandler *handler) = 0;
};
} // namespace framework
} // namespace ayisakov

#endif // AYI_ITIMER_H

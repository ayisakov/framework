#ifndef AYI_RELTIMERMS_H
#define AYI_RELTIMERMS_H

#include <boost/asio.hpp>
#include <functional>
#include "ITimer.h"

namespace ayisakov
{
namespace framework
{
class RelTimerMs : public ITimer
{
  public:
    RelTimerMs(boost::asio::io_service &ios, unsigned msec, const TimerHandler &handler);
    virtual ~RelTimerMs();
    virtual void cancel() override;
    virtual void start() override;
    virtual bool fail(TimerRetCode code) const override;

  protected:
    void handler(const boost::system::error_code &ec);

  private:
    TimerHandler m_userHandler;
    boost::asio::deadline_timer m_timer;
    unsigned m_waitMsec;
};
} // namespace framework
} // namespace ayisakov

#endif // AYI_RELTIMERMS_H

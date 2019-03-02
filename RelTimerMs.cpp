#include "RelTimerMs.h"
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>

namespace ayif = ayisakov::framework;


ayisakov::framework::RelTimerMs::RelTimerMs(boost::asio::io_service &ios,
                                            unsigned int msec,
                                            const TimerHandler &userHandler)
: m_timer(ios), m_userHandler(userHandler), m_waitMsec(msec)
{
}

ayisakov::framework::RelTimerMs::~RelTimerMs() {}

void ayisakov::framework::RelTimerMs::start()
{
    m_timer.expires_from_now(boost::posix_time::milliseconds(m_waitMsec));
    m_timer.async_wait(boost::bind(&ayisakov::framework::RelTimerMs::handler,
                                   this, boost::asio::placeholders::error));
}

void ayisakov::framework::RelTimerMs::cancel()
{
    m_timer.cancel();
}

void ayisakov::framework::RelTimerMs::handler(const boost::system::error_code &ec)
{
    if(ec != boost::asio::error::operation_aborted) {  // a canceled timer is fine
        throw std::runtime_error(ec.message());
    }
    if(m_userHandler) {
        m_userHandler(ec.value());
    }
}

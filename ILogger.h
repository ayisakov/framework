#ifndef ILOGGER_H
#define ILOGGER_H

#include <string>

/**
 * Author: Artem Isakov
 * The logger interface
 */
namespace ayisakov
{
namespace framework
{
class IMessage;
class ILogger
{
  public:
    ILogger() {}
    virtual ~ILogger() {}
    /**
     * Log this message
     *
     * @return 0 on success, nonzero value on failure
     */
    virtual int log(IMessage &msg) = 0;
    /**
     * Log this string
     *
     * @return 0 on success, nonzero value on failure
     */
    virtual int log(const std::string &str) = 0;
    /**
     * Returns true if the logger is ready to accept input
     */
    virtual bool isReady() = 0;
    /**
     * Syntactic sugar
     */
    //    virtual void operator<<(const std::string &message) = 0;
    //    virtual void operator<<(const char *message) = 0;
    //    virtual void operator<<(int integer) = 0;
    //    virtual void operator<<(double floating) = 0;
    //    virtual void operator<<(const char character) = 0;
};
} // namespace framework
} // namespace ayisakov
#endif // ILOGGER_H

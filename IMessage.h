#ifndef IMESSAGE_H
#define IMESSAGE_H

/*
 * File: Message.h
 * Author: Artem Isakov
 *
 * The IMessage interface. All message classes should inherit it.
 */

#include <string>

namespace ayisakov
{
namespace framework
{
    //typename IMessageSink;
class IMessage
{
  public:
    IMessage() {}
    virtual std::string toString() const = 0;
    //    virtual IMessageSink *recipient() const = 0;
    virtual ~IMessage() {}
};

} // namespace framework
} // namespace ayisakov

#endif // IMESSAGE_H

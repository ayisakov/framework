#ifndef IMESSAGE_H
#define IMESSAGE_H

/*
 * File: Message.h
 * Author: Artem Isakov
 *
 * The IMessage interface. All message classes should inherit it.
 */

#include <string>

namespace neu
{
namespace lustiglab
{
namespace framework
{
class IMessage
{
  public:
    IMessage() {}
    virtual std::string toString() const = 0;
    virtual ~IMessage(){};
};

} // namespace framework
} // namespace lustiglab
} // namespace neu

#endif // IMESSAGE_H

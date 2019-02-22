#ifndef IMESSAGEPTR_H
#define IMESSAGEPTR_H

#include <memory>
#include "IMessage.h"

namespace ayisakov
{
namespace framework
{
    typedef std::unique_ptr<ayisakov::framework::IMessage> IMessagePtr;
}
} // namespace ayisakov

#endif // IMESSAGEPTR_H

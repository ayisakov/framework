#ifndef IMESSAGESINK_H
#define IMESSAGESINK_H

#include "IMessagePtr.h"
#include "IMessageReceiver.h"

namespace ayisakov
{
namespace framework
{
typedef IMessageReceiver<IMessagePtr> IMessageSink;
}
} // namespace ayisakov

#endif // IMESSAGESINK_H

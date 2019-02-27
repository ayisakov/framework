#ifndef IMESSAGEPTR_H
#define IMESSAGEPTR_H

#include <memory>
#include "IMessage.h"

namespace ayisakov
{
namespace framework
{
using IMessagePtr = std::unique_ptr<IMessage>;
}
} // namespace ayisakov

#endif // IMESSAGEPTR_H

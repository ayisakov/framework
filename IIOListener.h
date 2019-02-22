#ifndef IIOLISTENER_H
#define IIOLISTENER_H

#include "IMessageSink.h"
#include "IRunnable.h"

namespace ayisakov
{
namespace framework
{
class IIOProvider;
/**
 * IIOListener defines an interface for a class of
 * objects that dispatch events for serial ports.
 */
class IIOListener
: public ayisakov::framework::IRunnable,
  public ayisakov::framework::IMessageSink
{
  public:
    IIOListener() {}
    virtual ~IIOListener() {}

    /**
     * Set a serial port provider, so that this listener can
     * dispatch events for its ports.
     * Only one provider can be set at a time. Attempting to set
     * more than one provider will return an error code.
     *
     * @return 0 on success
     */
    virtual int subscribe(IIOProvider *pProvider) = 0;

    /**
     * Returns true if the listener is running (usually in its own thread)
     */
    virtual bool isRunning() = 0;
};
} // namespace framework
} // namespace ayisakov

#endif // IIOLISTENER_H

#ifndef SIMPLEEVENTAPP_H
#define SIMPLEEVENTAPP_H

#include "ConcurrentQueue.h"
#include "IMessageReceiver.h"
#include "IRunnable.h"

namespace ayisakov
{
namespace framework
{
template <typename TEvent>
class SimpleEventApp : public IRunnable, public IMessageReceiver
{
  public:
    SimpleEventApp();
    virtual ~SimpleEventApp();
    virtual int run();

  protected:
    virtual void process(const TEvent &event);

  private:
    ConcurrentQueue<TEvent> m_queue;
};
} // namespace framework
} // namespace ayisakov

#endif // SIMPLEEVENTAPP_H

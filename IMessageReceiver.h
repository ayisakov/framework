#ifndef IMESSAGE_RECEIVER_H
#define IMESSAGE_RECEIVER_H


namespace neu
{
namespace lustiglab
{
namespace framework
{
/**
 * The message receiver interface.
 * All classes capable of receiving messages should inherit it.
 */
template <typename T> class IMessageReceiver
{
  public:
    IMessageReceiver() {}
    virtual ~IMessageReceiver() {}
    /**
     * Send a message to this receiver
     * @param [in] message the message
     * @return 0 on success
     */
    virtual int send(const T &message) = 0;
};
} // namespace framework
} // namespace lustiglab
} // namespace neu

#endif // IMESSAGE_RECEIVER_H

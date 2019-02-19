#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <boost/asio/serial_port.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include "ISerialPort.h"

namespace ayisakov
{
namespace framework
{
class IIOProvider;

class SerialPort : public ISerialPort
{
  public:
    SerialPort(IIOProvider *pProvider);
    virtual ~SerialPort();

    /**
     * Get the id of this port
     */
    const std::string &id() override;

    /**
     * Get the native UUID of this port
     */
    const boost::uuids::uuid &uuid();

    // indicate to the provider that the port is no longer being used
    virtual void release() override;

    /**
     * Reset this port, i.e. disconnect and remove association
     * with any physical device.
     */
    virtual void reset() override;

  private:
    IIOProvider *m_pProvider;
    boost::uuids::uuid m_uuid;
};
} // namespace framework
} // namespace ayisakov

#endif // SERIALPORT_H

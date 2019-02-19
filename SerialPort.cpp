#include "SerialPort.h"
#include "IIOProvider.h"

ayisakov::framework::SerialPort::SerialPort(IIOProvider* pProvider)
    : m_pProvider(pProvider)
{
}

ayisakov::framework::SerialPort::~SerialPort() {
    // TODO: see if any clean-up is needed that is not
    // performed by the underlying object's destructor
}

void ayisakov::framework::SerialPort::release() {
    if (m_pProvider) {
        m_pProvider->release(boost::uuids::to_string(m_uuid));
    }
}

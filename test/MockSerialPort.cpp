#include "MockSerialPort.h"

namespace ayif = ayisakov::framework;

ayif::MockSerialPort::MockSerialPort() : m_pSink(nullptr) {}

ayif::MockSerialPort::~MockSerialPort() {}

ayif::MockSerialPort::onWriteSuccess() {}

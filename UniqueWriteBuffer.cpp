#include "UniqueWriteBuffer.h"
#include <exception>

namespace ayif = ayisakov::framework;

ayisakov::framework::UniqueWriteBuffer::UniqueWriteBuffer(const char *data)
: m_data(data), m_bytesWritten(0), m_errorCode(0)
{
}

ayisakov::framework::UniqueWriteBuffer::UniqueWriteBuffer(const std::string &data)
: m_data(data), m_bytesWritten(0), m_errorCode(0)
{
}

ayisakov::framework::UniqueWriteBuffer::~UniqueWriteBuffer() {}

std::size_t ayif::UniqueWriteBuffer::length()
{
    return m_data.length();
}

ayif::BufferTag ayif::UniqueWriteBuffer::tag()
{
    // the address is unique enough for current purposes
    return reinterpret_cast<BufferTag>(this);
}

const std::uint8_t *ayif::UniqueWriteBuffer::contents()
{
    return reinterpret_cast<const std::uint8_t *>(m_data.c_str());
}

ayif::BufferProxy ayif::UniqueWriteBuffer::unwrittenContents()
{
    return BufferProxy(&(contents()[bytesWritten()]), length() - bytesWritten());
}

void ayif::UniqueWriteBuffer::bytesWritten(std::size_t written)
{
    if(written > length()) {
        throw std::out_of_range(
            "Bytes written is greater than buffer length");
    }
    m_bytesWritten = written;
}

std::size_t ayif::UniqueWriteBuffer::bytesWritten()
{
    return m_bytesWritten;
}

void ayif::UniqueWriteBuffer::error(BufferErrorCode code) { m_errorCode = code; }

ayif::BufferErrorCode ayif::UniqueWriteBuffer::error() { return m_errorCode; }

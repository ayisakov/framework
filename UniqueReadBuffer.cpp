#include "UniqueReadBuffer.h"
#include <exception>

namespace ayif = ayisakov::framework;

ayif::UniqueReadBuffer::UniqueReadBuffer(std::size_t length)
: m_data(length), m_bytesRead(0), m_errorCode(0)
{
}

ayif::UniqueReadBuffer::~UniqueReadBuffer() {}

std::size_t ayif::UniqueReadBuffer::length() { return m_data.size(); }

ayif::BufferTag ayif::UniqueReadBuffer::tag()
{
    // the address is unique enough for current purposes
    return reinterpret_cast<BufferTag>(this);
}

uint8_t *ayif::UniqueReadBuffer::contents()
{
    return m_data.data();
}

void ayif::UniqueReadBuffer::bytesRead(std::size_t read)
{
    if(read > length()) {
        throw std::out_of_range(
            "Bytes read is greater than buffer size.");
    }
    m_bytesRead = read;
}

std::size_t ayif::UniqueReadBuffer::bytesRead()
{
    return m_bytesRead;
}

void ayif::UniqueReadBuffer::error(BufferErrorCode code)
{
    m_errorCode = code;
}

ayif::BufferErrorCode ayif::UniqueReadBuffer::error()
{
    return m_errorCode;
}

#include "xapihexbuffer.h"

xApiHexBuffer::xApiHexBuffer(char *lpdata, int szdata)
    :m_lpdata(lpdata),
      m_szdata(szdata)
{
}

NByteStream::size_type xApiHexBuffer::size() const
{
    return m_szdata;
}

NByteStream::size_type xApiHexBuffer::max_size() const
{
    return m_szdata;
}

bool xApiHexBuffer::empty() const
{
    return true;
}

NByteStream::value_type xApiHexBuffer::read(NByteStream::size_type i) const
{
    return m_lpdata[i];
}

void xApiHexBuffer::write(NByteStream::size_type i, const NByteStream::value_type &v)
{

}

void xApiHexBuffer::clear()
{

}

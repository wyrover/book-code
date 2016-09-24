#ifndef XAPIHEXBUFFER_H
#define XAPIHEXBUFFER_H
#include "hexview/NByteStream.h"

class xApiHexBuffer : public NByteStream
{
public:
    xApiHexBuffer(char* lpdata,int szdata);

public:	// container information
    virtual size_type size() const;
    virtual size_type max_size() const;
    virtual bool empty() const;

protected:
    virtual value_type read(size_type i) const;
    virtual void write(size_type i, const value_type &v);

public:
    virtual void clear();
    char* m_lpdata;
    int   m_szdata;
};

#endif // XAPIHEXBUFFER_H

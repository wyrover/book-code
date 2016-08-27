/*!
* \file cactus_databuffer.cpp
* \brief 数据缓冲区实现 
* 
* 数据缓冲区实现
* 
* \author wangyang 
* \date 2015/01/30 
* \version 1.0
*/

#include "cactus/cactus_databuffer.h"


namespace cactus
{

    class DataBuffer_Impl
    {
    public:
        DataBuffer_Impl() : data(0), size(0), allocated_size(0)
        {
        }

        ~DataBuffer_Impl()
        {
            delete[] data;
        }

    public:
        char *data;
        unsigned int size;
        unsigned int allocated_size;
    };



    DataBuffer::DataBuffer()
        : impl(new DataBuffer_Impl())
    {
    }

    DataBuffer::DataBuffer(unsigned int new_size)
        : impl(new DataBuffer_Impl())
    {
        set_size(new_size);
    }

    DataBuffer::DataBuffer(const void *new_data, unsigned int new_size)
        : impl(new DataBuffer_Impl())
    {
        set_size(new_size);
        memcpy(impl->data, new_data, new_size);
    }

    DataBuffer::DataBuffer(const DataBuffer &new_data, unsigned int pos, unsigned int size)
        : impl(new DataBuffer_Impl())
    {
        set_size(size);
        memcpy(impl->data, new_data.get_data() + pos, size);
    }

    DataBuffer::~DataBuffer()
    {
    }

    /////////////////////////////////////////////////////////////////////////////
    // DataBuffer Attributes:

    char *DataBuffer::get_data()
    {
        return impl->data;
    }

    const char *DataBuffer::get_data() const
    {
        return impl->data;
    }

    unsigned int DataBuffer::get_size() const
    {
        return impl->size;
    }

    unsigned int DataBuffer::get_capacity() const
    {
        return impl->allocated_size;
    }

    char &DataBuffer::operator[](int i)
    {
        return impl->data[i];
    }

    const char &DataBuffer::operator[](int i) const
    {
        return impl->data[i];
    }

    char &DataBuffer::operator[](unsigned int i)
    {
        return impl->data[i];
    }

    const char &DataBuffer::operator[](unsigned int i) const
    {
        return impl->data[i];
    }

    /////////////////////////////////////////////////////////////////////////////
    // DataBuffer Operations:

    DataBuffer &DataBuffer::operator =(const DataBuffer &copy)
    {
        impl = copy.impl;
        return *this;
    }

    void DataBuffer::set_size(unsigned int new_size)
    {
        if (new_size > impl->allocated_size)
        {
            char *old_data = impl->data;
            impl->data = new char[new_size];
            memcpy(impl->data, old_data, impl->size);
            delete[] old_data;
            memset(impl->data+impl->size, 0, new_size-impl->size);
            impl->size = new_size;
            impl->allocated_size = new_size;
        }
        else
        {
            impl->size = new_size;
        }
    }

    void DataBuffer::set_capacity(unsigned int new_capacity)
    {
        if (new_capacity > impl->allocated_size)
        {
            char *old_data = impl->data;
            impl->data = new char[new_capacity];
            memcpy(impl->data, old_data, impl->size);
            delete[] old_data;
            memset(impl->data+impl->size, 0, new_capacity-impl->size);
            impl->allocated_size = new_capacity;
        }
    }

    bool DataBuffer::is_null() const
    {
        return impl->size == 0;
    }
}



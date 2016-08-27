/*!
* \file cactus_databuffer.h
* \brief 数据缓冲区声明 
* 
* 数据缓冲区声明
* 
* \author wangyang 
* \date 2015/01/30 
* \version 1.0
*/

#ifndef __CACTUS_CACTUS_DATABUFFER_H__
#define __CACTUS_CACTUS_DATABUFFER_H__

#include <cactus/cactus.h>
#include <boost/smart_ptr.hpp>

namespace cactus
{
    class DataBuffer_Impl;

    /// \brief General purpose data buffer.
    class CACTUS_API DataBuffer
    {

    public:
        /// \brief Constructs a data buffer of 0 size.
        DataBuffer();
        DataBuffer(unsigned int size);
        DataBuffer(const void *data, unsigned int size);
        DataBuffer(const DataBuffer &data, unsigned int pos, unsigned int size);
        ~DataBuffer();

    public:
        /// \brief Returns a pointer to the data.
        char *get_data();

        const char *get_data() const;

        template<typename Type>
        Type *get_data() { return reinterpret_cast<Type*>(get_data()); }

        template<typename Type>
        const Type *get_data() const { return reinterpret_cast<const Type*>(get_data()); }

        /// \brief Returns the size of the data.
        unsigned int get_size() const;

        /// \brief Returns the capacity of the data buffer object.
        unsigned int get_capacity() const;

        /// \brief Returns a char in the buffer.
        char &operator[](int i);
        const char &operator[](int i) const;
        char &operator[](unsigned int i);
        const char &operator[](unsigned int i) const;

        /// \brief Returns true if the buffer is 0 in size.
        bool is_null() const;

    public:
        DataBuffer &operator =(const DataBuffer &copy);

        /// \brief Resize the buffer.
        void set_size(unsigned int size);

        /// \brief Preallocate enough memory.
        void set_capacity(unsigned int capacity);

    private:
        boost::shared_ptr<DataBuffer_Impl> impl;

    };

}


#endif // __CACTUS_CACTUS_DATABUFFER_H__

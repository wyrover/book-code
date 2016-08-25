#include <iostream>
#include <string>

const static int DEFAULT_BUF_SIZE = 3;
const static int MAX_SIZE         = 4096;

class Message
{

public:
    Message(int bufSize = DEFAULT_BUF_SIZE) :
        bufSize_(bufSize),
        initBufSize_(bufSize),
        msgSize_(0),
        key_("")
    {
        buf_ = new char[bufSize]; // Note: now this is in the body
    }

    ~Message()
    {
        delete[] buf_;
    }

    // Exception-safe copy ctor
    Message(const Message& orig) :
        bufSize_(orig.bufSize_),
        initBufSize_(orig.initBufSize_),
        msgSize_(orig.msgSize_),
        key_(orig.key_)   // This can throw...
    {
        buf_ = new char[orig.bufSize_];  // ...so can this
        copy(orig.buf_, orig.buf_ + msgSize_, buf_); // This can't
    }

    // Exception-safe assignment, using the copy ctor
    Message& operator=(const Message& rhs)
    {
        Message tmp(rhs);   // Copy construct a temporary
        swapInternals(tmp); // Swap members with it
        return (*this);     // When we leave, tmp is destroyed, taking
        // the original data with it
    }

    const char* data()
    {
        return (buf_);
    }

private:
    void swapInternals(Message& msg)
    {
        // Since key_ is not a built-in data type it can throw,
        // so do it first.
        swap(key_, msg.key_);
        // If it hasn't thrown, then do all the primitives
        swap(bufSize_,     msg.bufSize_);
        swap(initBufSize_, msg.initBufSize_);
        swap(msgSize_,     msg.msgSize_);
        swap(buf_,         msg.buf_);
    }
    int bufSize_;
    int initBufSize_;
    int msgSize_;
    char* buf_;
    string key_;
};
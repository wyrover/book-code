template<typename T>
class MessageGeneric
{

public:
    MessageGeneric(int bufSize = DEFAULT_BUF_SIZE) :
        bufSize_(bufSize),
        initBufSize_(bufSize),
        msgSize_(0),
        buf_(new T[bufSize]) {}

    ~MessageGeneric()
    {
        delete[] buf_;
    }

    void appendData(int len, const T* data)
    {
        if (msgSize_ + len > MAX_SIZE) {
            throw out_of_range("Data size exceeds maximum size.");
        }

        if (msgSize_ + len > bufSize_) {
            int newBufSize = bufSize_;

            while ((newBufSize *= 2) < msgSize_ + len);

            T* p = new T[newBufSize];
            copy(buf_, buf_ + msgSize_, p);   // Can these throw?
            copy(data, data + len, p + msgSize_);
            msgSize_ += len;
            bufSize_ = newBufSize;
            delete[] buf_;  // Get rid of old buffer and point to new
            buf_ = p;
        } else {
            copy(data, data + len, buf_ + msgSize_);
            msgSize_ += len;
        }
    }

    // Copy the data out to the caller's buffer
    int getData(int maxLen, T* data)
    {
        if (maxLen < msgSize_) {
            throw out_of_range("This data is too big for your buffer.");
        }

        copy(buf_, buf_ + msgSize_, data);
        return (msgSize_);
    }

private:
    MessageGeneric(const MessageGeneric& orig) {}
    MessageGeneric& operator=(const MessageGeneric& rhs) {}
    int bufSize_;
    int initBufSize_;
    int msgSize_;
    T* buf_;
};
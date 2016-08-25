class Message
{

public:
    Message(int bufSize = DEFAULT_BUF_SIZE) :
        bufSize_(bufSize),
        initBufSize_(bufSize),
        msgSize_(0),
        buf_(NULL)
    {
        buf_ = new char[bufSize];
    }

    ~Message()
    {
        delete[] buf_;
    }

    // Append character data
    void appendData(int len, const char* data)
    {
        if (msgSize_ + len > MAX_SIZE) {
            throw out_of_range("Data size exceeds maximum size.");
        }

        if (msgSize_ + len > bufSize_) {
            int newBufSize = bufSize_;

            while ((newBufSize *= 2) < msgSize_ + len);

            char* p = new char[newBufSize];    // Allocate memory
            // for new buffer
            copy(buf_, buf_ + msgSize_, p);    // Copy old data
            copy(data, data + len, p + msgSize_); // Copy new data
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
    int getData(int maxLen, char* data)
    {
        if (maxLen < msgSize_) {
            throw out_of_range("This data is too big for your buffer.");
        }

        copy(buf_, buf_ + msgSize_, data);
        return (msgSize_);
    }

private:
    Message(const Message& orig) {}           // We will come to these
    Message& operator=(const Message& rhs) {} // in <link linkend="cplusplusckbk-CHP-9-SECT-5">Recipe 9.5</link>
    int bufSize_;
    int initBufSize_;
    int msgSize_;
    char* buf_;
};
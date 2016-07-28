#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/array.hpp>

template <typename T, size_t maxsize>
struct CircularQueue {
    CircularQueue() : head_(0), tail_(0) {}

    void pop()
    {
        boost::unique_lock<boost::mutex> lock(qlock);

        if (size() == 0) {
            canRead.wait(lock, [this] { return size() > 0; });
        }

        ++head_;
        lock.unlock();
        canWrite.notify_one();
    }

    T top()
    {
        boost::unique_lock<boost::mutex> lock(qlock);

        if (size() == 0) {
            canRead.wait(lock, [this] { return size() > 0; });
        }

        T ret = data[head_ % maxsize];
        lock.unlock();
        return ret;
    }

    void push(T&& obj)
    {
        boost::unique_lock<boost::mutex> lock(qlock);

        if (size() == capacity()) {
            canWrite.wait(lock, [this]
            { return size() < capacity(); });
        }

        data[tail_++ % maxsize] = std::move(obj);
        lock.unlock();
        canRead.notify_one();
    }

    size_t head() const
    {
        return head_;
    }
    size_t tail() const
    {
        return tail_;
    }

    size_t count() const
    {
        boost::unique_lock<boost::mutex> lock(qlock);
        return (tail_ - head_);
    }

private:
    boost::array<T, maxsize> data;
    size_t head_, tail_;

    size_t capacity() const
    {
        return maxsize;
    }
    size_t size() const
    {
        return (tail_ - head_);
    };

    mutable boost::mutex qlock;
    mutable boost::condition_variable canRead;
    mutable boost::condition_variable canWrite;
};

int main()
{
    CircularQueue<int, 200> ds;
    boost::thread producer([&ds] {
        for (int i = 0; i < 10000; ++i)
        {
            ds.push(std::move(i));
            std::cout << i << "-->"
            << " [" << ds.count() << "]\n";
        }
    });
    auto func = [&ds] {
        for (int i = 0; i < 2500; ++i)
        {
            std::cout << "\t\t<--" << ds.top() << "\n";
            ds.pop();
        }
    };
    boost::thread_group consumers;

    for (int i = 0; i < 4; ++i) {
        consumers.create_thread(func);
    }

    producer.join();
    consumers.join_all();
}


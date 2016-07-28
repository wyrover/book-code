#include <boost/current_function.hpp>
#include <boost/container/static_vector.hpp>
#include <iostream>

class ChattyInit
{
public:
    ChattyInit()
    {
        std::cout << BOOST_CURRENT_FUNCTION << '\n';
    }
};

int main()
{
    boost::container::static_vector<ChattyInit, 10> myvector;
    std::cout << "boost::container::static_vector initialized"
              << '\n';

    while (myvector.size() < myvector.capacity()) {
        myvector.push_back(ChattyInit());
    }

    // cisv.push_back(ChattyInit()); // runtime error
}


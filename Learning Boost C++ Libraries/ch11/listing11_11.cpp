#include <boost/asio.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
namespace asio = boost::asio;

int main()
{
    std::srand(std::time(nullptr));
    std::vector<char> v1(10);
    char a2[10];
    std::vector<asio::mutable_buffer> bufseq(2);
    bufseq.push_back(asio::mutable_buffer(v1.data(),
                                          v1.capacity()));
    bufseq.push_back(asio::mutable_buffer(a2, sizeof(a2)));

    for (auto cur = asio::buffers_begin(bufseq),
         end = asio::buffers_end(bufseq); cur != end; cur++) {
        *cur = 'a' + rand() % 26;
    }

    std::cout << "Size: " << asio::buffer_size(bufseq) << '\n';
    std::string s1(v1.begin(), v1.end());
    std::string s2(a2, a2 + sizeof(a2));
    std::cout << s1 << '\n' << s2 << '\n';
}


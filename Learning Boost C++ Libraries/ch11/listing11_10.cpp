#include <boost/asio.hpp>
#include <iostream>
#include <cassert>
namespace asio = boost::asio;

int main()
{
    char buf[10];
    asio::mutable_buffer mbuf(buf, sizeof(buf));
    asio::const_buffer cbuf(buf, 5);
    std::cout << asio::buffer_size(mbuf) << '\n';
    std::cout << asio::buffer_size(cbuf) << '\n';
    char *mptr = asio::buffer_cast<char*>(mbuf);
    const char *cptr = asio::buffer_cast<const char*>(cbuf);
    assert(mptr == cptr && cptr == buf);
    size_t offset = 5;
    asio::mutable_buffer mbuf2 = mbuf + offset;
    assert(asio::buffer_cast<char*>(mbuf2)
           - asio::buffer_cast<char*>(mbuf) == offset);
    assert(asio::buffer_size(mbuf2) ==
           asio::buffer_size(mbuf) - offset);
}


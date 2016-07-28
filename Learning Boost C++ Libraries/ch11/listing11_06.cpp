#include <boost/asio.hpp>
#include <iostream>
#include <cassert>
#include <vector>
namespace asio = boost::asio;
namespace sys = boost::system;
using namespace asio::ip;

void printAddrProperties(const address& addr)
{
    std::cout << "\n\n" << addr << ": ";

    if (addr.is_v4()) {
        std::cout << "netmask=" << address_v4::netmask(addr.to_v4());
    } else if (addr.is_v6()) {
        /* ... */
    }

    if (addr.is_unspecified()) {
        std::cout << "is unspecified, ";
    }

    if (addr.is_loopback()) {
        std::cout << "is loopback, ";
    }

    if (addr.is_multicast()) {
        std::cout << "is multicast, ";
    }
}

int main()
{
    sys::error_code ec;
    std::vector<address> addresses;
    std::vector<const char*> addr_strings{"127.0.0.1",
        "10.28.25.62", "137.2.33.19", "223.21.201.30",
        "232.28.25.62", "140.28.25.62/22"};
    addresses.push_back(address_v4());       // default: 0.0.0.0
    addresses.push_back(address_v4::any());  // INADDR_ANY

    for (const auto& v4str : addr_strings) {
        address_v4 addr = address_v4::from_string(v4str, ec);

        if (!ec) {
            addresses.push_back(addr);
        }
    }

    for (const address& addr1 : addresses) {
        printAddrProperties(addr1);
    }
}


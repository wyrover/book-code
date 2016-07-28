#include <boost/asio.hpp>
#include <iostream>
#include <vector>
namespace asio = boost::asio;
namespace sys = boost::system;
using namespace asio::ip;

void printAddr6Properties(const address_v6& addr)
{
    if (addr.is_v4_mapped()) {
        std::cout << "is v4-mapped, ";
    } else {
        if (addr.is_link_local()) {
            std::cout << "is link local";
        }
    }
}

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
    std::vector<const char*> addr_strings{"::1", "::",
        "fe80::20", "::ffff:223.18.221.9", "2001::1e0:0:0:1a:2a"};

    for (const auto& v6str : addr_strings) {
        address addr = address_v6::from_string(v6str, ec);

        if (!ec) {
            addresses.push_back(addr);
        }
    }

    for (const auto& addr : addresses) {
        printAddrProperties(addr);
    }
}


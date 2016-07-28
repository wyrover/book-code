#include <boost/asio.hpp>
#include <iostream>
namespace asio = boost::asio;

int main()
{
    asio::io_service service;
    // Hello Handler ÎíÎñ dispatch behaves like post
    service.dispatch([]() {
        std::cout << "Hello\n";
    });
    service.post(
    [&service] { // English Handler
        std::cout << "Hello, world!\n";
        service.dispatch([] {  // Spanish Handler, immediate
            std::cout << "Hola, mundo!\n";
        });
    });
    // German Handler
    service.post([&service] {std::cout << "Hallo, Welt!\n"; });
    service.run();
}


#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
#include <iostream>
#include <cassert>
namespace fs = boost::filesystem;

void make_relative_paths()
{
    fs::path p2(".."); // relative path
    p2 /= "..";
    std::cout << "Relative path: " << p2.string() << '\n';
    std::cout << "Absolute path: "
              << fs::absolute(p2, "E:\\DATA\\photos").string() << '\n';
    std::cout << "Absolute path wrt CWD: "
              << fs::absolute(p2).string() << '\n';
    std::cout << fs::canonical(p2).string() << '\n';
}

void handle_canonical_errors()
{
    fs::path p3 = "E:\\DATA"; // absolute path
    auto p4 = p3 / "boost" / "boost_1_56";  // append elements
    std::cout << p4.string() << '\n';
    std::cout.put('\n');
    boost::system::error_code ec;
    auto p5 = p4 / ".." / "boost_1_100";  // append elements
    auto p6 = canonical(p5, ec);

    if (ec.value() == 0) {
        std::cout << "Normalized: " << p6.string() << '\n';
    } else {
        std::cout << "Error (file=" << p5.string()
                  << ") (code=" << ec.value() << "): "
                  << ec.message() << '\n';
    }
}

int main()
{
    fs::path p1; // empty path
    assert(p1.empty());  // does not fire
    p1 = "/opt/boost";   // assign an absolute path
    assert(!p1.empty());
    p1.clear();
    assert(p1.empty());
}


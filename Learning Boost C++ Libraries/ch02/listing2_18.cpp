#include <boost/predef.h>
#include <iostream>

void checkOs()
{
    // identify OS
#if defined(BOOST_OS_WINDOWS)
    std::cout << "Windows" << '\n';
#elif defined(BOOST_OS_LINUX)
    std::cout << "Linux" << '\n';
#elif defined(BOOST_OS_MACOS)
    std::cout << "MacOS" << '\n';
#elif defined(BOOST_OS_UNIX)
    std::cout << "Another UNIX" << '\n'; // *_AIX, *_HPUX, etc.
#endif
}

void checkArch()
{
    // identify architecture
#if defined(BOOST_ARCH_X86)
#if defined(BOOST_ARCH_X86_64)
    std::cout << "x86-64 bit" << '\n';
#else
    std::cout << "x86-32 bit" << '\n';
#endif
#elif defined(BOOST_ARCH_ARM)
    std::cout << "ARM" << '\n';
#else
    std::cout << "Other architecture" << '\n';
#endif
}

void checkCompiler()
{
    // identify compiler
#if defined(BOOST_COMP_GNUC)
    std::cout << "GCC, Version: " << BOOST_COMP_GNUC << '\n';
#elif defined(BOOST_COMP_MSVC)
    std::cout << "MSVC, Version: " << BOOST_COMP_MSVC << '\n';
#else
    std::cout << "Other compiler" << '\n';
#endif
}
#include <boost/predef.h>
#include <iostream>

void checkCpp11()
{
    // Do version checks
#if defined(BOOST_COMP_GNUC)
#if BOOST_COMP_GNUC < BOOST_VERSION_NUMBER(4, 8, 1)
    std::cout << "Incomplete C++ 11 support" << '\n';
#else
    std::cout << "Most C++ 11 features supported" << '\n';
#endif
#elif defined(BOOST_COMP_MSVC)
#if BOOST_COMP_MSVC < BOOST_VERSION_NUMBER(12, 0, 0)
    std::cout << "Incomplete C++ 11 support" << '\n';
#else
    std::cout << "Most C++ 11 features supported" << '\n';
#endif
#endif
}

int main()
{
    checkOs();
    checkArch();
    checkCompiler();
    checkCpp11();
}

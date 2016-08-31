#include <catch.hpp>
#include <tracetool/tracetool.h>
#include "common/dns_query.h"
#include <Windows.h>
#include <iostream>

TEST_CASE("Test for common/dns_query", "[common/dns_query]")
{
    SECTION("testIsUserInAdminGroup", "[common/dns_query]") {
        std::wstring dns_ret = dns_query();
        std::wcout << dns_ret << std::endl;
    }
}


#include <catch.hpp>
#include <tracetool/tracetool.h>
#include "common/environment_variable.h"
#include <Windows.h>
#include <iostream>

TEST_CASE("Test for common/environment_variable", "[common/environment_variable]")
{
    SECTION("common/environment_variable", "[common/environment_variable]") {
        REQUIRE(ExpandEnvironmentVariables(L"") == L"");
        REQUIRE(ExpandEnvironmentVariables(L"%%") == L"%%");
        REQUIRE(ExpandEnvironmentVariables(L"%%%") == L"%%%");
        REQUIRE(ExpandEnvironmentVariables(L"%cd%") == L"%cd%");
        REQUIRE(ExpandEnvironmentVariables(L"%COMPUTERNAME%") == GetEnvironmentVariableW(L"COMPUTERNAME"));
        REQUIRE(ExpandEnvironmentVariables(L"%COMPUTERNAME%%COMPUTERNAME%") == GetEnvironmentVariableW(L"COMPUTERNAME") + GetEnvironmentVariableW(L"COMPUTERNAME"));
        REQUIRE(ExpandEnvironmentVariables(L"%COMPUTERNAME") == L"%COMPUTERNAME");
        REQUIRE(ExpandEnvironmentVariables(L"COMPUTERNAME%") == L"COMPUTERNAME%");
        REQUIRE(ExpandEnvironmentVariables(L"{%COMPUTERNAME%}") == L"{" + GetEnvironmentVariableW(L"COMPUTERNAME") + L"}");
    }
}


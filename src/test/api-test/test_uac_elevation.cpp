#include <catch.hpp>
#include <tracetool/tracetool.h>
#include "common/uac_elevation.h"
#include <Windows.h>
#include <iostream>

TEST_CASE("Test for common/uac_elevation", "[common/uac_elevation]")
{
    SECTION("testIsUserInAdminGroup", "[common/uac_elevation]") {
        REQUIRE(cactus::IsUserInAdminGroup());
    }
    SECTION("testIsRunAsAdmin", "[common/uac_elevation]") {
        REQUIRE(cactus::IsRunAsAdmin());
    }
    SECTION("testIsElevationSupported", "[common/uac_elevation]") {
        OSVERSIONINFO osver = { sizeof(osver) };
        REQUIRE(cactus::IsElevationSupported() == (GetVersionEx(&osver) && osver.dwMajorVersion >= 6));
    }
    SECTION("testIsProcessElevated", "[common/uac_elevation]") {
        if (cactus::IsElevationSupported()) {
            // As build is run as an admin this should return true
            REQUIRE(cactus::IsProcessElevated());
        }
    }
    SECTION("testShellElevated", "[common/uac_elevation]") {
        if (cactus::IsElevationSupported()) {
            REQUIRE(cactus::ShellElevated(NULL, L"cmd.exe", L"C:\\", L"/c exit 0"));
        }
    }
    SECTION("testRestartElevated", "[common/uac_elevation]") {
        if (cactus::IsElevationSupported()) {
            // This will restart this test elevated and run one test.
            REQUIRE(cactus::RestartElevated(NULL, L"DVLib::UnitTests::UACElevationUnitTests::testIsProcessElevated"));
        }
    }
}


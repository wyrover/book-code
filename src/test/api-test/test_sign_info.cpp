#include <catch.hpp>
#include <tracetool/tracetool.h>
#include "common/sign_info.h"
#include <Windows.h>
#include <iostream>

TEST_CASE("Test for common/sign_info", "[common/sign_info]")
{
    SECTION("testIsUserInAdminGroup", "[common/uac_elevation]") {
        std::wstring sTestFile = L"c:\\windows\\regedit.exe";
        cactus::ZLSignInfo signer;
        REQUIRE(signer.Load(NULL) == FALSE);
        REQUIRE(signer.Load(sTestFile.c_str()) == FALSE);
        cactus::ZLSignInfo::IsDigitalSignatureHasTimestamp(sTestFile.c_str());
    }
}


#include <catch.hpp>
#include <tracetool/tracetool.h>
#include "common/verify_signature.h"
#include <Windows.h>
#include <iostream>

TEST_CASE("Test for common/verify_signature", "[common/verify_signature]")
{
    SECTION("test_verify_signature", "[common/verify_signature]") {
        const wchar_t* file_name = L"Diablo III Launcher.exe";

        if (verify_signature(file_name) == true) {
            wprintf_s(L"The file \"%s\" is signed and the signature was verified.\n", file_name);
            std::wstring signer = get_signer(file_name);
            wprintf_s(L"%s signer is %s\n", file_name, signer.c_str());
        } else {
            wprintf_s(L"The file \"%s\" is not trusted.\n", file_name);
        }
    }
}


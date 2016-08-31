#include <catch.hpp>
#include <tracetool/tracetool.h>
#include <Windows.h>
#include <wtypes.h>
#include <tchar.h>
#include <strsafe.h>
#include <atlstr.h>
#include <iostream>
#include <bitset>


#define DG_CONSOLE_APP 0x0004
#define DG_CONSOLE_MASK (DG_CONSOLE_APP << 16)

#define DG_CONSOLE_ERROR_CODE(severity, code) \
    ( APPLICATION_ERROR_MASK | DG_CONSOLE_MASK | severity | code )

#define DG_CONSOLE_ERROR(code)      DG_CONSOLE_ERROR_CODE(ERROR_SEVERITY_ERROR, code)
#define DG_CONSOLE_INFO(code)       DG_CONSOLE_ERROR_CODE(ERROR_SEVERITY_INFORMATIONAL, code)
#define DG_CONSOLE_WARNING(code)    DG_CONSOLE_ERROR_CODE(ERROR_SEVERITY_WARNING, code)
#define DG_CONSOLE_SUCCESS 0L


//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//

//#define APPLICATION_ERROR_MASK       0x20000000
//#define ERROR_SEVERITY_SUCCESS       0x00000000
//#define ERROR_SEVERITY_INFORMATIONAL 0x40000000
//#define ERROR_SEVERITY_WARNING       0x80000000
//#define ERROR_SEVERITY_ERROR         0xC0000000




#define DG_CONSOLE_ERROR_REGISTRY_KEY_NOT_FOUND             DG_CONSOLE_ERROR(0x1F)
#define DG_CONSOLE_ERROR_REGISTRY_KEY_NOT_FOUND_1             DG_CONSOLE_ERROR(0x22)

#define DG_CONSOLE_INFO_REGISTRY_KEY_NOT_FOUND_1             DG_CONSOLE_INFO(0x22)
#define DG_CONSOLE_WARNING_REGISTRY_KEY_NOT_FOUND_1             DG_CONSOLE_WARNING(0x22)




CString FormatErrCode(int nCode)
{
    CString retval;
    retval.Format(TEXT("Error Code: %d (0x%08X)"), nCode, nCode);
    return retval;
}

CString FormatWindowsErrorMessage(int nCode)
{
    CString strText;

    if ((nCode & APPLICATION_ERROR_MASK) == 0) {
        LPTSTR lpszErrorMessage;
        BOOL fSuccess = ::FormatMessage(
                            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                            NULL, nCode, 0, (LPTSTR) &lpszErrorMessage, 0, NULL);

        if (fSuccess) {
            strText.Format(TEXT("%s, Error Code: %d (0x%08X)"),
                           lpszErrorMessage, nCode, nCode);
            ::LocalFree(lpszErrorMessage);
        }
    }

    return strText;
}



TEST_CASE("Test for errorcode", "[errorcode]")
{
    SECTION("test_errorcode", "[errorcode]") {
        std::cout << bitset<32>(DG_CONSOLE_APP) << " DG_CONSOLE" << std::endl;
        std::cout << bitset<32>(DG_CONSOLE_MASK) << " DG_CONSOLE_MASK" << std::endl;
        std::cout << bitset<32>(DG_CONSOLE_ERROR_REGISTRY_KEY_NOT_FOUND) << " DG_CONSOLE_ERROR_REGISTRY_KEY_NOT_FOUND" << std::endl;
        std::cout << bitset<32>(DG_CONSOLE_ERROR_REGISTRY_KEY_NOT_FOUND_1) << " DG_CONSOLE_ERROR_REGISTRY_KEY_NOT_FOUND_1" << std::endl;
        std::cout << bitset<32>(DG_CONSOLE_INFO_REGISTRY_KEY_NOT_FOUND_1) << " DG_CONSOLE_INFO_REGISTRY_KEY_NOT_FOUND_1" << std::endl;
        std::cout << bitset<32>(DG_CONSOLE_WARNING_REGISTRY_KEY_NOT_FOUND_1) << " DG_CONSOLE_WARNING_REGISTRY_KEY_NOT_FOUND_1" << std::endl;
        std::cout << bitset<32>(DG_CONSOLE_SUCCESS) << " DG_CONSOLE_SUCCESS" << std::endl;
        std::cout << std::endl;
        std::wcout << (LPCWSTR)FormatErrCode(DG_CONSOLE_APP) << std::endl;
        std::wcout << (LPCWSTR)FormatErrCode(DG_CONSOLE_MASK) << std::endl;
        std::wcout << (LPCWSTR)FormatErrCode(DG_CONSOLE_ERROR_REGISTRY_KEY_NOT_FOUND) << std::endl;
        std::wcout << (LPCWSTR)FormatErrCode(DG_CONSOLE_ERROR_REGISTRY_KEY_NOT_FOUND_1) << std::endl;
        std::wcout << L"INFO:" << (LPCWSTR)FormatErrCode(DG_CONSOLE_INFO_REGISTRY_KEY_NOT_FOUND_1) << std::endl;
        std::wcout << (LPCWSTR)FormatErrCode(DG_CONSOLE_WARNING_REGISTRY_KEY_NOT_FOUND_1) << std::endl;
        std::wcout << (LPCWSTR)FormatErrCode(DG_CONSOLE_SUCCESS) << std::endl;
        std::wcout << std::endl;
        std::wcout << (LPCWSTR)FormatWindowsErrorMessage(STRSAFE_E_END_OF_FILE) << std::endl;
        std::cout <<  bitset<32>(0x00000014 & DG_CONSOLE_APP) << std::endl;

        // &    与      两个位都为1时，结果才为1
        // |    或      两个位都为0时，结果才为0
        // ^    异或        两个位相同为0，相异为1
        // ~    取反        0变1，1变0
        // <<   左移        各二进位全部左移若干位，高位丢弃，低位补0
        // >>   右移        各二进位全部右移若干位，对无符号数，高位补0，有符号数，各编译器处理方法不一样，有的补符号位（算术右移），有的补0（逻辑右移）

        if ((0x00000014 & DG_CONSOLE_APP) == DG_CONSOLE_APP) {
            std::cout << "有标志位" << std::endl;
        }

        std::cout << std::endl;
    }
    SECTION("test_bitset", "[errorcode]") {
        unsigned int a;
        std::bitset<32> x;
        a = -1;
        x = std::bitset<32>(a);
        std::cout << "-1" << a << " " << x << std::endl;
        a <<= 1;
        x = std::bitset<32>(a);
        std::cout << "-1 左移 1 位" << a << " " << x << std::endl;
        a = -1;
        a >>= 1;
        x = std::bitset<32>(a);
        std::cout << "-1 右移 1 位" << a << " " << x << std::endl;
        std::cout << std::endl;
        std::cout << setw(7) << setiosflags(ios::right) << "1:" << setw(7) << setiosflags(ios::right) << 0x01 << " " << setw(32) << setiosflags(ios::right) << std::bitset<32>(1) << '\n';
        std::cout << setw(7) << setiosflags(ios::right) << "2:" << setw(7) << setiosflags(ios::right) << 0x02 << " " << setw(32) << setiosflags(ios::right) << std::bitset<32>(2) << '\n';
        std::cout << setw(7) << setiosflags(ios::right) << "4:" << setw(7) << setiosflags(ios::right) << 0x04 << " " << setw(32) << setiosflags(ios::right) << std::bitset<32>(4) << '\n';
        std::cout << setw(7) << setiosflags(ios::right) << "8:" << setw(7) << setiosflags(ios::right) << 0x08 << " " << setw(32) << setiosflags(ios::right) << std::bitset<32>(8) << '\n';
        std::cout << setw(7) << setiosflags(ios::right) << "16:" << setw(7) << setiosflags(ios::right) << 0x10 << " " << setw(32) << setiosflags(ios::right) << std::bitset<32>(16) << '\n';
        std::cout << setw(7) << setiosflags(ios::right) << "32:" << setw(7) << setiosflags(ios::right) << 0x20 << " " << setw(32) << setiosflags(ios::right) << std::bitset<32>(32) << '\n';
        std::cout << setw(7) << setiosflags(ios::right) << "64:" << setw(7) << setiosflags(ios::right) << 0x40 << " " << setw(32) << setiosflags(ios::right) << std::bitset<32>(64) << '\n';
        std::cout << setw(7) << setiosflags(ios::right) << "128:" << setw(7) << setiosflags(ios::right) << 0x80 << " " << setw(32) << setiosflags(ios::right) << std::bitset<32>(128) << '\n';
        std::cout << std::endl;
    }
    SECTION("test_htonl", "[errorcode]") {
        int y = (0x12345678);
        char* p = (char*)&y;
        printf("小端字节序(低位放在前面):%x\n", *p);
        std::cout <<  bitset<8>(*p) << std::endl;
        int x = htonl(0x12345678);
        // 溜 Big Endian栏肺 历厘
        p = (char*)&x;
        printf("大端字节序(高位放在前面)%x\n", *p);   // 12 ? 78 ?
        std::cout <<  bitset<8>(*p) << std::endl;
        std::cout << std::endl;
    }
}


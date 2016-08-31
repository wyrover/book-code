#include <catch.hpp>
#include <tracetool/tracetool.h>
#include "common/wmi.h"
#include "common/restore_point.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <map>

typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned __int64 uint64_t;

using namespace cactus;

#include <Windows.h>

class DisableWow64FileSystemRedirection
{
public:
    DisableWow64FileSystemRedirection(void);
    ~DisableWow64FileSystemRedirection(void);

private:
    void Wow64DisableWow64FsRedirection(LPCSTR func);

private:
    PVOID m_OldValue;
    HMODULE m_hModule;
};

DisableWow64FileSystemRedirection::DisableWow64FileSystemRedirection(void)
    : m_OldValue(NULL)
{
    m_hModule = ::LoadLibraryEx(L"Kernel32.dll", NULL, LOAD_WITH_ALTERED_SEARCH_PATH);

    if (m_hModule)
        Wow64DisableWow64FsRedirection("Wow64DisableWow64FsRedirection");
}

DisableWow64FileSystemRedirection::~DisableWow64FileSystemRedirection(void)
{
    if (m_hModule) {
        Wow64DisableWow64FsRedirection("Wow64RevertWow64FsRedirection");
        ::FreeLibrary(m_hModule);
    }
}


void DisableWow64FileSystemRedirection::Wow64DisableWow64FsRedirection(LPCSTR func)
{
    typedef BOOL (WINAPI * PROC)(PVOID*);
    PROC lpfWow64FsRedirection;

    if (NULL != m_hModule) {
        lpfWow64FsRedirection = (PROC)GetProcAddress(m_hModule, func);

        if (lpfWow64FsRedirection != NULL) {
            (*lpfWow64FsRedirection)(&m_OldValue);
        }
    }
}


TEST_CASE("Test for wmi", "[wmi]")
{
//   SECTION("wmi", "[wmi]") {
//       REQUIRE(TRUE == TRUE);
//       CWmiUtilities* wmi = NULL;
//       try {
//           wmi = new CWmiUtilities();
//           wmi->ConnectWmi(L"\\\\.\\root\\cimv2");
//           wmi->ExecQuery(L"SELECT * FROM Win32_DiskDrive");
//           while (wmi->Next()) {
//               TRACE_SEND_FORMAT("%d", wmi->GetUInt<uint32_t>(L"BytesPerSector"));
//               TRACE_SEND_FORMAT("%d", wmi->GetUInt<uint32_t>(L"Partitions"));
//               TRACE_SEND_FORMAT("%d", wmi->GetUInt<uint64_t>(L"Size"));
//               //TRACE_SEND_FORMAT(L"%s", wmi->GetString(L"Status"));
//               //TRACE_SEND_FORMAT(L"%s", wmi->GetString(L"Model"));
//           }
//       } catch (...) {
//           delete wmi;
//       }
//       delete wmi;
//   }
    //SECTION("Win32_ComputerSystem", "[Win32_ComputerSystem]") {
    //  TRACE_SEND_W(L"Win32_SystemEnclosure");
    //  REQUIRE(TRUE == TRUE);
    //  CWmiUtilities* wmi = NULL;
    //  try {
    //      wmi = new CWmiUtilities();
    //      wmi->ConnectWmi(L"\\\\.\\root\\cimv2");
    //      if (wmi->BeginEnumClassObject(L"Win32_DiskDrive")) {
    //          TRACE_SEND_A("Win32_ComputerSystem");
    //          while (wmi->Next()) {
    //
    //
    //              std::wstring test_value;
    //              if (wmi->GetStringProperty(L"Manufacturer", test_value)) {
    //                  TRACE_SEND_FORMAT(L"System Manufacturer = %s", test_value.c_str());
    //              }
    //              //TRACE_SEND_FORMAT(L"%s", wmi->GetString(L"Model"));
    //          }
    //      }
    //  } catch (...) {
    //      delete wmi;
    //  }
    //  delete wmi;
    //}
    SECTION("restore_point", "[restore_point]") {
        TRACE_SEND_W(L"restore_point");
        //DisableWow64FileSystemRedirection diswow64;
        CSnapshotManager* snapshotManager = NULL;

        try {
            snapshotManager = new CSnapshotManager();

            if (snapshotManager->init()) {
                snapshotManager->listSnapshotList();
            }
        } catch (...) {
            delete snapshotManager;
        }

        delete snapshotManager;
    }
}
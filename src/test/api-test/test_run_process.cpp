#include <catch.hpp>
#include <tracetool/tracetool.h>
#include "common/run_process.h"
#include "common/conpipe.h"
#include "common/environment_variable.h"
#include "common/find_window.h"
#include "common/path.h"
#include "common/format.h"
#include <Windows.h>
#include <iostream>

TEST_CASE("Test for common/run_process", "[common/run_process]")
{
    SECTION("testrun_process", "[common/run_process]") {
        std::string cmdline;
        cmdline.append("cd /d D:\\ \n");
        cmdline.append("dir /w /c\n");
        cmdline.append("exit \n");
        std::string output;
        GetCommandOutput(cmdline.c_str(), output);
        std::cout << output.c_str() << std::endl;
    }
    SECTION("testconpipe", "[common/conpipe]") {
        std::wstring cmdline;
        cmdline.append(L"xcopy.exe /?");
        std::string output;
        FILE* fin;
        fin = openpipe((LPWSTR)cmdline.c_str(), false);

        if (!fin || feof(fin) || ferror(fin)) {
            printf("error open pipe\n");
        }

        char c;

        while (!feof(fin)) {
            fread(&c, 1, 1, fin);
        }

        pclose(fin);
        //std::cout << output.c_str() << std::endl;
    }
    SECTION("testRunCmd", "[common/run_process]") {
        RunCmd(L"cmd.exe /C exit /b 0");
        PROCESS_INFORMATION pi = { 0 };
        RunCmd(L"cmd.exe /C exit /b 0", & pi);
        REQUIRE(pi.dwProcessId > 0);
        REQUIRE(WAIT_OBJECT_0 == ::WaitForSingleObject(pi.hProcess, INFINITE));

        if (pi.hThread)
            ::CloseHandle(pi.hThread);

        if (pi.hProcess)
            ::CloseHandle(pi.hProcess);
    }
    SECTION("testExecCmd", "[common/run_process]") {
        REQUIRE(0 == ExecCmd(L"cmd.exe /C"));
        REQUIRE(123 == ExecCmd(L"cmd.exe /C exit /b 123"));
        // hide window
        REQUIRE(456 == ExecCmd(L"cmd.exe /C exit /b 456", L"", SW_HIDE));
    }
    SECTION("testDetachCmd", "[common/run_process]") {
    }
    SECTION("testShellCmd", "[common/run_process]") {
        ShellCmd(L"cmd.exe /C");
        ShellCmd(L"\"" + GetEnvironmentVariable(L"SystemRoot") + L"\\system32\\cmd.exe\" /C");
        HANDLE hProcess;
        ShellCmd(L"\"cmd.exe\" /C dir", NULL, &hProcess);
        REQUIRE(hProcess != NULL);
        REQUIRE(WAIT_OBJECT_0 == ::WaitForSingleObject(hProcess, INFINITE));

        if (hProcess)
            ::CloseHandle(hProcess);
    }
    SECTION("testRunCmdWithHiddenWindow", "[common/run_process]") {
        using namespace cactus;
        // Arrange
        int nShow = SW_HIDE;
        // Act
        PROCESS_INFORMATION pi = { 0 };
        RunCmd(L"cmd.exe /C ping -n 6 127.0.0.1 > nul && exit /b 0", & pi, 0, L"", nShow);
        // Assert
        REQUIRE(pi.dwProcessId > 0);
        REQUIRE(NULL == FindWindow::FindWindowFromProcessId(pi.dwProcessId));
        REQUIRE(WAIT_OBJECT_0 == ::WaitForSingleObject(pi.hProcess, INFINITE));

        if (pi.hThread)
            ::CloseHandle(pi.hThread);

        if (pi.hProcess)
            ::CloseHandle(pi.hProcess);
    }
    SECTION("testShellCmdWithHiddenWindow", "[common/run_process]") {
        using namespace cactus;
        // Arrange
        int nShow = SW_HIDE;
        HANDLE hProcess;
        // Act
        ShellCmd(L"\"cmd.exe\" /C ping -n 6 127.0.0.1 > nul", NULL, &hProcess, NULL, L"", nShow);
        // Assert
        REQUIRE(hProcess != NULL);
        REQUIRE(NULL == FindWindow::FindWindowFromProcess(hProcess));
        REQUIRE(WAIT_OBJECT_0 == ::WaitForSingleObject(hProcess, INFINITE));

        if (hProcess)
            ::CloseHandle(hProcess);
    }
    SECTION("testRunCmdWithoutWorkingDirectorySpecified", "[common/run_process]") {
        using namespace cactus;
        // Arrange
        std::wstring working_directory = cactus::GetCurrentDirectoryW();
        PROCESS_INFORMATION pi = { 0 };
        std::wstring command = cactus::FormatMessage(
                                   L"cmd.exe /C if '%%cd%%'=='%s' (exit /b 0) else (echo '%%cd%%'!='%s' && exit /b 1)",
                                   working_directory.c_str(),
                                   working_directory.c_str());
        // Act
        RunCmd(command, &pi, 0);
        // Assert
        REQUIRE(pi.dwProcessId > 0);
        REQUIRE(WAIT_OBJECT_0 == ::WaitForSingleObject(pi.hProcess, INFINITE));
        DWORD exitCode = 0;
        ::GetExitCodeProcess(pi.hProcess, &exitCode);
        REQUIRE(exitCode == 0);

        if (pi.hThread)
            ::CloseHandle(pi.hThread);

        if (pi.hProcess)
            ::CloseHandle(pi.hProcess);
    }
    SECTION("testRunCmdWithWorkingDirectorySpecified", "[common/run_process]") {
        using namespace cactus;
        // Arrange
        std::wstring working_directory = cactus::GetTemporaryDirectoryW();
        // Act
        PROCESS_INFORMATION pi = { 0 };
        RunCmd(L"cmd.exe /C if '%%cd%%'=='%%temp%%' (exit /b 0) else (echo '%%cd%%'!='%%temp%%' && exit /b 1)", &pi, 0, working_directory);
        // Assert
        REQUIRE(pi.dwProcessId > 0);
        REQUIRE(WAIT_OBJECT_0 == ::WaitForSingleObject(pi.hProcess, INFINITE));
        DWORD exitCode = 0;
        ::GetExitCodeProcess(pi.hProcess, &exitCode);
        REQUIRE(exitCode == 0);

        if (pi.hThread)
            ::CloseHandle(pi.hThread);

        if (pi.hProcess)
            ::CloseHandle(pi.hProcess);
    }
    SECTION("testShellCmdWithoutWorkingDirectorySpecified", "[common/run_process]") {
        using namespace cactus;
        // Arrange
        std::wstring working_directory = cactus::GetCurrentDirectoryW();
        std::wstring command = cactus::FormatMessage(
                                   L"cmd.exe /C if '%%cd%%'=='%s' (exit /b 0) else (echo '%%cd%%'!='%s' && exit /b 1)",
                                   working_directory.c_str(),
                                   working_directory.c_str());
        HANDLE hProcess;
        // Act
        ShellCmd(command.c_str(), NULL, &hProcess, NULL);
        // Assert
        REQUIRE(hProcess != NULL);
        REQUIRE(WAIT_OBJECT_0 == ::WaitForSingleObject(hProcess, INFINITE));
        DWORD exitCode = 0;
        ::GetExitCodeProcess(hProcess, &exitCode);
        REQUIRE(exitCode == 0);

        if (hProcess)
            ::CloseHandle(hProcess);
    }
    SECTION("testShellCmdWithWorkingDirectorySpecified", "[common/run_process]") {
        using namespace cactus;
        // Arrange
        std::wstring working_directory = cactus::GetTemporaryDirectoryW();
        HANDLE hProcess;
        // Act
        ShellCmd(L"\"cmd.exe\" /C if '%%cd%%'=='%%temp%%' (exit /b 0) else (echo '%%cd%%'!='%%temp%%' && exit /b 1)", NULL, &hProcess, NULL, working_directory);
        // Assert
        REQUIRE(hProcess != NULL);
        REQUIRE(WAIT_OBJECT_0 == ::WaitForSingleObject(hProcess, INFINITE));
        DWORD exitCode = 0;
        ::GetExitCodeProcess(hProcess, &exitCode);
        REQUIRE(exitCode == 0);

        if (hProcess)
            ::CloseHandle(hProcess);
    }
}


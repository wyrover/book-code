#include <catch.hpp>
#include <tracetool/tracetool.h>
#include "common/file.h"
#include "common/stopwatch.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <map>
#include <boost/filesystem.hpp>
#include <Shlwapi.h>


namespace fs = boost::filesystem;

void find_file(const fs::path& dir)
{
    using namespace std;

    if (!fs::exists(dir)) return;

    fs::directory_iterator end_itr; // 缺省构造生成一个结束迭代器

    for (fs::directory_iterator itr(dir);
         itr != end_itr;
         ++itr) {
        if (fs::is_directory(itr->status())) {
            find_file(itr->path());   //递归查找
        } else {
            std::wcout << itr->path().wstring() << std::endl;
            //std::wstring newName = itr->path().parent_path().wstring() + L"\\" + itr->path().stem().wstring() + L"_0-49" + itr->path().extension().wstring();
            //std::wcout << newName << std::endl;
            //fs::rename(itr->path(), newName);
        }
    }
}

TEST_CASE("Test for boost filesystem", "[boost filesystem]")
{
    SECTION("enum", "[enum]") {
        fs::path dir(L"C:\\Program Files\\TortoiseGit\\bin");
        find_file(dir);
    }
    SECTION("fs::path", "[fs::path]") {
        boost::filesystem::path p("C:\\Windows\\System");
        std::wcout << p.native() << std::endl;
        std::cout << p.string() << std::endl;
        std::wcout << p.wstring() << std::endl;
        std::cout << p.generic_string() << std::endl;
        std::wcout << p.generic_wstring() << std::endl;
        boost::filesystem::path path1("/");
        REQUIRE(wcscmp(path1.wstring().c_str(), L"/") == 0);
        REQUIRE(wcscmp(path1.generic_wstring().c_str(), L"/") == 0);
    }
}

void WINAPI findGskFile(const std::wstring& filename, void* param)
{
    std::vector<std::wstring>* list = reinterpret_cast<std::vector<std::wstring>* >(param);

    if (filename.rfind(L".exe") + 4 == filename.length()) {
        list->push_back(filename);
    }
}

TEST_CASE("Test for common/file", "[common/file]")
{
    SECTION("testCreateDeepDirectory", "[common/file]") {
        REQUIRE(CreateDeepDirectory(L"D:\\111\\222\\333") == TRUE);
        REQUIRE(CreateDeepDirectory(L"D:\\111\\222\\444\\") == TRUE);
    }
    SECTION("test_enumFile", "[common/file]") {
        std::vector<std::wstring> fileList;
        StopWatch sw;
        cactus::enumFile(L"./", findGskFile, &fileList);
        sw.stop();
        TRACE_SEND_FORMAT(L"elapsed : %dmsec", sw.getElapsedMilliSecUntilStoped());
        std::vector<std::wstring>::iterator it = fileList.begin();

        for (; it < fileList.end(); ++it) {
            TRACE_SEND_W((*it).c_str());
        }
    }
}

TEST_CASE("Test for Shlwapi", "[Shlwapi]")
{
    SECTION("testPathFileExistsW", "[common/file]") {
        TRACE_SEND_W(L"PathFileExistsW 测试目录存在");
        REQUIRE(PathFileExistsW(L"C:\\Program Files\\TortoiseGit\\bin") == TRUE);
        TRACE_SEND_W(L"PathFileExistsW 测试目录不存在");
        REQUIRE(PathFileExistsW(L"C:\\Program Files\\TortoiseGit\\bin1") == FALSE);
        TRACE_SEND_W(L"PathFileExistsW 测试文件存在");
        REQUIRE(PathFileExistsW(L"C:\\Program Files\\TortoiseGit\\bin\\pageant.exe") == TRUE);
        TRACE_SEND_W(L"PathFileExistsW 测试文件不存在");
        REQUIRE(PathFileExistsW(L"C:\\Program Files\\TortoiseGit\\bin\\pageant1.exe") == FALSE);
    }
    SECTION("testPathAddBackslashW", "[common/file]") {
        // 不太实用，实际上经常传 const wchar_t* 路径, 无法修改字符数组, 可在此基础上再封装
        TRACE_SEND_W(L"PathAddBackslashW 添加路径分割符");
        wchar_t dir[] = L"C:\\Program Files\\TortoiseGit\\bin";
        LPCWSTR dir1 = PathAddBackslashW(dir);
        TRACE_SEND_W(dir);
        REQUIRE(wcscmp(dir, L"C:\\Program Files\\TortoiseGit\\bin\\") == 0);
    }
    SECTION("testPathAddExtensionW", "[common/file]") {
        TRACE_SEND_W(L"PathAddExtensionW 添加扩展名");
        wchar_t filename[] = L"C:\\Program Files\\TortoiseGit\\bin\\pageant";
        REQUIRE(PathAddExtensionW(filename, L".exe") == TRUE);
        REQUIRE(wcscmp(filename, L"C:\\Program Files\\TortoiseGit\\bin\\pageant.exe") == 0);
    }
    SECTION("Shlwapi", "[Shlwapi]") {
        TRACE_SEND_W(L"PathAppendW 拼接路径");
        wchar_t filename1[] = L"C:\\Program Files\\TortoiseGit\\bin";
        REQUIRE(PathAppendW(filename1, L"..\\..\\bin") == TRUE);
        TRACE_SEND_W(filename1);
        REQUIRE(wcscmp(filename1, L"C:\\Program Files\\bin") == 0);
        TRACE_SEND_W(L"PathBuildRootW 根路径");
        wchar_t filename2[] = L"";
        PathBuildRootW(filename2, 0);
        TRACE_SEND_W(filename2);
        REQUIRE(wcscmp(filename2, L"A:\\") == 0);
        TRACE_SEND_W(L"PathCombineW 拼接路径");
        wchar_t filename3[] = L"C:\\Program Files\\TortoiseGit\\bin";
        wchar_t filename4[] = L"";
        LPWSTR lpCHAR = PathCombineW(filename4, filename3, L"..\\..\\bin");
        TRACE_SEND_W(filename4);
        TRACE_SEND_W(lpCHAR);
        //REQUIRE(wcscmp(filename1, L"C:\\Program Files\\bin") == 0);
        TRACE_SEND_W(L"PathCommonPrefixW 公共路径");
        wchar_t filename5[] = L"C:\\Program Files\\TortoiseGit\\bin\\notepad2.exe";
        wchar_t filename6[] = L"C:\\Program Files\\TortoiseGit\\bin\\pageant.exe";
        wchar_t filename7[] = L"";
        int retval = PathCommonPrefixW(filename5, filename6, filename7);
        TRACE_SEND_W(filename7);
        TRACE_SEND_W(L"PathIsDirectoryW 是否是真实存在的目录");
        wchar_t filename8[] = L"C:\\Program Files\\TortoiseGit\\bin";
        wchar_t filename9[] = L"D:\\dev";
        //REQUIRE(PathIsDirectoryW(filename8) == FALSE);
        //REQUIRE(PathIsDirectoryW(filename9) == TRUE);
        REQUIRE(PathIsDirectoryEmptyW(filename8) == FALSE);
        TRACE_SEND_W(L"PathMatchSpecW 好像只能检测文件扩展名, 跟  dos 方式差不多");
        wchar_t filename10[] = L"C:\\Program Files\\TortoiseGit\\bin\\test.txt";
        REQUIRE(PathMatchSpecW(filename10, L"*.txt") == TRUE);
        TRACE_SEND_W(L"PathRemoveArgsW 移除路径参数");
        wchar_t filename11[ ] = L"c:\\a\\b\\FileA Arg1 Arg2";
        PathRemoveArgsW(filename11);
        TRACE_SEND_W(filename11);
        wchar_t filename12[ ] = L"C:\\Program Files\\TortoiseGit\\bin\\";
        PathRemoveBackslashW(filename12);
        REQUIRE(wcscmp(filename12, L"C:\\Program Files\\TortoiseGit\\bin") == 0);
        wchar_t filename13[ ] = L"C:\\Program Files\\TortoiseGit\\bin\\test.txt";
        PathRemoveExtensionW(filename13);
        TRACE_SEND_W(filename13);
        REQUIRE(wcscmp(filename13, L"C:\\Program Files\\TortoiseGit\\bin\\test") == 0);
        TRACE_SEND_W(L"PathRemoveFileSpecW 移除最后一段路径");
        wchar_t filename14[ ] = L"C:\\Program Files\\TortoiseGit\\bin\\test.txt";
        PathRemoveFileSpecW(filename14);
        REQUIRE(wcscmp(filename14, L"C:\\Program Files\\TortoiseGit\\bin") == 0);
        PathRemoveFileSpecW(filename14);
        TRACE_SEND_W(filename14);
        REQUIRE(wcscmp(filename14, L"C:\\Program Files\\TortoiseGit") == 0);
        TRACE_SEND_W(L"PathRenameExtensionW 替换扩展名");
        wchar_t filename15[ ] = L"C:\\Program Files\\TortoiseGit\\bin\\test.txt";
        PathRenameExtensionW(filename15, L".png");
        REQUIRE(wcscmp(filename15, L"C:\\Program Files\\TortoiseGit\\bin\\test.png") == 0);
        //wchar_t filename16[ ] = L"C:\\Program Files\\TortoiseGit\\bin";
        //PathSkipRootW(filename16);
        //TRACE_SEND_W(filename16);
        //REQUIRE(wcscmp(filename16, L"Program Files\\TortoiseGit\\bin") == 0);
        TRACE_SEND_W(L"PathStripPathW 只剩最后的路径");
        wchar_t filename17[ ] = L"C:\\Program Files\\TortoiseGit\\bin\\test.txt";
        PathStripPathW(filename17);
        REQUIRE(wcscmp(filename17, L"test.txt") == 0);
        wchar_t filename171[ ] = L"C:\\Program Files\\TortoiseGit\\bin";
        PathStripPathW(filename171);
        REQUIRE(wcscmp(filename171, L"bin") == 0);
        wchar_t filename18[ ] = L"C:\\Program Files\\TortoiseGit\\bin\\";
        PathStripPathW(filename18);
        REQUIRE(wcscmp(filename18, L"bin\\") == 0);
        TRACE_SEND_W(L"PathStripToRootW 只剩根路径");
        wchar_t filename19[ ] = L"C:\\Program Files\\TortoiseGit\\bin\\";
        PathStripToRootW(filename19);
        REQUIRE(wcscmp(filename19, L"C:\\") == 0);
        //wchar_t filename20[] = L"";
        //PathUnExpandEnvStringsW(L"%APPDATA%", filename20, MAX_PATH);
        //TRACE_SEND_W(filename20);
    }
}
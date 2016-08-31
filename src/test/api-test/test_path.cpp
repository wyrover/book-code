#include <catch.hpp>
#include <tracetool/tracetool.h>
#include "common/path.h"
#include "common/file.h"
#include "common/directory.h"
#include "common/guid.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <map>

TEST_CASE("Test for path", "[common/path]")
{
    SECTION("testDirectoryExists", "[restore_point]") {
        std::string path = cactus::GetTemporaryFileNameA();
        std::cout << std::endl << "Temporary filename: " << path;
        std::string directory = cactus::GetFileDirectoryA(path);
        std::cout << std::endl << "Directory: " << directory;
        REQUIRE(cactus::DirectoryExists(directory));
        REQUIRE(! cactus::DirectoryExists(path));
        cactus::FileDelete(path);
        REQUIRE(cactus::DirectoryExists("C:\\"));
        REQUIRE(! cactus::DirectoryExists(""));
        std::string filename = cactus::GenerateGUIDStringA();
        std::string guidpath = "C:\\" + filename;
        std::cout << std::endl << "Guid path: " << guidpath;
        REQUIRE(! cactus::DirectoryExists(guidpath));
    }
    SECTION("testDirectoryCreate", "[restore_point]") {
        std::wstring root = cactus::GetTemporaryDirectoryW();
        // check that we can create one level
        std::wstring guid = cactus::GenerateGUIDStringW();
        std::wstring path1 = root + guid;
        std::wcout << std::endl << L"Creating: " << path1;
        REQUIRE(! cactus::DirectoryExists(path1));
        std::wstring path1_created = cactus::DirectoryCreate(path1);
        REQUIRE(cactus::DirectoryExists(path1));
        REQUIRE(path1 == path1_created);
        // check that we can create two more levels
        std::wstring path2 = path1 + L"\\" + guid;
        std::wstring path3 = path2 + L"\\" + guid;
        std::wcout << std::endl << L"Creating: " << path3;
        REQUIRE(! cactus::DirectoryExists(path3));
        std::wstring path3_created = cactus::DirectoryCreate(path3);
        REQUIRE(cactus::DirectoryExists(path2));
        // the first created directory is path2
        REQUIRE(path2 == path3_created);
        // delete the final result
        cactus::DirectoryDelete(path1);
    }
    SECTION("testDirectoryDelete", "[restore_point]") {
        std::wstring root = cactus::GetTemporaryDirectoryW();
        std::wstring guid = cactus::GenerateGUIDStringW();
        std::wstring deeppath = root + guid + L"\\" + guid + L"\\" + guid;
        std::wstring newpath = cactus::DirectoryCreate(deeppath);
        std::wcout << std::endl << "Path: " << newpath;
        // delete files in a directory that doesn't have any files or subdirectories
        cactus::DirectoryDelete(deeppath, cactus::DELETE_DIRECTORY_FILES);
        // delete files in a directory that doesn't have any files, but has subdirectories
        cactus::DirectoryDelete(newpath, cactus::DELETE_DIRECTORY_FILES);
        REQUIRE(cactus::DirectoryExists(newpath));
        // create two files
        std::wstring filename1 = newpath + L"\\" + guid + L"-1.tmp";
        std::wstring filename2 = newpath + L"\\" + guid + L"-2.tmp";
        cactus::FileCreate(filename1);
        cactus::FileCreate(filename2);
        REQUIRE(cactus::FileExists(filename1));
        REQUIRE(cactus::FileExists(filename2));
        // two directories + two files
        std::list<std::wstring> files = cactus::GetFiles(newpath, L"*.*", cactus::GET_FILES_ALL);
        REQUIRE(4 == files.size());

        // try to delete effectively nothing, this will fail
        try {
            cactus::DirectoryDelete(newpath, 0);
            throw "DeleteDirectory was expected to throw std::exception";
        } catch (std::exception& ex) {
            // expected exception, this directory can't be deleted since it has files in it
            std::cout << std::endl << "Expected exception: " << ex.what();
            REQUIRE(cactus::FileExists(filename1));
            REQUIRE(cactus::FileExists(filename2));
        }

        // make sure nothing was deleted
        REQUIRE(4 == cactus::GetFiles(newpath, L"*.*", cactus::GET_FILES_ALL).size());
        // delete without deleting files, the empty subdirectories will be deleted, leaving us with 2 files
        cactus::DirectoryDelete(newpath, cactus::DELETE_DIRECTORY_EMPTY);
        REQUIRE(2 == cactus::GetFiles(newpath, L"*.*", cactus::GET_FILES_ALL).size());
        // delete with file and non-empty subdirectories
        cactus::DirectoryDelete(newpath);
        REQUIRE(! cactus::FileExists(filename1));
        REQUIRE(! cactus::FileExists(filename2));
        REQUIRE(! cactus::DirectoryExists(newpath));
    }
    SECTION("testGetFilesRecursive", "[restore_point]") {
        std::list<std::wstring> files = cactus::GetFiles(
                                            cactus::GetModuleDirectoryW(), L"*.exe", cactus::GET_FILES_ALL);
        bool found = false;

        for each(const std::wstring & file in files)
            found |= (file == cactus::GetModuleFileNameW());

        REQUIRE(found);
    }
    SECTION("testGetFilesWithWildcard", "[restore_point]") {
        std::list<std::wstring> files = cactus::GetFiles(
                                            cactus::GetModuleDirectoryW() + L"\\*.exe", cactus::GET_FILES_ALL);
        bool found = false;

        for each(const std::wstring & file in files)
            found |= (file == cactus::GetModuleFileNameW());

        REQUIRE(found);
    }
    SECTION("testGetFilesWithNoWildcard", "[restore_point]") {
        std::list<std::wstring> files = cactus::GetFiles(
                                            cactus::GetModuleFileNameW(), cactus::GET_FILES_FILES);
        REQUIRE(files.size() == 1);
        REQUIRE(* files.begin() == cactus::GetModuleFileNameW());
    }
    SECTION("testGetFilesWithInvalidWildcard", "[restore_point]") {
        //LPCWSTR file_wildcards[] =
        //{
        //  { L"x:\\temp\\*\\*.txt" },
        //  { L"x:\\temp\\?\\*.txt" },
        //  { L"x:\\temp\\?*\\*.txt" },
        //  { L"x:\\temp\\*?\\*.txt" },
        //  { L"?*\\no wildcard here" },
        //};
        //for (int i = 0; i < ARRAYSIZE(file_wildcards); i++)
        //{
        //  try
        //  {
        //      cactus::GetFiles(L"x:\\temp\\*\\*.txt", cactus::GET_FILES_FILES);
        //      throw "expected std::exception";
        //  }
        //  catch(std::exception&)
        //  {
        //      // expected
        //  }
        //}
    }
    SECTION("testDirectoryCreateMultipleSlashes", "[restore_point]") {
        std::wstring root = cactus::GetTemporaryDirectoryW();
        // check that we can create one level
        std::wstring guid = cactus::GenerateGUIDStringW();
        // append various numbers of backslashes
        std::wstring path_0 = root + guid;
        std::wstring path_1 = root + L"\\" + guid;
        std::wstring path_2 = root + L"\\\\" + guid;
        std::wcout << std::endl << L"Creating: " << path_0;
        REQUIRE(! cactus::DirectoryExists(path_0));
        REQUIRE(! cactus::DirectoryExists(path_1));
        REQUIRE(! cactus::DirectoryExists(path_2));
        REQUIRE(path_0 == cactus::DirectoryCreate(path_0));
        REQUIRE(cactus::DirectoryDelete(path_0));
        REQUIRE(path_0 == cactus::DirectoryCreate(path_1));
        REQUIRE(cactus::DirectoryDelete(path_1));
        REQUIRE(path_0 == cactus::DirectoryCreate(path_2));
        REQUIRE(cactus::DirectoryDelete(path_2));
    }
    SECTION("testDirectoryNormalize", "[restore_point]") {
        struct TestData {
            LPCWSTR path;
            LPCWSTR expected;
        };
        TestData testdata[] = {
            { L"", L"" },
            { L"C:", L"C:" },
            { L"C:\\", L"C:\\" },
            { L"C:\\\\", L"C:\\" },
            { L"C:\\path", L"C:\\path" },
            { L"D:\\\\path", L"D:\\path" },
            { L"E:\\path\\", L"E:\\path\\" },
            { L"C:\\\\path\\\\", L"C:\\path\\" },
            // { L"C:\\\\\\path\\\\", L"C:\\path\\" },
        };

        for (int i = 0; i < ARRAYSIZE(testdata); i++) {
            std::wstring normalized_path = cactus::DirectoryNormalize(testdata[i].path);
            std::wcout << std::endl << L" " << i << L": " << normalized_path;
            REQUIRE(normalized_path == testdata[i].expected);
        }
    }
}
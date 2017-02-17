// test_filesystem.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "test_filesystem.h"

#include <iostream>
#include <fstream>


#ifdef _DEBUG
    #define new DEBUG_NEW
#endif

int test_mfc_c_stduio_file();
int test_mfc_c_stduio_file_append();
int test_ostream_file();
int test_ostream_file_overide_begin();
int test_ostream_file_append();
int test_wfopen_file();
int test_wfopen_file_append();

int test_wfopen_s_file();
int test_wfopen_s_file_buffer();


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    int nRetCode = 0;

    // 初始化 MFC 并在失败时显示错误
    if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
        // TODO: 更改错误代码以符合您的需要
        _tprintf(_T("错误: MFC 初始化失败\n"));
        nRetCode = 1;
    } else {
        test_mfc_c_stduio_file();
        test_mfc_c_stduio_file_append();
        test_ostream_file();
        test_ostream_file_overide_begin();
        test_ostream_file_append();
        test_wfopen_file();
        test_wfopen_file_append();
        test_wfopen_s_file();
        test_wfopen_s_file_buffer();

        system("pause");
    }    

    return nRetCode;
}




int test_mfc_c_stduio_file()
{
    // 新建写
    CStdioFile file;
    CString filename = L"mfc_file.txt";
    CString info = L"hello world3!";

    if (file.Open((LPCTSTR)filename, CFile::modeCreate | CFile::modeWrite)) {
        file.SeekToEnd();
        file.WriteString(info);
        file.Close();
    }

    return 0;
}

int test_mfc_c_stduio_file_overide_begin()
{
    // 覆盖写文件开头
    CStdioFile file;
    CString filename = L"mfc_file_append.txt";
    CString info = L"hello world5!";

    if (file.Open((LPCTSTR)filename, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)) {
        //file.SeekToEnd();
        file.WriteString(info);
        file.Close();
    }

    return 0;
}

int test_mfc_c_stduio_file_append()
{
    // 追加写
    CStdioFile file;
    CString filename = L"mfc_file_append.txt";
    CString info = L"hello world5!";

    if (file.Open((LPCTSTR)filename, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)) {
        file.SeekToEnd();
        file.WriteString(info);
        file.Close();
    }

    return 0;
}

int test_ostream_file()
{
    // 新建写
    std::string filenmae = "ostream_file.txt";
    CStringA info = "世界，你好2!";
    std::ofstream file(filenmae.c_str(), std::ios::binary | std::ios::trunc);

    if (file.is_open()) {
        file.write((LPCSTR)info, info.GetLength());
        file.close();
    }

    return 0;
}

// 有问题
int test_ostream_file_overide_begin()
{
    // 覆盖写
    std::string filenmae = "ostream_file_overide_begin.txt";
    CStringA info = "世界，你好4!";
    std::fstream file(filenmae.c_str(), std::ios::binary | std::ios::app);

    if (file.is_open()) {
        file.seekp(0, std::ios::beg);
        file.write((LPCSTR)info, info.GetLength());
        file.close();
    }

    return 0;
}

int test_ostream_file_append()
{
    // 追加写
    std::string filenmae = "ostream_file_append.txt";
    CStringA info = "世界，你好3!";
    std::ofstream file(filenmae.c_str(), std::ios::binary | std::ios::app);

    if (file.is_open()) {
        file.write((LPCSTR)info, info.GetLength());
        file.close();
    }

    return 0;
}


int test_wfopen_file()
{
    // 新建写
    CString filename = L"_wfopen_file.txt";
    CStringA info = "世界，你好3!";
    FILE* fp_file = _wfopen((LPCWSTR)filename, L"w+b");

    if (fp_file) {
        fwrite((LPCSTR)info, 1, info.GetLength(), fp_file);
        // 使用 fwprintf 字符编码会错误
        //fwprintf(fp_file, L"%s", L"世界，你好3!");
        fflush(fp_file);
        fclose(fp_file);
    }

    return 0;
}

int test_wfopen_file_append()
{
    // 新建写
    CString filename = L"_wfopen_file_append.txt";
    CStringA info = "世界，你好3!";
    FILE* fp_file = _wfopen((LPCWSTR)filename, L"a+b");

    if (fp_file) {
        fwrite((LPCSTR)info, 1, info.GetLength(), fp_file);
        // 使用 fwprintf 字符编码会错误
        //fwprintf(fp_file, L"%s", L"世界，你好3!");
        fflush(fp_file);
        fclose(fp_file);
    }

    return 0;
}


int test_wfopen_file_read()
{
    // 新建写
    CString filename = L"_wfopen_file_append.txt";
    CStringA info = "世界，你好3!";
    FILE* fp_file = _wfopen((LPCWSTR)filename, L"a+b");

    if (fp_file) {
        fwrite((LPCSTR)info, 1, info.GetLength(), fp_file);
        // 使用 fwprintf 字符编码会错误
        //fwprintf(fp_file, L"%s", L"世界，你好3!");
        fflush(fp_file);
        fclose(fp_file);
    }

    return 0;
}

int test_wfopen_s_file()
{
    FILE *fp;
    CString filename = L"_wfopen_s_file.txt";
    CStringA info = "世界，你好3!";
    if (_wfopen_s(&fp, (LPCWSTR)filename, L"wb") == 0)
    {
        fwrite((LPCSTR)info, 1, info.GetLength(), fp);
        fflush(fp);
        fclose(fp);
    }

    return 0;
}

int test_wfopen_s_file_buffer()
{
    FILE *fp;
    CString filename = L"_wfopen_s_file.txt";
    std::string data;
    char szBuf[1024];
    if (_wfopen_s(&fp, (LPCWSTR)filename, L"rb") == 0)
    {
        // 定位到 2
        fseek(fp, 2, SEEK_SET);
        while (!feof(fp)) {
            size_t n = fread(szBuf, 1, sizeof(szBuf), fp);
            data.append(szBuf, n);
        }        
        fclose(fp);
    }

    std::cout << data.c_str() << std::endl;
    

    return 0;
}
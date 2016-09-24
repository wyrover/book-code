// Icmp.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Icmp.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

//#pragma comment(lib, "ws2_32.lib")
//#pragma comment (lib, "Mpr.Lib")
//#pragma comment (lib, "IPHlpApi.Lib")

//
//  Note!
//
//      If this DLL is dynamically linked against the MFC
//      DLLs, any functions exported from this DLL which
//      call into MFC must have the AFX_MANAGE_STATE macro
//      added at the very beginning of the function.
//
//      For example:
//
//      extern "C" BOOL PASCAL EXPORT ExportedFunction()
//      {
//          AFX_MANAGE_STATE(AfxGetStaticModuleState());
//          // normal function body here
//      }
//
//      It is very important that this macro appear in each
//      function, prior to any calls into MFC.  This means that
//      it must appear as the first statement within the
//      function, even before any object variable declarations
//      as their constructors may generate calls into the MFC
//      DLL.
//
//      Please see MFC Technical Notes 33 and 58 for additional
//      details.
//

/////////////////////////////////////////////////////////////////////////////
// CIcmpApp

BEGIN_MESSAGE_MAP(CIcmpApp, CWinApp)
    //{{AFX_MSG_MAP(CIcmpApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIcmpApp construction

CIcmpApp::CIcmpApp()
{
    hInst = NULL;
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CIcmpApp object

CIcmpApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CIcmpApp initialization

//先声明调用原ICMP.dll所需要的三个函数。
typedef HANDLE(WINAPI *ICMPCREATEFILE)(VOID);
typedef BOOL(WINAPI *ICMPCLOSEHANDLE)(HANDLE IcmpHandle);
typedef DWORD(WINAPI *ICMPSENDECHO)(
    HANDLE IcmpHandle,             //句柄
    IPAddr DestinationAddress,     //目的IP地址
    LPVOID RequestData,            //所发送的数据
    WORD RequestSize,              //所发送数据的长度
    PIP_OPTION_INFORMATION RequestOptions,//查询时所用的选项。
    LPVOID ReplyBuff,              //接收缓冲区。
    DWORD ReplySize,               //接收缓冲区的长度。
    DWORD Timeout);                //超时值。

//由于本程序也是一个DLL文件，并且为了兼容原有ICMP.dll，
//所以下面将其函数都改为另外的名称。
ICMPCREATEFILE IcmpCreateFileFromDll;
ICMPCLOSEHANDLE IcmpCloseHandleFromDll;
ICMPSENDECHO IcmpSendEchoFromDll;

BOOL CIcmpApp::InitInstance()
{
    //打开原DLL文件，注意两种错误类型不进行详细区分。
    hInst = LoadLibrary("ICMP2.dll");

    if (hInst == NULL)
        return FALSE; //库文件不存在，或不是有效的DLL文件。

    IcmpCreateFileFromDll = (ICMPCREATEFILE)GetProcAddress(hInst, "IcmpCreateFile");
    IcmpCloseHandleFromDll = (ICMPCLOSEHANDLE)GetProcAddress(hInst, "IcmpCloseHandle");
    IcmpSendEchoFromDll = (ICMPSENDECHO)GetProcAddress(hInst, "IcmpSendEcho");

    if (IcmpCreateFileFromDll == NULL || IcmpCloseHandleFromDll == NULL ||
        IcmpSendEchoFromDll == NULL)
        return FALSE; //DLL文件中不存在所需要的三个函数，可能ICMP2.dll文件格式或版本有错。

    return TRUE;
}

int CIcmpApp::ExitInstance()
{
    // TODO: Add your specialized code here and/or call the base class
    if (hInst != NULL)
        FreeLibrary(hInst);

    return CWinApp::ExitInstance();
}

HANDLE CIcmpApp::IcmpCreateFile()
{
    //打开ICMP操作。
    //AfxMessageBox("调用IcmpCreateFile函数。");
    return IcmpCreateFileFromDll();
}

BOOL CIcmpApp::IcmpCloseHandle(HANDLE IcmpHandle)
{
    //关闭ICMP操作。
    //AfxMessageBox("调用IcmpCloseHandle函数。");
    return IcmpCloseHandleFromDll(IcmpHandle);
}

DWORD CIcmpApp::IcmpSendEcho(HANDLE IcmpHandle,
                             IPAddr DestinationAddress, LPVOID RequestData,
                             WORD RequestSize, PIP_OPTION_INFORMATION RequestOptions,
                             LPVOID ReplyBuff, DWORD ReplySize,
                             DWORD Timeout)
{
    //完整的ICMP过程。
    //AfxMessageBox("调用IcmpSendEcho函数。");
    //注意下面的函数是模态的，一定要等到函数返回才有结果。
    DWORD dwRet = IcmpSendEchoFromDll(IcmpHandle, DestinationAddress, RequestData,
                                      RequestSize, RequestOptions, ReplyBuff, ReplySize, Timeout);
    CString strTemp, strTitle, strAll;
    strTitle.Format("时间,目的IP地址,回复IP地址,TTL,TOS,耗时\r\n");
    PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuff; //回复缓冲区
    CString strFilename = ".\\IcmpLog.csv";
    CFile file;
    CFileStatus fs;

    if (file.GetStatus(strFilename, fs)) {
        //如果文件存在，则打开文件。
        file.Open(strFilename, CFile::modeReadWrite, NULL);
    } else {
        //如果文件不存在，则新创建一个文件。
        file.Open(strFilename, CFile::modeCreate | CFile::modeReadWrite, NULL);
        file.Write(strTitle, strTitle.GetLength());
    }

    file.SeekToEnd();
    CTime time = CTime::GetCurrentTime();
    strAll += time.Format("%H:%M:%S,");
    struct in_addr addr;
    addr.S_un.S_addr = DestinationAddress;
    strTemp.Format("%s,", inet_ntoa(addr));
    strAll += strTemp;
    addr.S_un.S_addr = pEchoReply->Address;
    strTemp.Format("%s,", inet_ntoa(addr));
    strAll += strTemp;
    strTemp.Format("%d,", RequestOptions->Ttl);
    strAll += strTemp;
    strTemp.Format("%d,", RequestOptions->Tos);
    strAll += strTemp;

    if (dwRet != 0) {
        if (pEchoReply->RoundTripTime == 0) {
            strTemp += "小于1 ms";
        } else {
            strTemp.Format("%d ms", pEchoReply->RoundTripTime);
        }
    } else {
        strTemp.Format("超时");
    }

    strAll += strTemp + "\r\n";
    //AfxMessageBox(strTitle+"\r\n"+strAll);
    //写入日志，并关闭
    file.Write(strAll, strAll.GetLength());
    file.Close();
    return dwRet;
}


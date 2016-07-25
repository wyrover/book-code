// LogModule.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "LogModule.h"

#include "ReadLog.h"
#include "WriteLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CLogModuleApp

BEGIN_MESSAGE_MAP(CLogModuleApp, CWinApp)
	//{{AFX_MSG_MAP(CLogModuleApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogModuleApp construction

CLogModuleApp::CLogModuleApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLogModuleApp object

CLogModuleApp theApp;
volatile BOOL m_gTagThreadEnd;
CRITICAL_SECTION m_gLogCriticalSection;



extern "C" __declspec(dllexport) void readLog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CReadLog readLog;
	readLog.readLog();
	AfxEndThread(0);

}
extern "C" __declspec(dllexport) void writeLog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CWriteLog writeLog;
	writeLog.initWriteLog();
	
	writeLog.writeLog();
	AfxEndThread(0);

}
extern "C" __declspec(dllexport) void initLogModule()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	m_pConnection = pConnection;
	InitializeCriticalSection(&m_gLogCriticalSection);
	m_gTagThreadEnd = FALSE;
}


extern "C" __declspec(dllexport) void setEnd()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_gTagThreadEnd = TRUE;
}
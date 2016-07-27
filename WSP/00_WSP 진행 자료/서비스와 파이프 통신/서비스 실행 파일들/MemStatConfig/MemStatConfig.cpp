/*************************************************************************************

  SERVICE 설치 프로그램 

**************************************************************************************/
#include <windows.h>
#include <io.h>
#include "resource.h"

#define EXENAME "MemStatService.exe"
#define SRVNAME "MemStatService"
#define DISPNAME "Memory Status Log Service"

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
HINSTANCE g_hInst;
HWND hDlgMain, hStatic;

void Install();
void UnInstall();

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
         ,LPSTR lpszCmdParam,int nCmdShow)
{
    g_hInst=hInstance;
    
    DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, MainDlgProc);

    return 0;
}

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	SC_HANDLE hScm, hSrv;

    switch(iMessage)
    {
    case WM_INITDIALOG:
		SetWindowPos(hDlg,HWND_TOP,100,100,0,0,SWP_NOSIZE);
        hDlgMain = hDlg;
		hStatic=GetDlgItem(hDlg,IDC_STATIC1);

		// 서비스가 설치되어 있는지 조사해 본다.
		hScm=OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE);
		if (hScm!=NULL) {
			hSrv=OpenService(hScm,SRVNAME,SERVICE_ALL_ACCESS);
			if (hSrv!=NULL) {
				SetWindowText(hStatic,"현재 상태:설치되어 있습니다");
				CloseServiceHandle(hSrv);
			} else {
				SetWindowText(hStatic,"현재 상태:설치되지 않았습니다");
			}
			CloseServiceHandle(hScm);
		}
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
		case IDC_INSTALL:
			Install();
			return TRUE;
		case IDC_UNINSTALL:
			UnInstall();
			return TRUE;
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlgMain,0);
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

// 서비스를 설치한다.
void Install()
{
	SC_HANDLE hScm, hSrv;
	char SrvPath[MAX_PATH];
	SERVICE_DESCRIPTION lpDes;
	char Desc[1024];

	// SCM을 연다
	hScm=OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE);
	if (hScm==NULL) {
		MessageBox(hDlgMain,"SCM을 열 수 없습니다.","알림",MB_OK);
		return;
	}

	// 등록할 서비스 파일이 있는지 조사해 보고 경로를 구한다.
	GetCurrentDirectory(MAX_PATH,SrvPath);
	strcat(SrvPath, "\\");
	strcat(SrvPath, EXENAME);
	if (_access(SrvPath,0) != 0) {
		CloseServiceHandle(hScm);
		MessageBox(hDlgMain,"같은 디렉토리에 서비스 파일이 없습니다.","알림",MB_OK);
		return;
	}

	// 서비스를 등록한다. SERVICE_WIN32_OWN_PROCESS //GUI 연동 SERVICE_INTERACTIVE_PROCESS
	hSrv=CreateService(hScm,SRVNAME,DISPNAME,SERVICE_PAUSE_CONTINUE | SERVICE_CHANGE_CONFIG,
		SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,SERVICE_DEMAND_START,SERVICE_ERROR_IGNORE,SrvPath,
		NULL,NULL,NULL,NULL,NULL);
	if (hSrv==NULL) {
		MessageBox(hDlgMain,"설치하지 못했습니다.","알림",MB_OK);
	} else {
		// 설명을 등록한다.
		GetDlgItemText(hDlgMain,IDC_DESC,Desc,1024);
		lpDes.lpDescription=Desc;
		ChangeServiceConfig2(hSrv, SERVICE_CONFIG_DESCRIPTION, &lpDes);

		MessageBox(hDlgMain,"설치했습니다.","알림",MB_OK);
		SetWindowText(hStatic,"현재 상태:설치되어 있습니다");
		CloseServiceHandle(hSrv);
	}

	CloseServiceHandle(hScm);
}

// 서비스를 제거한다.
void UnInstall()
{
	SC_HANDLE hScm, hSrv;
	SERVICE_STATUS ss;

	// SCM을 연다
	hScm=OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE);
	if (hScm==NULL) {
		MessageBox(hDlgMain,"SCM을 열 수 없습니다.","알림",MB_OK);
		return;
	}

	// 서비스의 핸들을 구한다.
	hSrv=OpenService(hScm,SRVNAME,SERVICE_ALL_ACCESS);
	if (hSrv==NULL) {
		CloseServiceHandle(hScm);
		MessageBox(hDlgMain,"서비스가 설치되어 있지 않습니다.","알림",MB_OK);
		return;
	}

	// 실행중이면 중지시킨다.
	QueryServiceStatus(hSrv,&ss);
	if (ss.dwCurrentState != SERVICE_STOPPED) {
		ControlService(hSrv,SERVICE_CONTROL_STOP,&ss);
		Sleep(2000);
	}

	// 서비스 제거
	if (DeleteService(hSrv)) {
		MessageBox(hDlgMain,"서비스를 제거했습니다.","알림",MB_OK);
		SetWindowText(hStatic,"현재 상태:설치되지 않았습니다");
	} else {
		MessageBox(hDlgMain,"서비스를 제거하지 못했습니다.","알림",MB_OK);
	}
	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);
}

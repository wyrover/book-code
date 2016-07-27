/****************************************************
  [서비스 제어 프로그램 ]
******************************************************/

#include <windows.h>
#include "resource.h"

#define SRVNAME "StartService"
#define SERVICE_CONTROL_NEWFILE 128

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
HINSTANCE g_hInst;
HWND hDlgMain, hStatic;
SC_HANDLE hScm, hSrv;
SERVICE_STATUS ss;

void MemStart();
void MemControl(DWORD dwControl);
void QueryService();

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
         ,LPSTR lpszCmdParam,int nCmdShow)
{
    g_hInst=hInstance;
    
    DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, MainDlgProc);

    return 0;
}

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
    switch(iMessage)
    {
    case WM_INITDIALOG:
		SetWindowPos(hDlg,HWND_TOP,100,100,0,0,SWP_NOSIZE);
        hDlgMain = hDlg;
		hStatic=GetDlgItem(hDlg,IDC_STATIC1);

		// SCM을 전역 변수로 열어 놓는다.
		hScm=OpenSCManager(NULL,NULL,GENERIC_READ);
		if (hScm==NULL) {
			MessageBox(hDlg,"SCM을 열 수 없습니다","알림",MB_OK);
			EndDialog(hDlg,0);
		}

		// 서비스가 설치되어 있지 않으면 실행할 수 없다.
		hSrv=OpenService(hScm,SRVNAME,SERVICE_ALL_ACCESS);
		if (hSrv==NULL) {
			MessageBox(hDlg,"StartService 서비스가 설치되어 있지 않습니다","알림",MB_OK);
			EndDialog(hDlg,0);
		} else {
			CloseServiceHandle(hSrv);
		}
		QueryService();
        return TRUE;
	case WM_DESTROY:
		CloseServiceHandle(hScm);
		return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
		case IDC_START:
			MemStart();
			return TRUE;
		case IDC_STOP:
			MemControl(SERVICE_CONTROL_STOP);
			return TRUE;
		case IDC_PAUSE:
			MemControl(SERVICE_CONTROL_PAUSE);
			return TRUE;
		case IDC_CONTINUE:
			MemControl(SERVICE_CONTROL_CONTINUE);
			return TRUE;
		case IDC_NEWFILE:
			MemControl(SERVICE_CONTROL_NEWFILE);
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

// 현재 서비스 상태에 따라 버튼의 상태를 변경시킨다.
void QueryService()
{
	hSrv=OpenService(hScm,SRVNAME,SERVICE_INTERROGATE);

	do {
		ControlService(hSrv,SERVICE_CONTROL_INTERROGATE,&ss);
	} while ((ss.dwCurrentState != SERVICE_STOPPED) &&
		(ss.dwCurrentState != SERVICE_RUNNING) &&
		(ss.dwCurrentState != SERVICE_PAUSED));

	EnableWindow(GetDlgItem(hDlgMain,IDC_START),FALSE);
	EnableWindow(GetDlgItem(hDlgMain,IDC_STOP),FALSE);
	EnableWindow(GetDlgItem(hDlgMain,IDC_PAUSE),FALSE);
	EnableWindow(GetDlgItem(hDlgMain,IDC_CONTINUE),FALSE);
	EnableWindow(GetDlgItem(hDlgMain,IDC_NEWFILE),FALSE);
	switch(ss.dwCurrentState) {
	case SERVICE_STOPPED:
		EnableWindow(GetDlgItem(hDlgMain,IDC_START),TRUE);
		SetWindowText(hStatic,"현재 상태:중지");
		break;
	case SERVICE_RUNNING:
		EnableWindow(GetDlgItem(hDlgMain,IDC_STOP),TRUE);
		EnableWindow(GetDlgItem(hDlgMain,IDC_PAUSE),TRUE);
		EnableWindow(GetDlgItem(hDlgMain,IDC_NEWFILE),TRUE);
		SetWindowText(hStatic,"현재 상태:실행중");
		break;
	case SERVICE_PAUSED:
		EnableWindow(GetDlgItem(hDlgMain,IDC_STOP),TRUE);
		EnableWindow(GetDlgItem(hDlgMain,IDC_CONTINUE),TRUE);
		SetWindowText(hStatic,"현재 상태:일시중지");
		break;
	}

	CloseServiceHandle(hSrv);
}

// 서비스를 시작시킨다.
void MemStart()
{
	hSrv=OpenService(hScm,"StartService",SERVICE_START | SERVICE_QUERY_STATUS);

	// 서비스를 시작시키고 완전히 시작할 때까지 대기한다.
	SetCursor(LoadCursor(NULL,IDC_WAIT));
	if (StartService(hSrv,0,NULL)==TRUE) {
		QueryServiceStatus(hSrv, &ss);
		while (ss.dwCurrentState != SERVICE_RUNNING) {
			Sleep(ss.dwWaitHint);
			QueryServiceStatus(hSrv, &ss);
		}
	}
	SetCursor(LoadCursor(NULL,IDC_ARROW));

	CloseServiceHandle(hSrv);
	QueryService();
}

// 서비스에 제어 코드를 보낸다.
void MemControl(DWORD dwControl)
{
	hSrv=OpenService(hScm,"StartService",GENERIC_EXECUTE);

	ControlService(hSrv,dwControl,&ss);

	CloseServiceHandle(hSrv);
	QueryService();
}


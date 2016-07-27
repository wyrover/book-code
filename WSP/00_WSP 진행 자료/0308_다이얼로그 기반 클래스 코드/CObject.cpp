#include <windows.h>
#include "stdafx.h"
#include "resource.h"
#include "CObject.h"
#include "cview.h"
#include "CDocument.h"


extern CView app; // one global object
char CObject::szAppName[] = "NotePad";

void CObject::InitInstance(HINSTANCE hInstance,PSTR szCmdLine,
						int iCmdShow) 
{
	//------------ Doc 개체 생성 --------------------
	pDoc = new CDocument();

	UINT ret = DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINDLG),
			0, DlgProc);
}


void CObject::Run() 
{

}



WPARAM CObject::ExitInstance() 
{
	delete pDoc;

    return 0;
}

LRESULT CObject::OnInitDialog(HWND h, WPARAM wParam,LPARAM lParam)
{
	hwnd = h;
	SetWindowText(hwnd, szAppName);
	return 1;
}




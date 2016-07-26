/////////////////////////////////////////////////
// main.cpp文件

#include "GroupTalk.h"

#include <windows.h>
#include "resource.h"

BOOL __stdcall DlgProc(HWND, UINT, WPARAM, LPARAM);
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	int nResult = ::DialogBoxParam(
		hInstance,		// 实例句柄
		(LPCTSTR)IDD_MAIN,	// 对话框资源ID号
		NULL,			// 父窗口句柄
		DlgProc,		// 消息处理函数
		NULL); 			// 对话框初始化的值，在WM_INITDIALOG消息的lParam参数中取出

	return 0;
}

CGroupTalk *g_pTalk;

void HandleGroupMsg(HWND hDlg, GT_HDR *pHeader)
{
	switch(pHeader->gt_type)
	{
	case MT_JION:		// 新用户加入
		{
			// 显示给用户
			char szText[56];
			wsprintf(szText, " 用户：《%s》加入！", pHeader->szUser);
			::SetWindowText(::GetDlgItem(hDlg, IDC_SYSMSG), szText);

			// 将新用户信息添加到列表框中
			int nCurSel = ::SendDlgItemMessage(hDlg, IDC_USERS, CB_GETCURSEL, 0, 0);
			int nIndex = ::SendDlgItemMessage(hDlg, IDC_USERS, CB_ADDSTRING, 0, (long)pHeader->szUser);
			::SendDlgItemMessage(hDlg, IDC_USERS, CB_SETITEMDATA, nIndex, (long)pHeader->dwAddr);
			if(nCurSel == -1)
				nCurSel = nIndex;
			::SendDlgItemMessage(hDlg, IDC_USERS, CB_SETCURSEL, nCurSel, 0);
		}
		break;

	case MT_LEAVE:		// 用户离开
		{
			// 显示给用户
			char szText[56];
			wsprintf(szText, " 用户：《%s》离开！", pHeader->szUser);
			::SetWindowText(::GetDlgItem(hDlg, IDC_SYSMSG), szText);

			// 将离开的用户从列表框中移除
			int nCount = ::SendDlgItemMessage(hDlg, IDC_USERS, CB_GETCOUNT, 0, 0);
			for(int i=0; i<nCount; i++)
			{
				int nIndex = ::SendDlgItemMessage(hDlg, IDC_USERS, CB_FINDSTRING, i, (long)pHeader->szUser);
				if((DWORD)::SendDlgItemMessage(hDlg, IDC_USERS, CB_GETITEMDATA, nIndex, 0) == pHeader->dwAddr)
				{
					::SendDlgItemMessage(hDlg, IDC_USERS, CB_DELETESTRING, nIndex, 0);
					break;
				}
			}	
		}
		break;

	case MT_MESG:		// 用户发送消息
		{
			char *psz = pHeader->data();
			psz[pHeader->nDataLength] = '\0';
			char szText[1024];
			wsprintf(szText, "【%s 说】", pHeader->szUser);
			strncat(szText, psz, 1024 - strlen(szText));
			::SendDlgItemMessage(hDlg, IDC_RECORD, LB_INSERTSTRING, 0, (long)szText);
		}
		break;
	}
}

BOOL __stdcall DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{	
	case WM_INITDIALOG: 
		{
			// 创建CGroupTalk对象
			g_pTalk = new CGroupTalk(hDlg, ::inet_addr("234.5.6.7"));
			::CheckDlgButton(hDlg, IDC_SELGROUP, 1);
			::SendMessage(hDlg, WM_SETICON, ICON_SMALL, 
					(long)::LoadIcon(::GetModuleHandle(NULL), (LPCTSTR)IDI_MAIN));
		}
		break;

	case WM_GROUPTALK:
		{
			// 处理CGroupTalk对象发来的消息
			if(wParam != 0) 
				::MessageBox(hDlg, (LPCTSTR)lParam, "出错！", 0);
			else
				HandleGroupMsg(hDlg, (GT_HDR*)lParam);
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_SEND:		// 用户按下发送消息按钮
			{
				// 取得要发送的消息
				char szText[1024];
				int nLen = ::GetWindowText(::GetDlgItem(hDlg, IDC_SENDMSG), szText, 1024);
				if(nLen == 0)
					break;

				// 是面向组，还是面向用户？
				BOOL bToAll = ::IsDlgButtonChecked(hDlg, IDC_SELGROUP);
				DWORD dwAddr;
				if(bToAll)
				{
					dwAddr = 0;
				}
				else
				{
					int nIndex = ::SendDlgItemMessage(hDlg, IDC_USERS, CB_GETCURSEL, 0, 0);
					if(nIndex == -1)
					{
						::MessageBox(hDlg, "请选择一个用户！", "GroupTalk", 0);
						break;
					}
					// 取得用户IP地址
					dwAddr = ::SendDlgItemMessage(hDlg, IDC_USERS, CB_GETITEMDATA, nIndex, 0);
				}
				// 发送消息
				if(g_pTalk->SendText(szText, nLen, dwAddr) == nLen)
						::SetWindowText(::GetDlgItem(hDlg, IDC_SENDMSG), "");
			}
			break;

		case IDC_CLEAR:		// 用户按下清除按钮
				::SendDlgItemMessage(hDlg, IDC_RECORD, LB_RESETCONTENT, 0, 0);
			break;

		case IDCANCEL:		// 用户关闭程序
			{
				delete g_pTalk;
				::EndDialog (hDlg, IDCANCEL);
			}
			break;
		}
		break;
	}
	return 0;
}


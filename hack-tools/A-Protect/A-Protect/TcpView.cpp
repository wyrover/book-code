#include "stdafx.h"
#include "A-ProtectView.h"
#include "TcpView.h"
#include "A-Protect.h"

VOID Tcpview(HWND m_hWnd,ULONG ID,CMyList *m_list)
{
	DWORD dwReadByte;
	BOOL bIsConnect = FALSE;
	int i=0;

	CMyAProtectApp *imgApp=(CMyAProtectApp*)AfxGetApp();
	SHFILEINFO shfileinfo;
	TcpViewImg.Create(16,16, ILC_COLOR32, 2, 200);
	HIMAGELIST hImageList = NULL;
	bool HasFile=TRUE;

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描网络连接，请稍后...");

	int ItemNum = m_list->GetItemCount();

	TCPUDPInfo = (PTCPUDPINFO)VirtualAlloc(0, sizeof(TCPUDPINFO)*1025,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (TCPUDPInfo)
	{
		memset(TCPUDPInfo,0,sizeof(TCPUDPINFO)*1025);
		ReadFile((HANDLE)LIST_TCPUDP,TCPUDPInfo,sizeof(TCPUDPINFO)*1025,&dwReadByte,0);
		for ( i=0;i< (int)TCPUDPInfo->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",TCPUDPInfo->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzType[20];
			WCHAR lpwzConnectType[20];

			WCHAR lpwzLocalAddress[50];
			//WCHAR lpwzLocalPort[20];

			WCHAR lpwzRemoteAddress[50];
			WCHAR lpwzRemotePort[20];

			WCHAR lpwzPid[20];
			WCHAR lpwzFullPath[260];
			WCHAR lpwzFileMd5[40];
			WCHAR lpwzTrue[40];
			
			memset(lpwzType,0,sizeof(lpwzType));
			memset(lpwzConnectType,0,sizeof(lpwzConnectType));

			memset(lpwzLocalAddress,0,sizeof(lpwzLocalAddress));
			//memset(lpwzLocalPort,0,sizeof(lpwzLocalPort));

			memset(lpwzRemoteAddress,0,sizeof(lpwzRemoteAddress));
			//memset(lpwzRemotePort,0,sizeof(lpwzRemotePort));

			memset(lpwzPid,0,sizeof(lpwzPid));
			memset(lpwzFullPath,0,sizeof(lpwzFullPath));
			memset(lpwzFileMd5,0,sizeof(lpwzFileMd5));

			if (TCPUDPInfo->TCPUDP[i].ulType == 1)
			{
				wcscpy(lpwzType, L"UDP");
				wcscpy(lpwzConnectType, L"*");
				goto InsertData;
			}
			wcscpy(lpwzType, L"TCP");

			switch (TCPUDPInfo->TCPUDP[i].ulConnectType)
			{
			case MIB_TCP_STATE_CLOSED:
				wcscpy(lpwzConnectType, L"CLOSED");
				break;
			case MIB_TCP_STATE_TIME_WAIT:
				wcscpy(lpwzConnectType, L"TIME_WAIT");
				break;
			case MIB_TCP_STATE_LAST_ACK:
				wcscpy(lpwzConnectType, L"LAST_ACK");
				break;
			case MIB_TCP_STATE_CLOSING:
				wcscpy(lpwzConnectType, L"CLOSING");
				break;
			case MIB_TCP_STATE_CLOSE_WAIT:
				wcscpy(lpwzConnectType, L"CLOSE_WAIT");
				break;
			case MIB_TCP_STATE_FIN_WAIT1:
				wcscpy(lpwzConnectType, L"FIN_WAIT1");
				break;
			case MIB_TCP_STATE_ESTAB:
				wcscpy(lpwzConnectType, L"ESTABLISHED");
				bIsConnect = TRUE;
				break;
			case MIB_TCP_STATE_SYN_RCVD:
				wcscpy(lpwzConnectType, L"SYN_RCVD");
				break;
			case MIB_TCP_STATE_SYN_SENT:
				wcscpy(lpwzConnectType, L"SYN_SENT");
				break;
			case MIB_TCP_STATE_LISTEN:
				wcscpy(lpwzConnectType, L"LISTEN");
				break;
			case MIB_TCP_STATE_DELETE_TCB:
				wcscpy(lpwzConnectType, L"DELETE");
				break;
			default:
				break;
			}
InsertData:
			if (bIsConnect)
			{
				m_list->InsertItem(i,lpwzType,RGB(238,118,0));
			}else
				m_list->InsertItem(i,lpwzType,RGB(77,77,77));

			bIsConnect = FALSE;
			char lpszLocalAddr[50];
			WCHAR lpwzLocalAddr[260];

			memset(lpszLocalAddr,0,sizeof(lpszLocalAddr));
			memset(lpwzLocalAddr,0,sizeof(lpwzLocalAddr));

			struct in_addr InLocalAddr;
			memcpy(&InLocalAddr, &TCPUDPInfo->TCPUDP[i].ulLocalAddress, 4);

			wsprintfA(lpszLocalAddr,"%s", inet_ntoa(InLocalAddr));
			MultiByteToWideChar(
				CP_ACP,
				0, 
				lpszLocalAddr,
				-1, 
				lpwzLocalAddr, 
				strlen(lpszLocalAddr)
				);

			char lpszRemoteAddr[100];
			WCHAR lpwzRemoteAddr[100];

			memset(lpszRemoteAddr,0,sizeof(lpszRemoteAddr));
			memset(lpwzRemoteAddr,0,sizeof(lpwzRemoteAddr));

			struct in_addr InRemoteAddr;
			memcpy(&InRemoteAddr, &TCPUDPInfo->TCPUDP[i].ulRemoteAddress, 4);

			wsprintfA(lpszRemoteAddr,"%s", inet_ntoa(InRemoteAddr));

			MultiByteToWideChar(
				CP_ACP,
				0, 
				lpszRemoteAddr,
				-1, 
				lpwzRemoteAddr, 
				strlen(lpszRemoteAddr)
				);

			wsprintfW(lpwzLocalAddress,L"%ws:%d",lpwzLocalAddr,TCPUDPInfo->TCPUDP[i].ulLocalPort);

			wsprintfW(lpwzRemoteAddress,L"%ws:%d",lpwzRemoteAddr,TCPUDPInfo->TCPUDP[i].ulRemotePort);

			wsprintfW(lpwzPid,L"%d",TCPUDPInfo->TCPUDP[i].ulPid);

			WCHAR lpwzWinDir[256];
			WCHAR lpwzSysDisk[10];

			memset(lpwzWinDir,0,sizeof(lpwzWinDir));
			memset(lpwzSysDisk,0,sizeof(lpwzSysDisk));
			memset(lpwzFullPath,0,sizeof(lpwzFullPath));

			memset(lpwzTrue,0,sizeof(lpwzTrue));

			if (wcsstr(TCPUDPInfo->TCPUDP[i].lpwzFullPath,L"\\Device\\"))
			{
// 				GetWindowsDirectoryW(lpwzWinDir,sizeof(lpwzWinDir));
// 				memcpy(lpwzSysDisk,lpwzWinDir,6);
// 				wcsncat(lpwzFullPath,lpwzSysDisk,wcslen(lpwzSysDisk));
// 				wcsncat(lpwzFullPath+wcslen(lpwzSysDisk),TCPUDPInfo->TCPUDP[i].lpwzFullPath+wcslen(L"\\Device\\HarddiskVolume1\\"),wcslen(TCPUDPInfo->TCPUDP[i].lpwzFullPath)-wcslen(L"\\Device\\HarddiskVolume1\\"));
				NtFilePathToDosFilePath(TCPUDPInfo->TCPUDP[i].lpwzFullPath,lpwzFullPath);
				//计算md5
				char lpszFullPath[1024] = {0};
				memset(lpszFullPath,0,sizeof(lpszFullPath));
				WideCharToMultiByte( CP_ACP,
					0,
					lpwzFullPath,
					-1,
					lpszFullPath,
					wcslen(lpwzFullPath)*2,
					NULL,
					NULL);
				FILE * fp=fopen(lpszFullPath,"rb");
				if(fp)
				{
					MD5VAL val;
					val = md5File(fp);
					wsprintfW(lpwzFileMd5,L"%08x%08x%08x%08x",conv(val.a),conv(val.b),conv(val.c),conv(val.d));
					wcscat(lpwzTrue,L"-");

					FileVerify(lpszFullPath,lpwzFileMd5,lpwzTrue);
					
					fclose(fp);
				}
			}else
			{
				wcsncat(lpwzFullPath,TCPUDPInfo->TCPUDP[i].lpwzFullPath,wcslen(TCPUDPInfo->TCPUDP[i].lpwzFullPath));
				HasFile=FALSE;
			}
/*
			MultiByteToWideChar(
				CP_ACP,
				0, 
				TCPUDPInfo->TCPUDP[i].lpszFileMd5,
				-1, 
				lpwzFileMd5, 
				strlen(TCPUDPInfo->TCPUDP[i].lpszFileMd5)
				);
*/
			//m_list->InsertItem(ItemNum,lpwzType,RGB(77,77,77));
			m_list->SetItemText(i,1,lpwzLocalAddress);
			m_list->SetItemText(i,2,lpwzRemoteAddress);
			m_list->SetItemText(i,3,lpwzConnectType);
			m_list->SetItemText(i,4,lpwzPid);
			m_list->SetItemText(i,5,lpwzFullPath);
			m_list->SetItemText(i,6,lpwzTrue);

			if(HasFile)
			{
				hImageList=(HIMAGELIST)::SHGetFileInfo(lpwzFullPath,0,&shfileinfo,sizeof(shfileinfo),SHGFI_ICON);
				TcpViewImg.Add(shfileinfo.hIcon);
			}else
			{
				TcpViewImg.Add(imgApp->LoadIconW(IDI_WHITE));
			}
			m_list->SetImageList(&TcpViewImg);
			m_list->SetItemImageId(i,i);
			DestroyIcon(shfileinfo.hIcon);
			HasFile=TRUE;
		}
		VirtualFree(TCPUDPInfo,sizeof(TCPUDPINFO)*1025,MEM_RESERVE | MEM_COMMIT);
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"网络连接扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

}
BOOL IsNumber(char *lpszString)
{
	int i,x;
	BOOL bRetOK = FALSE;

	if (!strlen(lpszString))
	{
		return bRetOK;
	}
	for (i = 0;i<strlen(lpszString);i++)
	{
		bRetOK = FALSE;

		if (lpszString[i]>=0x30&&lpszString[i]<=0x39)
		{
			bRetOK = TRUE;
		}
		if (!bRetOK)
		{
			break;
		}
	}
	return bRetOK;
}
void TcpProKill(HWND m_hWnd,ULONG ID,CMyList *m_list)
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox(_T("OnMsgTcpView"));
	//m_list->DeleteAllItems();
	//Insert();
	CString PidNum;
	DWORD dwReadByte;

	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	PidNum.Format(L"%s",m_list->GetItemText(Item,4));

	WCHAR lpwzNum[10];
	char lpszNum[10];

	memset(lpszNum,0,sizeof(lpszNum));
	memset(lpwzNum,0,sizeof(lpwzNum));
	wcscat(lpwzNum,PidNum);

	WideCharToMultiByte (CP_OEMCP,NULL,lpwzNum,-1,lpszNum,wcslen(lpwzNum),NULL,FALSE);
	if (IsNumber(lpszNum))
	{
		//HWND hWnd = GetForegroundWindow();

		if (MessageBoxW(m_hWnd,L"结束进程可能导致系统或网络不稳定!!\r\n\r\n是否继续？\r\n",L"A盾电脑防护",MB_YESNO | MB_ICONWARNING) == IDYES)
		{
			//MessageBoxA(0,lpszNum,0,0);
			ReadFile((HANDLE)KILL_PROCESS_BY_PID,0,atoi(lpszNum),&dwReadByte,0);
			//Sleep(2000);
			m_list->DeleteAllItems();
			Sleep(1000);
			Tcpview(m_hWnd,ID,m_list);
		}
	}
}
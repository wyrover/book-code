#include "stdafx.h"
#include "A-ProtectView.h"
#include "FilterDriver.h"

void QueryFilterDriver(HWND m_hWnd,ULONG ID,CMyList *m_list)
{
	DWORD dwReadByte;
	int i =0;
	int ItemNum = m_list->GetItemCount();


	SHFILEINFO shfileinfo;
	DriverImg.Create(16,16, ILC_COLOR32, 2, 100);
	HIMAGELIST hImageList = NULL;

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描过滤驱动，请稍后...");

	if (KernelFilterDriver)
	{
		VirtualFree(KernelFilterDriver,(sizeof(KERNEL_FILTERDRIVER)+sizeof(KERNEL_FILTERDRIVER_INFO))*1025,MEM_RESERVE | MEM_COMMIT);
		KernelFilterDriver = 0;
	}
	KernelFilterDriver = (PKERNEL_FILTERDRIVER)VirtualAlloc(0,(sizeof(KERNEL_FILTERDRIVER)+sizeof(KERNEL_FILTERDRIVER_INFO))*1025,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (KernelFilterDriver)
	{
		memset(KernelFilterDriver,0,(sizeof(KERNEL_FILTERDRIVER)+sizeof(KERNEL_FILTERDRIVER_INFO))*1025);

		ReadFile((HANDLE)LIST_KERNEL_FILTER_DRIVER,KernelFilterDriver,(sizeof(KERNEL_FILTERDRIVER)+sizeof(KERNEL_FILTERDRIVER_INFO))*1025,&dwReadByte,0);

		for (i=0;i< KernelFilterDriver->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",KernelFilterDriver->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzObjType[256];
			WCHAR lpwzAttachDevice[256];
			WCHAR lpwzFileName[256];
			WCHAR lpwzFilePath[256];
			//宿主
			WCHAR lpwzHostFileName[256];

			memset(lpwzObjType,0,sizeof(lpwzObjType));
			memset(lpwzAttachDevice,0,sizeof(lpwzAttachDevice));
			memset(lpwzFileName,0,sizeof(lpwzFileName));
			memset(lpwzFilePath,0,sizeof(lpwzFilePath));
			memset(lpwzHostFileName,0,sizeof(lpwzHostFileName));

			if (wcsstr(KernelFilterDriver->KernelFilterDriverInfo[i].HostFileName,L"\\FileSystem\\") != NULL)
			{
				wcscat(lpwzObjType,L"File");
			}
			if (wcsstr(KernelFilterDriver->KernelFilterDriverInfo[i].HostFileName,L"\\Driver\\") != NULL)
			{
				wcsncat(lpwzObjType,KernelFilterDriver->KernelFilterDriverInfo[i].HostFileName+wcslen(L"\\Driver\\"),wcslen(KernelFilterDriver->KernelFilterDriverInfo[i].HostFileName)-wcslen(L"\\Driver\\"));
			}
			wsprintfW(lpwzAttachDevice,L"0x%X",KernelFilterDriver->KernelFilterDriverInfo[i].ulAttachDevice);
			wsprintfW(lpwzFileName,L"%ws",KernelFilterDriver->KernelFilterDriverInfo[i].FileName);
			wsprintfW(lpwzFilePath,L"%ws",KernelFilterDriver->KernelFilterDriverInfo[i].FilePath);
			wsprintfW(lpwzHostFileName,L"%ws",KernelFilterDriver->KernelFilterDriverInfo[i].HostFileName);

			WCHAR lpwzDosFullPath[256];
			WCHAR lpwzWinDir[256];
			WCHAR lpwzSysDisk[256];

			memset(lpwzWinDir,0,sizeof(lpwzWinDir));
			memset(lpwzSysDisk,0,sizeof(lpwzSysDisk));
			memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));

			GetWindowsDirectoryW(lpwzWinDir,sizeof(lpwzWinDir));
			memcpy(lpwzSysDisk,lpwzWinDir,4);

			if (wcsstr(lpwzFilePath,L"\\??\\"))
			{
				//开始这种路径的处理
				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
				wcsncpy(lpwzDosFullPath,lpwzFilePath+wcslen(L"\\??\\"),wcslen(lpwzFilePath)-wcslen(L"\\??\\"));
				goto Next;
			}
			if (wcsstr(lpwzFilePath,L"\\WINDOWS\\system32\\"))
			{
				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
				wcscat(lpwzDosFullPath,lpwzSysDisk);
				wcscat(lpwzDosFullPath,lpwzFilePath);
				//MessageBoxW(lpwzDosFullPath,lpwzFullSysName,0);
				goto Next;
			}
			if (wcsstr(lpwzFilePath,L"\\SystemRoot\\"))
			{
				WCHAR lpwzTemp[256];
				memset(lpwzTemp,0,sizeof(lpwzTemp));
				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
				wcscat(lpwzTemp,lpwzSysDisk);
				wcscat(lpwzTemp,L"\\WINDOWS\\");
				wcscat(lpwzDosFullPath,lpwzTemp);
				wcsncpy(lpwzDosFullPath+wcslen(lpwzTemp),lpwzFilePath+wcslen(L"\\SystemRoot\\"),wcslen(lpwzFilePath) - wcslen(L"\\SystemRoot\\"));
				goto Next;
			}
			//if (wcslen(lpwzHookModuleImage) == wcslen(lpwzHookModuleImage))
			//{
			memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
			wcscat(lpwzDosFullPath,lpwzSysDisk);
			wcscat(lpwzDosFullPath,L"\\WINDOWS\\system32\\drivers\\");
			wcscat(lpwzDosFullPath,lpwzFilePath);
			goto Next;
			//}
Next:
			m_list->InsertItem(i,lpwzObjType,RGB(77,77,77));
			m_list->SetItemText(i,1,lpwzFileName);
			m_list->SetItemText(i,2,lpwzAttachDevice);
			m_list->SetItemText(i,3,lpwzHostFileName);
			m_list->SetItemText(i,4,lpwzDosFullPath);

			hImageList=(HIMAGELIST)::SHGetFileInfo(lpwzDosFullPath,0,&shfileinfo,sizeof(shfileinfo),SHGFI_ICON);
			DriverImg.Add(shfileinfo.hIcon);
			m_list->SetImageList(&DriverImg);
			m_list->SetItemImageId(i,i);
			DestroyIcon(shfileinfo.hIcon);
		}
	}else{
		WCHAR lpwzTextOut[100];
		memset(lpwzTextOut,0,sizeof(lpwzTextOut));
		wsprintfW(lpwzTextOut,L"申请内存错误， 请重新运行A盾\r\n错误代码:%d\n",GetLastError());
		MessageBox(0,lpwzTextOut,0,0);
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"过滤驱动扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

}
void DeleteSelectFilterDriver(HWND hWnd,ULONG ID,CMyList *m_list)
{
	DWORD dwReadByte;
	CString Num;
	CString FunctionStr;

	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	FunctionStr.Format(L"%s",m_list->GetItemText(Item,3));

	WCHAR lpwzFilter[256];

	memset(lpwzFilter,0,sizeof(lpwzFilter));
	wcscat(lpwzFilter,FunctionStr);
	ReadFile((HANDLE)INIT_KERNEL_FILTER_DRIVER,lpwzFilter,wcslen(lpwzFilter),&dwReadByte,0);

	WCHAR lpwzNum1[20];
	WCHAR lpwzNum[20];
	char lpszNum[20];

	WCHAR lpwzDeviceObj[20];
	//取设备
	Num.Format(L"%s",m_list->GetItemText(Item,2));

	memset(lpwzNum,0,sizeof(lpwzNum));

	wcscat(lpwzNum,Num);

	for (int i=0;i< KernelFilterDriver->ulCount;i++)
	{
		memset(lpwzDeviceObj,0,sizeof(lpwzDeviceObj));
		wsprintfW(lpwzDeviceObj,L"0x%X",KernelFilterDriver->KernelFilterDriverInfo[i].ulAttachDevice);
		if (_wcsnicmp(lpwzNum,lpwzDeviceObj,wcslen(lpwzDeviceObj)) == 0)
		{
			ReadFile((HANDLE)DELETE_KERNEL_FILTER_DRIVER,0,KernelFilterDriver->KernelFilterDriverInfo[i].ulAttachDevice,&dwReadByte,0);
			break;
		}
	}
	m_list->DeleteAllItems();
	QueryFilterDriver(hWnd,ID,m_list);
}
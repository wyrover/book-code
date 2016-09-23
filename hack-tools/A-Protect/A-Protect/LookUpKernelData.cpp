// LookUpKernelData.cpp : 实现文件
//

#include "stdafx.h"
#include "A-Protect.h"
#include "LookUpKernelData.h"
#include "afxdialogex.h"

#include <math.h>

typedef struct _KERNEL_DATA {

	ULONG ulAddress;

	ULONG ulStack1;
	ULONG ulStack2;
	ULONG ulStack3;
	ULONG ulStack4;

} KERNEL_DATA, *PKERNEL_DATA;

typedef struct _LOOKUP_KERNEL_DATA {          //KernelData
	ULONG ulCount;
	KERNEL_DATA KernelData[1];
} LOOKUP_KERNEL_DATA, *PLOOKUP_KERNEL_DATA;

PLOOKUP_KERNEL_DATA LookupKernelData;

ULONG GetKernelInfo(char *lpKernelName,ULONG *ulBase,ULONG *ulSize);

// CLookUpKernelData 对话框

IMPLEMENT_DYNAMIC(CLookUpKernelData, CDialogEx)

CLookUpKernelData::CLookUpKernelData(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLookUpKernelData::IDD, pParent)
	, m_strLookUpSize(_T(""))
	, m_strKernelData(_T(""))
{

}

CLookUpKernelData::~CLookUpKernelData()
{
}

void CLookUpKernelData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_COMBO_KERNELBASE, m_ccbKernelBase);
	DDX_Text(pDX, IDC_COMBO_KERNELBASE, m_ccbKernelBase);

	DDX_Text(pDX, IDC_EDIT_LOOKUPSIZE, m_strLookUpSize);
	DDX_Text(pDX, IDC_EDIT_SHOWKERNELDATA, m_strKernelData);
}


BEGIN_MESSAGE_MAP(CLookUpKernelData, CDialogEx)
	ON_BN_CLICKED(ID_BTNOK, &CLookUpKernelData::OnBnClickedBtnok)
END_MESSAGE_MAP()


// CLookUpKernelData 消息处理程序


BOOL CLookUpKernelData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_strKernelData=L"";
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

DWORD StringToHex(char* strSource)
{ 
	DWORD nTemp=0;
	char strTemp[64];

	if( strSource == NULL )
		return -1;

	memcpy(strTemp,strSource,64);

	for(char cc='G',dd='g';cc<='Z',dd<='z';  cc++,dd++)    //判断输入的字符串是否合法
	{
		if( strchr(strTemp,cc) != NULL || strchr(strTemp,dd) != NULL )
		{
			return -1;
		}
	}

	for(int i = 0;  i < (int)::strlen(strSource);  i++)
	{
		int nDecNum;
		switch(strSource[i])
		{
		case 'a':
		case 'A':
			nDecNum = 10;
			break;
		case 'b':
		case 'B':
			nDecNum = 11;
			break;
		case 'c':
		case 'C':
			nDecNum = 12;
			break;
		case 'd':
		case 'D':
			nDecNum = 13;
			break;
		case 'e':
		case 'E':
			nDecNum = 14;
			break;
		case 'f':
		case 'F':
			nDecNum = 15;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			nDecNum = strSource[i] - '0';
			break;
		default:
			return 0;   
		}
		nTemp += (WORD)nDecNum * (double)::pow((double)16,(int)strlen(strSource)-i-1);
	}
	return nTemp;
}
void CLookUpKernelData::OnBnClickedBtnok()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString text;
	//m_ccbKernelBase.GetLBText(m_ccbKernelBase.GetCurSel(),text);
	UpdateData();//窗口数据更新到CString

	ULONG ulBase;
	ULONG ulSize;
	char lpszKernelPath[260] = {0};
	DWORD dwReadByte;
	int i =0;
	CString m_str;
	CString m_strKernel;

	char lpszLookupBase[100] = {0};
	char lpszLookupSize[100] = {0};

	ULONG ulLookupBase;
	ULONG ulLookupSize;

	m_strKernel=L"";

	memset(lpszLookupBase,0,sizeof(lpszLookupBase));
	memset(lpszLookupSize,0,sizeof(lpszLookupSize));
	memset(lpszKernelPath,0,sizeof(lpszKernelPath));

	if (GetKernelInfo(lpszKernelPath,&ulBase,&ulSize))
	{
		LookupKernelData = (PLOOKUP_KERNEL_DATA)VirtualAlloc(0,ulSize+1024,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (LookupKernelData)
		{
			memset(LookupKernelData,0,ulSize+1024);

			WideCharToMultiByte( CP_ACP,
				0,
				m_ccbKernelBase,
				-1,
				lpszLookupBase,
				wcslen(m_ccbKernelBase)*2,
				NULL,
				NULL
				);
			WideCharToMultiByte( CP_ACP,
				0,
				m_strKernel+m_strLookUpSize,
				-1,
				lpszLookupSize,
				wcslen(m_strKernel+m_strLookUpSize)*2,
				NULL,
				NULL
				);
			ulLookupBase = StringToHex(lpszLookupBase);
			ulLookupSize = StringToHex(lpszLookupSize);
			if (ulLookupBase == -1 ||
				ulLookupSize == -1)
			{
				WCHAR lpTest[50] = {0};
				wsprintfW(lpTest,L"\r\n请输入正确的16进制 --> Base:80800000 Size:100\r\n当前错误输入 --> Base:%08x Size:%08x\r\n\r\n",ulLookupBase,ulLookupSize);
				m_str.Format(lpTest);
				m_strKernelData+=m_str;
				UpdateData(false);//这个用来刷新数据，可以加到循环外

				VirtualFree(LookupKernelData,ulSize+1024,MEM_RESERVE | MEM_COMMIT);
				return;
			}
// 			char lptest[50] = {0};
// 			wsprintfA(lptest,"%08x %08x\n",ulLookupBase,ulLookupSize);
// 			MessageBoxA(0,lptest,0,0);

			ReadFile((HANDLE)INIT_KERNEL_DATA_BASE,0,ulLookupBase,&dwReadByte,0);
			ReadFile((HANDLE)INIT_KERNEL_DATA_SIZE,0,ulLookupSize,&dwReadByte,0);

			ReadFile((HANDLE)LIST_KERNEL_DATA,LookupKernelData,ulSize+1024,&dwReadByte,0);
			if (LookupKernelData->ulCount > 1)
			{
				WCHAR *lpForMat = L"******************************************************\r\n"
					              L"Base:0x%08X Size:0x%X\r\n\r\n"; 

				WCHAR lpBuffer[260] = {0};
				wsprintfW(lpBuffer,lpForMat,ulLookupBase,ulLookupSize);
				m_str.Format(lpBuffer);
				m_strKernelData+=m_str;
				UpdateData(false);

			}else{
				m_str.Format(L"\r\n起始地址不可访问!\r\n\r\n");
				m_strKernelData+=m_str;
				UpdateData(false);
				VirtualFree(LookupKernelData,ulSize+1024,MEM_RESERVE | MEM_COMMIT);
				return;
			}
			for (i=0;i< (int)LookupKernelData->ulCount;i++)
			{
				WCHAR *lpForMat1 = L"0x%08x  %08x %08x %08x %08x\r\n";
				WCHAR lpBuffer1[260] = {0};

				wsprintfW(lpBuffer1,lpForMat1,
					LookupKernelData->KernelData[i].ulAddress,
					LookupKernelData->KernelData[i].ulStack1,
					LookupKernelData->KernelData[i].ulStack2,
					LookupKernelData->KernelData[i].ulStack3,
					LookupKernelData->KernelData[i].ulStack4);
				m_str.Format(lpBuffer1);
				m_strKernelData+=m_str;
				UpdateData(false);
			}
			VirtualFree(LookupKernelData,ulSize+1024,MEM_RESERVE | MEM_COMMIT);
		}
	}
}

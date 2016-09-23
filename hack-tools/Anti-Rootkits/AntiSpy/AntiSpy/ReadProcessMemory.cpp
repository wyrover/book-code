
#include "stdafx.h"
#include "AntiSpy.h"
#include "ReadProcessMemory.h"
#include "IOControlCode.h"
#include "afxdialogex.h"

#define  READ_MEMORY 10

IMPLEMENT_DYNAMIC(CReadProcessMemory, CDialog)

CReadProcessMemory::CReadProcessMemory(CWnd* pParent)
	: CDialog(CReadProcessMemory::IDD, pParent)
	, m_Address(_T(""))
	, m_Length(0)
	, m_ProcessMemoryData(_T(""))
	, m_Binary(_T(""))
	, m_Address_Edit(_T(""))
{

}

CReadProcessMemory::~CReadProcessMemory()
{
}

void CReadProcessMemory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_Address);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_Length);
	DDX_Text(pDX, IDC_EDIT_DATA, m_ProcessMemoryData);
	DDX_Text(pDX, IDC_EDIT_BINARY, m_Binary);
	DDX_Text(pDX, IDC_EDIT_ADDRESS_EDIT, m_Address_Edit);
	DDX_Control(pDX, IDC_EDIT_BINARY, m_Address_Control);
}


BEGIN_MESSAGE_MAP(CReadProcessMemory, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_READ, &CReadProcessMemory::OnBnClickedButtonRead)
END_MESSAGE_MAP()





void CReadProcessMemory::OnBnClickedButtonRead()
{
	ULONG_PTR  ulControlCode = READ_MEMORY;
	SendIoControlCode(ulControlCode);
}


BOOL CReadProcessMemory::SendIoControlCode(ULONG_PTR ulControlCode)
{

	if (ulControlCode==READ_MEMORY)
	{
		if(ReadMemory()==TRUE)
		{
			UpdateData(FALSE);
		}
	}

	return TRUE;
}



BOOL CReadProcessMemory::ReadMemory()
{
	UpdateData(TRUE);

	struct  
	{
		ULONG_PTR  ulBase;
		ULONG_PTR  ulSize;
		ULONG      ulProcessID;
	}Data;

	BOOL bRet = FALSE;
	DWORD ulReturnSize = 0;
	int   temp = 0;
	int iTemp  = 0;
	int   v1 = 15;
	int   v2 = 15;
	char* szBuffer = NULL;
	UCHAR* szBinary = NULL;

	memset(&Data,0,sizeof(Data));


	swscanf(m_Address.GetBuffer()+2,L"%p",&Data.ulBase);
	swscanf(m_Address.GetBuffer()+2,L"%p",&iTemp);

	Data.ulSize = m_Length;
	Data.ulProcessID = m_ProcessID;

	
	szBuffer = (CHAR*)malloc(m_Length);
	szBinary = (UCHAR*)malloc(m_Length);

	memset(szBinary,0,m_Length);
	memset(szBuffer,0,m_Length);

	bRet =   DeviceIoControl(g_hDevice,CTL_READROCESSMEMORY,
		&Data,
		sizeof(Data),
		szBuffer,
		m_Length,
		&ulReturnSize,
		NULL);

	memcpy(szBinary,szBuffer,m_Length);

	
	

	for (int i=0;i<m_Length;i++)
	{
		CString strTemp;
		if(szBuffer[i]=='\0')
		{
			m_Binary += "00";
			m_Binary += " ";

			if(i!=0&&i%v1==0)
			{
				m_Binary += "\r\n";
				v1 += 16;
			}
			continue;
		}
		strTemp.Format(L"%1x",szBinary[i]);

		swscanf(strTemp,L"%x",&temp);

		if(temp>=0&&temp<=15)
		{
			strTemp.Format(L"0%1x",szBinary[i]);
		}
		m_Binary += strTemp;
		m_Binary += " ";

		if(i!=0&&i%v1==0)
		{
			m_Binary += "\r\n";
			v1 += 16;
		}

	}


	for (int i=0;i<m_Length;i++)
	{
		if(szBuffer[i]=='\0')
		{
			m_ProcessMemoryData += ".";
			m_ProcessMemoryData += " ";

			continue;
		}

		m_ProcessMemoryData += szBuffer[i];
	}
		
		
/*
	for (int i=0;i<m_Address_Control.GetLineCount();i++)
	{
		CString str;
		str.Format(L"0x%p",iTemp);
		if(i==0)
		{
			m_Address_Edit.Format(L"0x%p",iTemp);
		}

		m_Address_Edit += "\r\n";
		m_Address_Edit += str;

		iTemp += 16;
	}*/

	if(szBuffer!=NULL)
	{
		free(szBuffer);
		szBuffer = NULL;
	}

	return TRUE;
}



// lassNetSetting.cpp: implementation of the ClassNetSetting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GetNetSetting.h"
#include "lassNetSetting.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ClassNetSetting::ClassNetSetting()
{
   	m_TotalNetCards = 0;
	_tcscpy(m_szDomain,_T(""));
	_tcscpy(m_szHostName,_T(""));
	m_IPEnableRouter = 2;
	m_EnableDNS = 2;
	m_SystemType = Unknow;
}

ClassNetSetting::~ClassNetSetting()
{

}

BOOL ClassNetSetting::GetSetting()
{
    m_SystemType = GetSystemType();
	
	if (m_SystemType == Windows9X)
		return GetSettingOfWin9X();
	else if(m_SystemType == WinNT4orHigher)
		return GetSettingOfWinNT();
	else//不支持老旧的操作系统
		return FALSE;
}

Win32Type ClassNetSetting::GetSystemType()
{
    Win32Type  SystemType;
	DWORD winVer;
	OSVERSIONINFO *osvi;
	
	winVer = GetVersion();
	if(winVer < 0x80000000)
	{
		/*NT */
		SystemType = WinNT3;
		osvi = (OSVERSIONINFO *)malloc(sizeof(OSVERSIONINFO));
		if (osvi != NULL)
		{
			memset(osvi,0,sizeof(OSVERSIONINFO));
			osvi->dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			GetVersionEx(osvi);
			if (osvi->dwMajorVersion >= 4L)
				SystemType = WinNT4orHigher;//yup, it is NT 4 or higher!
			free(osvi);
		}
	}
	else if  (LOBYTE(LOWORD(winVer)) < 4)
		SystemType=Win32s;/*Win32s*/
	else
		SystemType=Windows9X;/*Windows9X*/
	return SystemType;
}

BOOL ClassNetSetting::GetSettingOfWin9X()
{
    LONG lRet;
	HKEY hMainKey;
	TCHAR szNameServer[256];
	
//得到域名，网关和DNS的设置
	lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("System\\CurrentControlSet\\Services\\VxD\\MSTCP"),0,KEY_READ,&hMainKey);
	if(lRet == ERROR_SUCCESS)
	{
		DWORD dwType,dwDataSize = 256;
		::RegQueryValueEx(hMainKey,_T("Domain"),NULL,&dwType,(LPBYTE)m_szDomain,&dwDataSize);
		dwDataSize = 256;
		::RegQueryValueEx(hMainKey,_T("Hostname"),NULL,&dwType,(LPBYTE)m_szHostName,&dwDataSize);
		dwDataSize = 256;
		::RegQueryValueEx(hMainKey,_T("EnableDNS"),NULL,&dwType,(LPBYTE)&m_EnableDNS,&dwDataSize);
		dwDataSize = 256;
		::RegQueryValueEx(hMainKey,_T("NameServer"),NULL,&dwType,(LPBYTE)szNameServer,&dwDataSize);
	}
	::RegCloseKey(hMainKey);

	HKEY hNetCard = NULL;
//调用CTcpCfg类的静态函数得到网卡的数目和相应的MAC地址
	m_TotalNetCards = GetMacAddress(m_MacAddr);

	lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("System\\CurrentControlSet\\Services\\Class\\Net"),0,KEY_READ,&hNetCard);
	if(lRet != ERROR_SUCCESS)//此处失败就返回
	{
		if(hNetCard != NULL)
			::RegCloseKey(hNetCard);
		return FALSE;
	}

	DWORD dwSubKeyNum = 0,dwSubKeyLen = 256;
//得到子键的个数，通常与网卡个数相等
	lRet = ::RegQueryInfoKey(hNetCard,NULL,NULL,NULL,&dwSubKeyNum,&dwSubKeyLen,
		NULL,NULL,NULL,NULL,NULL,NULL);
	if(lRet == ERROR_SUCCESS)
	{
//		m_TotalNetCards = dwSubKeyNum;//网卡个数以此为主
		LPTSTR lpszKeyName = new TCHAR[dwSubKeyLen + 1];
		DWORD dwSize;
		for(int i = 0; i < (int)m_TotalNetCards; i++)
		{
			TCHAR szNewKey[256];
			HKEY hNewKey;
			DWORD dwType = REG_SZ,dwDataSize = 256;
			dwSize = dwSubKeyLen + 1;
			lRet = ::RegEnumKeyEx(hNetCard,i,lpszKeyName,&dwSize,NULL,NULL,NULL,NULL);
			if(lRet == ERROR_SUCCESS)
			{
				lRet = ::RegOpenKeyEx(hNetCard,lpszKeyName,0,KEY_READ,&hNewKey);
				if(lRet == ERROR_SUCCESS)
					::RegQueryValueEx(hNewKey,_T("DriverDesc"),NULL,&dwType,(LPBYTE)m_Cards[i].szDescription,&dwDataSize);
				::RegCloseKey(hNewKey);
				wsprintf(szNewKey,_T("System\\CurrentControlSet\\Services\\Class\\NetTrans\\%s"),lpszKeyName);
				lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,szNewKey,0,KEY_READ,&hNewKey);
				if(lRet == ERROR_SUCCESS)
				{
					dwDataSize = 256;
					::RegQueryValueEx(hNewKey,_T("DefaultGateway"),NULL,&dwType,(LPBYTE)m_Cards[i].szGateWay,&dwDataSize);
					ProcessMultiString(m_Cards[i].szGateWay,dwDataSize);
					dwDataSize = 256;
					::RegQueryValueEx(hNewKey,_T("IPAddress"),NULL,&dwType,(LPBYTE)m_Cards[i].szIpAddress,&dwDataSize);
					ProcessMultiString(m_Cards[i].szIpAddress,dwDataSize);
					dwDataSize = 256;
					::RegQueryValueEx(hNewKey,_T("IPMask"),NULL,&dwType,(LPBYTE)m_Cards[i].szIpMask,&dwDataSize);
					ProcessMultiString(m_Cards[i].szIpMask,dwDataSize);
//拷贝前面得到的DNS主机名
					_tcscpy(m_Cards[i].szDNSNameServer,szNameServer);
				}
				::RegCloseKey(hNewKey);
			}
			m_Cards[i].szMacAddr[0] = m_MacAddr[i].b1;
			m_Cards[i].szMacAddr[1] = m_MacAddr[i].b2;
			m_Cards[i].szMacAddr[2] = m_MacAddr[i].b3;
			m_Cards[i].szMacAddr[3] = m_MacAddr[i].b4;
			m_Cards[i].szMacAddr[4] = m_MacAddr[i].b5;
			m_Cards[i].szMacAddr[5] = m_MacAddr[i].b6;
		}
	}
	::RegCloseKey(hNetCard);
	return lRet == ERROR_SUCCESS ? TRUE : FALSE;
}

int ClassNetSetting::GetMacAddress(LPMAC_ADDRESS pMacAddr)
{
    NCB ncb; 
	UCHAR uRetCode;
	int num = 0;
	LANA_ENUM lana_enum; 
	memset(&ncb, 0, sizeof(ncb) ); 
	ncb.ncb_command = NCBENUM; 
	ncb.ncb_buffer = (unsigned char *)&lana_enum; 
	ncb.ncb_length = sizeof(lana_enum); 
	//向网卡发送NCBENUM命令,以获取当前机器的网卡信息,如有多少个网卡
	//每张网卡的编号等 
	uRetCode = Netbios(&ncb);
	if (uRetCode == 0) 
	{
		num = lana_enum.length;
//对每一张网卡,以其网卡编号为输入编号,获取其MAC地址 
		for (int i = 0; i < num; i++)
		{
			ASTAT Adapter;
			if(GetAddressByIndex(lana_enum.lana[i],Adapter) == 0)
			{
				pMacAddr[i].b1 = Adapter.adapt.adapter_address[0];
				pMacAddr[i].b2 = Adapter.adapt.adapter_address[1];
				pMacAddr[i].b3 = Adapter.adapt.adapter_address[2];
				pMacAddr[i].b4 = Adapter.adapt.adapter_address[3];
				pMacAddr[i].b5 = Adapter.adapt.adapter_address[4];
				pMacAddr[i].b6 = Adapter.adapt.adapter_address[5];
			}
		}
	}
	return num;
}

BOOL ClassNetSetting::GetSettingOfWinNT()
{
   LONG lRtn;
	HKEY hMainKey;
	TCHAR szParameters[256];
//获得域名，主机名和是否使用IP路由
	_tcscpy(szParameters,_T("SYSTEM\\ControlSet001\\Services\\Tcpip\\Parameters"));
	lRtn = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,szParameters,0,KEY_READ,&hMainKey);
	if(lRtn == ERROR_SUCCESS)
	{
		DWORD dwType,dwDataSize = 256;
		::RegQueryValueEx(hMainKey,_T("Domain"),NULL,&dwType,(LPBYTE)m_szDomain,&dwDataSize);
		dwDataSize = 256;
		::RegQueryValueEx(hMainKey,_T("Hostname"),NULL,&dwType,(LPBYTE)m_szHostName,&dwDataSize);
		dwDataSize = 256;
//		BYTE szBuf[16];
		::RegQueryValueEx(hMainKey,_T("IPEnableRouter"),NULL,&dwType,(LPBYTE)&m_IPEnableRouter,&dwDataSize);
	}
	::RegCloseKey(hMainKey);

//获得IP地址和DNS解析等其他设置
	HKEY hNetCard = NULL;
	m_TotalNetCards = GetMacAddress(m_MacAddr);

	lRtn = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards"),0,KEY_READ,&hNetCard);
	if(lRtn != ERROR_SUCCESS)//此处失败就返回
	{
		if(hNetCard != NULL)
			::RegCloseKey(hNetCard);
		return FALSE;
	}
	
	DWORD dwSubKeyNum = 0,dwSubKeyLen = 256;
//得到子键的个数，通常与网卡个数相等
	lRtn = ::RegQueryInfoKey(hNetCard,NULL,NULL,NULL,&dwSubKeyNum,&dwSubKeyLen,
		NULL,NULL,NULL,NULL,NULL,NULL);
	if(lRtn == ERROR_SUCCESS)
	{
		m_TotalNetCards = dwSubKeyNum;//网卡个数以此为主
		LPTSTR lpszKeyName = new TCHAR[dwSubKeyLen + 1];
		DWORD dwSize;
		for(int i = 0; i < (int)dwSubKeyNum; i++)
		{
			TCHAR szServiceName[256];
			HKEY hNewKey;
			DWORD dwType = REG_SZ,dwDataSize = 256;
			dwSize = dwSubKeyLen + 1;
			::RegEnumKeyEx(hNetCard,i,lpszKeyName,&dwSize,NULL,NULL,NULL,NULL);
			lRtn = ::RegOpenKeyEx(hNetCard,lpszKeyName,0,KEY_READ,&hNewKey);
			if(lRtn == ERROR_SUCCESS)
			{
				lRtn = ::RegQueryValueEx(hNewKey,_T("Description"),NULL,&dwType,(LPBYTE)m_Cards[i].szDescription,&dwDataSize);
				dwDataSize = 256;
				lRtn = ::RegQueryValueEx(hNewKey,_T("ServiceName"),NULL,&dwType,(LPBYTE)szServiceName,&dwDataSize);
				if(lRtn == ERROR_SUCCESS)
				{
					TCHAR szNewKey[256];
					wsprintf(szNewKey,_T("%s\\Interfaces\\%s"),szParameters,szServiceName);
					HKEY hTcpKey;
					lRtn = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,szNewKey,0,KEY_READ,&hTcpKey);
					if(lRtn == ERROR_SUCCESS)
					{
						dwDataSize = 256;
						::RegQueryValueEx(hTcpKey,_T("DefaultGateway"),NULL,&dwType,(LPBYTE)m_Cards[i].szGateWay,&dwDataSize);
						ProcessMultiString(m_Cards[i].szGateWay,dwDataSize);
						dwDataSize = 256;
						::RegQueryValueEx(hTcpKey,_T("IPAddress"),NULL,&dwType,(LPBYTE)m_Cards[i].szIpAddress,&dwDataSize);
						ProcessMultiString(m_Cards[i].szIpAddress,dwDataSize);
						dwDataSize = 256;
						::RegQueryValueEx(hTcpKey,_T("SubnetMask"),NULL,&dwType,(LPBYTE)m_Cards[i].szIpMask,&dwDataSize);
						ProcessMultiString(m_Cards[i].szIpMask,dwDataSize);
						dwDataSize = 256;
						::RegQueryValueEx(hTcpKey,_T("NameServer"),NULL,&dwType,(LPBYTE)m_Cards[i].szDNSNameServer,&dwDataSize);
					}
					::RegCloseKey(hTcpKey);
				}
			}
			::RegCloseKey(hNewKey);
			m_Cards[i].szMacAddr[0] = m_MacAddr[i].b1;
			m_Cards[i].szMacAddr[1] = m_MacAddr[i].b2;
			m_Cards[i].szMacAddr[2] = m_MacAddr[i].b3;
			m_Cards[i].szMacAddr[3] = m_MacAddr[i].b4;
			m_Cards[i].szMacAddr[4] = m_MacAddr[i].b5;
			m_Cards[i].szMacAddr[5] = m_MacAddr[i].b6;
		}
		delete[] lpszKeyName;
	}
	::RegCloseKey(hNetCard);
	return lRtn == ERROR_SUCCESS ? TRUE : FALSE;
}


UCHAR ClassNetSetting::GetAddressByIndex(int lana_num, ASTAT &Adapter)
{
   NCB ncb; 
	UCHAR uRetCode; 
	memset(&ncb, 0, sizeof(ncb) ); 
	ncb.ncb_command = NCBRESET; 
	ncb.ncb_lana_num = lana_num; 
//指定网卡号,首先对选定的网卡发送一个NCBRESET命令,以便进行初始化 
	uRetCode = Netbios(&ncb ); 
	memset(&ncb, 0, sizeof(ncb) ); 
	ncb.ncb_command = NCBASTAT; 
	ncb.ncb_lana_num = lana_num;   //指定网卡号 
	strcpy((char *)ncb.ncb_callname,"*      " ); 
	ncb.ncb_buffer = (unsigned char *)&Adapter; 
//指定返回的信息存放的变量 
	ncb.ncb_length = sizeof(Adapter); 
//接着,可以发送NCBASTAT命令以获取网卡的信息 
	uRetCode = Netbios(&ncb ); 
	return uRetCode;
} 

void ClassNetSetting::ProcessMultiString(LPTSTR lpszString, DWORD dwSize)
{ 
   for(int i = 0; i < int(dwSize - 2); i++)
	{
		if(lpszString[i] == _T('\0'))
			lpszString[i] = _T(',');
	}
}

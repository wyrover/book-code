// lassNetSetting.h: interface for the ClassNetSetting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LASSNETSETTING_H__16D8BE23_3C61_4511_B8BB_75E9264B515B__INCLUDED_)
#define AFX_LASSNETSETTING_H__16D8BE23_3C61_4511_B8BB_75E9264B515B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Nb30.h"

//操作系统类型
enum Win32Type{
	Unknow,
	Win32s,
	Windows9X,
	WinNT3,
	WinNT4orHigher
};

typedef struct tagASTAT 
{ 
    ADAPTER_STATUS adapt; 
    NAME_BUFFER    NameBuff [30]; 
}ASTAT,*LPASTAT; 

//存储网卡的MAC地址的结构
typedef struct tagMAC_ADDRESS
{
	BYTE b1,b2,b3,b4,b5,b6;
}MAC_ADDRESS,*LPMAC_ADDRESS;

//网卡信息的数据结构，包括记录网卡的厂商及型号，与之绑定的IP地址，网关
//DNS序列，子网掩码和物理地址
typedef struct tagNET_CARD
{
	TCHAR szDescription[256];
	BYTE  szMacAddr[6];
	TCHAR szGateWay[128];
	TCHAR szIpAddress[128];
	TCHAR szIpMask[128];
	TCHAR szDNSNameServer[128];
}NET_CARD,*LPNET_CARD;


#define MAX_CARD  10

class ClassNetSetting  
{
public:
	void ProcessMultiString(LPTSTR lpszString,DWORD dwSize);
	UCHAR GetAddressByIndex(int lana_num,ASTAT & Adapter);
	BOOL GetSettingOfWinNT();
	int GetMacAddress(LPMAC_ADDRESS pMacAddr);
	BOOL GetSetting();
	ClassNetSetting();
	virtual ~ClassNetSetting();
public:
	BOOL GetSettingOfWin9X();
	Win32Type GetSystemType();
	int			m_TotalNetCards;//系统的网卡数
	TCHAR		m_szDomain[16];//域名
	TCHAR		m_szHostName[16];//主机名
	int			m_IPEnableRouter;//是否允许IP路由0-不允许,1-允许,2-未知
	int			m_EnableDNS;//是否允许DNS解析0-不允许,1-允许,2-未知
	NET_CARD    m_Cards[MAX_CARD];//默认的最大网卡数是10
	Win32Type   m_SystemType;//操作系统类型
	MAC_ADDRESS m_MacAddr[MAX_CARD];//允许10个网卡

};

#endif // !defined(AFX_LASSNETSETTING_H__16D8BE23_3C61_4511_B8BB_75E9264B515B__INCLUDED_)

//////////////////////////////////////////////////////
// FILE  : MibAccess.h
//
//

#ifndef _SNMP_ACCESS_H_
#define _SNMP_ACCESS_H_

#include <snmp.h>

//////////////////////////////////////////////////////////////
// Definition of pointers to the four functions in the Mib Dll
//
typedef BOOL (WINAPI *pSnmpExtensionInit)(IN  DWORD               dwTimeZeroReference,
										  OUT HANDLE              *hPollForTrapEvent,
										  OUT AsnObjectIdentifier *supportedView);

typedef BOOL (WINAPI *pSnmpExtensionTrap)(OUT AsnObjectIdentifier *enterprise,
										  OUT AsnInteger          *genericTrap,
										  OUT AsnInteger          *specificTrap,
										  OUT AsnTimeticks        *timeStamp,
										  OUT RFC1157VarBindList  *variableBindings);

typedef BOOL (WINAPI *pSnmpExtensionQuery)(IN BYTE                   requestType,
										   IN OUT RFC1157VarBindList *variableBindings,
										   OUT AsnInteger            *errorStatus,
										   OUT AsnInteger            *errorIndex);

typedef BOOL (WINAPI *pSnmpExtensionInitEx)(OUT AsnObjectIdentifier *supportedView);

typedef struct
{
	long		type;
	BYTE		MACLength;
	BYTE		MAC[14];
	BYTE		IP[4];
	BYTE		SubnetMask[4];
	BYTE		Description[64];
} tSTRUCTNICINFO;


#define		ERROR_MIB_DLL			-1
#define		ERROR_MIB_WINSOCK		-2
#define		ERROR_MIB_INIT			-3
	

class MibExtLoad
{
public:
	MibExtLoad(LPSTR MibDllName);
	~MibExtLoad();
	
	BOOL	Init(DWORD dwTimeZeroReference, HANDLE *hPollForTrapEvent, AsnObjectIdentifier *supportedView);
	BOOL	InitEx(AsnObjectIdentifier *supportedView);
	BOOL	Query(BYTE requestType, OUT RFC1157VarBindList *variableBindings,
			AsnInteger *errorStatus, AsnInteger *errorIndex);
	BOOL	Trap(AsnObjectIdentifier *enterprise, AsnInteger *genericTrap, 
			AsnInteger *specificTrap, AsnTimeticks *timeStamp, RFC1157VarBindList  *variableBindings);

	BOOL	GetDLLStatus();

private:	
	HINSTANCE				m_hInst;
	pSnmpExtensionInit		m_Init;
	pSnmpExtensionInitEx	m_InitEx;
	pSnmpExtensionQuery		m_Query;
	pSnmpExtensionTrap		m_Trap;
};


class MibII: public MibExtLoad
{
public:
	MibII();
	~MibII();

	int						Init();
	UINT					GetNICCount(BOOL bDialup, BOOL bLoopback);
	void					GetNICInfo(tSTRUCTNICINFO *pNICInfo);

private:
	int						m_rvWSA;
	UINT					m_ifCount;
	DWORD					*m_ifIndex;
	DWORD					*m_ifEntryNum;
	tSTRUCTNICINFO			*m_pNICInfo;
	BOOL					m_bDialup;
	BOOL					m_bLoopback;

	void					MatchNICEntries(UINT NICCount, tSTRUCTNICINFO *pNICInfo);
};


#endif
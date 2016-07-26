////////////////////////////////////////////////////////
// ptutils.h文件


HANDLE PtOpenControlDevice();
HANDLE PtOpenAdapter(PWSTR pszAdapterName);
BOOL PtAdapterRequest(HANDLE hAdapter, PPTUSERIO_OID_DATA pOidData, BOOL bQuery);


#define MAX_ADAPTERS 10
class CIMAdapters		// ptutils.h文件
{
public:
	// 枚举IM绑定的适配器
	BOOL EnumAdapters(HANDLE hControlDevice);
	int m_nAdapters;
	LPWSTR m_pwszAdapterName[MAX_ADAPTERS];
	LPWSTR m_pwszVirtualName[MAX_ADAPTERS];
protected:
	char m_buffer[MAX_ADAPTERS*256];
};



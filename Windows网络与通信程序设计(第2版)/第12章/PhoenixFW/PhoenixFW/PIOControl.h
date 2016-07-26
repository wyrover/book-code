////////////////////////////////////////
// PIOControl.h文件

// 管理DLL共享内存

#include "../common/PMacRes.h"
#include "../common/TypeStruct.h"

#ifndef __PIOCONTROL_H__
#define __PIOCONTROL_H__

class CPIOControl
{
public:
	CPIOControl();
	~CPIOControl();
	// 设置工作模式
	void SetWorkMode(int nWorkMode);
	// 获取工作模式
	int GetWorkMode();

	// 设置规则文件
	void SetRuleFile(RULE_FILE_HEADER *pHeader, RULE_ITEM *pRules);
	// 设置主模块句柄
	void SetPhoenixInstance(HWND hWnd, TCHAR *pszPathName);

	// 获取询问的应用程序和设置询问的结果
	LPCTSTR GetQueryApp(int nIndex);
	void SetQueryApp(int nIndex, BOOL bPass);

	// 获取一个会话信息
	void GetSession(SESSION *pSession, int nIndex);

private:
	PFNLSPIoControl m_fnIoControl;
	HMODULE			m_hLSPModule;	
	LSP_IO_CONTROL m_IoControl;
};

#endif // __PIOCONTROL_H__
//////////////////////////////////////
// PRuleFile.h

// 规则文件

#include "../common/TypeStruct.h"
#include "../common/PMacRes.h"

//  从将规则从文件读出，保存到CPRuleFile对象中。再做相反的动作
// Phoenix Personal firewall 

#ifndef __PRULEFILE_H__
#define __PRULEFILE_H__

class CPRuleFile
{
public:
	CPRuleFile();
	~CPRuleFile();

	// 从文件加载过滤规则
	BOOL LoadRules();
	// 将过滤规则保存到文件
	BOOL SaveRules();

	// 添加nCount个应用层（核心层）过滤规则
	BOOL AddLspRules(RULE_ITEM *pItem, int nCount);
	BOOL AddKerRules(PassthruFilter *pItem, int nCount);

	// 删除一个应用层（核心层）过滤规则
	BOOL DelLspRule(int nIndex);
	BOOL DelKerRule(int nIndex);

	// 文件数据
	RULE_FILE_HEADER m_header;		// 文件头
	RULE_ITEM *m_pLspRules;			// 应用层规则指针
	PassthruFilter *m_pKerRules;	// 核心层规则指针

private:
	// 初始化文件数据
	void InitFileData();
	// 打开磁盘文件，如果没有会自动创建，并进行初始化
	BOOL OpenFile();
	// 将规则保存到指定文件
	BOOL WriteRules(TCHAR *pszPathName);

	HANDLE m_hFile;
	TCHAR m_szPathName[MAX_PATH];
	int m_nLspMaxCount;
	int m_nKerMaxCount;
	BOOL m_bLoad;
};

#endif // __PRULEFILE_H__
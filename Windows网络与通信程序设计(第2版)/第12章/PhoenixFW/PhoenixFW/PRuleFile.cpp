//////////////////////////////////////////////////
// PRuleFile.cpp

#include "stdafx.h"

#include "PRuleFile.h"


CPRuleFile::CPRuleFile()
{
	// 获取规则文件的完整路径
	TCHAR *p;
	::GetFullPathName(RULE_FILE_NAME, MAX_PATH, m_szPathName, &p);

	m_hFile = INVALID_HANDLE_VALUE;

	// 为过滤规则预申请内存空间
	m_nLspMaxCount = 50;
	m_nKerMaxCount = 50;
	m_pLspRules = new RULE_ITEM[m_nLspMaxCount];
	m_pKerRules = new PassthruFilter[m_nKerMaxCount];
	m_bLoad = FALSE;
}

CPRuleFile::~CPRuleFile()
{
	if(m_hFile != INVALID_HANDLE_VALUE)
		::CloseHandle(m_hFile);	
	delete[] m_pLspRules;
	delete[] m_pKerRules;
}


void CPRuleFile::InitFileData()
{
	// 初始化文件头
	wcscpy(m_header.szSignature, RULE_HEADER_SIGNATURE);
	m_header.ulHeaderLength = sizeof(m_header);
	
	m_header.ucMajorVer = RULE_HEADER_MAJOR;
	m_header.ucMinorVer = RULE_HEADER_MINOR;

	m_header.dwVersion = RULE_HEADER_VERSION;

	wcscpy(m_header.szWebURL, RULE_HEADER_WEB_URL); 

	wcscpy(m_header.szEmail, RULE_HEADER_EMAIL);

	m_header.ulLspRuleCount = 0;
	m_header.ulKerRuleCount = 0;

	m_header.ucLspWorkMode = PF_QUERY_ALL;
	m_header.ucKerWorkMode = IM_START_FILTER;

	m_header.bAutoStart = FALSE;
}

// 将规则写入指定文件
BOOL CPRuleFile::WriteRules(TCHAR *pszPathName) 
{
	DWORD dw;
	if(m_hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hFile);
	}
	// 打开文件
	m_hFile = ::CreateFile(pszPathName, GENERIC_WRITE, 
					0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
	if(m_hFile != INVALID_HANDLE_VALUE)
	{
		// 写文件头
		::WriteFile(m_hFile, &m_header, sizeof(m_header), &dw, NULL);
		// 写应用层过滤规则
		if(m_header.ulLspRuleCount > 0)
		{
			::WriteFile(m_hFile, 
						m_pLspRules, m_header.ulLspRuleCount * sizeof(RULE_ITEM), &dw, NULL);
		}
		// 写核心层过滤规则
		if(m_header.ulKerRuleCount > 0)
		{
			::WriteFile(m_hFile, 
						m_pKerRules, m_header.ulKerRuleCount * sizeof(PassthruFilter), &dw, NULL);
		}

		::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
		return TRUE;
	}
	return FALSE;
}

BOOL CPRuleFile::OpenFile()
{
	// 首先保证文件已经存在
	if(::GetFileAttributes(m_szPathName) == -1)
	{
		InitFileData();
		if(!WriteRules(m_szPathName))
			return FALSE;
	}
	// 如果没有关闭，就关闭
	if(m_hFile != INVALID_HANDLE_VALUE)
		::CloseHandle(m_hFile);

	// 以只读方式打开文件
	m_hFile = ::CreateFile(m_szPathName, GENERIC_READ, 
					FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 

	return m_hFile != INVALID_HANDLE_VALUE;
}

///////////////////////////////////////////////////////////////////////////////////////
// 接口成员

BOOL CPRuleFile::LoadRules()
{
	// 先打开文件
	if((!OpenFile()) || (::SetFilePointer(m_hFile, 0, NULL, FILE_BEGIN) == -1))
		return FALSE;

	// 从文件中读取数据
	DWORD dw = 0;
	do
	{
		// 读文件头
		::ReadFile(m_hFile, &m_header, sizeof(m_header), &dw, NULL);
		// 如果签名不正确，退出
		if((dw != sizeof(m_header)) || 
				(_tcscmp(m_header.szSignature, RULE_HEADER_SIGNATURE) != 0))
			break;

		// 读应用层过滤规则
		if(m_header.ulLspRuleCount > 0)
		{		
			if(m_header.ulLspRuleCount > (ULONG)m_nLspMaxCount)
			{
				m_nLspMaxCount = m_header.ulLspRuleCount;
				delete[] m_pLspRules;
				m_pLspRules = new RULE_ITEM[m_nLspMaxCount];
			}
			if(!::ReadFile(m_hFile, m_pLspRules, 
				m_header.ulLspRuleCount * sizeof(RULE_ITEM), &dw, NULL))
				break;
		}

		// 读核心层过滤规则
		if(m_header.ulKerRuleCount > 0)
		{		
			if(m_header.ulKerRuleCount > (ULONG)m_nKerMaxCount)
			{
				m_nKerMaxCount = m_header.ulKerRuleCount;
				delete[] m_pKerRules;
				m_pKerRules = new PassthruFilter[m_nKerMaxCount];
			}
			if(!::ReadFile(m_hFile, m_pKerRules, 
				m_header.ulKerRuleCount * sizeof(PassthruFilter), &dw, NULL))
				break;
		}

		m_bLoad = TRUE;
	}
	while(FALSE);

	::CloseHandle(m_hFile);
	m_hFile = INVALID_HANDLE_VALUE;

	return m_bLoad;
}

BOOL CPRuleFile::SaveRules()
{
	// 如果没有加载，退出
	if(!m_bLoad)
		return FALSE;
	// 保存规则
	return WriteRules(m_szPathName);
}

BOOL CPRuleFile::AddLspRules(RULE_ITEM *pItem, int nCount)
{
	if((pItem == NULL) || !m_bLoad)
		return FALSE;

	// 首先保证有足够大的内存空间
	if(m_header.ulLspRuleCount + nCount > (ULONG)m_nLspMaxCount)
	{
		m_nLspMaxCount = 2*(m_header.ulLspRuleCount + nCount);

		RULE_ITEM *pTmp = new RULE_ITEM[m_header.ulLspRuleCount];
		memcpy(pTmp, m_pLspRules, m_header.ulLspRuleCount);

		delete[] m_pLspRules;
		m_pLspRules = new RULE_ITEM[m_nLspMaxCount];
		memcpy(m_pLspRules, pTmp, m_header.ulLspRuleCount);
		delete[] pTmp;
	}
	// 添加规则
	memcpy(m_pLspRules + m_header.ulLspRuleCount, pItem, nCount * sizeof(RULE_ITEM));
	m_header.ulLspRuleCount += nCount;
	return TRUE;
}

BOOL CPRuleFile::AddKerRules(PassthruFilter *pItem, int nCount)
{
	if((pItem == NULL) || !m_bLoad)
		return FALSE;

	// 首先保证有足够大的内存空间
	if(m_header.ulKerRuleCount + nCount > (ULONG)m_nKerMaxCount)
	{
		m_nKerMaxCount = 2*(m_header.ulKerRuleCount + nCount);

		PassthruFilter *pTmp = new PassthruFilter[m_header.ulKerRuleCount];
		memcpy(pTmp, m_pKerRules, m_header.ulKerRuleCount);

		delete[] m_pKerRules;
		m_pKerRules = new PassthruFilter[m_nKerMaxCount];
		memcpy(m_pKerRules, pTmp, m_header.ulKerRuleCount);
		delete[] pTmp;
	}
	// 添加规则
	memcpy(m_pKerRules + m_header.ulKerRuleCount, pItem, nCount * sizeof(PassthruFilter));
	m_header.ulKerRuleCount += nCount;
	return TRUE;
}

BOOL CPRuleFile::DelLspRule(int nIndex)
{
	if(((ULONG)nIndex >= m_header.ulLspRuleCount) || !m_bLoad)
		return FALSE;
	// 删除一个成员
	memcpy(&m_pLspRules[nIndex], 
		&m_pLspRules[nIndex + 1], (m_header.ulLspRuleCount - nIndex) * sizeof(RULE_ITEM));

	m_header.ulLspRuleCount --;
	return TRUE;
}

BOOL CPRuleFile::DelKerRule(int nIndex)
{
	if(((ULONG)nIndex >= m_header.ulKerRuleCount) || !m_bLoad)
		return FALSE;
	// 删除一个成员
	memcpy(&m_pKerRules[nIndex], 
		&m_pKerRules[nIndex + 1], (m_header.ulKerRuleCount - nIndex) * sizeof(PassthruFilter));

	m_header.ulKerRuleCount --;
	return TRUE;
}

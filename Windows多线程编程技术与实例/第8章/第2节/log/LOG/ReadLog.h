// ReadLog.h: interface for the CReadLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_READLOG_H__E576D990_C442_4C76_A86C_450F52E6494B__INCLUDED_)
#define AFX_READLOG_H__E576D990_C442_4C76_A86C_450F52E6494B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlbase.h>

struct _asc_DataLog {
	//    UINT Log_ID;
    CString LogFileNm;
    COleDateTime TimeStamp;//time //COleDateTime
    CString Rule_ID;//rule_id
    UINT Category_ID;//category_id 
    CString Client_IP;// client_ip 
    UINT IP_Zone;//ip_zone			 null
    UINT Size;//size
    UINT Body_Size;//body_size
    CString Mail_File;//mail_file 
	
    CString Condition;//condition,	null
    UINT Action; //action
	
    CString Sender;//sender,
    CString Subject;//subject,
    CString Receiver;//mail_header中的 to:
    CString CC;//抄送人//mail_header中的cc:  null
    CString BCC;//密送人//mail_header中的bcc: null

	CString Header_Ctt;//mail_header  null
	
	CComBSTR Mail_Content;//mail_content null
	
    CString Log_CMT;//数据日志注释	null
	//样本邮件
	CString emlPathNm;
	CString emlContent;

	CString emlwholeDir;	
	CString strTimeStamp;

};

class CReadLog  
{
public:
	void readLog();
	CReadLog();
	virtual ~CReadLog();
protected:
	_asc_DataLog m_gDataLog;	
	CString m_strHome;
public:
	int nRecordCount;
	
	int getDataLogIndex(BSTR& name);
	
	void deleteXmlMark(CString& str,CString& retStr);
	
	BOOL XmlToList(CString& fileName); 
	void getRecieve(CString& str,CString& strReceive);
	void getCC(CString& str,CString& strCC);
	void getBCC(CString& str,CString& strBCC);
	void initializeStructDataLog(_asc_DataLog& dataLog);
	bool checkDataLogRecord(_asc_DataLog& dataLog);
	bool getStrHome(CString& str);
	
	BOOL compareTwoLogFileTime(CString& filename1,CString& filename2);
	
	
private:

	UINT m_readCountInDir;

// 	CString m_strVerifyFile;
	
	BOOL reademlfile(CString& emlWholeName,CString& emlContent);
	void search(CString& homeDir);
	void lastDeleteOutOfTimeFile(CString& strDir,CString& fileName);
};

#endif // !defined(AFX_READLOG_H__E576D990_C442_4C76_A86C_450F52E6494B__INCLUDED_)

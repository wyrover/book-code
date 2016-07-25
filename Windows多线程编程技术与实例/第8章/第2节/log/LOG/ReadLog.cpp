// ReadLog.cpp: implementation of the CReadLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogModule.h"
#include "ReadLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <afxtempl.h>
#include <Atlbase.h>


#import <msxml.dll>		//for XMLDOM
using namespace MSXML;		//remove namespace

extern volatile BOOL m_gTagThreadEnd;
extern CRITICAL_SECTION m_gLogCriticalSection;

BOOL strtodate(CString& str, COleDateTime& dateTime);
extern CString vartostr(const _variant_t& var);
extern short vartoi(const _variant_t& var);
COleDateTime vartodate(const _variant_t& var);

CList<_asc_DataLog,_asc_DataLog> m_dataLogList(100);

//HANDLE g_readEvent;
//HANDLE  g_writeEvent;
#define MAXLISTRECORD  100
#define SLEEPMSNUM		300
#define MAX_READREC_PERDIR 1000

CString m_strTime;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReadLog::CReadLog()
{
	::CoInitialize(NULL);		// 初始化COM环境，for XML
	m_readCountInDir = 0;



}

CReadLog::~CReadLog()
{
	::CoUninitialize();

}

///////////////////////////////////

void CReadLog::readLog() 
{
	// TODO: Add your control notification handler code here

	bool bl = getStrHome(m_strHome);
	TRACE("----------------------%d--%s	--\n",bl,m_strHome);
	if (!bl) {
		AfxMessageBox( "请先安装SSH服务器", MB_OK,  0 );
		return;
	}
	int intTemp = m_strHome.GetLength();
	if (m_strHome[intTemp-1] != '\\') {
		m_strHome +=  "\\";
		TRACE("-------------innn---------%d--%s--\n",bl,m_strHome);
	}
	search(m_strHome);
}






int CReadLog::getDataLogIndex(BSTR& name)
{
	CString str = vartostr(name);
	CString strarray[14] = {"time","rule_id","category_id","client_ip","ip_zone","size","body_size","mail_file","condition","action","sender","subject","mail_header","mail_content"};

	int retInt = 0;
	for(int i=0;i<14;i++){
		retInt = str.Compare( strarray[i].GetBuffer(20) );
		strarray[i].ReleaseBuffer();
		if (retInt == 0) {
			return i;			
		}
	}	
	return -1;	
}

void CReadLog::deleteXmlMark(CString& str,CString& retStr)
{
	int length = str.GetLength();
	if (str[0] == '<') {
		retStr = str.Mid(1,length-2);
	}
	else
		retStr = str;
}


void CReadLog::getRecieve(CString& str,CString& strReceive)
{
	int retInt = 0;
	int tempInt = 0;
	int tempInt1 = 0;

	int tempInt2 = 0;

	retInt = str.Find("To:");
	TRACE("=======================%d\n",retInt);
	if (retInt == -1 ) {
		strReceive = "";
		TRACE("=======================\n");
	}
	else
	{
		tempInt1 =str.Find('<',retInt);
		tempInt = str.Find('>',retInt);


		if(tempInt != -1 && tempInt1 != -1){
			strReceive = str.Mid(tempInt1+1,tempInt-tempInt1-1);
			TRACE("=================%s	======\n",strReceive);
				
		}
		else{
			tempInt2 = str.Find('\n',retInt);
			strReceive = str.Mid(retInt+4,tempInt2-retInt-4);

		}
	}
}

void CReadLog::getCC(CString& str,CString& strCC)
{
	int retInt = 0;
	int tempInt = 0;
	int tempInt1 = 0;
	int tempInt2 = 0;

	retInt = str.Find("Cc:");
	TRACE("===========cc===========%d\n",retInt);
	if (retInt == -1 ) {
		strCC = "";
		TRACE("========cc===============\n");
	}

	else
	{
		tempInt1 =str.Find('<',retInt);
		tempInt = str.Find('>',retInt);


		if(tempInt != -1 && tempInt1 != -1){
			strCC = str.Mid(tempInt1+1,tempInt-tempInt1-1);
		}
		else{
			tempInt2 = str.Find('\n',retInt);
			strCC = str.Mid(retInt+4,tempInt2-retInt-4);

		}
	}
}
	

void CReadLog::getBCC(CString& str,CString& strBCC)
{
	int retInt = 0;
	int tempInt = 0;
	int tempInt1 = 0;
	int tempInt2 = 0;
	
	retInt = str.Find("Bcc:");
	TRACE("=======================%d\n",retInt);
	if (retInt == -1 ) {
		strBCC = "";
		TRACE("=======================\n");
	}
	else
	{
		tempInt1 =str.Find('<',retInt);
		tempInt = str.Find('>',retInt);


		if(tempInt != -1 && tempInt1 != -1){
			strBCC = str.Mid(tempInt1+1,tempInt-tempInt1-1);
		}
		else{
			tempInt2 = str.Find('\n',retInt);
			strBCC = str.Mid(retInt+5,tempInt2-retInt-5);

		}
	}
}





COleDateTime vartodate(const _variant_t& var)
{
	COleDateTime value;
	switch (var.vt) 
	{
	case VT_DATE:
		{
			value = var.date;
		}
		break;
	case VT_EMPTY:
	case VT_NULL:
		value.SetStatus(COleDateTime::null);
		break;
	default:
		value.SetStatus(COleDateTime::null);
		TRACE(_T("Warning: 未处理的 _variant_t 类型值; 文件: %s; 行: %d\n"), __FILE__, __LINE__);
	}
	return value;
}

void CReadLog::initializeStructDataLog(_asc_DataLog& dataLog)
{
	dataLog.LogFileNm  = _T("");

	dataLog.Rule_ID = _T("");//rule_id
	dataLog.Category_ID = 0;//category_id 
    dataLog.Client_IP = _T("");// client_ip 
    dataLog.IP_Zone = 0;//ip_zone			 null
    dataLog.Size = 0;//size
	dataLog.Body_Size = 0;//body_size
    dataLog.Mail_File = _T("");//mail_file 
	
    dataLog.Condition = _T("");//condition,	null
	dataLog.Action = 0; //action
	
    dataLog.Sender = _T("");//sender,
    dataLog.Subject = _T("");//subject,
    dataLog.Receiver = _T("");//mail_header中的 to:
    dataLog.CC = _T("");//抄送人//mail_header中的cc:  null
    dataLog.BCC = _T("");//密送人//mail_header中的bcc: null
    dataLog.Log_CMT = _T("");//数据日志注释	null

	dataLog.Header_Ctt = _T("");
	dataLog.Mail_Content = _T("");

	dataLog.emlContent = _T("");
	dataLog.emlPathNm = _T("");

	dataLog.emlwholeDir = _T("");


	
}

bool CReadLog::checkDataLogRecord(_asc_DataLog& dataLog)
{

/*
		if(dataLog.Rule_ID == "")	
		{
			return false;
		}
	
		if(dataLog.Client_IP == "")	
		{
			return false;
		}
	
		if(dataLog.Size == 0)	
		{
			return false;
		}
		if(dataLog.Body_Size == 0)	
		{
			return false;
		}
		if(dataLog.Mail_File == "")	
		{
			return false;
		}
	
		if(dataLog.Action == 0)	
		{
			return false;
		}
		*/
	
	if(dataLog.Sender == "")	
	{
		return false;
	}
	if(dataLog.Subject == "")	
	{
		return false;
	}

	if(dataLog.Receiver == "")	
	{
		return false;
	}

	return true;

}


bool CReadLog::getStrHome(CString& str)
{
	CRegKey regKey;
	long retLong = regKey.Open( HKEY_LOCAL_MACHINE, "SOFTWARE\\Cygnus Solutions\\Cygwin\\mounts v2\\/home", KEY_ALL_ACCESS);
	if (retLong == ERROR_SUCCESS) {
		TRACE("--------------in-------------\n");
		DWORD dwLen;
		LPTSTR lpStr;
		CString strValueName = "native";
		if(regKey.QueryValue(NULL, strValueName, &dwLen)==ERROR_SUCCESS)
		{
			lpStr=str.GetBuffer(dwLen+1);
			regKey.QueryValue(lpStr, strValueName, &dwLen);
			str.ReleaseBuffer();
			return true;

		}
		else{
			str = "";
			return false;
		}
	}
	else{
		str = "";
		return false;
	}
}



BOOL CReadLog::XmlToList(CString& fileName) 
{
	// TODO: Add your control notification handler code here
	BOOL bRuleID = FALSE;
	try{	
		IXMLDOMDocumentPtr pIXMLDOMDocument(__uuidof(DOMDocument));
		pIXMLDOMDocument->load((const char *)fileName);
		HRESULT hr = 0;
		int proIndex = 0;
		
		IXMLDOMNode *pIDOMNode = NULL;
		_variant_t varValue;

		IXMLDOMElement *pRootNode = NULL;
		IXMLDOMNode    *pRecNode = NULL;
		IXMLDOMNode    *pLogItem = NULL;
		IXMLDOMNode    *pLogItem1 = NULL;
		IXMLDOMNode    *pProItem = NULL;
		BSTR			szName;
		IXMLDOMNamedNodeMap *pIXMLDOMNamedNodeMap = NULL;
	
		CString str = "";
		CString strTemp = "";
		long lLength = 0;
		
		AfxMessageBox("node before");
		pIXMLDOMDocument->get_documentElement(&pRootNode);	
		if (pRootNode) {
			AfxMessageBox("get root node");
			pRootNode->get_firstChild(&pRecNode);
		}
		else{
			AfxMessageBox("the logfile is not a xmlfile");
			return FALSE;
		}
		if (pRecNode) {
			AfxMessageBox("get the first childnode");
			pRecNode->get_firstChild(&pRecNode);
		}
		else
			return FALSE;	
	//	for(; pRecNode != NULL; pRecNode->get_nextSibling(&pRecNode)) 
	//	{	
		AfxMessageBox("the format of xml is right");
		if (pRecNode != NULL) {
			initializeStructDataLog(m_gDataLog);
			hr = pRecNode->get_attributes(&pIXMLDOMNamedNodeMap);
			if(SUCCEEDED(hr) && pIXMLDOMNamedNodeMap)
			{
				hr = pIXMLDOMNamedNodeMap->get_length(&lLength);
				if(SUCCEEDED(hr) )
				{			
					for(int j=0;j<lLength;j++)
					{
						pIXMLDOMNamedNodeMap->get_item(j,&pProItem);
						pProItem->get_baseName(&szName);				
						str = szName;				
						proIndex = getDataLogIndex(szName);
						str.Format("%d",proIndex);
						
						pProItem->get_nodeValue(&varValue);				
						str = vartostr(varValue);
						switch(proIndex) {
						case -1:												
							break;
						case 0:
							strTemp = vartostr(varValue);
							//日期标志不对时,用系统时间代替
							if (!strtodate(strTemp,m_gDataLog.TimeStamp)) {	
								SYSTEMTIME sysTime ;
								GetSystemTime(&sysTime);				
								m_gDataLog.TimeStamp.SetDateTime(sysTime.wYear, sysTime.wMonth, 
									sysTime.wDay,(sysTime.wHour+8), sysTime.wMinute, sysTime.wSecond );
								m_strTime.Format("%d-%d-%d %d:%d:%d",sysTime.wYear, sysTime.wMonth, 
									sysTime.wDay,(sysTime.wHour+8), sysTime.wMinute, sysTime.wSecond );
							}						
							break;
						case 1:
							m_gDataLog.Rule_ID = vartostr(varValue);
							bRuleID = TRUE;
							break;
						case 2:
							m_gDataLog.Category_ID = atoi(vartostr(varValue));
							break;
						case 3:
							m_gDataLog.Client_IP = vartostr(varValue);
							break;
						case 4:
							m_gDataLog.IP_Zone = atoi(vartostr(varValue));
							break;
						case 5:
							m_gDataLog.Size = atoi(vartostr(varValue));
							break;
						case 6:
							m_gDataLog.Body_Size = atoi(vartostr(varValue));
							break;
						case 7:
							m_gDataLog.Mail_File = vartostr(varValue);
							break;						
						}
					}
				}		
				else
					return FALSE;
			}
			else
				return FALSE;	

			bRuleID = FALSE;	
			pIXMLDOMNamedNodeMap->Release();
			pIXMLDOMNamedNodeMap = NULL;	
			pRecNode->get_firstChild(&pLogItem);

			for(; pLogItem != NULL; pLogItem->get_nextSibling(&pLogItem)) 
			{
			//处理名字与值
				pLogItem->get_nodeName(&szName);
				str = szName;
	
				proIndex = getDataLogIndex(szName);
				str.Format("%d",proIndex);
				if(proIndex != 13) {
					pLogItem->get_text(&szName);
				}
			
				switch(proIndex) {
				case -1:
					break;
				case 8:
					strTemp = vartostr(szName);
					deleteXmlMark(strTemp,m_gDataLog.Condition);
					if (!bRuleID) {					
						m_gDataLog.Rule_ID = m_gDataLog.Condition;
					}					
					break;
				case 9:
					m_gDataLog.Action = atoi(vartostr(szName));
					break;
				case 10:
					strTemp = vartostr(szName);		
					deleteXmlMark(strTemp,m_gDataLog.Sender);
					break;
				case 11:
					m_gDataLog.Subject = vartostr(szName);
					break;
				case 12:
					strTemp = vartostr(szName);
					m_gDataLog.Header_Ctt = strTemp;					
					getRecieve(strTemp,m_gDataLog.Receiver);			
					getCC(strTemp,m_gDataLog.CC);
					getBCC(strTemp,m_gDataLog.BCC);
					break;
				case 13:				
					pLogItem->get_text(&(m_gDataLog.Mail_Content));
					break;
				}		
			}	
		}

	//处理eml样本文件
		proIndex = fileName.ReverseFind('\\');		
		m_gDataLog.LogFileNm = fileName.Mid(proIndex+1,fileName.GetLength()-proIndex-1);
		strTemp = fileName.Mid(proIndex+1,fileName.GetLength()-proIndex-4);
		m_gDataLog.emlPathNm = strTemp + "eml";
		m_gDataLog.emlwholeDir = fileName.Mid(0,proIndex);

		m_gDataLog.emlContent = "";
		m_gDataLog.strTimeStamp = m_strTime;
		if(checkDataLogRecord(m_gDataLog)){
			EnterCriticalSection(&m_gLogCriticalSection);
			m_dataLogList.AddTail(m_gDataLog);
			nRecordCount = m_dataLogList.GetCount();
			LeaveCriticalSection(&m_gLogCriticalSection);

			while(nRecordCount >= MAXLISTRECORD)
			{
				Sleep(SLEEPMSNUM);			
				EnterCriticalSection(&m_gLogCriticalSection);				
				nRecordCount = m_dataLogList.GetCount();
				LeaveCriticalSection(&m_gLogCriticalSection);					
			}		
			AfxMessageBox("read one");
			return TRUE;
		}
	}	
	catch (...) {
//		AfxMessageBox("exception in xmltoList");
	}
	bRuleID = FALSE;
	return FALSE;	
}





//filename1比filename2早就返回TURE,filename2就不应该delete
BOOL CReadLog::compareTwoLogFileTime(CString& filename1,CString& filename2)
{

	CString str1 = "";
	CString str2 = "";
	int index1 = filename1.Find('.');
	int index2 = filename2.Find('.');
	if (index2 == -1 || index1 == -1) {
		return TRUE;
	}
	str1 = filename1.Mid((index1+1),4);
	str2 = filename2.Mid((index2+1),4);
	if (atoi(str1)<atoi(str2)) {
		return TRUE;
	}
	else
	{
		if (atoi(str1)>atoi(str2)) {
			return FALSE;
		}
		else{
			str1 = filename1.Mid((index1+5),2);
			str2 = filename2.Mid((index2+5),2);
			if (atoi(str1)<atoi(str2)) {
				return TRUE;
			}
			else
			{
				if (atoi(str1)>atoi(str2)) {
					return FALSE;
				}
				else{
					str1 = filename1.Mid((index1+7),2);
					str2 = filename2.Mid((index2+7),2);
					if (atoi(str1)<atoi(str2)) {
						return TRUE;
					}
					else
					{
						if (atoi(str1)>atoi(str2)) {
							return FALSE;
						}
						else
						{
							str1 = filename1.Mid((index1+9),2);
							str2 = filename2.Mid((index2+9),2);
							if (atoi(str1)<atoi(str2)) {
								return TRUE;
							}
							else
							{
								if (atoi(str1)>atoi(str2)) {
									return FALSE;
								}
								else
								{
									str1 = filename1.Mid((index1+11),2);
									str2 = filename2.Mid((index2+11),2);
									if (atoi(str1)<atoi(str2)) {
										return TRUE;
									}
									else{
										if (atoi(str1)>atoi(str2)) {
											return FALSE;
										}
										else
										{
											str1 = filename1.Mid((index1+13),2);
											str2 = filename2.Mid((index2+13),2);
											if (atoi(str1)<atoi(str2)) {
												return TRUE;
											}
											else
											{
												if (atoi(str1)>atoi(str2)) {
													return FALSE;
												}
												else//时间相同时
												{
													return TRUE;
												}

											}

										}
									}	

								}
							}


						}

					}

				}
			}
		}
	}

}

void CReadLog::lastDeleteOutOfTimeFile(CString& strUserDir,CString& fileName)
{
	CString str = strUserDir;
	CString strFileName = "";
	str = str +  "*.*";
	CFileFind file_finder;
	BOOL bl = FALSE;

	bl = file_finder.FindFile(str);
	
	while (bl) 
	{
		bl = file_finder.FindNextFile();
		strFileName = file_finder.GetFilePath();

		if (!file_finder.IsDirectory()) 
		{
			
			if (!compareTwoLogFileTime(fileName,strFileName)) {
				TRACE("-----------------%s--%s---\n",fileName,strFileName);
				DeleteFile(strFileName);
			}

		}
		//处理最后一个文件
		if (!bl) {
			if (!file_finder.IsDirectory()) 
			{
				if (!compareTwoLogFileTime(fileName,strFileName)) {
					DeleteFile(strFileName);
				}
			}
		}
	}
}

void CReadLog::search(CString &homeDir)
{
	try{	
		CString strUserDir,strName;
		CString strLogWholeName,strEmlWholeName,strEmlsigWholeName;// ,strSigWholeName
		CString strName1,strName2,strName3,strName4;
		
		CFileFind file_finder;
		CFileFind logfile_finder,sigfile_finder,emlfile_finder,file_finder4;
		BOOL bFind = FALSE;
		BOOL bFindLog = FALSE;
		BOOL bFindSig = FALSE;
		BOOL bFindEml = FALSE;
		BOOL bFindEmlsig = FALSE;
		BOOL bAddSuccess = FALSE;
		
		
		
		homeDir += "*";
		nRecordCount = 0;
		
		while (1) {
			if (m_gTagThreadEnd) {		//是否终止线程
				return;
			}
			AfxMessageBox("readlog thread begin");
			bFind = file_finder.FindFile(homeDir);
			while (bFind) 
			{   
				m_readCountInDir = 0;
				if (m_gTagThreadEnd) {		//是否终止线程
					return;
				}
				bFind = file_finder.FindNextFile();
				if (file_finder.IsDirectory() && !file_finder.IsDots()) 
				{
					
					strUserDir = file_finder.GetFilePath()+"\\";
					TRACE("--------%s\n",strUserDir);				
					strLogWholeName = strUserDir + "log\\*.log";
					bFindLog = logfile_finder.FindFile(strLogWholeName);							
					
					
					//处理此目录(strUserDir+"log\\")下的所有文件				
					while (bFindLog) 						
					{	
						++m_readCountInDir;
						if (m_readCountInDir == MAX_READREC_PERDIR) {
							break;
						}
						bFindLog = logfile_finder.FindNextFile();
						if (!logfile_finder.IsDirectory()) 
						{
							strLogWholeName = logfile_finder.GetFilePath();						
							AfxMessageBox(strLogWholeName);
							
							//处理eml
							strEmlWholeName = strLogWholeName.Left(strLogWholeName.GetLength()-4) + ".eml";
							bFindEml = emlfile_finder.FindFile(strEmlWholeName);
							
							
							
							//处理xml
							if (  bFindEml)// bFindSig && && bFindEmlsig
							{
								bAddSuccess = XmlToList(strLogWholeName);
								if (!bAddSuccess) {
									try{
										::DeleteFile(strLogWholeName);
										//	::DeleteFile(strSigWholeName);
										::DeleteFile(strEmlWholeName);//stremlTemp
									}
									catch (...) {							
									}
								}
								else{
									try
									{
										::DeleteFile(strLogWholeName);
										//	::DeleteFile(strSigWholeName);						
									}
									catch (...) {				
									}			
								}								
								
							}
							else{//文件不全时
								try{
									::DeleteFile(strLogWholeName);
									//	::DeleteFile(strSigWholeName);
									::DeleteFile(strEmlWholeName);//stremlTemp
								}
								catch (...) {
								}
							}
						}		
					}			
				}
			}		
		}
	}

	catch (...) {
//		AfxMessageBox("没有签名文件或不能给list分配足够空间");
	}

}



BOOL strtodate(CString& str, COleDateTime& dateTime)
{
// 	20031220161916
	TRACE("%s=================================\n",str);
	int len = str.GetLength();
	int i = 0,j = 0;
	if (len >= 14) {
		CString strTemp1 = "";
		strTemp1 = str.Mid(0,4);
		CString strTemp2 = "";
		strTemp2 = str.Mid(4,2);
		CString strTemp3 = "";
		strTemp3 = str.Mid(6,2);		


		CString strTemp4 = "";
		strTemp4 = str.Mid(8,2);
		CString strTemp5 = "";
		strTemp5 = str.Mid(10,2);
		CString strTemp6 = "";
		strTemp6 = str.Mid(12,2);

		m_strTime.Format("%d-%d-%d %d:%d:%d",atoi(strTemp1), atoi(strTemp2), atoi(strTemp3),atoi(strTemp4), atoi(strTemp5), atoi(strTemp6) );
		i = dateTime.SetDateTime(atoi(strTemp1), atoi(strTemp2), atoi(strTemp3),atoi(strTemp4), atoi(strTemp5), atoi(strTemp6) );
		if (i == 0) {
			return TRUE;
		}
		else
			return FALSE;
	}
	return FALSE;
}

BOOL CReadLog::reademlfile(CString &emlWholeName, CString &emlContent)
{
	CFile sourceFile;
	if (!sourceFile.Open(emlWholeName,
		CFile::modeRead | CFile::shareDenyWrite, NULL))
	{
		return FALSE;
//		AfxMessageBox("can open file");
	}
	char pbuf[1000];
	int dwRead = 1000;
	while (dwRead == 1000) {
		dwRead = sourceFile.Read(pbuf, 1000);
		if (dwRead < 1000) {
			pbuf[dwRead] = '\0';
		}
		emlContent += pbuf;
	}
	sourceFile.Close();
	return TRUE;

}



CString vartostr(const _variant_t& var)
{
	CString strValue;
	
	switch (var.vt)
	{
	case VT_BSTR://字符串
	case VT_LPSTR://字符串
	case VT_LPWSTR://字符串
		strValue = (LPCTSTR)(_bstr_t)var;
		break;
	case VT_I1:
	case VT_UI1:
		strValue.Format("%d", var.bVal);
		break;
	case VT_I2://短整型
		strValue.Format("%d", var.iVal);
		break;
	case VT_UI2://无符号短整型
		strValue.Format("%d", var.uiVal);
		break;
	case VT_INT://整型
		strValue.Format("%d", var.intVal);
		break;
	case VT_I4: //整型
		strValue.Format("%d", var.lVal);
		break;
	case VT_I8: //长整型
		strValue.Format("%d", var.lVal);
		break;
	case VT_UINT://无符号整型
		strValue.Format("%d", var.uintVal);
		break;
	case VT_UI4: //无符号整型
		strValue.Format("%d", var.ulVal);
		break;
	case VT_UI8: //无符号长整型
		strValue.Format("%d", var.ulVal);
		break;
	case VT_VOID:
		strValue.Format("%8x", var.byref);
		break;
	case VT_R4://浮点型
		strValue.Format("%.4f", var.fltVal);
		break;
	case VT_R8://双精度型
		strValue.Format("%.8f", var.dblVal);
		break;
	case VT_DECIMAL: //小数
		strValue.Format("%.8f", (double)var);
		break;
	case VT_CY:
		{
			COleCurrency cy = var.cyVal;
			strValue = cy.Format();
		}
		break;
	case VT_BLOB:
	case VT_BLOB_OBJECT:
	case 0x2011:
		strValue = "[BLOB]";
		break;
	case VT_BOOL://布尔型
		
		strValue = var.boolVal ? "TRUE" : "FALSE";
		break;
	case VT_DATE: //日期型
		{
			DATE dt = var.date;
			COleDateTime da = COleDateTime(dt); 
			strValue = da.Format("%Y-%m-%d %H:%M:%S");
		}
		break;
	case VT_NULL://NULL值
		strValue = "";
		break;
	case VT_EMPTY://空
		strValue = "";
		break;
	case VT_UNKNOWN://未知类型
	default:
		strValue = "UN_KNOW";
		break;
	}
	return strValue;
}






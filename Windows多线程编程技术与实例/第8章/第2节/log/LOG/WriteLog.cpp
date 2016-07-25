// WriteLog.cpp: implementation of the CWriteLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogModule.h"
#include "WriteLog.h"

#include <afxtempl.h>

#include "DBConnetParamRegConfig.h"

#define DELETE_USELESSFILE_NUM 500

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern volatile BOOL m_gTagThreadEnd;
extern CRITICAL_SECTION m_gLogCriticalSection;

// extern _ConnectionPtr m_pConnection;
extern CList<_asc_DataLog,_asc_DataLog> m_dataLogList;
extern CString vartostr(const _variant_t& var);
extern short vartoi(const _variant_t& var);


extern HANDLE g_readEvent;
extern HANDLE  g_writeEvent;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWriteLog::CWriteLog()
{


	m_pRecordsetSelect = NULL;


	sql = _T("");
	exist =FALSE;
	m_strTimeStamp = _T("");

	m_CountAfterDel = 0;




}

CWriteLog::~CWriteLog()
{	


	m_pRecordsetSelect->Close();
}



void CWriteLog::initWriteLog()
{
	setConnect();

}




void CWriteLog::writeLog()
{
	int i = 0;
	int nCount = 0;
	DWORD emlLength = 0;        
	char* pemlBuf = NULL;
	CString strEmlWholePathFile = "";//eml的全路径文件名
	_asc_DataLog datalog;

	while (1) {
		if (m_gTagThreadEnd)
		{	//是否终止线程
			return;
		}
		AfxMessageBox("start w thread");
		EnterCriticalSection(&m_gLogCriticalSection);
		nCount = m_dataLogList.GetCount();
		LeaveCriticalSection(&m_gLogCriticalSection);

		if (nCount != 0) 
		{
			AfxMessageBox("list is not null");			
			for(i=0;i<nCount;i++)
			{					
				EnterCriticalSection(&m_gLogCriticalSection);
				datalog = m_dataLogList.RemoveHead();
				LeaveCriticalSection(&m_gLogCriticalSection);
				
			//	COleDateTime oletime = datalog.TimeStamp;
			//get emlbuf and strMd5
				strEmlWholePathFile = datalog.emlwholeDir + "\\" +datalog.emlPathNm;							
				CFile feml;
				AfxMessageBox(strEmlWholePathFile);
				if( !feml.Open( strEmlWholePathFile, CFile::modeRead, NULL ) )
				{
					AfxMessageBox("open file failure");
					continue;
				}
				else{
					emlLength = feml.GetLength();
					pemlBuf = new char[emlLength];
					if (pemlBuf == NULL || feml.Read(pemlBuf, emlLength) != emlLength)
					{
						if (pemlBuf != NULL) delete[] pemlBuf;
						feml.Close();
						continue;						
					}						
					feml.Close();
				}
			//add to DB
			//	ColeDateToStr(oletime,m_strTimeStamp);

				CString strMailSampleSql = "";
				CString strLogDataSql = "";// ,m_strTimeStamp
				
				strMailSampleSql.Format("INSERT INTO MailSample (emlPathNm) VALUES('%s') ",datalog.emlPathNm);
	
				datalog.emlPathNm = datalog.LogFileNm;
				datalog.emlPathNm.Replace("log","eml");
				strLogDataSql.Format("insert into DataLog values('%s','%s','%s','%d','%s','%d',\
																'%d','%d','%s','%s','%d','%s',\
																 '%s','%s','%s','%s','%s')",\
															 datalog.LogFileNm,datalog.strTimeStamp,datalog.Rule_ID,(long)datalog.Category_ID,datalog.Client_IP,(long)datalog.IP_Zone,\
															(long)datalog.Size,(long)datalog.Body_Size,datalog.emlPathNm,\
															 datalog.Condition,(long)datalog.Action,datalog.Sender,\
														 datalog.Subject,datalog.Receiver,datalog.CC,datalog.BCC,datalog.Log_CMT);

				try{
					m_pConnection->BeginTrans();
					AfxMessageBox(strLogDataSql);
					m_pConnection->Execute(_bstr_t(strLogDataSql), NULL, adCmdText);
					AfxMessageBox("table dataspl suc");
					
					AfxMessageBox(strMailSampleSql);
					m_pConnection->Execute(_bstr_t(strMailSampleSql), NULL, adCmdText);
					AfxMessageBox("table MailSample suc");

					CString strGetLastOfSample = "";
					strGetLastOfSample.Format("select * from MailSample where emlPathNm = '%s'",datalog.emlPathNm);//max(emlID)	
					try{
						m_pRecordsetSelect->Close();
					}
					catch(...){
					}
					m_pRecordsetSelect->Open(_variant_t(LPCTSTR(strGetLastOfSample)),_variant_t((IDispatch*)m_pConnection,true),adOpenStatic,adLockOptimistic,adCmdText);
				//	BOOL bAppend = AppendChunk("emlContent", strEmlWholePathFile);				
					FieldPtr pField = m_pRecordsetSelect->GetFields()->GetItem("emlContent");
					BOOL bAppend = AppendChunk(pField, pemlBuf, emlLength);
				//	m_pRecordsetSelect->GetFields()->GetItem("emlContent")->AppendChunk(_variant_t(testPstr));
					if(bAppend){
						AfxMessageBox("append success");
					}
					else{
						AfxMessageBox("append failue");
					}
					m_pRecordsetSelect->Update();
					
					m_pConnection->CommitTrans();
					AfxMessageBox("add");
			

				}
				catch (...) {
					AfxMessageBox("add failure");
					m_pConnection->RollbackTrans(); 
				}
			//清理过期文件
				++m_CountAfterDel;				
				if (m_CountAfterDel == DELETE_USELESSFILE_NUM) {
					lastDeleteOutOfTimeFile(datalog.emlwholeDir,datalog.LogFileNm);
				}
			//release memory
				delete[] pemlBuf;
			//delete the current file
				try{
					CString delFile = datalog.emlwholeDir + "\\" + datalog.LogFileNm;
					DeleteFile(delFile);//delete log file
					delFile += ".sig";
					DeleteFile(delFile);//delete sig file
					DeleteFile(strEmlWholePathFile);
				}
				catch(...){			
				}
			}
		}
 	}
}



void CWriteLog::lastDeleteOutOfTimeFile(CString& strDir,CString& fileName)
{
	CString str = strDir;
	CString strFileName = "";
	CString strFilePath = "";
	str = str +  "\\*.*";
	CFileFind file_finder;
	BOOL bl = FALSE;
	
	bl = file_finder.FindFile(str);
	
	while (bl) 
	{
		bl = file_finder.FindNextFile();
		strFileName = file_finder.GetFileName();
		strFilePath = file_finder.GetFilePath();
//		AfxMessageBox(strFilePath);
		if (!file_finder.IsDirectory()) 
		{
			
			if (!compareTwoLogFileTime(fileName,strFileName)) {
				TRACE("-----------------%s--%s---\n",fileName,strFileName);
//				AfxMessageBox(strFilePath);
				DeleteFile(strFilePath);
			}
			else
				break;//因估计后面的文件都是没有过期的，所以强行退出，可以省时
			
		}
	}
}



//filename1比filename2早就返回TURE,filename2就不应该delete
BOOL CWriteLog::compareTwoLogFileTime(CString& filename1,CString& filename2)
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
													return FALSE;
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




void CWriteLog::setConnect()
{
	CoInitialize(NULL);
	
	//注册表操作
	CDBConnetParamRegConfig m_DBConnetParamRegConfig;
	m_DBConnetParamRegConfig.Load();
	
	CString connectStr = "";
	connectStr.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",m_DBConnetParamRegConfig.srvName,m_DBConnetParamRegConfig.DBName,m_DBConnetParamRegConfig.loginNm,m_DBConnetParamRegConfig.pwd);
	
	
	
	HRESULT hr;
	try
	{
		hr = m_pConnection.CreateInstance("ADODB.Connection");///创建Connection对象
		if(SUCCEEDED(hr))
		{
			hr = m_pConnection->Open((_bstr_t)connectStr,"","",adModeUnknown);///连接数据库
			
		}
	}
	catch(_com_error e)///捕捉异常
	{
		CString errormessage;
		errormessage.Format("连接数据库失败!\r\n错误信息:%s",e.ErrorMessage());		
	} 
	
}


BOOL CWriteLog::AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes)
{
	SAFEARRAY FAR *pSafeArray = NULL;
	SAFEARRAYBOUND rgsabound[1];

	try
	{
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = nBytes;
		pSafeArray = SafeArrayCreate(VT_UI1, 1, rgsabound);

		for (long i = 0; i < (long)nBytes; i++)
		{
			UCHAR &chData	= ((UCHAR*)lpData)[i];
			HRESULT hr = SafeArrayPutElement(pSafeArray, &i, &chData);
			if (FAILED(hr))	return FALSE;
		}

		_variant_t varChunk;
		varChunk.vt = VT_ARRAY | VT_UI1;
		varChunk.parray = pSafeArray;
		return (pField->AppendChunk(varChunk) == S_OK);
	}
	catch (_com_error &e)
	{
		TRACE(_T("Warning: AppendChunk 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}



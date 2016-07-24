// HTTPDownload.cpp: implementation of the CHTTPDownload class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Download.h"
#include "HTTPDownload.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHTTPDownload::CHTTPDownload()
{
	for(int i = 0; i < 4; i++){
		m_bTerminate[i] = FALSE;
	}

	m_bSupportResume = FALSE;
	m_bResume = FALSE;
}

CHTTPDownload::~CHTTPDownload()
{
	m_lsTask.RemoveAll();
}

BOOL CHTTPDownload::StartTask(CString remoteurl, CString localfile)
{
	if(remoteurl.IsEmpty())
		return FALSE;
	if(!ParseURL(remoteurl)){
		remoteurl = _T("http://") + remoteurl;
		if(!ParseURL(remoteurl)){
			TRACE("Requested URL is invalid!\n");
			return FALSE;
		}
	}

	m_strSavePath = localfile;
	m_strSavePath.TrimLeft();
	m_strSavePath.TrimRight();
	if(m_strSavePath.IsEmpty() )
		return FALSE;
	m_strTempSavePath = m_strSavePath;
	m_strTempSavePath += ".down";
	
	FILE* fp = NULL;
	if((fp = fopen(m_strTempSavePath, "r")) == NULL){
		m_state.range[0] = -1;
		m_bResume = FALSE;
	}
	else{
		m_bResume = TRUE;
		char* str = new char[1024];
		memset(str, 0, 1024);
		fgets(str, 1024, fp);
		m_state.url.Empty();
		m_state.url += str;
		m_state.url = m_state.url.Left(m_state.url.GetLength() - 2);
		memset(str, 0, 1024);
		fgets(str, 1024, fp);
		m_state.localfile.Empty();
		m_state.localfile += str;
		m_state.localfile = m_state.localfile.Left(m_state.localfile.GetLength() - 2);
		delete [] str;
		fread(&m_state.length, sizeof(LONG), 1, fp);
		fread(&m_state.time, sizeof(CTime), 1, fp);
		fread(m_state.range, sizeof(LONG), 8, fp);
		fclose(fp);
	}

	if(SendRequest() != SENDREQUEST_SUCCESS){ 
		TRACE("Remote Web Server is not reachable,or somthing else error occured!\n");
		return FALSE;
	}
	if(m_state.range[0] == -1){
		m_state.localfile = localfile; 
		m_state.url = remoteurl;
		m_state.time = m_TimeLastModified;
		m_state.length = m_dwFileSize;
		for(int i = 0; i < 4; i++){
			m_state.range[i * 2] = i * (m_dwFileSize / 4);
			m_state.range[i * 2 + 1] = (i + 1) * (m_dwFileSize / 4) - 1;
		}
		m_state.range[7] = m_dwFileSize - 1;
	}
	else{
		if(m_state.url != remoteurl){
			AfxMessageBox("Maybe the download file is not you want,you can try to save as another file!");
			return FALSE;
		}
		if(m_state.time < m_TimeLastModified || m_state.range[7] != (LONG)(m_dwFileSize - 1)){
			m_state.time = m_TimeLastModified;
			m_state.length = m_dwFileSize;
			for(int i = 0; i < 4; i++){
				m_state.range[i * 2] = i * (m_dwFileSize / 4);
				m_state.range[i * 2 + 1] = (i + 1) * (m_dwFileSize / 4);
			}
			m_state.range[7] = m_dwFileSize - 1;
		}
	}
	
	return TRUE;
}

BOOL CHTTPDownload::ParseURL(CString str)
{
	str.TrimLeft();
	if(str.IsEmpty())
		return FALSE;

	CString strURL = str;
	// 清除数据
	m_strServer = _T("");
	m_strObject = _T("");
	m_nPort	  = 0;

	int nPos = strURL.Find("://");
	if( nPos == -1 )
		return FALSE;

	// 进一步验证是否为http://
	CString strTemp = strURL.Left( nPos+lstrlen("://") );
	strTemp.MakeLower();
	if( strTemp.Compare("http://") != 0 )
		return FALSE;

	strURL = strURL.Mid( strTemp.GetLength() );
	nPos = strURL.Find('/');
	if ( nPos == -1 )
		return FALSE;

	m_strObject = strURL.Mid(nPos);
	strTemp   = strURL.Left(nPos);
	
	///////////////////////////////////////////////////////////////
	/// 注意：并没有考虑URL中有用户名和口令的情形和最后有#的情形
	/// 例如：http://abc@def:www.yahoo.com:81/index.html#link1
	/// 
	//////////////////////////////////////////////////////////////

	// 查找是否有端口号
	nPos = strTemp.Find(":");
	if( nPos == -1 )
	{
		m_strServer = strTemp;
		m_nPort	  = DEFAULT_HTTP_PORT;
	}
	else
	{
		m_strServer = strTemp.Left( nPos );
		strTemp	  = strTemp.Mid( nPos+1 );
		m_nPort	  = (USHORT)_ttoi((LPCTSTR)strTemp);
	}
	return TRUE;
}

UINT CHTTPDownload::SendRequest(BOOL bHead)
{
	CString strVerb;
	if( bHead )
		strVerb = _T("HEAD ");
	else
		strVerb = _T("GET ");
	
	CString			strSend,strHeader,strRange;
	
	int				iStatus = 0,nRet;
	char			szReadBuf[1025];
	DWORD			dwContentLength,dwStatusCode;
	
	while (TRUE)
	{
		if(m_pSocket.m_hSocket != NULL)
			m_pSocket.Close();
		m_pSocket.Create();
		m_pSocket.Connect(m_strServer, m_nPort);
		
		strSend  = strVerb  + m_strObject + " HTTP/1.1\r\n";
		strSend += "Host: " + m_strServer + "\r\n";
		strSend += "Accept: */*\r\n";
		strSend += "Pragma: no-cache\r\n"; 
		strSend += "Cache-Control: no-cache\r\n";
		if( !m_strReferer.IsEmpty() )
			strSend += "Referer: " + m_strReferer + "\r\n";
		strSend += "Connection: close\r\n";
		strRange = "Range: bytes=100-\r\n";
		strSend += strRange;
		//必须要加一个空行，否则Http服务器将不会应答
		strSend += "\r\n";
		
		int ret = m_pSocket.Send(strSend.GetBuffer(0), strSend.GetLength());
		strSend.ReleaseBuffer();
		
		strHeader.Empty();
		while( TRUE )
		{
			ZeroMemory(szReadBuf,1025);
			ret = m_pSocket.Receive(szReadBuf, 1025);
			
			if( szReadBuf[0] == '\0' ) // We have encountered "\r\n\r\n"
				break; 
			
			strHeader += szReadBuf;
			if( iStatus == 0)
				strHeader += "\r\n";
		}
		
		nRet = GetInfo(strHeader,dwContentLength,
			dwStatusCode,m_TimeLastModified);
		switch ( nRet )
		{
		case HTTP_FAIL:
			return SENDREQUEST_FAIL;
			break;
		case HTTP_ERROR:
			return SENDREQUEST_ERROR;
			break;
		case HTTP_REDIRECT:
			continue;
			break;
		case HTTP_OK:
			m_dwDownloadSize = dwContentLength + 100;
			// 应该判断一下服务器是否支持断点续传
			if( strRange.IsEmpty() )
				m_dwFileSize = dwContentLength + 100; // 整个文件的长度
			else
			{
				if ( dwStatusCode == 206 )	//支持断点续传
				{
					m_bSupportResume = TRUE;
					m_dwFileSize = dwContentLength + 100;
				}
				else						//不支持断点续传
				{
					m_bSupportResume = FALSE;
					m_dwFileSize = dwContentLength + 100;
				}
			}
			return SENDREQUEST_SUCCESS;
			break;
		default:
			return SENDREQUEST_FAIL;
			break;
		}
	}
	m_pSocket.Close();
	return SENDREQUEST_SUCCESS;
}

UINT CHTTPDownload::GetInfo(LPCTSTR lpszHeader, DWORD &dwContentLength,
							DWORD &dwStatusCode, CTime &TimeLastModified)
{
	dwContentLength = 0;
	dwStatusCode	= 0;
	TimeLastModified= CTime::GetCurrentTime();

	CString strHeader = lpszHeader;
	strHeader.MakeLower();

	//拆分出HTTP应答的头信息的第一行
	int nPos = strHeader.Find("\r\n");
	if (nPos == -1)
		return HTTP_FAIL;
	CString strFirstLine = strHeader.Left(nPos);

	// 获得返回码: Status Code
	strFirstLine.TrimLeft();
	strFirstLine.TrimRight();
	nPos = strFirstLine.Find(' ');
	if( nPos == -1 )
		return HTTP_FAIL;
	strFirstLine = strFirstLine.Mid(nPos+1);
	nPos = strFirstLine.Find(' ');
	if( nPos == -1 )
		return HTTP_FAIL;
	strFirstLine = strFirstLine.Left(nPos);
	dwStatusCode = (DWORD)_ttoi((LPCTSTR)strFirstLine);
	
	// 检查返回码
	if( dwStatusCode >= 300 && dwStatusCode < 400 ) //首先检测一下服务器的应答是否为重定向
	{
		nPos = strHeader.Find("location:");
		if (nPos == -1)
			return HTTP_FAIL;

		CString strRedirectFileName = strHeader.Mid(nPos + strlen("location:"));
		nPos = strRedirectFileName.Find("\r\n");
		if (nPos == -1)
			return HTTP_FAIL;

		strRedirectFileName = strRedirectFileName.Left(nPos);
		strRedirectFileName.TrimLeft();
		strRedirectFileName.TrimRight();
		
		// 设置Referer
		m_strReferer = m_strDownloadUrl;

		// 判断是否重定向到其他的服务器
		nPos = strRedirectFileName.Find("http://");
		if( nPos != -1 )
		{
			m_strDownloadUrl = strRedirectFileName;
			// 检验要下载的URL是否有效
			if ( !ParseURL(m_strDownloadUrl))
				return HTTP_FAIL;
			return HTTP_REDIRECT;
		}

		// 重定向到本服务器的其他地方
		strRedirectFileName.Replace("\\","/");
		
		// 是相对于根目录
		if( strRedirectFileName[0] == '/' )
		{
			m_strObject = strRedirectFileName;
			return HTTP_REDIRECT;
		}
		
		// 是相对当前目录
		int nParentDirCount = 0;
		nPos = strRedirectFileName.Find("../");
		while (nPos != -1)
		{
			strRedirectFileName = strRedirectFileName.Mid(nPos+3);
			nParentDirCount++;
			nPos = strRedirectFileName.Find("../");
		}
		for (int i=0; i<=nParentDirCount; i++)
		{
			nPos = m_strDownloadUrl.ReverseFind('/');
			if (nPos != -1)
				m_strDownloadUrl = m_strDownloadUrl.Left(nPos);
		}
		m_strDownloadUrl = m_strDownloadUrl+"/"+strRedirectFileName;

		if ( !ParseURL(m_strDownloadUrl))
			return HTTP_FAIL;
		return HTTP_REDIRECT;
	}

	// 服务器错误，可以重试
	if( dwStatusCode >= 500 )
		return HTTP_ERROR;

	// 客户端错误，重试无用
	if( dwStatusCode >=400 && dwStatusCode <500 )
		return HTTP_FAIL;
		
	// 获取ContentLength
	nPos = strHeader.Find("content-length:");
	if (nPos == -1)
		return HTTP_FAIL;

	CString strDownFileLen = strHeader.Mid(nPos + strlen("content-length:"));	
	nPos = strDownFileLen.Find("\r\n");
	if (nPos == -1)
		return HTTP_FAIL;

	strDownFileLen = strDownFileLen.Left(nPos);	
	strDownFileLen.TrimLeft();
	strDownFileLen.TrimRight();

	// Content-Length:
	dwContentLength = (DWORD) _ttoi( (LPCTSTR)strDownFileLen );

	// 获取Last-Modified:
	nPos = strHeader.Find("last-modified:");
	if (nPos != -1)
	{
		CString strTime = strHeader.Mid(nPos + strlen("last-modified:"));
		nPos = strTime.Find("\r\n");
		if (nPos != -1)
		{
			strTime = strTime.Left(nPos);
			strTime.TrimLeft();
			strTime.TrimRight();
			TimeLastModified = GetTime(strTime);
		}
	}
	return HTTP_OK;
}

CTime CHTTPDownload::GetTime(LPCTSTR lpszTime)
{
	int nDay,nMonth,nYear,nHour,nMinute,nSecond;

	CString strTime = lpszTime;
	int nPos = strTime.Find(',');
	if (nPos != -1)
	{
		strTime = strTime.Mid(nPos+1);
		strTime.TrimLeft();

		CString strDay,strMonth,strYear,strHour,strMinute,strSecond;
		CString strAllMonth = "jan,feb,mar,apr,may,jan,jul,aug,sep,oct,nov,dec";
		strDay = strTime.Left(2);
		nDay = atoi(strDay);
		strMonth = strTime.Mid(3,3);
		strMonth.MakeLower();
		nPos = strAllMonth.Find(strMonth);
		if (nPos != -1)
		{
			strMonth.Format("%d",((nPos/4)+1));
			nMonth = atoi(strMonth);
		}
		else
			nMonth = 1;
		strTime = strTime.Mid(6);
		strTime.TrimLeft();
		nPos = strTime.FindOneOf(" \t");
		if (nPos != -1)
		{
			strYear = strTime.Left(nPos);
			nYear = atoi(strYear);
		}
		else
			nYear = 2000;
		strTime = strTime.Mid(nPos+1);
		strHour = strTime.Left(2);
		nHour = atoi(strHour);
		strMinute = strTime.Mid(3,2);
		nMinute = atoi(strMinute);
		strSecond = strTime.Mid(6,2);
		nSecond = atoi(strSecond);
	}
	
	CTime time(nYear,nMonth,nDay,nHour,nMinute,nSecond);
	return time;
}

UINT CHTTPDownload::ThreadFunc(int index)
{
	CSocket pSocket;

	pSocket.Create();
	pSocket.Connect(m_strServer, m_nPort);
	
	CString strSend, strRange, strHeader;
	char szReadBuf[1025];

	strSend  = "GET " + m_strObject + " HTTP/1.1\r\n";
	strSend += "Host: " + m_strServer + "\r\n";
	strSend += "Accept: */*\r\n";
	strSend += "Pragma: no-cache\r\n"; 
	strSend += "Cache-Control: no-cache\r\n";
	if( !m_strReferer.IsEmpty() )
		strSend += "Referer: " + m_strReferer + "\r\n";
	strSend += "Connection: close\r\n";
	strRange.Format("Range: bytes=%d-%d\r\n", m_state.range[2 * index], m_state.range[2 * index + 1]);
	if(m_bSupportResume)
		strSend += strRange;
	//必须要加一个空行，否则Http服务器将不会应答
	strSend += "\r\n";

	int ret = pSocket.Send(strSend.GetBuffer(0), strSend.GetLength());
	strSend.ReleaseBuffer();

	DWORD lpArgument;
	if(!pSocket.AsyncSelect(0) && !pSocket.IOCtl(FIONBIO, &lpArgument))
		return -1; 
	strHeader.Empty();
	int iStatus = 0;
	ZeroMemory(szReadBuf,1025);
	ret = pSocket.Receive(szReadBuf, 1025);
		
	int n = GetHeadLength(szReadBuf);		
	
	CFile file;
	CString name;
	name.Format("%d", index);
	name = m_strTempSavePath + name;
	int bOpen = 0;
	if(!m_bResume)
		bOpen = file.Open(name, CFile::modeCreate | CFile::modeWrite);
	else
		bOpen = file.Open(name, CFile::modeWrite);

	if(bOpen == 0){
		TRACE("Error in file open!\n");
		return -10;
	}
	file.SeekToEnd();
	TRACE("Entering the key section!\n");
	file.Write(szReadBuf + n, ret - n);
	int sum = ret - n, num = 0;
	while(1){
		if(m_bTerminate[index]){
			m_state.range[2 * index] = m_state.range[2 * index] + sum;
			return -5;
		}
		ZeroMemory(szReadBuf,1025);
		if(!(num = pSocket.Receive(szReadBuf, 1025)) 
			|| num == SOCKET_ERROR)
			break; // (EOF||network error)
		else{
			file.Write(szReadBuf, num);
			sum += num;
		}
	}
	
	file.Close();
	return 0;
}

int CHTTPDownload::GetHeadLength(char *lpData)
{
	int ndx = 0;
	CString str;
	while(1){
		str.Empty();
		str = GetLine(lpData, ndx);
		if(str.IsEmpty())
			break;
	}
	return (ndx);
}

CString CHTTPDownload::GetLine(char *lpData, int& ndx)
{
	BOOL bLine = FALSE;
	CString str;
	while ( bLine == FALSE && ndx < 1025 )
	{
		char ch = (char)(lpData[ndx]);
		switch( ch )
		{
		case '\r': // ignore
			break;
		case '\n': // end-of-line
			bLine = TRUE;
			break;
		default:   // other....
			str += ch;
			break;
		}
		++ndx;
	}
	return str;
}

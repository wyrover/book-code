// SearchLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ASC.h"
#include "SearchLogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchLogDlg dialog
extern _ConnectionPtr g_pConnection;
CString vartostr(const _variant_t& var);


CSearchLogDlg::CSearchLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchLogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearchLogDlg)
	m_fromTimeValue1 = 0;
	m_fromTimeValue2 = 0;
	m_toTimeValue1 = 0;
	m_toTimeValue2 = 0;
	//}}AFX_DATA_INIT
}


void CSearchLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchLogDlg)
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_toTime2);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_toTime1);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_fromTime2);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_fromTime1);
	DDX_Control(pDX, IDC_LIST1, m_logList);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_fromTimeValue1);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_fromTimeValue2);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER3, m_toTimeValue1);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER4, m_toTimeValue2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchLogDlg, CDialog)
	//{{AFX_MSG_MAP(CSearchLogDlg)
	ON_BN_CLICKED(IDB_SERACH, OnSearch)
	ON_BN_CLICKED(IDB_SEEMAIL, OnSeemail)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchLogDlg message handlers

BOOL CSearchLogDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_logList.InsertColumn(0, _T("处理时间"),       LVCFMT_CENTER, 80);

	m_logList.InsertColumn(1, _T("邮件名"),         LVCFMT_CENTER, 160);
	m_logList.InsertColumn(2, _T("发件人"),       LVCFMT_CENTER, 140);

	m_logList.InsertColumn(3, _T("收件人"),       LVCFMT_CENTER, 140);
	m_logList.InsertColumn(4, _T("邮件主题"),         LVCFMT_CENTER, 140);
	m_logList.InsertColumn(5, _T("邮件大小"),       LVCFMT_CENTER, 60);

	// set some extnded styles
	m_logList.SetExtendedStyle (LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_FLATSB);
	
	CTime fromtimeIni(2004,5,1,0,0,0);
	m_fromTime1.SetTime(&fromtimeIni);
	m_fromTime2.SetTime(&fromtimeIni);

	LPSYSTEMTIME lpSystemTime = new SYSTEMTIME;
	GetSystemTime(lpSystemTime);
	m_toTime1.SetTime(lpSystemTime);
	m_toTime2.SetTime(lpSystemTime);

	delete lpSystemTime;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSearchLogDlg::OnSearch() 
{
//	CoInitialize(NULL);

	
	_RecordsetPtr m_pRecordsetSearchLog;
	m_pRecordsetSearchLog.CreateInstance("ADODB.Recordset");
	

	CString strFilter1 = "";
	CString strTimeFrom;
	CString strTimeTo;
	UpdateData(TRUE);
	TimeToStr(m_fromTimeValue1,m_fromTimeValue2,strTimeFrom);
	TimeToStr(m_toTimeValue1,m_toTimeValue2,strTimeTo);
	strFilter1.Format("TimeStamp > '%s' and TimeStamp < '%s'",strTimeFrom,strTimeTo);
	m_pRecordsetSearchLog->Filter = strFilter1.AllocSysString();
	
	try{

		m_pRecordsetSearchLog->Open("DataLog",_variant_t((IDispatch*)g_pConnection,true),adOpenStatic,adLockOptimistic,adCmdTable);
		ASSERT(m_pRecordsetSearchLog != NULL);
	}
	catch (...) {
		AfxMessageBox("m_pRecordsetSearchLog record failure");
		return ;
	}

	CString strTimeStamp;
	CString strSize;
	int iItem = 0;
	m_logList.DeleteAllItems( );
	while(!m_pRecordsetSearchLog->adoEOF){
		m_searchLogData.TimeStamp = m_pRecordsetSearchLog->GetCollect("TimeStamp").date;
		m_searchLogData.Size = m_pRecordsetSearchLog->GetCollect("Size").iVal;// 
		strSize.Format("%d",m_searchLogData.Size);
		m_searchLogData.Sender = m_pRecordsetSearchLog->GetCollect("Sender").bstrVal;
		m_searchLogData.Receiver = m_pRecordsetSearchLog->GetCollect("Receiver").bstrVal;
		m_searchLogData.Mail_File = m_pRecordsetSearchLog->GetCollect("Mail_File").bstrVal;
		m_searchLogData.Subject = m_pRecordsetSearchLog->GetCollect("Subject").bstrVal;
		
		strTimeStamp.Format("%d-%d-%d %d:%d:%d",m_searchLogData.TimeStamp.GetYear()
			,m_searchLogData.TimeStamp.GetMonth(),m_searchLogData.TimeStamp.GetDay()
			,m_searchLogData.TimeStamp.GetHour(),m_searchLogData.TimeStamp.GetMinute()
			,m_searchLogData.TimeStamp.GetSecond());
		
		m_logList.InsertItem(iItem, strTimeStamp);
		m_logList.SetItemText(iItem,1, m_searchLogData.Mail_File);
	
		m_logList.SetItemText(iItem, 2,m_searchLogData.Sender);
		m_logList.SetItemText(iItem,3, m_searchLogData.Receiver);
		m_logList.SetItemText(iItem,4, m_searchLogData.Subject);
		m_logList.SetItemText(iItem,5, strSize);

		m_pRecordsetSearchLog->MoveNext();
		++iItem;
		


	}
	
	m_pRecordsetSearchLog->Close();
	// TODO: Add your control notification handler code here
	
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

void CSearchLogDlg::TimeToStr(CTime& time1,CTime& time2,CString& strTime)
{
//	CString strTime = "";
	strTime.Format("%d-%d-%d %d:%d:%d",time1.GetYear(),time1.GetMonth(),time1.GetDay()
		,time2.GetHour(),time2.GetMinute(),time2.GetSecond());


}

void CSearchLogDlg::OnSeemail() 
{
	// TODO: Add your control notification handler code here
	UINT uSelectedCount = m_logList.GetSelectedCount();
	int  nItem = -1;

	CString mailfileName;
	_RecordsetPtr m_pRecordsetSelect;
	m_pRecordsetSelect.CreateInstance("ADODB.Recordset");


	//according the select record ,show the eml
	if (uSelectedCount == 1)
	{
		
		//get the emlfilename
		nItem = m_logList.GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		int len =  m_logList.GetItemText( nItem, 1, mailfileName.GetBuffer(60), 60 );
		mailfileName.ReleaseBuffer();

		//get the record from MailSample
		CString strMailSampleSql = _T("");
		strMailSampleSql.Format("select * from MailSample where emlPathNm ='%s'",mailfileName);
		try
		{
			m_pRecordsetSelect->Open(_variant_t(LPCTSTR(strMailSampleSql))
				,_variant_t((IDispatch*)g_pConnection,true),adOpenStatic,adLockOptimistic,adCmdText);
		}
		catch (...) {
			return;
		}
		
		//get the eml context
		long emlSize = m_pRecordsetSelect->GetFields()->GetItem("emlContent")->ActualSize;
		char* lpData = new char[emlSize];
		ASSERT(lpData);
		GetChunk(m_pRecordsetSelect->GetFields()->GetItem("emlContent"), lpData);
		
		//write tempemlfile
		CFile tempEmlfile;
		CFileException e;
		if( !tempEmlfile.Open( "C:\\tempfile.eml", CFile::modeCreate | CFile::modeWrite, &e ) )
		{
			TRACE("open file failue:%s\n",e.m_cause );
			return;
		}
		tempEmlfile.Write(lpData,emlSize);
		
		//show eml
		ShellExecute(this->m_hWnd, 	"open",	"C:\\tempfile.eml", NULL, NULL,SW_SHOW );		
	}
	else{
		if(uSelectedCount <= 0)
		{
			MessageBox("请先选中记录!");
		}
		else
		{
			MessageBox("你选中了多条记录,请选择其中的一条记录!");
		}
		
	}
	
}



BOOL CSearchLogDlg::GetChunk(FieldPtr pField, LPVOID lpData)// _RecordsetPtr pRecordset,
{
	ASSERT(pField != NULL);
	ASSERT(lpData != NULL);
	
	UCHAR chData;
	long index = 0;
	long size = pField->ActualSize;

	while (index < size)
	{ 
		try
		{
			_variant_t varChunk = pField->GetChunk(100);
			if (varChunk.vt != (VT_ARRAY | VT_UI1))
			{
				return FALSE;
			}
			
            for (long i = 0; i < 100; i++)
            {
                if (SUCCEEDED( SafeArrayGetElement(varChunk.parray, &i, &chData) ))
                {
					((UCHAR*)lpData)[index] = chData;
                    index++;
                }
                else
				{
                    break;
				}
            }
		}
		catch (_com_error e)
		{
			TRACE(_T("Warning: GetChunk 方法发生异常. 错误信息: %s; 文件: %s; 行: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
			return FALSE;
		}
	}
	
	return TRUE;
}



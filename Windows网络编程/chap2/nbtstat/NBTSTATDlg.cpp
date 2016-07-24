// NBTSTATDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NBTSTAT.h"
#include "NBTSTATDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define destPORT 137  //nbtstat name port
#define myPORT 4321

/////////////////////////////////////////////////////////////////////////////
// CNBTSTATDlg dialog
//--------------------------------var-------------------------------
CNBTSTATDlg* pDlg;
BYTE bs[50]={0x0,0x00,0x0,0x10,0x0,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x20,0x43,0x4b,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x0,0x0,0x21,0x0,0x1};
unsigned char B1[4],B2[4];
HANDLE wait_handle;

CString strOldIP="";
bool bExit=false;
int i;

//------------------------------------------------------------------


CNBTSTATDlg::CNBTSTATDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNBTSTATDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNBTSTATDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//---------------------------------------------
	pDlg=this;
	if(!m_UDPSocket.Create(myPORT,SOCK_DGRAM))
	{
		AfxMessageBox("Failed Create Socket");
	}
	//---------------------------------------------
}

void CNBTSTATDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNBTSTATDlg)
	DDX_Control(pDX, IDC_SPIN1, m_spin);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_IPADDRESS2, m_IPEdit2);
	DDX_Control(pDX, IDC_LIST3, m_ListView);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IPEdit1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNBTSTATDlg, CDialog)
	//{{AFX_MSG_MAP(CNBTSTATDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, OnFieldchangedIpaddress1)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_BTN_ABOUT, OnBtnAbout)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST3, OnDblclkListView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNBTSTATDlg message handlers

BOOL CNBTSTATDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	m_IPEdit1.SetAddress(210,28,128,1);
	m_IPEdit2.SetAddress(210,28,143,255);
	wait_handle=CreateEvent(NULL,true,false,"receive data");
	GetDlgItem(IDC_BTN_EXIT)->EnableWindow(false);
	m_spin.SetRange(100,10000);
	m_spin.SetPos(100);

	// -----------------ListView initialize---------------------
    DWORD dwStyle=GetWindowLong(m_ListView.GetSafeHwnd(),GWL_STYLE);
	dwStyle&=~LVS_TYPEMASK;
	dwStyle|=LVS_REPORT;
	SetWindowLong(m_ListView.GetSafeHwnd(),GWL_STYLE,dwStyle);

    m_ListView.InsertColumn(0,"MAC地址",LVCFMT_LEFT,120);
	m_ListView.InsertColumn(0,"用户\\其它",LVCFMT_LEFT,100);
	m_ListView.InsertColumn(0,"主机",LVCFMT_LEFT,80);
	m_ListView.InsertColumn(0,"工作组",LVCFMT_LEFT,80);
	m_ListView.InsertColumn(0,"IP地址",LVCFMT_LEFT,100);
    
	m_ListView.SetExtendedStyle(LVS_EX_GRIDLINES);
    ::SendMessage(m_ListView.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
      LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	
	//----------------------------------------------------------

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNBTSTATDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNBTSTATDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


//--------------------------nbtstat线程----------------------
UINT NbtstatThread(LPVOID param)
{
	//bool bFirst=true;
	do
	{
		if(bExit)
		{
			AfxMessageBox("exit thread!");
			pDlg->GetDlgItem(IDC_BTN_SEND)->EnableWindow(true);
			pDlg->GetDlgItem(IDC_EDIT1)->EnableWindow(true);
	        pDlg->GetDlgItem(IDC_SPIN1)->EnableWindow(true);
			pDlg->GetDlgItem(IDC_IPADDRESS1)->EnableWindow(true);
			pDlg->GetDlgItem(IDC_IPADDRESS2)->EnableWindow(true);
			pDlg->GetDlgItem(IDC_BTN_EXIT)->EnableWindow(false);
			bExit=false;
			return 1;
		}

		pDlg->m_strIP.Format("%d.%d.%d.%d",B1[0],B1[1],B1[2],B1[3]);
		pDlg->m_ListBox.InsertString(0,pDlg->m_strIP);
		if(B1[3]!=0&&B1[2]!=0)
		pDlg->m_UDPSocket.SendTo((void*)bs,50,destPORT,pDlg->m_strIP,0);

		
        int nWait=pDlg->m_spin.GetPos();
		WaitForSingleObject(
		wait_handle,        // handle to object to wait for
		nWait   // time-out interval in milliseconds
		);
		ResetEvent(wait_handle);
		

		//=============================================
		if(B1[2]<=B2[2])
		{
		   //if(B1[3]==255)AfxMessageBox("here0");
		   if(B1[3]<B2[3])B1[3]++;
		   else if(B1[2]<B2[2]&&B1[3]<255)B1[3]++;
		   else if(B1[2]<B2[2]&&B1[3]==255)
		   {
			   //AfxMessageBox("here");
			   B1[3]=0;
			   B1[2]++;
		   }
		}
		else break;
		if(B1[3]>=B2[3]&&B1[2]>=B2[2])break;
		//if(!bFirst)break;
		//if(B1[3]=B2[3])bFirst=false;

	}while(B1[2]<=255&&B1[3]<=255);
	pDlg->m_ListBox.InsertString(0,"-----complete!-----");
	pDlg->GetDlgItem(IDC_BTN_SEND)->EnableWindow(true);
	pDlg->GetDlgItem(IDC_EDIT1)->EnableWindow(true);
	pDlg->GetDlgItem(IDC_SPIN1)->EnableWindow(true);
	pDlg->GetDlgItem(IDC_IPADDRESS1)->EnableWindow(true);
	pDlg->GetDlgItem(IDC_IPADDRESS2)->EnableWindow(true);
	pDlg->GetDlgItem(IDC_BTN_EXIT)->EnableWindow(false);

	return 0;
}
//-----------------------------------------------------------

void CNBTSTATDlg::OnBtnSend() 
{
	// TODO: Add your control notification handler code here
	
	m_IPEdit1.GetAddress(B1[0],B1[1],B1[2],B1[3]);
	m_IPEdit2.GetAddress(B2[0],B2[1],B2[2],B2[3]);

	if(B2[2]<B1[2]){AfxMessageBox("终止地址应大于起始地址！"); return ;}
	else if(B2[2]==B1[2]&&B2[3]<B1[3]){AfxMessageBox("终止地址应大于起始地址！"); return ;}
	if(B2[0]!=B1[0]||B2[1]!=B1[1]){AfxMessageBox("不支持A类或B类网！"); return ;}
 
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(false);
	GetDlgItem(IDC_EDIT1)->EnableWindow(false);
	GetDlgItem(IDC_SPIN1)->EnableWindow(false);
	GetDlgItem(IDC_IPADDRESS1)->EnableWindow(false);
	GetDlgItem(IDC_IPADDRESS2)->EnableWindow(false);
	GetDlgItem(IDC_BTN_EXIT)->EnableWindow(true);

	AfxBeginThread(NbtstatThread,this->GetSafeHwnd(),THREAD_PRIORITY_NORMAL);

}

void CNBTSTATDlg::OnReceive()
{
	
	BYTE Buf[500];
	//m_UDPSocket.Receive(Buf,500,0);

	CString str,strIP,strHost,strHex,strMac,Host,Group,User;
	UINT dport;
	m_UDPSocket.ReceiveFrom(Buf,500,strIP,dport,0);

	if(strIP==(char)NULL||strIP==strOldIP)return;
	strOldIP=strIP;

	int index=m_ListView.InsertItem(0,strIP);

	strHost="";
	strHex="";
	User="?";
	Host="\\";
	
	int tem=0,num=0;
	bool bAdd=true;

	//if(m_strIP==strIP)
	for(i=57;i<500;i++) //57-72
	{
		if(Buf[i]==0xcc)break; 
		if(Buf[i]==0x20)bAdd=false;
		if(bAdd)
		{
			str.Format("%c",Buf[i]);
			if(Buf[i]>=' ')strHost+=str;

			str.Format("%02x.",Buf[i]);
			strHex+=str;
		}

		if((++tem)%18==0)
		{
            bAdd=true; 
			//m_ListBox.AddString(strHost);//
			strHost.TrimRight((char)NULL);
			if(strHost=="")
			{
   				strMac.Delete(17,strMac.GetLength()-17);
				m_ListView.SetItem(index,4,LVIF_TEXT,strMac, 0, 0, 0,0);
				break;
			}


			if(num==0&&strHost!="")
			{
				m_ListView.SetItem(index,2,LVIF_TEXT,strHost, 0, 0, 0,0);
				Host=strHost;
				num++;
			}
			else
			{
				if(Host!=strHost&&num==1&&strHost!="")
				{
					m_ListView.SetItem(index,1,LVIF_TEXT,strHost, 0, 0, 0,0);
					Group=strHost;
					//AfxMessageBox(strHost);
				    num++;
				}
				else 
				{
					if(strHost!=Host&&strHost!=Group&&num==2&&strHost!="")
					{
						User=strHost;
						if(User!="__MSBROWSE__")
						{
							m_ListView.SetItem(index,3,LVIF_TEXT,User, 0, 0, 0,0);
							num++;
						}
						//num++;
					}
				}

			}
			
			strMac=strHex;
			strHost="";
			strHex="";
			
		}
		
	}
	SetEvent(wait_handle);
	

}

void CNBTSTATDlg::OnFieldchangedIpaddress1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	unsigned char AddrB[4];
	m_IPEdit1.GetAddress(AddrB[0],AddrB[1],AddrB[2],AddrB[3]);
	m_IPEdit2.SetAddress(AddrB[0],AddrB[1],AddrB[2],255);	

	*pResult = 0;
}

void CNBTSTATDlg::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	bExit=true;
}

CString MakeLen(CString str,int len)
{
	if(str.GetLength()<len)
	{
		for(int i=str.GetLength();i<len;i++)str+=' ';
	}
	else str+=' ';

	return str;
}

void CNBTSTATDlg::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	int Count=m_ListView.GetItemCount();
	CFile f;
	CFileException e;
	CString strFileName;

	CTime t=CTime::GetCurrentTime();
	CString strTime = t.Format( "%d-%H-%M-%S" );
	strTime="NBT_LOG-"+strTime;
	//char* filename=(LPSTR)(LPCTSTR)strTime;

	CFileDialog fileDlg(FALSE,"txt",strTime,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"txt",NULL); //TRUE 为打开对话框
	if(fileDlg.DoModal()==IDOK)
	{
		UpdateData(true);
		strFileName=fileDlg.GetFileName();
		UpdateData(false);
		
		if( f.Open(strFileName, CFile::modeCreate | CFile::modeWrite, &e ) )
		{
		
			CString str;
			str="IP地址           工作组        主机          用户          MAC地址\r\n\r\n";
			f.Write( str, str.GetLength() );

			for(int i=0;i<Count;i++)
			{
				str=MakeLen(m_ListView.GetItemText(i,0),17)
					+MakeLen(m_ListView.GetItemText(i,1),14)
				    +MakeLen(m_ListView.GetItemText(i,2),14)
					+MakeLen(m_ListView.GetItemText(i,3),14)
					+MakeLen(m_ListView.GetItemText(i,4),17);
				str+="\r\n\r\n";
				f.Write( str, str.GetLength() );
			}
			f.Close();
		}
	}	
}

void CNBTSTATDlg::OnBtnAbout() 
{
	// TODO: Add your control notification handler code here
	AfxMessageBox("----------加强版的Nbtstat--------\r\n\r\n        Copyright 2001 朱巍");
}

void CNBTSTATDlg::OnDblclkListView(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	CString str;

	//int SelIndex=m_ListView.GetSelectionMark( );
	POSITION pos=m_ListView.GetFirstSelectedItemPosition();
	if(pos==NULL)
	{
		AfxMessageBox("请选择想要打开的主机。");
		return;
	}
    int SelIndex = m_ListView.GetNextSelectedItem(pos);

	str=m_ListView.GetItemText(SelIndex,2);
	str="\\\\"+str;
	char* host=&char(*str);
	::ShellExecute(this->GetSafeHwnd(),"open",host,NULL,"c:\\",SW_SHOW);
	//AfxMessageBox(host);

	*pResult = 0;
}

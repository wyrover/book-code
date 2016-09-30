// ConnectionClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ConnectionClient.h"
#include "ConnectionClientDlg.h"
#include "Sink.h"
#include <atlbase.h>
#include <unknwn.h>

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectionClientDlg dialog

CConnectionClientDlg::CConnectionClientDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CConnectionClientDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CConnectionClientDlg)
    m_number1 = 0;
    m_number2 = 0;
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConnectionClientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CConnectionClientDlg)
    DDX_Text(pDX, IDC_EDIT1, m_number1);
    DDX_Text(pDX, IDC_EDIT2, m_number2);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConnectionClientDlg, CDialog)
    //{{AFX_MSG_MAP(CConnectionClientDlg)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CConnectionClientDlg::OnSendToServer)
    //}}AFX_MSG_MAP


END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectionClientDlg message handlers

BOOL CConnectionClientDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    m_number1 = 1;
    m_number2 = 2;
    UpdateData(0);
    HRESULT hr = CoInitialize(NULL);

    if (hr != S_OK)
        return -1;

    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CConnectionClientDlg::OnPaint()
{
    if (IsIconic()) {
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
    } else {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CConnectionClientDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CConnectionClientDlg::OnSendToServer()
{
    HRESULT  hr;
    UpdateData(1);
    //call CoInitialize for com initialisation
    //hr =CoInitialize(NULL); I will rather put this
    //if(hr != S_OK)          in InitDialog
    //  return -1;
    // create an instance of the com object



	// CLSID_Add 组件 ID
    CComPtr<IAdd> pAdd;
    hr = pAdd.CoCreateInstance(CLSID_Add);

    if (hr != S_OK)
        return;


	// 查询是否支持连接点
    IConnectionPointContainer   * pCPC;
    // IConnectionPoint          * pCP;   //these are declared as a member
    //DWORD                     dwAdvise; //variable,shown here for completeness
    //check if this interface supports connectable objects
    hr = pAdd->QueryInterface(IID_IConnectionPointContainer, (void **)&pCPC);

    if (!SUCCEEDED(hr)) {
        return;
    }

    //
    //ok it does now get the correct connection point interface
    // in our case IID_IAddEvents
    hr = pCPC->FindConnectionPoint(IID__IAddEvents, &pCP);

    if (!SUCCEEDED(hr)) {
        return;
    }

    //we have done with the connection point container interface
    pCPC->Release();




	// 定义事件响应对象
    IUnknown *pSinkUnk;
    // create a notification object from our CSink class
    //
    CSink *pSink;
    pSink = new CSink;

    if (NULL == pSink) {
        return;
    }

    //Get the pointer to CSink's IUnknown pointer
    hr = pSink->QueryInterface(IID_IUnknown, (void **)&pSinkUnk);

    //Pass it to the com through the com's  _IAddEvents interface (pCP) retrieved
    // through the earlier FindConnectoinPoint call
    hr = pCP->Advise(pSinkUnk, &dwAdvise);
    //dwAdvise is the number returned ,through which
    //  IConnectionPoint:UnAdvise is called to break the connection
    pAdd->Add(m_number1 , m_number2);
    pCP->Unadvise(dwAdvise); //call this when you need to
    //                         disconnect from server
    pCP->Release();
    //CoUninitialize();
}




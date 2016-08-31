// MessageTable.cpp : implementation file
//

#include "stdafx.h"
#include "CursorIconExplorer.h"
#include "MessageTable.h"
#include "CursorIconMessages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageTable dialog


CMessageTable::CMessageTable(CWnd* pParent /*=NULL*/)
        : CDialog(CMessageTable::IDD, pParent)
{
        //{{AFX_DATA_INIT(CMessageTable)
        //}}AFX_DATA_INIT
        lang = MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT);
        msgid = 0;
        icon = NULL;
}


void CMessageTable::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CMessageTable)
        DDX_Control(pDX, IDC_ICONDEMO, c_Icon);
        DDX_Control(pDX, IDC_MSGID, c_MsgID);
        DDX_Control(pDX, IDC_MSG, c_Msg);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMessageTable, CDialog)
        //{{AFX_MSG_MAP(CMessageTable)
        ON_BN_CLICKED(IDC_ENGLISH, OnEnglish)
        ON_BN_CLICKED(IDC_FRENCH, OnFrench)
        ON_BN_CLICKED(IDC_GERMAN, OnGerman)
        ON_BN_CLICKED(IDC_GOODBYE, OnGoodbye)
        ON_BN_CLICKED(IDC_HELLO, OnHello)
        ON_BN_CLICKED(IDC_CAT, OnCat)
        ON_BN_CLICKED(IDC_FISH, OnFish)
        ON_BN_CLICKED(IDC_SPANISH, OnSpanish)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageTable message handlers

void CMessageTable::makeMessage()
    {
     LPTSTR result;
     TCHAR tmp[20];

     if(msgid == 0)
        return;
     wsprintf(tmp, _T("%08lx"), msgid);
     c_MsgID.SetWindowText(tmp);
     
     FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                   FORMAT_MESSAGE_FROM_HMODULE,
                   AfxGetInstanceHandle(),
                   msgid,
                   lang,
                   (LPTSTR)&result,
                   0,
                   NULL);
     
     c_Msg.SetWindowText(result);
     LocalFree(result);

     HICON i;
     if(icon != 0)
	i = AfxGetApp()->LoadIcon(icon);
     else
	i = NULL;

     c_Icon.SetIcon(i);
    }
            

void CMessageTable::OnCat() 
{
 msgid = MSG_CAT;
 icon = IDI_CAT;
 makeMessage();
        
}

void CMessageTable::OnEnglish() 
{
        lang = MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT);
        makeMessage();
        
}

void CMessageTable::OnFrench() 
{
        lang = MAKELANGID(LANG_FRENCH, SUBLANG_DEFAULT);
        makeMessage();
        
}

void CMessageTable::OnGerman() 
{
        lang = MAKELANGID(LANG_GERMAN, SUBLANG_DEFAULT);
        makeMessage();
        
}

void CMessageTable::OnGoodbye() 
{
 msgid = MSG_GOODBYE;
 icon = NULL;
 makeMessage();
        
}

void CMessageTable::OnHello() 
{
 msgid = MSG_HELLO;
 icon = NULL;
 makeMessage();
        
}

void CMessageTable::OnFish() 
{
 msgid = MSG_FISH;
 icon = IDI_JMNCO;
 makeMessage();

}

void CMessageTable::OnSpanish() 
{
        lang = MAKELANGID(LANG_SPANISH, SUBLANG_DEFAULT);
        makeMessage();
}

BOOL CMessageTable::OnInitDialog() 
{
        CDialog::OnInitDialog();
        
        CheckRadioButton(IDC_ENGLISH, IDC_FRENCH, IDC_ENGLISH);
        lang = MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT);
        

        CheckRadioButton(IDC_HELLO, IDC_FISH, IDC_HELLO);
        msgid = MSG_HELLO;
        icon = NULL;
        
        makeMessage();

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

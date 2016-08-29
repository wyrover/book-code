// ListDir.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "ixable.h"
#include "msglog.h"
#include "loggable.h"
#include "listops.h"
#include "TinyButt.h"
#include "ListDir.h"

#include "is95.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DIM(x) (sizeof(x) / sizeof(x[0]))

/////////////////////////////////////////////////////////////////////////////
// CListDir property page

IMPLEMENT_DYNCREATE(CListDir, CListOps)

CListDir::CListDir() : CListOps(CListDir::IDD)
{
        //{{AFX_DATA_INIT(CListDir)
        //}}AFX_DATA_INIT
}

CListDir::~CListDir()
{
}

void CListDir::DoDataExchange(CDataExchange* pDX)
{
        CListOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CListDir)
        DDX_Control(pDX, IDC_GETSHORTPATHNAME, c_GetShortPathName);
        DDX_Control(pDX, IDC_FILENAME, c_Filename);
        DDX_Control(pDX, IDC_DIR, c_Dir);
        DDX_Control(pDX, IDC_DDL_SYSTEM, c_System);
        DDX_Control(pDX, IDC_DDL_READWRITE, c_ReadWrite);
        DDX_Control(pDX, IDC_DDL_READONLY, c_ReadOnly);
        DDX_Control(pDX, IDC_DDL_HIDDEN, c_Hidden);
        DDX_Control(pDX, IDC_DDL_EXCLUSIVE, c_Exclusive);
        DDX_Control(pDX, IDC_DDL_DRIVES, c_Drives);
        DDX_Control(pDX, IDC_DDL_DIRECTORY, c_Directory);
        DDX_Control(pDX, IDC_DDL_ARCHIVE, c_Archive);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListDir, CListOps)
        //{{AFX_MSG_MAP(CListDir)
        ON_BN_CLICKED(IDC_DIR, OnDir)
        ON_BN_CLICKED(IDC_GETSHORTPATHNAME, OnGetshortpathname)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListDir message handlers


/****************************************************************************
*                            CListDir::getFileAttr
* Result: int
*       A set of flags representing the control settings
****************************************************************************/

int CListDir::getFileAttr()
    {
     int attr = 0;

     if(c_Archive.GetCheck())
        attr |= DDL_ARCHIVE;

     if(c_Directory.GetCheck())
        attr |= DDL_DIRECTORY;

     if(c_Drives.GetCheck())
        attr |= DDL_DRIVES;

     if(c_Exclusive.GetCheck())
        attr |= DDL_EXCLUSIVE;

     if(c_Hidden.GetCheck())
        attr |= DDL_HIDDEN;

     if(c_ReadOnly.GetCheck())
        attr |= DDL_READONLY;

     if(c_ReadWrite.GetCheck())
        attr |= DDL_READWRITE;

     if(c_System.GetCheck())
        attr |= DDL_SYSTEM;

     return attr;
    }

BOOL CListDir::OnSetActive() 
{
        return CListOps::OnSetActive();
}

BOOL CListDir::OnInitDialog() 
{
        CListOps::OnInitDialog();
        
        // if(Is95())
        //      EnableWindow(c_ShortPathName, FALSE);

        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CListDir::OnDir() 
{
 CListBox * box = getListBox();
 CString s;

 c_Filename.GetWindowText(s);

 int attr = getFileAttr();

 int result = box->Dir(attr, s);

 showResult_x_s_LBERR(IDS_LB_DIR, attr, s, result);
        
}

void CListDir::OnGetshortpathname() 
{
 CString s;
 TCHAR temp[256];
 c_Filename.GetWindowText(s);
 ::GetShortPathName(s, temp, DIM(temp));
 c_Filename.SetWindowText(temp);
}

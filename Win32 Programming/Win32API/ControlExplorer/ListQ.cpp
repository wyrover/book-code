// ListQ.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "ixable.h"
#include "msglog.h"
#include "indexpg.h"
#include "loggable.h"
#include "listops.h"
#include "ListQ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListQuery property page

IMPLEMENT_DYNCREATE(CListQuery, CListOps)

CListQuery::CListQuery() : CListOps(CListQuery::IDD)
{
        //{{AFX_DATA_INIT(CListQuery)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CListQuery::~CListQuery()
{
}

void CListQuery::DoDataExchange(CDataExchange* pDX)
{
        CListOps::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CListQuery)
                // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListQuery, CListOps)
        //{{AFX_MSG_MAP(CListQuery)
        ON_BN_CLICKED(IDC_GETANCHORINDEX, OnGetanchorindex)
        ON_BN_CLICKED(IDC_GETCOUNT, OnGetcount)
        ON_BN_CLICKED(IDC_GETCURSEL, OnGetcursel)
        ON_BN_CLICKED(IDC_GETHORIZONTALEXTENT, OnGethorizontalextent)
        ON_BN_CLICKED(IDC_GETLOCALE, OnGetlocale)
        ON_BN_CLICKED(IDC_GETSELCOUNT, OnGetselcount)
        ON_BN_CLICKED(IDC_GETSELITEMS, OnGetselitems)
        ON_BN_CLICKED(IDC_GETTOPINDEX, OnGettopindex)
        ON_BN_CLICKED(IDC_GETCARETINDEX, OnGetcaretindex)
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListQuery message handlers

BOOL CListQuery::OnSetActive() 
{
        return CListOps::OnSetActive();
}

BOOL CListQuery::OnInitDialog() 
{
        CListOps::OnInitDialog();
        
        return TRUE;  // return TRUE unless you set the focus to a control
                      // EXCEPTION: OCX Property Pages should return FALSE
}

void CListQuery::OnGetanchorindex() 
{
 CListBox * box = getListBox();
 int result = box->SendMessage(LB_GETANCHORINDEX);
 showResult_N_N_LBERR(IDS_LB_GETANCHORINDEX, result);
}

void CListQuery::OnGetcount() 
{
 CListBox * box = getListBox();
 int result = box->SendMessage(LB_GETCOUNT);
 showResult_N_N_d(IDS_LB_GETCOUNT, result);
}

void CListQuery::OnGetcursel() 
{
 CListBox * box = getListBox();
 int result = box->SendMessage(LB_GETCURSEL);
 showResult_N_N_LBERR(IDS_LB_GETCURSEL, result);
}

void CListQuery::OnGethorizontalextent() 
{
 CListBox * box = getListBox();
 int result = box->SendMessage(LB_GETHORIZONTALEXTENT);
 showResult_N_N_d(IDS_LB_GETHORIZONTALEXTENT, result);
}

void CListQuery::OnGetlocale() 
{
 CListBox * box = getListBox();
 DWORD result = box->SendMessage(LB_GETLOCALE);
 showResult_N_N_DW(IDS_LB_GETLOCALE, result);
        
}

void CListQuery::OnGetselcount() 
{
 CListBox * box = getListBox();
 int result = box->SendMessage(LB_GETSELCOUNT);
 showResult_N_N_LBERR(IDS_LB_GETSELCOUNT, result);
}

void CListQuery::OnGetselitems() 
{
 CListBox * box = getListBox();
 int count = box->SendMessage(LB_GETSELCOUNT);
 if(count == LB_ERR)
    { /* not multiselect */
     showResult_d_B_LBERR(IDS_LB_GETSELITEMS, 0, count);
    } /* not multiselect */
 else
    { /* multiselect */
     int * buffer = new int[count];
     int result = box->SendMessage(LB_GETSELITEMS, count, (LPARAM)buffer);

     CString s(_T(""));
     CString sep(_T(""));
     for(int i = 0; i < count; i++)
        { /* format result */
         CString t;
         t.Format(_T("%s%d"), sep, buffer[i]);
         s += t;
         sep = _T(", ");
        } /* format result */

     CString text;
     text.Format(_T("%d: [%s]"), count, s);
     showResult_d_B_s(IDS_LB_GETSELITEMS, count, text);
     delete buffer;
    } /* multiselect */
}

void CListQuery::OnGettopindex() 
{
 CListBox * box = getListBox();
 int result = box->SendMessage(LB_GETTOPINDEX);
 showResult_N_N_d(IDS_LB_GETTOPINDEX, result);
}

void CListQuery::OnGetcaretindex() 
{
 CListBox * box = getListBox();
 int result = box->SendMessage(LB_GETCARETINDEX);
 showResult_N_N_d(IDS_LB_GETCARETINDEX, result);
}


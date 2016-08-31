// loggable.cpp : implementation file
//

#include "stdafx.h"
#include "ControlExplorer.h"
#include "ixable.h"
#include "msglog.h"
#include "loggable.h"
#include "xform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef UNICODE
#define HEXCHAR _T("%04x")
#else
#define HEXCHAR _T("%02x")
#endif
/////////////////////////////////////////////////////////////////////////////
// CLoggingPage property page

IMPLEMENT_DYNCREATE(CLoggingPage, CIndexablePage)

CLoggingPage::CLoggingPage(int idd) : CIndexablePage(idd)
{
}

CLoggingPage::CLoggingPage() : CIndexablePage((int)0)  // should never call default constructor!
{
        //{{AFX_DATA_INIT(CLoggingPage)
                // NOTE: the ClassWizard will add member initialization here
        //}}AFX_DATA_INIT
}

CLoggingPage::~CLoggingPage()
{
}

void CLoggingPage::DoDataExchange(CDataExchange* pDX)
{
        CIndexablePage::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CLoggingPage)
                // NOTE: the ClassWizard will add DDX and DDV calls here
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoggingPage, CIndexablePage)
        //{{AFX_MSG_MAP(CLoggingPage)
                // NOTE: the ClassWizard will add message map macros here
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*****************************************************************************
   The naming convention:

   ShowResult_w_l_r  where 'w' is the wParam value flag, l is the lParam 
                     value flag, and 'r' is the result value flag.  The
                     flags are defined as below:

          b - Boolean
          B - "Buffer", the string "&b" to indicate a buffer argument
          c - character
          CBERR - used only in the result position, will print a value
                  as either its integer value or as "-1 (CB_ERR)"
          d - value as decimal number
          DW - value as a DWORD in hex with LOWORD and HIWORD display decimal
          LBERR - used only in the result position, will print a value
                  as either its integer value or as "-1 (LB_ERR)"
          N - no value present
          R - "Rectangle", the string "&r" to indicate a RECT argument
          s - string
          x - value as hex number

    Given the values possible, we have |w| * |l| * |r| possible functions
    where |x| indicates the count of the number of flags valid for the
    position.  At the moment, |w|==|l| and |w|+1==|r|.  Note that we do
    not currently support every possible permutation of values.

    The question may arise: why didn't I use overloading in C++?  The
    answer is that overload resolution is too weak to distinguish wanting
    to print values of the same type in different formats (since it doesn't
    have, for example, the "branded types" of more sophisticated class
    models such as Modula-3).  This was the best compromise; the alternative
    was to use overloading for some cases and different function names
    for other cases, which introduced a level of complexity I did not feel
    like dealing with.

*****************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CLoggingPage message handlers

/****************************************************************************
*                             CLoggginPage::errstr
* Inputs:
*       int result: result value
* Result: CString
*       "-1 (LB_ERR)"
****************************************************************************/

CString CLoggingPage::errstr(int result, int errid)
    {
     CString s;
     CString err;

     err.LoadString(errid);

     if(result == LB_ERR || result == CB_ERR)
        s.Format(_T("%d (%s)"), result, err);
     else
        s.Format(_T("%d"), result);

     return s;
    }

/****************************************************************************
*                       CLoggingPage::showResult_b_DW_N
* Inputs:
*       int msg: Message id
*       BOOL b: wParam flag
*       DWORD lParam: Doubleword value
* Result: void
*       
* Effect: 
*       �
****************************************************************************/

void CLoggingPage::showResult_b_DW_N(int msg, BOOL b, DWORD lParam)
    {
     CString t;
     t.LoadString(msg);

     CString bstr;
     if(b)
        bstr.LoadString(IDS_TRUE);
     else
        bstr.LoadString(IDS_FALSE);
     
     CString s;
     s.Format(_T("%s(%s, %08x=(%d, %d))"), t, bstr, lParam, 
                                LOWORD(lParam), HIWORD(lParam));
     addMessage(s);
    }

/****************************************************************************
*                       CLoggingPage::showResult_b_d_N
* Inputs:
*       int msg: Message id
*       BOOL b: wParam flag
*       int lParam: 
* Result: void
*       
* Effect: 
*       �
****************************************************************************/

void CLoggingPage::showResult_b_d_N(int msg, BOOL b, int lParam)
    {
     CString t;
     t.LoadString(msg);

     CString bstr;
     if(b)
        bstr.LoadString(IDS_TRUE);
     else
        bstr.LoadString(IDS_FALSE);
     
     CString s;
     s.Format(_T("%s(%s, %d)"), t, bstr, lParam);
     addMessage(s);
    }

/****************************************************************************
*                       CLoggingPage::showResult_b_N_N
* Inputs:
*       int msg: Message id
*       BOOL b: wParam flag
* Result: void
*       
* Effect: 
*       �
****************************************************************************/

void CLoggingPage::showResult_b_N_N(int msg, BOOL b)
    {
     CString t;
     t.LoadString(msg);

     CString bstr;
     if(b)
        bstr.LoadString(IDS_TRUE);
     else
        bstr.LoadString(IDS_FALSE);
     
     CString s;
     s.Format(_T("%s(%s)"), t, bstr);
     addMessage(s);
    }

/****************************************************************************
*                           CLoggingPage::showResult_N_N_DW
* Inputs:
*       int msg: Message id
*       DWORD result: DWORD that is to be shown as two integers
* Result: void
*       
* Effect: 
*       Displays a DWORD-returning message of no arguments, where the
*       value is a DWORD with a HIWORD and LOWORD component
****************************************************************************/

void CLoggingPage::showResult_N_N_DW(int msg, DWORD result)
{
 CString t;
 t.LoadString(msg);

 CString s;
 s.Format(_T("%s =>0x%08x (%d, %d)"), t, result, LOWORD(result), HIWORD(result));
 addMessage(s);
}

/****************************************************************************
*                      CLoggingPage::showResult_N_DW_ERR
* Inputs:
*       int msg: Message id
*       DWORD lparam: DWORD that is to be shown as two integers
*       int result: Result code
*       int err: Error code
* Result: void
*       
* Effect: 
*       Displays the message
****************************************************************************/

void CLoggingPage::showResult_N_DW_ERR(int msg, DWORD lParam, int result, int err)
{
 CString t;
 t.LoadString(msg);

 CString s;
 s.Format(_T("%s(0, 0x%08x (%d, %d))"), t, lParam, 
                                        LOWORD(lParam), HIWORD(lParam),
                                        errstr(result, err));
 addMessage(s);
}

/****************************************************************************
*                           CLoggingPage::showResult_N_N_d
* Inputs:
*       int msg: Message ID
*       int result: Result id
* Result: void
*       
* Effect: 
*       Displays a message of no parameters that returns an int
****************************************************************************/

void CLoggingPage::showResult_N_N_d(int msg, int result)
{
 CString t;
 t.LoadString(msg);
 
 CString s;
 s.Format(_T("%s => %d"), t, result);
 addMessage(s);
}

/****************************************************************************
*                           CLoggingPage::showResult_N_R_N
* Inputs:
*       int msg: Message ID
*       LPRECT r: Rectangle pointer
* Result: void
*       
* Effect: 
*       Displays a message of no parameters that returns void; shows the
*       rectangle as well
****************************************************************************/

void CLoggingPage::showResult_N_R_N(int msg, LPRECT r)
{
 CString t;
 t.LoadString(msg);
 
 CString s;
 s.Format(_T("%s(&r); r=>[%d, %d, %d, %d]"), t, r->left, r->top, 
                                               r->right, r->bottom);
 addMessage(s);
}

/****************************************************************************
*                           CLoggingPage::showResult_b_R_b
* Inputs:
*       int msg: Message ID
*       BOOL w: wParam avlue
*       LPRECT r: Rectangle pointer
*       BOOL result: Result
* Result: void
*       
* Effect: 
*       Displays a message of no parameters that returns void; shows the
*       rectangle as well
****************************************************************************/

void CLoggingPage::showResult_b_R_b(int msg, BOOL w, LPRECT r, BOOL result)
{
 CString t;
 t.LoadString(msg);

 CString ws;
 ws.LoadString(w ? IDS_TRUE : IDS_FALSE);
 
 CString rs;
 rs.LoadString(result ? IDS_TRUE : IDS_FALSE);

 CString s;
 s.Format(_T("%s(%s, &r) => %s; r=>[%d, %d, %d, %d]"), 
                        t, ws, rs,
                        r->left, r->top, 
                        r->right, r->bottom);
 addMessage(s);
}

/****************************************************************************
*                       CLoggingPage::showResult_N_N_s
* Inputs:
*       int msg: Message ID
*       CString result: Result value as string
* Result: void
*       
* Effect: 
*       Displays a message of no parameters that returns a value which is
*       reformatted by the caller into a string
****************************************************************************/

void CLoggingPage::showResult_N_N_s(int msg, CString result)
{
 CString t;
 t.LoadString(msg);
 
 CString s;
 s.Format(_T("%s => %s"), t, result);
 addMessage(s);
}

/****************************************************************************
*                       CLoggingPage::showResult_d_B_s
* Inputs:
*       int msg: Message ID
*       CString result: Result value as string
* Result: void
*       
* Effect: 
*       Displays a message of wParam and a buffer pointer that returns a value
*       which is reformatted by the caller into a string
****************************************************************************/

void CLoggingPage::showResult_d_B_s(int msg, int w, CString result)
{
 CString t;
 t.LoadString(msg);
 
 CString s;
 s.Format(_T("%s(%d, &b) => %s"), t, w, result);
 addMessage(s);
}

/****************************************************************************
*                       CLoggingPage::showResult_d_B_d
* Inputs:
*       int msg: Message ID
*       int w: WPARAM value
*       int result: Result value 
* Result: void
*       
* Effect: 
*       Displays a message of wParam and a buffer pointer that returns a value
*       which is reformatted by the caller into a string
****************************************************************************/

void CLoggingPage::showResult_d_B_d(int msg, int w, int result)
{
 CString t;
 t.LoadString(msg);
 
 CString s;
 s.Format(_T("%s(%d, &b) => %d"), t, w, result);
 addMessage(s);
}

/****************************************************************************
*                       CLoggingPage::showResult_d_B_ERR
* Inputs:
*       int msg: Message ID
*       int w: wParam value
*       int result: Result value as string
*       int err: IDS_LB_ERR or IDS_CB_ERR
* Result: void
*       
* Effect: 
*       Displays a message of wParam and a buffer pointer that returns a value
*       which can be LB_ERR
****************************************************************************/

void CLoggingPage::showResult_d_B_ERR(int msg, int w, int result, int err)
{
 showResult_d_B_s(msg, w, errstr(result, err));
}

/****************************************************************************
*                          CLoggingPage::showResult_d_d_b
* Inputs:
*       int msg: ID of message
*       int w: value of wParam
*       int l: Value of lParam
*       BOOL result: Result
* Result: void
*       
* Effect: 
*       Displays a message of two params that returns a Boolean result
****************************************************************************/

void CLoggingPage::showResult_d_d_b(int msg, int w, int l, BOOL result)
    {
     CString t;
     t.LoadString(msg);

     CString val;
     val.LoadString(result ? IDS_TRUE : IDS_FALSE);

     CString s;
     s.Format(_T("%s(%d, %d) => %s"), t, w, l, val);
     addMessage(s);
    }

/****************************************************************************
*                          CLoggingPage::showResult_d_d_d
* Inputs:
*       int msg: ID of message
*       int w: value of wParam
*       int l: Value of lParam
*       int result: Result
* Result: void
*       
* Effect: 
*       Displays a message of two params that returns an integer result
****************************************************************************/

void CLoggingPage::showResult_d_d_d(int msg, int w, int l, int result)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(%d, %d) => %d"), t, w, l, result);
     addMessage(s);
    }

/****************************************************************************
*                          CLoggingPage::showResult_d_d_pt
* Inputs:
*       int msg: ID of message
*       int w: value of wParam
*       int l: Value of lParam
*       DWORD result: Result
* Result: void
*       
* Effect: 
*       Displays a message of two params that returns a POINTS result
****************************************************************************/

void CLoggingPage::showResult_d_d_pt(int msg, int w, int l, DWORD result)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(%d, %d) => (x: %d, y: %d)"), t, w, l, 
                                (int)(short)LOWORD(result), 
                                (int)(short)HIWORD(result));
     addMessage(s);
    }

/****************************************************************************
*                          CLoggingPage::showResult_d_d_N
* Inputs:
*       int msg: ID of message
*       int w: value of wParam
*       int l: Value of lParam
* Result: void
*       
* Effect: 
*       Displays a message of two params that returns an integer result
****************************************************************************/

void CLoggingPage::showResult_d_d_N(int msg, int w, int l)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(%d, %d)"), t, w, l);
     addMessage(s);
    }

/****************************************************************************
*                          CLoggingPage::showResult_d_d_s
* Inputs:
*       int msg: ID of message
*       int w: value of wParam
*       int l: Value of lParam
*       CString result: Result
* Result: void
*       
* Effect: 
*       Displays a message of two params that returns an result that has
*       been reformatted.
****************************************************************************/

void CLoggingPage::showResult_d_d_s(int msg, int w, int l, CString result)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(%d, %d) => %s"), t, w, l, result);
     addMessage(s);
    }

/****************************************************************************
*                          CLoggingPage::showResult_d_s_s
* Inputs:
*       int msg: ID of message
*       int w: value of wParam
*       CString l: Value of lParam
*       CString result: Result
* Result: void
*       
* Effect: 
*       Displays a message of two params that returns an result that has
*       been reformatted.
****************************************************************************/

void CLoggingPage::showResult_d_s_s(int msg, int w, CString l, CString result)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(%d, %s) => %s"), t, w, l, result);
     addMessage(s);
    }

/****************************************************************************
*                          CLoggingPage::showResult_d_s_b
* Inputs:
*       int msg: ID of message
*       int w: value of wParam
*       CString l: Value of lParam
*       BOOL result: Result
* Result: void
*       
* Effect: 
*       Displays a message of two params that returns a Boolean result
****************************************************************************/

void CLoggingPage::showResult_d_s_b(int msg, int w, CString l, BOOL result)
    {
     CString t;
     t.LoadString(msg);

     CString rstr;
     rstr.LoadString(result ? IDS_TRUE : IDS_FALSE);

     CString s;
     s.Format(_T("%s(%d, %s) => %s"), t, w, l, rstr);
     addMessage(s);
    }

/****************************************************************************
*                       CLoggingPage::showResult_N_i_b
* Inputs:
*       int id: Message id
*       BOOL result: result code
* Result: void
*       
* Effect: 
*       Displays the result
****************************************************************************/

void CLoggingPage::showResult_N_i_b(int id, BOOL result)
    {
     showResult_d_s_b(id, 0, _T("&item"), result);
    }

/****************************************************************************
*                       CLoggingPage::showResult_N_i_d
* Inputs:
*       int id: ID
*       int result: Result code
* Result: void
*       
* Effect: 
*       �
****************************************************************************/

void CLoggingPage::showResult_N_i_d(int id, int result)
    {
     showResult_d_s_d(id, 0, _T("&item"), result);
    }

/****************************************************************************
*                          CLoggingPage::showResult_s_N_s
* Inputs:
*       int msg: ID of message
*       CString w: value of wParam
*       CString result: Result
* Result: void
*       
* Effect: 
*       Displays a message of two params that returns an result that has
*       been reformatted.
****************************************************************************/

void CLoggingPage::showResult_s_N_s(int msg, CString w, CString result)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(%s, 0) => %s"), t, w, result);
     addMessage(s);
    }

/****************************************************************************
*                       CLoggingPage::showResult_b_N_b
* Inputs:
*       int msg: Message id
*       BOOL w: wParam value
*       BOOL result: Result value
* Result: void
*       
* Effect: 
*       Displays the message
****************************************************************************/

void CLoggingPage::showResult_b_N_b(int msg, BOOL w, BOOL result)
    {
     CString ws;
     CString res;

     ws.LoadString(w ? IDS_TRUE : IDS_FALSE);
     res.LoadString(result ? IDS_TRUE : IDS_FALSE);
     showResult_s_N_s(msg, ws, res);
    }

/****************************************************************************
*                          CLoggingPage::showResult_d_N_s
* Inputs:
*       int msg: ID of message
*       int w: value of wParam
*       CString result: Result
* Result: void
*       
* Effect: 
*       Displays a message of two params that returns an result that has
*       been reformatted.
****************************************************************************/

void CLoggingPage::showResult_d_N_s(int msg, int w, CString result)
    {
     showResult_d_d_s(msg, w, 0, result);
    }

/****************************************************************************
*                           CLoggingPage::showResult_d_N_N
* Inputs:
*       int msg: ID of message
*       int w: wParam value
* Result: void
*       
* Effect: 
*       Displays the fact that a void-returning message of wParam was sent
****************************************************************************/

void CLoggingPage::showResult_d_N_N(int msg, int w)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(%d, 0)"), t, w);
     addMessage(s);
    }

/****************************************************************************
*                           CLoggingPage::showResult_c_N_N
* Inputs:
*       int msg: ID of message
*       TCHAR w: wParam value
* Result: void
*       
* Effect: 
*       Displays the fact that a void-returning message of wParam was sent
****************************************************************************/

void CLoggingPage::showResult_c_N_N(int msg, TCHAR w)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s('%s', 0)"), t, charToStr(w));
     addMessage(s);
    }

/****************************************************************************
*                           CLoggingPage::showResult_d_N_d
* Inputs:
*       int msg: ID of message
*       int w: wParam value
* Result: void
*       
* Effect: 
*       Displays the fact that an int-returning message of wParam was sent
****************************************************************************/

void CLoggingPage::showResult_d_N_d(int msg, int w, int result)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(%d, 0) => %d"), t, w, result);
     addMessage(s);
    }

/****************************************************************************
*                            CLoggingPage::showResult_N_N_N
* Inputs:
*       int msg: ID of message
* Result: void
*       
* Effect: 
*       Displays the fact that a void-returning message of no params was sent
****************************************************************************/

void CLoggingPage::showResult_N_N_N(int msg)
    {
     CString s;
     s.LoadString(msg);

     addMessage(s);
    }

/****************************************************************************
*                           CLoggingPage::showResult_N_N_c
* Inputs:
*       int msg: ID to use for message
*       TCHAR ch: Character code
* Result: void
*       
* Effect: 
*       msg => '%c' (0x%02x)
****************************************************************************/

void CLoggingPage::showResult_N_N_c(int msg, TCHAR ch)
    {
     CString t;
     t.LoadString(msg);

     CString chstr = charToStr(ch);

     CString s;
     s.Format(_T("%s => '%s' (" HEXCHAR ")"), t, chstr, ch);
     addMessage(s);
    }
            

/****************************************************************************
*                          CLoggingPage::addMessage
* Inputs:
*       CString s: String to format
* Result: void
*       
* Effect: 
*       ASSERT(FALSE).  This is supposed to be implemented by each of the
*       subclasses and should never be called directly.
****************************************************************************/

void CLoggingPage::addMessage(CString s)
    {
     ASSERT(FALSE); // this must be implemented in derived classes
    }

/****************************************************************************
*                     CLoggingPage::showResult_N_N_ERR
* Inputs:
*       int msg: ID of message
*       int result: Result code of message
*       int err: ID of either LB_ERR or CB_ERR string
* Result: void
*       
* Effect: 
*       Shows the result either as a number or, if -1, as the string "-1
*       (LBERR)"
****************************************************************************/

void CLoggingPage::showResult_N_N_ERR(int msg, int result, int err)
    {
     showResult_N_N_s(msg, errstr(result, err));
    }

/****************************************************************************
*                     CLoggingPage::showResult_d_d_ERR
* Inputs:
*       int msg: ID of message
*       int result: Result code of message
*       int err: IDS_LB_ERR or IDS_CB_ERR
* Result: void
*       
* Effect: 
*       Shows the wParam and lParam, and displays the result either as a
*       number or, if -1, as the string "-1 ([LC]BERR)"
****************************************************************************/

void CLoggingPage::showResult_d_d_ERR(int msg, int w, int l, int result, int err)
    {
     showResult_d_d_s(msg, w, l, errstr(result, err));
    }

/****************************************************************************
*                     CLoggingPage::showResult_d_N_ERR
* Inputs:
*       int msg: ID of message
*       int result: Result code of message
*       int err: IDS_LB_ERR or IDS_CB_ERR
* Result: void
*       
* Effect: 
*       Shows the wParam and lParam, and displays the result either as a
*       number or, if -1, as the string "-1 ([LC]BERR)"
****************************************************************************/

void CLoggingPage::showResult_d_N_ERR(int msg, int w, int result, int err)
    {
     showResult_d_N_s(msg, w, errstr(result, err));
    }

/****************************************************************************
*                     CLoggingPage::showResult_N_s_ERR
* Inputs:
*       int msg:
*       CString s: LPARAM string value
*       int result: Result, possibly LB_ERR
*       int err: IDS_LB_ERR or IDS_CB_ERR
* Result: void
*       
* Effect: 
*       �
****************************************************************************/

void CLoggingPage::showResult_N_s_ERR(int msg, CString s, int result, int err)
    {
     showResult_N_s_s(msg, s, errstr(result, err));
    }

/****************************************************************************
*                          CLoggingPage::showResult_N_s_d
* Inputs:
*       int msg: ID of message
*       CString l: Value of lParam
*       int result: Result
* Result: void
*       
* Effect: 
*       Displays a message of a string lParam that returns an integer result
****************************************************************************/

void CLoggingPage::showResult_N_s_d(int msg, CString l, int result)
    {
     showResult_d_s_d(msg, 0, l, result);
    }

/****************************************************************************
*                          CLoggingPage::showResult_N_s_b
* Inputs:
*       int msg: ID of message
*       CString l: Value of lParam
*       BOOL result: Result
* Result: void
*       
* Effect: 
*       Displays a message of a string lParam that returns an integer result
****************************************************************************/

void CLoggingPage::showResult_N_s_b(int msg, CString l, BOOL result)
    {
     CString s;
     s.LoadString(result ? IDS_TRUE : IDS_FALSE);
     showResult_d_s_s(msg, 0, l, s);
    }

/****************************************************************************
*                          CLoggingPage::showResult_N_s_s
* Inputs:
*       int msg: ID of message
*       CString l: Value of lParam
*       CString result: Result
* Result: void
*       
* Effect: 
*       Displays a message of a string lParam that returns an integer result
****************************************************************************/

void CLoggingPage::showResult_N_s_s(int msg, CString l, CString result)
    {
     showResult_d_s_s(msg, 0, l, result);
    }

/****************************************************************************
*                          CLoggingPage::showResult_d_s_d
* Inputs:
*       int msg: ID of message
*       int w: value of wParam
*       CString l: Value of lParam
*       int result: Result
* Result: void
*       
* Effect: 
*       Displays a message of two params that returns an integer result
****************************************************************************/

void CLoggingPage::showResult_d_s_d(int msg, int w, CString l, int result)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(%d, %s) => %d"), t, w, l, result);
     addMessage(s);
    }

/****************************************************************************
*                          CLoggingPage::showResult_N_s_x
* Inputs:
*       int msg: ID of message
*       CString l: Value of lParam
*       DWORD result: Result
* Result: void
*       
* Effect: 
*       Displays a message of lParam that returns an integer result to be
*       shown in hex (e.g., an HWND)
****************************************************************************/

void CLoggingPage::showResult_N_s_x(int msg, CString l, DWORD result)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(0, %s) => %08x"), t, l, result);
     addMessage(s);
    }

/****************************************************************************
*                     CLoggingPage::showResult_d_s_ERR
* Inputs:
*       int msg: ID of message
*       int w: value of wParam
*       CString l: Value of lParam
*       int result: Result
*       int err: IDS_LB_ERR or IDS_CB_ERR
* Result: void
*       
* Effect: 
*       Displays a message of two params that returns an integer result
****************************************************************************/

void CLoggingPage::showResult_d_s_ERR(int msg, int w, CString l, int result, int err)
    {
     showResult_d_s_s(msg, w, l, errstr(result, err));
    }

/****************************************************************************
*                       CLoggingPage::showResult_s_s_s
* Inputs:
*       int msg:
*       CString w:
*       CString l:
*       CString result:
* Result: void
*       
* Effect: 
*       �
****************************************************************************/

void CLoggingPage::showResult_s_s_s(int msg, CString w, CString l, CString result)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(%s, %s) => %s"), t, w, l, result);
     addMessage(s);
    }

/****************************************************************************
*                       CLoggingPage::showResult_s_s_b
* Inputs:
*       int msg:
*       CString w:
*       CString l:
*       BOOL result:
* Result: void
*       
* Effect: 
*       �
****************************************************************************/

void CLoggingPage::showResult_s_s_b(int msg, CString w, CString l, BOOL result)
    {
     CString s;
     s.LoadString(result ? IDS_TRUE : IDS_FALSE);
     showResult_s_s_s(msg, w, l, s);
    }

/****************************************************************************
*                     CLoggingPage::showResult_x_s_ERR
* Inputs:
*       int msg: ID of message
*       int w: value of wParam
*       CString l: Value of lParam
*       int result: Result
*       int err: IDS_LB_ERR or IDS_CB_ERR
* Result: void
*       
* Effect: 
*       Displays a message of two params that returns an integer result.  If
*       the result is LB_ERR or CB_ERR the annotation is also displayed.
****************************************************************************/

void CLoggingPage::showResult_x_s_ERR(int msg, int w, CString l, int result, int err)
    {
     showResult_x_s_s(msg, w, l, errstr(result, err));
    }

/****************************************************************************
*                          CLoggingPage::showResult_x_s_d
* Inputs:
*       int msg: ID of message
*       int w: value of wParam, displayed in hex
*       CString l: Value of lParam
*       int result: Result
* Result: void
*       
* Effect: 
*       Displays a message of two params that returns an integer result
****************************************************************************/

void CLoggingPage::showResult_x_s_d(int msg, int w, CString l, int result)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(0x%04x, %s) => %d"), t, w, l, result);
     addMessage(s);
    }

/****************************************************************************
*                          CLoggingPage::showResult_x_s_s
* Inputs:
*       int msg: ID of message
*       int w: value of wParam, displayed in 04x format
*       CString l: Value of lParam
*       CString result: Result
* Result: void
*       
* Effect: 
*       Displays a message of two params that returns an result that has
*       been reformatted.
****************************************************************************/

void CLoggingPage::showResult_x_s_s(int msg, int w, CString l, CString result)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(0x%04x, %s) => %s"), t, w, l, result);
     addMessage(s);
    }

/****************************************************************************
*                     CLoggingPage::showResult_d_DW_ERR
* Inputs:
*       int msg: message name ID
*       int w: wParam value
*       DWORD l: lParam value
*       int result: Result of the message
*       int err: IDS_LB_ERR or IDS_CB_ERR
* Result: void
*       
* Effect: 
*       Displays the lParam as hex and decimal
****************************************************************************/

void CLoggingPage::showResult_d_DW_ERR(int msg, int w, DWORD l, int result, int err)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(%d, 0x%08x=(%d,%d)) => %s"), t, w, l, LOWORD(l), 
                                                           HIWORD(l), 
                                                           errstr(result, err));
     addMessage(s);
     
    }

/****************************************************************************
*                     CLoggingPage::showResult_d_DW_N
* Inputs:
*       int msg: message name ID
*       int w: wParam value
*       DWORD l: lParam value
* Result: void
*       
* Effect: 
*       Displays the lParam as hex and decimal
****************************************************************************/

void CLoggingPage::showResult_d_DW_N(int msg, int w, DWORD l)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(%d, 0x%08x=(%d,%d))"), t, w, l, LOWORD(l), 
                                                           HIWORD(l)); 
     addMessage(s);
     
    }

/****************************************************************************
*                     CLoggingPage::showResult_d_DW_d
* Inputs:
*       int msg: message name ID
*       int w: wParam value
*       DWORD l: lParam value
*       int result: Result of the message
* Result: void
*       
* Effect: 
*       Displays the lParam as hex and decimal
****************************************************************************/

void CLoggingPage::showResult_d_DW_d(int msg, int w, DWORD l, int result)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(%d, 0x%08x=(%d,%d)) => %d"), t, w, l, LOWORD(l), 
                                                           HIWORD(l), 
                                                           result);
     addMessage(s);
     
    }

/****************************************************************************
*                      CLoggingPage::showResult_DW_N_DW
* Inputs:
*       int msg: Message id
*       DWORD locale: new locale version that was set
*       DWORD result: old locale version that was returned
*       int err: IDS_LB_ERR or IDS_CB_ERR
* Result: void
*       
* Effect: 
*       �
****************************************************************************/

void CLoggingPage::showResult_DW_N_DW(int msg, DWORD locale, DWORD result, int err)
    {
     CString t;
     t.LoadString(msg);

     CString s;

     if(result == LB_ERR)
        s.Format(_T("%s(0x%08x=(%d,%d)) => %s"), t, locale, 
                                                LOWORD(locale),
                                                HIWORD(locale),
                                                errstr(result, err));
     else
         s.Format(_T("%s(0x%08x=(%d,%d)) => 0x%08x=(%d,%d)"),
                        t, locale, LOWORD(locale), HIWORD(locale),
                        result, LOWORD(result), HIWORD(result));

     addMessage(s);
    }

/****************************************************************************
*                      CLoggingPage::showResult_d_DW_DW
* Inputs:
*       int msg: Message id
*       int wParam: wParam value
*       int lParam: lParam value
*       DWORD result: result of message
* Result: void
*       
* Effect: 
*       �
****************************************************************************/

void CLoggingPage::showResult_d_DW_DW(int msg, int wParam, DWORD lParam, DWORD result)
    {
     CString t;
     t.LoadString(msg);

     CString s;

     s.Format(_T("%s(%d, 0x%08x=(%d,%d)) => 0x%08x=(%d,%d)"),
                        t, wParam, lParam, LOWORD(lParam), HIWORD(lParam),
                        result, LOWORD(result), HIWORD(result));

     addMessage(s);
    }

/****************************************************************************
*                       CLoggingPage::showResult_DW_d_d
* Inputs:
*       int DW: WPARAM, DWORD pair of integers
*       int d:  LPARAM, decimal
*       int result: Result, decimal
* Result: void
*       
* Effect: 
*       �
****************************************************************************/

void CLoggingPage::showResult_DW_d_d(int msg, WPARAM wParam, int lParam, int result)
    {
     CString t;
     t.LoadString(msg);

     CString s;

     s.Format(_T("%s(0x%08x=(%d,%d)) => %d"), t, 
                                                LOWORD(wParam),
                                                HIWORD(wParam),
                                                lParam,
                                                result);
     addMessage(s);
    }

/****************************************************************************
*                     CLoggingPage::showResult_d_R_ERR
* Inputs:
*       int msg: message name ID
*       int w: wParam value
*       LPRECT r: lParam value
*       int result: the result value of the message
*       int err: IDS_LB_ERR or IDS_CB_ERR
* Result: void
*       
* Effect: 
*       Displays the LPARAM as a Rect
****************************************************************************/

void CLoggingPage::showResult_d_R_ERR(int msg, int w, LPRECT r, int result, int err)
    {
     CString t;
     t.LoadString(msg);

     CString s;
     s.Format(_T("%s(%d, [%d, %d, %d, %d]) => %s"), t, w, 
                                r->left, r->top, r->right, r->bottom,
                                                           errstr(result, err));
     addMessage(s);
     
    }

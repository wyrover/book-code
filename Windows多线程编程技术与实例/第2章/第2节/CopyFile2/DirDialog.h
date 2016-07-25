// DirDialog.h: interface for the CDirDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRDIALOG_H__B4104ACE_CC6B_453F_992D_BFAFC06F0833__INCLUDED_)
#define AFX_DIRDIALOG_H__B4104ACE_CC6B_453F_992D_BFAFC06F0833__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDirDialog
{
  public:
    CDirDialog();
    virtual ~CDirDialog();
    int DoBrowse(CWnd *pwndParent);

    CString m_strPath;
    CString m_strInitDir;
    CString m_strSelDir;
    CString m_strWindowTitle;
    int  m_iImageIndex;
};

#endif // !defined(AFX_DIRDIALOG_H__B4104ACE_CC6B_453F_992D_BFAFC06F0833__INCLUDED_)

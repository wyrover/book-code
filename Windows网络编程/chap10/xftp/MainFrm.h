// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__12BB9B69_5C23_4A93_8206_EAFAAA22E1E4__INCLUDED_)
#define AFX_MAINFRM_H__12BB9B69_5C23_4A93_8206_EAFAAA22E1E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxinet.h>
enum
	{
		FILEICON,
		DIRECTORYICON,
		DIRECTORYSEL,
		LOCFILE,
		SERVFILE
	};

class CDialDlg;
class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CString GetFtpName();
	int GetFtpPort();
	CString GetFtpUser();
	CString GetFtpPassword();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	CSplitterWnd m_wndSplitter1;
	CSplitterWnd m_wndSplitter2;
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	CDialogBar m_wndMyDialogBar;
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnQuickconnect();
	afx_msg void OnFileDisconnect();
	afx_msg void OnUpdateFileDisconnect(CCmdUI* pCmdUI);
	afx_msg void OnFileReconnect();
	afx_msg void OnFileConnect();
	afx_msg void OnUpdateFileConnect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileReconnect(CCmdUI* pCmdUI);
	afx_msg void OnFileStop();
	afx_msg void OnUpdateFileStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSend(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileFtpDownload(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


class CFtpParaClass  
{
public:
	CFtpParaClass();
protected:
	HCURSOR m_hDragCursor;
	BOOL m_bDragging;
	CImageList* m_pDragImageList;
	CImageList m_ctImageList;
public:
	typedef struct
	{
		char szFileName[256];
		char szFileDate[20];
		char szFileSize[20];
		char szFilePath[MAX_PATH];
		int nType;
	}FILE_FTP_INFO;
	typedef struct
	{
		char fileName[256];
		UINT ufileFlag;
	}FILE_COUNT_INFO;
	typedef struct
	{
		char OldName[256];
		char NewName[256];
		LPVOID pThread;
	}FILE_CHANGE_INFO;
	typedef struct
	{
		FILE_COUNT_INFO* pCount;
		LPVOID pThread;
	}SEND_RECEIVE_INFO;

protected:
	int OnInsertDir(CComboBoxEx* pCombo,CString& str,int iIcon,int iIconSel);
	void OnFileName(CListCtrl* pListCtrl);
	void SetPopMenu(int nIndex);
	void OnInsertFile(CListCtrl* pListCtrl,LPVOID pIn,LPARAM lParam);
	void GetFileIcon(CString& fileName,int* iIcon,int* iIconSel=NULL);
	BOOL SetImageList(CListCtrl* pListCtrl,CComboBoxEx* pCombo=NULL);
	void SetFileColumns(CListCtrl* pListCtrl);
public:

	virtual ~CFtpParaClass();

};

/////////////////////////////////////////////////////////////////////////////
// CLocFileView form view



class CLocFileView : public CFormView ,public CFtpParaClass
{
protected:
	CLocFileView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLocFileView)

// Form Data
public:
	//{{AFX_DATA(CLocFileView)
	enum { IDD = IDD_LOC_FORMVIEW };
	CListCtrl	m_ctLocFile;
	CComboBoxEx	m_ctLocDir;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
protected:
	CMainFrame* m_pMainFrame;
	BOOL m_bHaveDotFlag;
	CWinThread* m_pSendFileThread;
	CWinThread* m_pFindLocFileThread;
	int m_iDirCount;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLocFileView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL BeginPreSendFile(LPVOID pInfo);
	BOOL BeginSendFile(LPVOID pInfo);
	BOOL BeginFindLocFile(LPVOID pInfo);
	void GetLocDir();
	int InsertLocDir(CString& str,int iIcon,int iIconSel);
	virtual ~CLocFileView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CLocFileView)
	afx_msg void OnDblclkLocFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeLocDir();
	afx_msg void OnBegindragLocFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRclickLocFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditLocFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditLocFile(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnReDisplayLocFile(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSetLocDirectoryDisplay(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnInsertLocFile(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSetLocRedrawFlag(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnPreSendFile(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSetLocDir(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLocFileSend();
	afx_msg void OnLocFileDelete();
	afx_msg void OnLocFileReName();
	afx_msg void OnLocFileOpen();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CServFileView form view

class CServFileView : public CFormView ,public CFtpParaClass
{
protected:
	CServFileView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CServFileView)

// Form Data
public:
	void OnQuickConnect();
	BOOL BeginRenameFile(LPVOID pInfo);
	BOOL BeginReceiveFile(LPVOID pIn);
	BOOL BeginPreDeleteFile(LPVOID pInfo);
	BOOL BeginPreReceiveFile(LPVOID pInfo);
	BOOL BeginFindServFile(LPVOID pInfo);
	BOOL BeginDeleteFile(LPVOID pInfo);
	//{{AFX_DATA(CServFileView)
	enum { IDD = IDD_SERV_FORMVIEW };
	CButton	m_ctBack;
	CListCtrl	m_ctServFile;
	CComboBoxEx	m_ctServDir;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
protected:
	HBITMAP hBitmap;
	CMainFrame* m_pMainFrame;
	CImageList m_ctServImageList;
	CWinThread*m_pRenameFileThread;
	CWinThread* m_pDeleteFileThread;
	CWinThread* m_pReceiveFileThread;
	CWinThread* m_pFindServFileThread;
	BOOL m_bHaveDotFlag;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServFileView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL BeginPreSendFile(LPVOID pInfo);
	BOOL SetServImageList(UINT nBitmapID);	
	virtual ~CServFileView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CServFileView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkServFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragServFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRclickServFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditServFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeleditServFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBack();
	afx_msg void OnSelchangeServDir();
	//}}AFX_MSG
	afx_msg LRESULT OnPreDeleteFile(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnInsertServFile(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSetServRedrawFlag(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSetServDirectoryDisplay(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnReDisplayServFile(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnSetServDir(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnPreReceiveFile(WPARAM wParam,LPARAM lParam);
	afx_msg void OnServFileDownload();
	afx_msg void OnServFileDelete();
	afx_msg void OnServFileReName();
	afx_msg void OnServFileOpen();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CFtpInfoView view

class CFtpInfoView : public CEditView
{
protected:
	CFtpInfoView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFtpInfoView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFtpInfoView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont m_NewFont;
	virtual ~CFtpInfoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CFtpInfoView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChange();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg LRESULT OnRecordFtpInfo(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CMyAppThread thread
//#include <afxinet.h>
class CMyAppThread : public CWinThread
{
	DECLARE_DYNCREATE(CMyAppThread)
public:
	
protected:
	char* m_szAppName;
	CString m_szFtpName;
	CString m_szFtpUser;
	CString m_szFtpPassword;
	CString m_szFtpDirectory;
	CString m_szLocDirectory;
	int   m_nPort;
	int   m_nCount;
	HINTERNET hFtpConn;

	
protected:
public:
	CMyAppThread();           // protected constructor used by dynamic creation

// Attributes
public:
	CMyAppThread(const char* szLocDirectory,const char* szFtpName=NULL,const char* szFtpUser=NULL,
		const char* szFtpPassword=NULL,int nPort=21,const char* szFtpDirectory=NULL,BOOL bIsFtp=FALSE,int nCount=0)
	{
		extern HINTERNET hInetSession;
		hFtpConn=NULL;
		m_szLocDirectory=szLocDirectory;
		if(bIsFtp)
		{
			m_szFtpName=szFtpName;
			m_szFtpUser=szFtpUser;
			m_szFtpPassword=szFtpPassword;
			m_nPort=nPort;
			m_szFtpDirectory=szFtpDirectory;
			m_nCount=nCount;
			if(!(hFtpConn=InternetConnect(hInetSession,m_szFtpName,m_nPort,m_szFtpUser,
				m_szFtpPassword,INTERNET_SERVICE_FTP,INTERNET_FLAG_PASSIVE,0)))
					hFtpConn=NULL;
		}
	}
public:
	static UINT StartPreDeleteFileThread(LPVOID lParam);
	static UINT StartPreReceiveFileThread(LPVOID lParam);
	static UINT StartPreSendFileThread(LPVOID lParam);
	static UINT StartRenameFileThread(LPVOID lParam);
	static UINT StartDeleteFileThread(LPVOID lParam);
	static UINT StartReceiveFileThread(LPVOID lParam);
	static UINT StartSendFileThread(LPVOID lParam);
	static UINT StartFindLocFileThread(LPVOID lParam);
	static UINT StartFindServFileThread(LPVOID lParam);
	
protected:
	virtual UINT PreDeleteFileThread(LPVOID lParam);
	virtual UINT PreRecieveFileThread(LPVOID lParam);
	virtual UINT PreSendFileThread(LPVOID lParam);
	virtual UINT RenameFileThread(LPVOID lParam);
	virtual UINT DeleteFileThread(LPVOID lParam);
	virtual UINT ReceiveFileThread(LPVOID lParam);
	virtual UINT SendFileThread(LPVOID lParam);
	virtual UINT FindLocFileThread(LPVOID lParam);
	virtual UINT FindServFileThread(LPVOID lParam);
protected:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyAppThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyAppThread();

	// Generated message map functions
	//{{AFX_MSG(CMyAppThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CInputEdit window

class CInputEdit : public CEdit
{
// Construction
public:
	CInputEdit(int iItem,int iSubItem,CString& szInitText,CListCtrl* pListCtrl);

// Attributes
public:

// Operations

	
private:
	CListCtrl* m_pListCtrl;
	CString m_szInitText;
	int m_iItem;
	int m_iSubItem;
	BOOL m_bEsc;
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInputEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInputEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CDialDlg dialog

class CDialDlg : public CDialog
{
// Construction
public:
	CDialDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialDlg)
	enum { IDD = IDD_DIALPROCDIG };
	CComboBox	m_ctEntryName;
	CString	m_szPassword;
	CString	m_szUserID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__12BB9B69_5C23_4A93_8206_EAFAAA22E1E4__INCLUDED_)

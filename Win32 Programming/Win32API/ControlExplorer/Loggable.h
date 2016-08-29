// loggable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLoggingPage dialog

class CLoggingPage : public CIndexablePage
{
	DECLARE_DYNCREATE(CLoggingPage)

// Construction
public:
	CLoggingPage();
	CLoggingPage(int idd);
	~CLoggingPage();
	CMessageLog * c_Messages;

// Dialog Data
	//{{AFX_DATA(CLoggingPage)
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLoggingPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString errstr(int result, int id);

//-----------------------------------------------------------------------------
// b_d_
	void showResult_b_d_N(int msg, BOOL b, int lParam);
	void showResult_b_N_N(int msg, BOOL b);
	void showResult_b_DW_N(int msg, BOOL b, DWORD lParam);
//-----------------------------------------------------------------------------
// b_N_
	void showResult_b_N_b(int msg, BOOL w, BOOL result);

//-----------------------------------------------------------------------------
// b_R_
	void showResult_b_R_b(int msg, BOOL w, LPRECT r, BOOL result);

//-----------------------------------------------------------------------------
// c_N_
	void showResult_c_N_N(int id, TCHAR w);
//-----------------------------------------------------------------------------
// d_B_
	void showResult_d_B_d(int id, int w, int result);
	void showResult_d_B_ERR(int id, int w, int result, int err);
	void showResult_d_B_LBERR(int id, int w, int result) 
			  { showResult_d_B_ERR(id, w, result, IDS_LB_ERR); }
	void showResult_d_B_CBERR(int id, int w, int result) 
			  { showResult_d_B_ERR(id, w, result, IDS_CB_ERR); }
	void showResult_d_B_s(int id, int w, CString result);
//-----------------------------------------------------------------------------
// d_DW_
	void showResult_d_DW_d(int msg, int w, DWORD l, int result);
	void showResult_d_DW_DW(int msg, int wParam, DWORD lParam, DWORD result);
	void showResult_d_DW_ERR(int id, int w, DWORD l, int result, int err);
	void showResult_d_DW_LBERR(int id, int w, DWORD l, int result) 
			  { showResult_d_DW_ERR(id, w, l, result, IDS_LB_ERR);}
	void showResult_d_DW_CBERR(int id, int w, DWORD l, int result) 
			  { showResult_d_DW_ERR(id, w, l, result, IDS_CB_ERR);}
	void showResult_d_DW_N(int msg, int w, DWORD l);
//-----------------------------------------------------------------------------
// d_d_
	void showResult_d_d_b(int id, int w, int l, int result);
	void showResult_d_d_d(int id, int w, int l, int result);
	void showResult_d_d_pt(int id, int w, int l, DWORD result);
	void showResult_d_d_ERR(int id, int w, int l, int result, int err);
	void showResult_d_d_LBERR(int id, int w, int l, int result) 
			  {showResult_d_d_ERR(id, w, l, result, IDS_LB_ERR); }
	void showResult_d_d_CBERR(int id, int w, int l, int result) 
			  {showResult_d_d_ERR(id, w, l, result, IDS_CB_ERR); }
	void showResult_d_d_N(int msg, int w, int l);
	void showResult_d_d_s(int id, int w, int l, CString result);

	void showResult_d_N_d(int id, int w, int result);
	void showResult_d_N_ERR(int id, int w, int result, int err);
	void showResult_d_N_LBERR(int id, int w, int result) 
			  { showResult_d_N_ERR(id, w, result, IDS_LB_ERR); }
	void showResult_d_N_CBERR(int id, int w, int result) 
			  { showResult_d_N_ERR(id, w, result, IDS_CB_ERR); }
	void showResult_d_N_N(int id, int w);
	void showResult_d_N_s(int id, int w, CString result);
//-----------------------------------------------------------------------------
// d_R_
	void showResult_d_R_ERR(int msg, int w, LPRECT r, int result, int err);
	void showResult_d_R_LBERR(int msg, int w, LPRECT r, int result) 
			  {showResult_d_R_ERR(msg, w, r, result, IDS_LB_ERR); }
	void showResult_d_R_CBERR(int msg, int w, LPRECT r, int result) 
			  {showResult_d_R_ERR(msg, w, r, result, IDS_CB_ERR); }
//-----------------------------------------------------------------------------
// d_s_
	void showResult_d_s_b(int msg, int w, CString l, BOOL result);
	void showResult_d_s_CBERR(int id, int w, CString l, int result) 
			  { showResult_d_s_ERR(id, w, l, result, IDS_CB_ERR); }
	void showResult_d_s_d(int id, int w, CString l, int result);
	void showResult_d_s_ERR(int id, int w, CString l, int result, int err);
	void showResult_d_s_LBERR(int id, int w, CString l, int result) 
			  { showResult_d_s_ERR(id, w, l, result, IDS_LB_ERR); }
	void showResult_d_s_s(int id, int w, CString l, CString result);
//-----------------------------------------------------------------------------
// DW_d_
	void showResult_DW_d_d(int msg, WPARAM wParam, int lParam, int result);
//-----------------------------------------------------------------------------
// DW_N_
	void showResult_DW_N_DW(int msg, DWORD locale, DWORD result, int err);
//-----------------------------------------------------------------------------
// N_DW_
	void showResult_N_DW_CBERR(int id, DWORD lParam, int result) 
			 {showResult_N_DW_ERR(id, lParam, result, IDS_CB_ERR);}
	void showResult_N_DW_ERR(int id, DWORD lParam, int result, int err);

//-----------------------------------------------------------------------------
// N_i_
	void showResult_N_i_b(int id, BOOL result);
	void showResult_N_i_d(int id, int result);
//-----------------------------------------------------------------------------
// N_N_
	void showResult_N_N_c(int id, TCHAR ch);
	void showResult_N_N_d(int id, int result);
	void showResult_N_N_DW(int id, DWORD result);
	void showResult_N_N_ERR(int id, int result, int err);
	void showResult_N_N_LBERR(int id, int result) 
				 { showResult_N_N_ERR(id, result, IDS_LB_ERR);}
	void showResult_N_N_CBERR(int id, int result) 
				 { showResult_N_N_ERR(id, result, IDS_CB_ERR);}
	void showResult_N_N_N(int id);
	void showResult_N_N_s(int id, CString s);
//-----------------------------------------------------------------------------
// N_R_
	void showResult_N_R_N(int id, LPRECT r);
//-----------------------------------------------------------------------------
// N_s_
	void showResult_N_s_b(int id, CString l, BOOL result);
	void showResult_N_s_d(int id, CString l, int result);
	void showResult_N_s_ERR(int id, CString s, int result, int err);
	void showResult_N_s_LBERR(int id, CString s, int result) 
			     {showResult_N_s_ERR(id, s, result, IDS_LB_ERR); }
	void showResult_N_s_CBERR(int id, CString s, int result) 
			     {showResult_N_s_ERR(id, s, result, IDS_CB_ERR); }
	void showResult_N_s_s(int id, CString l, CString result);
	void showResult_N_s_x(int id, CString l, DWORD result);
//-----------------------------------------------------------------------------
// s_N_
	void showResult_s_N_s(int id, CString w, CString result);
//-----------------------------------------------------------------------------
// s_s_
	void showResult_s_s_b(int id, CString w, CString l, BOOL result);
        void showResult_s_s_s(int id, CString w, CString l, CString result);
//-----------------------------------------------------------------------------
// x_s_
	void showResult_x_s_d(int id, int w, CString l, int result);
	void showResult_x_s_ERR(int id, int w, CString s, int result, int err);
	void showResult_x_s_LBERR(int id, int w, CString s, int result) 
			  { showResult_x_s_ERR(id, w, s, result, IDS_LB_ERR); }
	void showResult_x_s_CBERR(int id, int w, CString s, int result) 
		          { showResult_x_s_ERR(id, w, s, result, IDS_CB_ERR); }
	void showResult_x_s_s(int id, int w, CString l, CString result);
//-----------------------------------------------------------------------------
	virtual void addMessage(CString s);

	// Generated message map functions
	//{{AFX_MSG(CLoggingPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

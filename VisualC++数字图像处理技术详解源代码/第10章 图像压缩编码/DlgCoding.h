#if !defined(AFX_CDLGCODINGHUFFMAN_H__B7F7D45E_7936_4F76_9142_2F114477725B__INCLUDED_)
#define AFX_CDLGCODINGHUFFMAN_H__B7F7D45E_7936_4F76_9142_2F114477725B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDlgHuffman dialog

class CDlgHuffman : public CDialog
{
	// Construction
	public:
	// 灰度级（256）
	int nColorNum;
	
	// 灰度值出现概率
	double *  dProba;
	
	// 哈夫曼码表
	CString	* m_strCode;

	CDlgHuffman(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgHuffman)
	enum { IDD = IDD_CODING_HUFFMAN };
	CListCtrl	m_lstTable;
	double	m_dEntropy;
	double	m_dCodLength;
	double	m_dRatio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHuffman)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHuffman)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDlgShannon dialog

class CDlgShannon : public CDialog
{
// Construction
public:

	// 灰度级别数目
	int m_nColorNum;
	
	// 各个灰度值出现频率
	double *	m_dProba;
	
	// 香农－弗诺编码表
	CString	* m_strCode;

	CDlgShannon(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgShannon)
	enum { IDD = IDD_CODING_SHANFINO };
	CListCtrl	m_lstTable;
	double	m_dEntropy;
	double	m_dCodLength;
	double	m_dRatio;
	double	m_dAvgCodeLen;
	double	m_dEfficiency;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgShannon)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgShannon)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDlgArith dialog

class CDlgArith : public CDialog
{
// Construction
public:
	CDlgArith(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgArith)
	enum { IDD = IDD_CODING_ARITH };
	CButton	m_decoding;
	CButton	m_coding;
	CEdit	m_ConArithSer;
	CString	m_ArithSerial;
	CString	m_ArithOutput;
	CString	m_ArithDecode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgArith)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgArith)
	afx_msg void OnCoding();
	afx_msg void OnDecoding();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CDlgBitPlane dialog

class CDlgBitPlane : public CDialog
{
// Construction
public:
	CDlgBitPlane(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBitPlane)
	enum { IDD = IDD_CODING_BITPLANE };
	BYTE	m_BItNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBitPlane)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBitPlane)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGCODINGHUFFMAN_H__B7F7D45E_7936_4F76_9142_2F114477725B__INCLUDED_)

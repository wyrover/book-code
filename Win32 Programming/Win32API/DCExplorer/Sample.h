// Sample.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSample window

#define hv_Hq 0x01 // horizontal stripe, quantize to multiple
#define hv_Vq 0x02 // vertical stripe, quantize to multiple

// These two values, although disjoint with ones above, are mutually
// exclusive with their counterpart

#define hv_H  0x04 // horizontal stripe, full width
#define hv_V  0x08 // vertical stripe, full width

// This determines the rop2 code used

#define hv_2h 0x10  // use the ROP2 code for this one's horizontal
#define hv_2v 0x20  // use the ROP2 code for this one's vertical

class CSample : public CStatic
{
// Construction
public:
	CSample();
	int rop;
	int hv;
	void MouseMove(UINT nFlags, CPoint point);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSample)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSample();

	// Generated message map functions
protected:
	void hStripe(CDC * dc, int i, COLORREF color, int count, BOOL quantize);
	void vStripe(CDC * dc, int i, COLORREF color, int count, BOOL quantize);
	static CWnd * capture;  // window that has capture
	COLORREF lastcolor;
	CRect actual_rect;

	//{{AFX_MSG(CSample)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

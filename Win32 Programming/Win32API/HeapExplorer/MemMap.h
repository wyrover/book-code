// MemMap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMemoryMap window

class CMemoryMap : public CStatic
{
// Construction
public:
        CMemoryMap();

// Attributes
public:

// Operations
public:
        static LegendInfo ** getLegend();

// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CMemoryMap)
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CMemoryMap();

        // Generated message map functions
protected:
        //{{AFX_MSG(CMemoryMap)
        afx_msg void OnPaint();
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

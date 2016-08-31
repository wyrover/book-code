// DefHeap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDefaultHeap dialog

class CDefaultHeap : public CDialog
{
// Construction
public:
        CDefaultHeap(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
        //{{AFX_DATA(CDefaultHeap)
        enum { IDD = IDD_DEFAULTHEAP };
        CComboBox       c_Info;
        CLegend c_Legend;
        CHeapPicture    c_Heap;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDefaultHeap)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:

        void countheap();
        // Generated message map functions
        //{{AFX_MSG(CDefaultHeap)
        virtual BOOL OnInitDialog();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

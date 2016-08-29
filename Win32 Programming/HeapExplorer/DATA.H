// data.h : header file
//
#define MYHEAP_LIMIT   300
#define MYHEAP_QUANTUM  10
#define MAX_PAGES        2

class alloc {
        public:
        alloc();
        ~alloc();
        int pages;      // number of pages in allocation
        DWORD pagesize; // page size
        WORD seq;       // sequence number of this item
        WORD count;     // number of blocks to display
        DWORD lasterr;  // GetLastError result of allocation
        LPVOID here;    // pointer to this allocation
        LPVOID base;    // base of region
        DWORD quantum;  // quantum of allocation
        DWORD range;    // element range
        PROCESS_HEAP_ENTRY ph[(MYHEAP_LIMIT/MYHEAP_QUANTUM) * MAX_PAGES];
        RECT pr[(MYHEAP_LIMIT/MYHEAP_QUANTUM) * MAX_PAGES];
      };


/////////////////////////////////////////////////////////////////////////////
// CDataList window

class CDataList : public CListBox
{
// Construction
public:
        CDataList();

// Attributes
public:

// Operations
public:
        static LegendInfo ** GetLegend();
// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CDataList)
        public:
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
        virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
        virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
        //}}AFX_VIRTUAL

// Implementation
public:
        virtual ~CDataList();
        int AddString(alloc * p);
        alloc * GetItemData(int i);

        // Generated message map functions
protected:
        int addr_to_distance(alloc * data, DWORD end, DWORD start, DWORD range);
        //{{AFX_MSG(CDataList)
        afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
        //}}AFX_MSG

        DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

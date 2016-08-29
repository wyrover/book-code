// Heap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHeap dialog

class CHeap : public CDialog
{
// Construction
public:
        CHeap(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
        //{{AFX_DATA(CHeap)
        enum { IDD = IDD_HEAP };
        CPropButton     c_Free_9;
        CPropButton     c_Free_8;
        CPropButton     c_Free_7;
        CPropButton     c_Free_6;
        CPropButton     c_Free_5;
        CPropButton     c_Free_4;
        CPropButton     c_Free_30;
        CPropButton     c_Free_3;
        CPropButton     c_Free_29;
        CPropButton     c_Free_28;
        CPropButton     c_Free_27;
        CPropButton     c_Free_26;
        CPropButton     c_Free_25;
        CPropButton     c_Free_24;
        CPropButton     c_Free_23;
        CPropButton     c_Free_22;
        CPropButton     c_Free_21;
        CPropButton     c_Free_20;
        CPropButton     c_Free_2;
        CPropButton     c_Free_19;
        CPropButton     c_Free_18;
        CPropButton     c_Free_17;
        CPropButton     c_Free_16;
        CPropButton     c_Free_15;
        CPropButton     c_Free_14;
        CPropButton     c_Free_13;
        CPropButton     c_Free_12;
        CPropButton     c_Free_11;
        CPropButton     c_Free_10;
        CInfoBox        c_Info;
        CPropButton     c_Free_1;
        CLegend c_Legend;
        CButton c_HeapCompact;
        CButton c_10;
        CButton c_Pages;
        CButton c_Page1;
        CButton c_Page2;
        CButton c_Allocator;
        CMessage        c_Result;
        CDataList       c_Data;
        CButton c_HeapDestroy;
        CButton c_HeapCreate;
        CButton c_HeapFree;
        //}}AFX_DATA


// Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CHeap)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        DWORD result;
        HANDLE theHeap;
        DWORD pagesize;
        DWORD factor;  // factor to convert allocation to bytes
        int pages;

        void setMessage(DWORD result);
        void create_heap_entry(LPVOID data);
        void heap_to_controls();
        void enable_allocator(BOOL mode);
        void allocate(int size);
        void showSelection(int i);
        int showInfo(int i, PROCESS_HEAP_ENTRY * ph);
        void enable_Free(BOOL mode);
        void enable_Free_matrix();

        afx_msg void OnFreeButton(UINT id);
        afx_msg LRESULT OnShowInfo(WPARAM id, LPARAM);
        afx_msg LRESULT OnReleaseInfo(WPARAM, LPARAM);
        // Generated message map functions
        //{{AFX_MSG(CHeap)
        afx_msg void On10();
        afx_msg void On100();
        afx_msg void On20();
        afx_msg void On30();
        afx_msg void On40();
        afx_msg void On50();
        afx_msg void On60();
        afx_msg void On70();
        afx_msg void On80();
        afx_msg void On90();
        virtual BOOL OnInitDialog();
        afx_msg void OnDestroyHeap();
        afx_msg void OnCreateHeap();
        afx_msg void OnSelchangeData();
        afx_msg void OnDestroy();
        afx_msg void OnSelendcancelInfo();
        afx_msg void OnSelendokInfo();
        afx_msg void OnSelchangeInfo();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

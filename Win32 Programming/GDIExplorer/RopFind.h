// RopFind.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRopFinder dialog

class CRopFinder : public CPropertyPage
{
        DECLARE_DYNCREATE(CRopFinder)

// Construction
public:
        CRopFinder();
        ~CRopFinder();

// Dialog Data
        //{{AFX_DATA(CRopFinder)
        enum { IDD = IDD_ROPFINDER };
        CStatic c_RopCode;
        CBox    c_S7;
        CBox    c_S6;
        CBox    c_S5;
        CBox    c_S4;
        CBox    c_S3;
        CBox    c_S2;
        CBox    c_S1;
        CBox    c_S0;
        CBitRop c_Rop;
        CBox    c_R7;
        CBox    c_R6;
        CBox    c_R5;
        CBox    c_R4;
        CBox    c_R3;
        CBox    c_R2;
        CBox    c_R1;
        CBox    c_R0;
        CBox    c_P7;
        CBox    c_P6;
        CBox    c_P5;
        CBox    c_P4;
        CBox    c_P3;
        CBox    c_P2;
        CBox    c_P1;
        CBox    c_P0;
        CBox    c_D7;
        CBox    c_D6;
        CBox    c_D5;
        CBox    c_D4;
        CBox    c_D3;
        CBox    c_D2;
        CBox    c_D1;
        CBox    c_D0;
        //}}AFX_DATA


// Overrides
        // ClassWizard generate virtual function overrides
        //{{AFX_VIRTUAL(CRopFinder)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        void updateResult();
        void setResult(BYTE rop);
        void setRop();
        void setRopcode();
        BYTE getResult();

        // Generated message map functions
        //{{AFX_MSG(CRopFinder)
        afx_msg void OnR0();
        afx_msg void OnR1();
        afx_msg void OnR2();
        afx_msg void OnR3();
        afx_msg void OnR4();
        afx_msg void OnR5();
        afx_msg void OnR6();
        afx_msg void OnR7();
        afx_msg void OnSelendokRop();
        virtual BOOL OnInitDialog();
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

};

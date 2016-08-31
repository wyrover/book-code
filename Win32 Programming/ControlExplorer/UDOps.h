class CUDOps : public CLoggingPage
    {
	DECLARE_DYNCREATE(CUDOps)
     public:
	CSpinButtonCtrl * c_UpDown;
	CUDOps();
        CUDOps(int idd);
	virtual ~CUDOps();
     	virtual void addMessage(CString s);
    };
	    

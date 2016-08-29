class CTrackOps : public CLoggingPage
    {
	DECLARE_DYNCREATE(CTrackOps)
     public:
	CSliderCtrl * c_Slider;
	CTrackOps();
        CTrackOps(int idd);
	virtual ~CTrackOps();
     	virtual void addMessage(CString s);
    };
	    

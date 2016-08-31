class COwnerDrawMenu : public CMenu 
   {
    public:
	COwnerDrawMenu();
	~COwnerDrawMenu();

	virtual void MeasureItem(LPMEASUREITEMSTRUCT mis);
	virtual void DrawItem(LPDRAWITEMSTRUCT dis);
    protected:
	void DrawItem1(LPDRAWITEMSTRUCT dis);
	void DrawItem2(LPDRAWITEMSTRUCT dis);
   };

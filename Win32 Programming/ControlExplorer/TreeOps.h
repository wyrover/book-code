class CTreeOps : public CLoggingPage
    {
	DECLARE_DYNCREATE(CTreeOps)
     public:
	CTreeCtrl * c_Tree;
	CImageList * imagelist;
	CImageList * botlist;
	CImageList * pixel1;
	CTreeOps();
        CTreeOps(int idd);
	virtual ~CTreeOps();
     	virtual void addMessage(CString s);
	CString itemString(HTREEITEM item);
	BOOL walkTree(CTreeCtrl * tree, HTREEITEM item, 
			BOOL(*callback)(CTreeCtrl * tree, HTREEITEM item, 
						LPARAM data, int depth),
                        LPARAM data,
			int depth = 0);

        void loadComboFromTree(CComboBox * combo, BOOL include_TVI_ROOT = FALSE);
     protected:
	static BOOL addToCombo(CTreeCtrl * tree, HTREEITEM item, LPARAM data, int depth);
    };
	    

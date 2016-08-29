class odinfo {
		public:
		    odinfo();
		    ~odinfo();
		// attributes
		    BOOL subtractCheckWidth;  // TRUE to subtrack check width
		    int style;		      // style code
		    int width;		      // menu width
		    int height;		      // menu height
		// Colors used for Style1 drawing
		    COLORREF text;
		    COLORREF background;
		    COLORREF hilitetext;
		    COLORREF hilitebackground;
		    COLORREF gray;
		    HBITMAP  bmcheck;
		    CString  s;  	      // caption
		    int uid;		      // this instance's uid
		protected:
		    static int uidcounter;    // global counter: creates uid
	       };


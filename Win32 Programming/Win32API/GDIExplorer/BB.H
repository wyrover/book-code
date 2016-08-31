class CBoundingBox : public CRect {
    public:
        CBoundingBox();
        CBoundingBox(BOOL xinvert, BOOL yinvert);

        ~CBoundingBox();
        int SetTop(int newtop);
        int SetTop(CRect r) { return SetTop(r.top); }
        int SetTop(CPoint pt) { return SetTop(pt.y); }

        int SetLeft(int newleft);
        int SetLeft(CRect r) { return SetLeft(r.left); }
        int SetLeft(CPoint pt) { return SetLeft(pt.x); }

        int SetRight(int newright);
        int SetRight(CRect r) { return SetRight(r.right); } 
        int SetRight(CPoint pt) { return SetRight(pt.x); }

        int SetBottom(int newbottom);
        int SetBottom(CRect r) { return SetBottom(r.bottom); }
        int SetBottom(CPoint pt) { return SetBottom(pt.y); }

        void Set(CRect r);
        void Set(int left, int top, int right, int bottom);

        void Set(int x, int y);
        void Set(CPoint pt) { Set(pt.x, pt.y); }

        void SetX(int x);
        void SetY(int y);

        int SetWidth(int newwidth);
        int SetHeight(int newheight);
        void LPtoDP(CDC * dc);
        BOOL Xinvert(BOOL invert) { BOOL t; t = xinvert; xinvert = invert; return t;}
        BOOL Yinvert(BOOL invert) { BOOL t; t = yinvert; yinvert = invert; return t;}
  protected:
        BOOL xinvert;
        BOOL yinvert;
                                  };

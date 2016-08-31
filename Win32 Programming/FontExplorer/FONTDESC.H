class CFontDescriptor {
        public:
                CFontDescriptor(CWnd * wnd, CWnd * fontwnd = NULL);
                ~CFontDescriptor();
                CFont * font;
                CDC * dc;
                LOGFONT lf;
                TEXTMETRIC tm;
                OUTLINETEXTMETRIC * otm;
                ABC abc;
                COLORREF color;
        private:
                int saved;
              };

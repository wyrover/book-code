class CFontObj {
   public: // static methods
        // These deal with the FontObj entity as a whole
        static int GetCount() { return fonttable->GetSize(); }
        static CFontObj * GetFontObj(int i);
        static void FreeAllFonts();
        static void SaveFonts();

   public: // methods
        CFontObj(LPCSTR profile, LPCSTR description, LPCSTR sample = NULL);
        ~CFontObj();
        CFont * GetCurrentFont();
        CFont * GetBaseFont();
        CFont * GetNewFont();
        CFont * GetApplyFont();
        void SetBaseFont(CFont * font);
        void SetNewFont(CFont * font);
        void SetApplyFont(CFont * font);
        void ReplaceBaseFontWithNewFont();
        void SetFontDialog(CFontDialog * fd);
        BOOL MakeFont(LPCSTR facename, int size, int weight, CDC * dc);
        void Free();

   public: // methods for use by font managers
        CString * GetDescription();
        CString * GetSample();

   protected: // methods
        void SaveFont(LOGFONT * lf, CDC * dc);
        void DeleteNewFont();
        void DeleteBaseFont();
        void DeleteApplyFont();
        BOOL IsSameFont(LPLOGFONT lf, CFont * font);
        CString getString(LPCTSTR str);

   protected: // data members
        CFontDialog * fd; // font dialog is active on this font
        LPCTSTR Profile;
        LPCTSTR Description;
        LPCTSTR Sample;

        CString ProfileString;
        CString DescriptionString;
        CString SampleString;

        CFont * Font;  // current font
        CFont * NewFont;  // new font
        CFont * ApplyFont;
        BOOL changed;

   protected:  // Class static objects
        // This is the one-and-only font table for the entire class
        static CPtrArray * fonttable;
                };

#ifndef BMSearchH

    #define BMSearchH

    #include <vcl.h>

    #pragma link "BMSearch"

/*---------------------------------------------------------------------------

            Boyer-Moore ×Ö·û´®²éÕÒËã·¨

//-------------------------------------------------------------------------*/

namespace Bmsearch
{
    class TBMSearch
    {
        static const int NOT_FOUND = 0;

        private:

        int FDelta[256];
        int * FDelta2;
        AnsiString FPattern;

        bool FIsIgnoreCase;
        unsigned char FCompTable[256];

        int
        __fastcall
        Min( int a, int b ) { return (a>b) ? b : a; }

        int
        __fastcall
        Max( int a, int b ) { return (a>b) ? a : b; }

        protected:

        int
        __fastcall
        DoFind( const AnsiString & ATargetStr, int AStart );

        int
        __fastcall
        DoFindIgnoreCase( const AnsiString & ATargetStr, int AStart );

        public:

        __fastcall TBMSearch();
        virtual __fastcall~TBMSearch ();

        void
        __fastcall
        SetPattern( const AnsiString & APattern, bool AIsIgnoreCase );

        int
        __fastcall
        Find( const AnsiString & ATargetStr );

        int
        __fastcall
        Find( const AnsiString & ATargetStr, int AStart );
    };
}

using namespace Bmsearch;

#endif

typedef struct tagPrintData {
	PRINTDLG * pd;
	LPCTSTR DocName;
	LPCTSTR OutputName;
	LPVOID  UserData;
	HWND    Owner;

	// Set when the PrintData is passed to a heading function
	int pageno;  
	int ypos;
	int dy;     // default line height in default font
	RECT page;  // limits of printable area in device units
		    // excludes heading and footing
			    } PrintData, * LPPrintData;

// Flag values (bit values)
#define PrintIt_ENABLETEMPLATE 0x01
#define PrintIt_ENABLEHOOK     0x02

typedef int (CALLBACK * LPPrintitHeaderFunc)(LPPrintData data, LPTSTR buffer, int len);
typedef BOOL (CALLBACK * LPPrintitHookFunc)(HWND hdlg, int msg, WPARAM wParam, LPARAM lParam);
typedef int (CALLBACK * LPPrintitFunc)(LPPrintData data);

typedef struct tagPrintIt {
        DWORD lStructSize;  // length of structure
	// Set by caller
	PrintData prdata;
	PAGESETUPDLG * psu;
	DWORD Flags;

	// If the PrintIt_ENABLETEMPLATE flag is set, these two fields must
	// be valid, otherwise they are ignored
	HINSTANCE hInst;
	LPCTSTR Template;
	LPPrintitHookFunc Hook;
	BOOL Aborted;  // set by hook func to indicate abort 
	

	// The following function must be present
	LPPrintitFunc FormatLine;

	// The following functions may all be set to NULL pointers.  If
	// present they are called
	LPPrintitHeaderFunc TopLeft;
	LPPrintitHeaderFunc TopCenter;
	LPPrintitHeaderFunc TopRight;
	LPPrintitHeaderFunc BottomLeft;
	LPPrintitHeaderFunc BottomCenter;
	LPPrintitHeaderFunc BottomRight;
	LPPrintitFunc   Query;
	LPPrintitFunc   PrepareDC;
	LPPrintitFunc   FinishPage;
	
	// Set to 0 if no error or GetLastError() if any error
	DWORD error;
	       } PrintIt, * LPPrintIt;


#define Printit_Update_Page ("PRINTIT.UpdatePage")

// Values that can be returned from a fmtLine call
#define PI_Continue (-1)  // Line has been written, ypos advanced, continue
#define PI_Stop     (-2)  // Stop printing
#define PI_NewPage  (-3)  // End this page

// Values that can be returned from a heading call
// value > 0 is height of text written
#define PH_Continue (-1)  // callee has filled in buffer, changed DC as needed
#define PH_Stop     (-2)  // stop printing

// Values that can be returned from a query call
#define PQ_Continue (-1)
#define PQ_Stop     (-2)
#define PQ_NewPage  (-3)

// Values that can be returned from a finishpage call
#define PF_Continue (-1)
#define PF_Stop     (-2)

#ifdef __cplusplus
extern "C" {
#endif

_declspec(dllexport) DWORD APIENTRY doPrintIt(LPPrintIt pd);
_inline HDC prDC(LPPrintData pr) { return pr->pd->hDC; }
_declspec(dllexport)
DWORD APIENTRY getPrintItError(DWORD error, LANGID langid, LPTSTR Buffer, int length);

#define PRINTIT_PAGE_NOTIFICATION_MSG ("Printit.PageNotification")
#ifdef __cplusplus
	   }
#endif

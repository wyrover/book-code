#include "stdafx.h"
#include "resource.h"
#include "paper1.h"

/*****************************************************************************
*				 UWM_LOG_PAINT
* Inputs:
*	wParam: Handle of the dialog receiving a WM_PSD_* message
*	lParam: (LPCTSTR)(CString): String reference to a string which
*		is to be displayed to log the event, or NULL
* Result: LRESULT
*	0, always
* Effect:
*	If the lParam is not NULL, logs the message.  If the lParam is
*	NULL, indicates the start of a new sequence of messages.
*
* Notes:
*	The current implementation of the NULL lParam is to clear the list box,
*	but the sender of the message doesn't really know that.
*****************************************************************************/

#define UWM_LOG_PAINT_MSG _T("On.Log.PagePaint")

int UWM_LOG_PAINT = 0;

#define DIM(x) (sizeof(x) / sizeof(x[0]))

// The following table and function are used to decode the HIWORD(WPARAM)
// of the WM_PSD_PAGESETUPDLG message to give the paper, orientation and
// device type information

typedef struct {UINT id; UINT paper; UINT orient; UINT dev;} PgParms, *LPPgParms;

PgParms pgvals[] = {
	{0x0001, IDS_PAPER,    IDS_LANDSCAPE, IDS_DOT_MATRIX},
	{0x0003, IDS_PAPER,    IDS_LANDSCAPE, IDS_HPPCL},
	{0x0005, IDS_PAPER,    IDS_PORTRAIT,  IDS_DOT_MATRIX},
	{0x0007, IDS_PAPER,    IDS_PORTRAIT,  IDS_HPPCL},
	{0x0009, IDS_ENVELOPE, IDS_PORTRAIT,  IDS_UNKNOWN},  // undocumented
	{0x000B, IDS_ENVELOPE, IDS_LANDSCAPE, IDS_HPPCL},
	{0x000D, IDS_ENVELOPE, IDS_PORTRAIT,  IDS_DOT_MATRIX},
	{0x0019, IDS_ENVELOPE, IDS_LANDSCAPE, IDS_DOT_MATRIX},
	{0x001D, IDS_ENVELOPE, IDS_PORTRAIT,  IDS_UNKNOWN}, // undocumented
	{0x001F, IDS_ENVELOPE, IDS_PORTRAIT,  IDS_HPPCL},
	{0x0000, 0,            0,             0} // EOT
							  };

LPPgParms findPgParms(UINT id)
    {
     for(int i = 0; pgvals[i].paper != 0; i++)
	if(pgvals[i].id == id)
	   return &pgvals[i];
     return NULL;
    }


/****************************************************************************
*                                  logPageInit
* Result: void
*       
* Effect: 
*       Initializes the variables we need.  In particular, registers the
*	window message
****************************************************************************/

void logPageInit()
    {
     UWM_LOG_PAINT = RegisterWindowMessage(UWM_LOG_PAINT_MSG);     
    }

/****************************************************************************
*                                getLogWindow
* Inputs:
*       HWND hctl: Control handle of child control
* Result: CWnd *
*       Handle of window that receives logging message
* Notes: 
*       We have stored the handle of the logging window in the lCustData
*	field.  In the WM_INITDIALOG handler, we store the lParam value
*	in the GWL_USERDATA field so we can get it back.
*
*	If we weren't logging for pedagogical purposes, we would not need
*	to do this.
****************************************************************************/

CWnd * getLogWindow(HWND hctl)
    {
     HWND hdlg = ::GetParent(hctl);
     LPPAGESETUPDLG psd = (LPPAGESETUPDLG)::GetWindowLong(hdlg, GWL_USERDATA);
     return CWnd::FromHandle((HWND)psd->lCustData);
    }

/****************************************************************************
*                                logPageSetupDlg
* Inputs:
*       HWND hctl: Handle to window in which the page image is drawn
*	WPARAM wParam:
*	LPARAM lParam:
* Result: void
*       
* Effect: 
*       Logs the WM_PSD_PAGESETUPDLG message
****************************************************************************/

void logPageSetupDlg(HWND hctl, WPARAM wParam, LPARAM lParam)
    {
     CWnd * log = getLogWindow(hctl);
     CString s;
     CString m;
     CString papername;
     UINT paperid;
     LPPgParms parms;
     CString doctype;
     CString docorient;
     CString docdev;

     // Indicate the start of a message sequence

     log->SendMessage(UWM_LOG_PAINT, (WPARAM)hctl, NULL);
     
     paperid = getPaperID(LOWORD(wParam));
     if(paperid == 0)
	paperid = IDS_UNKNOWN;

     parms = findPgParms(HIWORD(wParam));

     if(parms != NULL)
        { /* load strings */
	 doctype.LoadString(parms->paper);
	 docorient.LoadString(parms->orient);
	 docdev.LoadString(parms->dev);
	} /* load strings */

     papername.LoadString(paperid);

     // format the message and log it
     m.LoadString(IDS_WM_PSD_PAGESETUPDLG); 
     		// wm_psd (%04x, %04x)=%s,%s:%s:%s, LP
     
     s.Format(m, LOWORD(wParam), HIWORD(wParam), papername, 
     				doctype,
				docorient,
				docdev,
				lParam);
     log->SendMessage(UWM_LOG_PAINT, (WPARAM)hctl, (LPARAM)(LPCTSTR)s);

    }

/****************************************************************************
*                                   logRect
* Inputs:
*       int id: Message ID
*	HWND hctl: Control handle
*	WPARAM wParam: HDC
*	LPARAM lParam: Rectangle
* Result: void
*       
* Effect: 
*       Logs the rectangle message
****************************************************************************/

void logRect(int id, HWND hctl, WPARAM wParam, LPARAM lParam)
    {
     CRect r((LPRECT)lParam);
     CString f;
     f.LoadString(id);
     CString s;
     s.Format(f, wParam, r.left, r.top, r.bottom, r.right);
     
     CWnd * log = getLogWindow(hctl);
     log->SendMessage(UWM_LOG_PAINT, (WPARAM)hctl, (LPARAM)(LPCTSTR)s);
    }

/****************************************************************************
*                               logFullPageRect
* Inputs:
*       HWND hctl: control handle
*	WPARAM wParam: HDC
*	LPARAM lParam: LPRECT
* Result: void
*       
* Effect: 
*       Logs the WM_PSD_FULLPAGERECT data
****************************************************************************/

void logFullPageRect(HWND hctl, WPARAM wParam, LPARAM lParam)
    {
     logRect(IDS_WM_PSD_FULLPAGERECT, hctl, wParam, lParam);
    }

/****************************************************************************
*                               logMinMarginRect
* Inputs:
*       HWND hctl: control handle
*	WPARAM wParam: HDC
*	LPARAM lParam: LPRECT
* Result: void
*       
* Effect: 
*       Logs the WM_PSD_MINMARGINRECT data
****************************************************************************/

void logMinMarginRect(HWND hctl, WPARAM wParam, LPARAM lParam)
    {
     logRect(IDS_WM_PSD_MINMARGINRECT, hctl, wParam, lParam);
    }

/****************************************************************************
*                               logGreekTextRect
* Inputs:
*       HWND hctl: control handle
*	WPARAM wParam: HDC
*	LPARAM lParam: LPRECT
* Result: void
*       
* Effect: 
*       Logs the WM_PSD_GREEKTEXTRECT data
****************************************************************************/

void logGreekTextRect(HWND hctl, WPARAM wParam, LPARAM lParam)
    {
     logRect(IDS_WM_PSD_GREEKTEXTRECT, hctl, wParam, lParam);
    }

/****************************************************************************
*                               logEnvStampRect
* Inputs:
*       HWND hctl: control handle
*	WPARAM wParam: HDC
*	LPARAM lParam: LPRECT
* Result: void
*       
* Effect: 
*       Logs the WM_PSD_ENVSTAMPRECT data
****************************************************************************/

void logEnvStampRect(HWND hctl, WPARAM wParam, LPARAM lParam)
    {
     logRect(IDS_WM_PSD_ENVSTAMPRECT, hctl, wParam, lParam);
    }

/****************************************************************************
*                               logYaFullPageRect
* Inputs:
*       HWND hctl: control handle
*	WPARAM wParam: HDC
*	LPARAM lParam: LPRECT
* Result: void
*       
* Effect: 
*       Logs the WM_PSD_YAFULLPAGERECT data
****************************************************************************/

void logYaFullPageRect(HWND hctl, WPARAM wParam, LPARAM lParam)
    {
     logRect(IDS_WM_PSD_YAFULLPAGERECT, hctl, wParam, lParam);
    }

/****************************************************************************
*                               logMarginRect
* Inputs:
*       HWND hctl: control handle
*	WPARAM wParam: HDC
*	LPARAM lParam: LPRECT
* Result: void
*       
* Effect: 
*       Logs the WM_PSD_MARGINRECT data
****************************************************************************/

void logMarginRect(HWND hctl, WPARAM wParam, LPARAM lParam)
    {
     logRect(IDS_WM_PSD_MARGINRECT, hctl, wParam, lParam);
    }

/****************************************************************************
*                              drawFullPageRect
* Inputs:
*       CDC * dc: Display context to use for drawing
*	LPRECT r: Rectangle for full page
* Result: void
*       
* Effect: 
*       Draws the background of the page
****************************************************************************/

static void drawFullPageRect(CDC * dc, LPRECT rp)
    {
     CBrush b(RGB(255, 255, 127));
     CRect r(rp);
     int n = dc->SaveDC();
     dc->SelectObject(b);
     dc->PatBlt(0, 0, r.Width(), r.Height(), PATCOPY);
     dc->RestoreDC(n);
    }

/****************************************************************************
*                              drawMinMarginRect
* Inputs:
*       CDC * dc: Display context to use for drawing
*	LPRECT r: Rectangle for minimum margins
* Result: void
*       
* Effect: 
*       Draws the minimum margins space
****************************************************************************/

static void drawMinMarginRect(CDC * dc, LPRECT r)
    {
     
    }

/****************************************************************************
*                              drawMarginRect
* Inputs:
*       CDC * dc: Display context to use for drawing
*	LPRECT r: Rectangle for current margins
* Result: void
*       
* Effect: 
*       Draws the current margins space
****************************************************************************/

static void drawMarginRect(CDC * dc, LPRECT r)
    {
     
    }

/****************************************************************************
*                                   doGreek
* Inputs:
*	HWND hctl: control which is the sample document
*       CDC * dc: DC to write it
*	CRECT r: Rectangle in which we Greek the text
* Result: void
*       
* Effect: 
*       Draws simulated text in the area
****************************************************************************/

void doGreek(HWND hctl, CDC * dc, CRect r)
    {
     TEXTMETRIC tm;

     r.InflateRect(-1, -1);  // leave a bit of a border

     // This is one of those hacks where we see if anyone is actually
     // paying attention to the body of the text, by not just doing
     // something random but by really putting out some text.

     // The choice of text is essentially random; the goal was to pick
     // something well-known, not copyrighted, and relatively short.

     // Since we're being really lazy, we're going to let Windows actually
     // do the line breaks.  We create an invisible, disabled edit control
     // and use it to obtain the lines.  We use this same hack in the
     // msglog module used in the various Explorers.

     CEdit wrap;
     wrap.Create(WS_DISABLED | WS_CHILD | ES_AUTOVSCROLL | ES_MULTILINE, 
     					r, CWnd::FromHandle(hctl), 0);

     CFont itsyfont;
     itsyfont.CreateFont(3, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
     				ANSI_CHARSET, OUT_TT_PRECIS, 
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
				VARIABLE_PITCH | FF_DONTCARE, "Arial");
     wrap.SetFont(&itsyfont);

     CString sample;
     CString oneline;

     for(int i = IDS_PAGE_SAMPLE; i < IDS_PAGE_SAMPLE_END; i++)
        { /* build sample */
	 oneline.LoadString(i);
	 sample += oneline;
	} /* build sample */

     wrap.SetWindowText(sample);

     dc->SelectObject(itsyfont);
     dc->GetTextMetrics(&tm);

     // load lines into the box

     for(int lineno = 0, pos = r.top; pos < r.bottom; 
			 lineno++, pos += tm.tmHeight + tm.tmExternalLeading)
        { /* each line */
	 TCHAR temp[256];
	 int chars = wrap.GetLine(lineno, temp, DIM(temp));
	 temp[chars] = _T('\0');
	 dc->TextOut(r.left, pos, temp, chars);
	} /* each line */

     wrap.DestroyWindow();
    }

/****************************************************************************
*                              drawGreekText
* Inputs:
*	HWND hctl: Handle to control that is the sample
*       CDC * dc: Display context to use for drawing
*	LPRECT r: Rectangle for minimum margins
* Result: BOOL
*       TRUE if it draws the text
*	FALSE if it doesn't (right now, it always does, so only returns TRUE)
* Effect: 
*       Draws the minimum margins space
****************************************************************************/

static BOOL drawGreekText(HWND hctl, CDC * dc, LPRECT rp)
    {
     // If we have an envelope, don't do our own Greeking.

     LPPgParms parms = findPgParms(HIWORD(::GetWindowLong(hctl, GWL_USERDATA)));

     if(parms != NULL && parms->paper == IDS_ENVELOPE)
	return FALSE;  // use default Greeking.

     CRect r(rp);
     // we are going to show a 3-column page.  Note we could have used
     // a custom dialog to get the gap between the columns.

     /*	
	Here's our layout:
	
	       COL_WIDTH   COL_GAP
	+-----------------+--+----------------+--+----------------+
	|                 |  |                |  |                |

	Since the purpose of this is to illustrate the techniques,
	the details of how we compute these columns isn't terribly
	critical.  So I've assigned the sizes in the ratios shown
	above.  The definitions below take care of this.  These are
	relative ratios.

	We might consider adding a custom option in a custom dialog
	that lets the user select the number of columns.  For now we
	hardwire this value.
     */

#define COL_NO 3
#define COL_WIDTH 16
#define COL_GAP    2

     // Do the computations in floating point to handle subpixel cases
     float width = (float)r.Width() / (float)((COL_WIDTH * COL_NO) 
     						+ (COL_GAP * (COL_NO - 1)));

     // width is now one logical unit

     CPen p(PS_SOLID, 1, RGB(0, 255, 255));

     int saved = dc->SaveDC();

     dc->SelectStockObject(HOLLOW_BRUSH);
     dc->SelectObject(p);

     for(int i= 0; i < COL_NO; i++)
        { /* draw columns */
	 int origin = r.left + i * (int)(width * (float)(COL_WIDTH + COL_GAP));
	 CRect cr(origin, r.top,
	          origin + (int)(width * (float)COL_WIDTH), r.bottom);
	 	  
	 dc->Rectangle(&cr);

	 doGreek(hctl, dc, cr);
	} /* draw columns */


     dc->RestoreDC(saved);
     
     return TRUE;
    }

/****************************************************************************
*                                  pagePaint
* Inputs:
*       HWND hdlg: Handle to the window which is the control that represents
*		   the page which is being drawn
* Result: UINT
*       based on the message type
* Effect: 
*       The paint handle
****************************************************************************/

UINT APIENTRY pagePaint(HWND hctl, UINT msg, WPARAM wParam, LPARAM lParam)
{
 switch(msg)
    { /* decode message */
     case WM_PSD_PAGESETUPDLG:
	     logPageSetupDlg(hctl, wParam, lParam);

	     // handle the message

	     // All we do is save the flags so we don't do the wrong
	     // Greeking for an envelope

	     ::SetWindowLong(hctl, GWL_USERDATA, wParam);
	     // does nothing right now
	     return FALSE;  // we want the rest of the messages to come thru

     case WM_PSD_FULLPAGERECT:
	     logFullPageRect(hctl, wParam, lParam);
	     drawFullPageRect(CDC::FromHandle((HDC)wParam), (LPRECT)lParam);
	     // return FALSE to continue to get messages
	     break;
     case WM_PSD_MINMARGINRECT:
	     logMinMarginRect(hctl, wParam, lParam);
	     drawMinMarginRect(CDC::FromHandle((HDC)wParam), (LPRECT)lParam);
	     break;
     case WM_PSD_MARGINRECT:
	     logMarginRect(hctl, wParam, lParam);
	     drawMarginRect(CDC::FromHandle((HDC)wParam), (LPRECT)lParam);
	     break;
     case WM_PSD_GREEKTEXTRECT:
	     logGreekTextRect(hctl, wParam, lParam);
	     return drawGreekText(hctl, CDC::FromHandle((HDC)wParam), 
	     						(LPRECT)lParam);
     case WM_PSD_ENVSTAMPRECT:
	     logEnvStampRect(hctl, wParam, lParam);
	     // We don't support envelopes, so we don't do anything here
	     break;
     case WM_PSD_YAFULLPAGERECT:
	     logYaFullPageRect(hctl, wParam, lParam);
	     // We don't support envelopes, so we don't do anything here
	     break;
    } /* decode message */
 return FALSE;  // we didn't do anything
}

/****************************************************************************
*                                  pageSetup
* Inputs:
*       UINT hdlg: Dialog handle
*	UINT msg: Message
*	WPARAM wParam:
*	LPARAM lParam:
* Result: LRESULT
*       FALSE, always, to indicate that the dialog should handle the message
* Effect: 
*	on WM_INITDIALOG, attach the lParam (pointer to the PRINTSETUPDLG)
*	to the GWL_USERDATA so we can get it back as needed.
* Notes:
*	In order to log the messages to the logging window, we need a handle
*	to the window that is supposed to receive the data.  We store this
*	window handle in the lCustData field of the PAGESETUPDLG structure.
*	When we get a message that needs to be logged, we use this value
*	to direct the message
****************************************************************************/

UINT APIENTRY pageSetup(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam)
    {
     switch(msg)
        { /* msg */
	 case WM_INITDIALOG:
		 ::SetWindowLong(hdlg, GWL_USERDATA, lParam);
		 break;
	} /* msg */
     return 0;
    }

#define STATIC
#include <windows.h>
#include <windowsx.h>

HBRUSH hbrushIris ;
HBRUSH hbrushBlack;
HBRUSH hbrushWhite;
static POINT    ptOldCursorPos ;
BOOL useRegion = TRUE;


static void 
drawEye (HWND hwnd, HDC hdc, RECT rectEye, POINT ptCursor)
{
    HBRUSH   hbrushOrig ;
    int      nRiseSign = 1;
    int	     nRunSign = 1 ;
    int      nHeight;
    int      nWidth;
    int      nXDist;
    int      nYDist ;
    int      xDelta;
    int      yDelta;
    int      nOldROP ;
    LONG     lRise;
    int      lRun ;
    POINT    ptBoundingCenter;
    POINT    ptIrisCenter ;
    RECT     rectBounding ;
    HRGN     ellipse = NULL;

    int	     saved = SaveDC(hdc);

    /* Draw the sclera for one eye. */
    SelectBrush (hdc, hbrushWhite) ;

    if(!useRegion)
       { /* normal ellipse */
	Ellipse (hdc, rectEye.left,  rectEye.top,
                  rectEye.right, rectEye.bottom) ;
       } /* normal ellipse */
    else
       { /* region ellipse */
        // We want to clip the region.  Note, however, that the size of
        // the region and the size of an ellipse of the same parameters
        // is not identical; this is acknowleged as a bug.  See KBase
        // article Q83807.  The workaround it suggests is to use
        // FillRgn (and FrameRgn) to draw the filled, outlined
        // ellipse
        ellipse = CreateEllipticRgn(rectEye.left, rectEye.top,
    			          rectEye.right, rectEye.bottom);

	Ellipse (hdc, rectEye.left,  rectEye.top,
                  rectEye.right, rectEye.bottom) ;

	SelectClipRgn(hdc, ellipse);
       } /* region ellipse */

    /* Calculate the rectangle in which the iris can move. */
    nHeight = rectEye.bottom - rectEye.top ;
    nWidth  = rectEye.right  - rectEye.left ;
    rectBounding = rectEye ;
    InflateRect (&rectBounding, -29 * nWidth / 100, -27 * nHeight / 100) ;
    ScreenToClient (hwnd, &ptCursor) ;
    ptBoundingCenter.x = rectBounding.left +
                         (rectBounding.right - rectBounding.left) / 2 ;
    ptBoundingCenter.y = rectBounding.top +
                         (rectBounding.bottom - rectBounding.top) / 2 ;
    xDelta = ptCursor.x - ptBoundingCenter.x ;
    yDelta = ptCursor.y - ptBoundingCenter.y ;
    if (!PtInRect (&rectBounding, ptCursor)) {
        /* Cant center the iris around the cursor point.     */
        /* Use the line from the center of the iris's area of */
        /* movement to the cursor.  Place the iris at the     */
        /* intersection of this line and the iris movement    */
        /* bounding rectangle.                                */
        lRise = ptCursor.y - ptBoundingCenter.y ;
        nRiseSign = (lRise < 0) ? lRise = labs (lRise), -1 : 1 ;
        lRun  = ptCursor.x - ptBoundingCenter.x ;
        nRunSign = (lRun < 0) ? lRun = labs (lRun), -1 : 1 ;
        nXDist = rectBounding.right  - ptBoundingCenter.x ;
        nYDist = rectBounding.bottom - ptBoundingCenter.y ;
        if (lRun != 0 && lRise != 0) {
            yDelta = (int) ((lRise * nXDist) / lRun) ;
            xDelta = nXDist ;
            if (yDelta > nYDist) {
                yDelta = nYDist ;
                xDelta = (int) ((lRun * nYDist) / lRise) ;
            }
        }
        else if (lRun == 0) {
            yDelta = nYDist ;
            xDelta = 0 ;
        }
        else {
            yDelta = 0 ;
            xDelta = nXDist ;
        }
    }    
    ptIrisCenter.x = ptBoundingCenter.x + xDelta * nRunSign ;
    ptIrisCenter.y = ptBoundingCenter.y + yDelta * nRiseSign ;
    /* Draw the iris. */
    hbrushOrig = SelectBrush (hdc, hbrushIris) ;
    nWidth /= 4 ;
    nHeight /= 4 ;
    nOldROP = SetROP2 (hdc, R2_MASKPEN) ;
    Ellipse (hdc, ptIrisCenter.x - nWidth, ptIrisCenter.y - nHeight,
                  ptIrisCenter.x + nWidth, ptIrisCenter.y + nHeight) ;
    SetROP2 (hdc, nOldROP) ;
    SelectBrush (hdc, hbrushBlack) ;
    Ellipse (hdc, ptIrisCenter.x - nWidth / 3,
                  ptIrisCenter.y - nHeight / 3,
                  ptIrisCenter.x + nWidth / 3,
                  ptIrisCenter.y + nHeight / 3) ;
    RestoreDC(hdc, saved);

    if(ellipse != NULL)
       DeleteObject(ellipse);
}

void drawEyes (HWND hwnd, HDC hdc)
    {
     POINT  ptCursor ;
     RECT   rectEye ;
     RECT   rectClient;

     GetClientRect (hwnd, &rectClient) ;
     GetCursorPos (&ptCursor) ;
     if (ptCursor.x == ptOldCursorPos.x &&
	 ptCursor.y == ptOldCursorPos.y)
        return ;
     ptOldCursorPos = ptCursor ;
     rectEye.left   = rectClient.left ;
     rectEye.right  = rectClient.right / 2 ;
     rectEye.top    = rectClient.top ;
     rectEye.bottom = 2 * rectClient.bottom / 3 ;
     drawEye (hwnd, hdc, rectEye, ptCursor) ;
     rectEye.left  = rectEye.right ;
     rectEye.right = rectClient.right ;
     drawEye (hwnd, hdc, rectEye, ptCursor) ;
    }


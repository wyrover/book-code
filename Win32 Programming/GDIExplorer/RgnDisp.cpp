// RgnDisp.cpp : implementation file
//

#include "stdafx.h"
#include "GDIExplorer.h"
#include "RgnDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegionDisplay

CRegionDisplay::CRegionDisplay()
{
 Rgn1 = NULL;
 Rgn2 = NULL;
 Result = NULL;
}

CRegionDisplay::~CRegionDisplay()
{
 if(Rgn1 != NULL)
    ::DeleteObject(Rgn1);
 if(Rgn2 != NULL)
    ::DeleteObject(Rgn2);
 if(Result != NULL)
    ::DeleteObject(Result);
}


BEGIN_MESSAGE_MAP(CRegionDisplay, CStatic)
        //{{AFX_MSG_MAP(CRegionDisplay)
        ON_WM_PAINT()
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegionDisplay message handlers


/****************************************************************************
*                         CRegionDisplay::initialize
* Result: void
*       
* Effect: 
*       No effect.  Implement in subclasses
* Notes:
*       There is either a bug or a design error in MFC; if you allocate
*       a temporary object using CBrush::FromHandle and then delete it,
*       the MFC temporary object map is corrupted.  Hence the bizarre
*       use of attach; this is a workaround.
****************************************************************************/

void CRegionDisplay::initialize()
    {
     ASSERT(FALSE); // implement in subclasses
    }

void CRegionDisplay::OnPaint() 
{
        CPaintDC dc(this); // device context for painting
        CBrush br;
        CBrush edge(RGB(0,0,0));
        
        int op = GetParent()->SendMessage(UWM_QUERY_RGNOP);
        int which = GetParent()->SendMessage(UWM_QUERY_RGNWHAT);
        int how = GetParent()->SendMessage(UWM_QUERY_RGNHOW);
        int w = ::GetSystemMetrics(SM_CXBORDER);
        BOOL mono = (BOOL)GetParent()->SendMessage(UWM_QUERY_MONO);

#define COLOR1 RGB(255, 0, 0)
#define COLOR2 RGB(0, 255, 0)
#define COLOR3 RGB(255, 255, 0)

        CRect r;
        GetClientRect(&r);

        if(mono)
           dc.FillRect(&r, CBrush::FromHandle((HBRUSH)::GetStockObject(WHITE_BRUSH)));
        else
           dc.DrawEdge(&r, EDGE_SUNKEN, BF_RECT | BF_MIDDLE);

        switch(which)
           { /* which */
            case RGN_1:
                    ::CombineRgn(Result, Rgn1, NULL, RGN_COPY);
                    if(mono)
                       br.Attach((HBRUSH)::GetStockObject(LTGRAY_BRUSH));
                    else
                       { /* ugly workaround RGN1 */
                        CBrush tbr(COLOR1);
                        HBRUSH b = (HBRUSH)tbr.Detach();
                        br.Attach(b);
                       } /* ugly workaround RGN1 */
                    break;
            case RGN_2:  
                    ::CombineRgn(Result, Rgn2, NULL, RGN_COPY);
                    if(mono)
                       br.Attach((HBRUSH)::GetStockObject(DKGRAY_BRUSH));
                    else
                       { /* ugly workaround RGN2 */
                        CBrush tbr(COLOR2);
                        HBRUSH b = (HBRUSH)tbr.Detach();
                        br.Attach(b);
                       } /* ugly workaround RGN2 */
                    break;
            case RGN_RESULT:
                    ::CombineRgn(Result, Rgn1, Rgn2, op);
                    if(mono)
                       br.Attach((HBRUSH)::GetStockObject(GRAY_BRUSH));
                    else
                       { /* ugly workaround Resuslt */
                        CBrush tbr(COLOR3);
                        HBRUSH b = (HBRUSH)tbr.Detach();
                        br.Attach(b);
                       } /* ugly workaround Result */
                    break;
            default:
                    return;  // don't know what to do
           } /* which */

        if(how & RGN_FILL)
           ::FillRgn(dc.m_hDC, Result, (HBRUSH)br.m_hObject);
        if(how & RGN_FRAME)
           ::FrameRgn(dc.m_hDC, Result, (HBRUSH)edge.m_hObject, w, w);

        // when br is deleted, ::DeleteObject will be called; if the
        // handle is a stock brush, the ::DeleteObject will be ignored
}

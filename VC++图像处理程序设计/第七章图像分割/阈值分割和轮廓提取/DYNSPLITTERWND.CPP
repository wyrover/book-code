// SAGA Incorporated MFC Goodies
//
// SAGA Incorporated (saga@gis.net) MFC Goodies
// Developed by Andrew Slivker for www.codeguru.com
// 
// DynSplitterWnd.cpp : implementation file
//
// Version:			1.0
// Revision:		02/03/98

#include "stdafx.h"
#include <afxext.h>
#include "DynSplitterWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CX_BORDER	1
#define CY_BORDER	1

#define FAR_POINT_XY		-10000


/////////////////////////////////////////////////////////////////////////////
// CDynSplitterWnd

CDynSplitterWnd::CDynSplitterWnd() 
	:m_OldPoint(FAR_POINT_XY,FAR_POINT_XY), m_bDynSplit(TRUE)
{
}

CDynSplitterWnd::~CDynSplitterWnd()
{
}


void CDynSplitterWnd::OnInvertTracker(const CRect& rect)
{
	if(!m_bDynSplit)
		CSplitterWnd::OnInvertTracker(rect);
	else
		return;
}

void CDynSplitterWnd::StartTracking(int ht)
{
	if(!m_bDynSplit)
	{
		CSplitterWnd::StartTracking(ht);
		return;
	}
	
	ASSERT_VALID(this);
	if (ht == noHit)
		return;

	// GetHitRect will restrict 'm_rectLimit' as appropriate
	GetInsideRect(m_rectLimit);

	if (ht >= splitterIntersection1 && ht <= splitterIntersection225)
	{
		// split two directions (two tracking rectangles)
		int row = (ht - splitterIntersection1) / 15;
		int col = (ht - splitterIntersection1) % 15;

		GetHitRect(row + vSplitterBar1, m_rectTracker);
		int yTrackOffset = m_ptTrackOffset.y;
		m_bTracking2 = TRUE;
		GetHitRect(col + hSplitterBar1, m_rectTracker2);
		m_ptTrackOffset.y = yTrackOffset;
	}
	else if (ht == bothSplitterBox)
	{
		// hit on splitter boxes (for keyboard)
		GetHitRect(vSplitterBox, m_rectTracker);
		int yTrackOffset = m_ptTrackOffset.y;
		m_bTracking2 = TRUE;
		GetHitRect(hSplitterBox, m_rectTracker2);
		m_ptTrackOffset.y = yTrackOffset;

		// center it
		m_rectTracker.OffsetRect(0, m_rectLimit.Height()/2);
		m_rectTracker2.OffsetRect(m_rectLimit.Width()/2, 0);
	}
	else
	{
		// only hit one bar
		GetHitRect(ht, m_rectTracker);
	}

	// steal focus and capture
	SetCapture();

	// set tracking state and appropriate cursor
	m_bTracking = TRUE;
	m_htTrack = ht;
	SetSplitCursor(ht);
}


void CDynSplitterWnd::StopTracking(BOOL bAccept)
{
	ASSERT_VALID(this);

	if (!m_bTracking)
		return;

	ReleaseCapture();

	// erase tracker rectangle
	OnInvertTracker(m_rectTracker);
	if (m_bTracking2)
		OnInvertTracker(m_rectTracker2);
	m_bTracking = m_bTracking2 = FALSE;

	// save old active view
	CWnd* pOldActiveView = GetActivePane();

	// m_rectTracker is set to the new splitter position (without border)
	// (so, adjust relative to where the border will be)
	m_rectTracker.OffsetRect(-CX_BORDER , -CY_BORDER);
	m_rectTracker2.OffsetRect(-CX_BORDER, -CY_BORDER);

	if (bAccept)
	{
		if (m_htTrack == vSplitterBox)
		{
			SplitRow(m_rectTracker.top);
		}
		else if (m_htTrack >= vSplitterBar1 && m_htTrack <= vSplitterBar15)
		{
			// set row height
			TrackRowSize(m_rectTracker.top, m_htTrack - vSplitterBar1);
			RecalcLayout();
		}
		else if (m_htTrack == hSplitterBox)
		{
			SplitColumn(m_rectTracker.left);
		}
		else if (m_htTrack >= hSplitterBar1 && m_htTrack <= hSplitterBar15)
		{
			// set column width
			TrackColumnSize(m_rectTracker.left, m_htTrack - hSplitterBar1);
			RecalcLayout();
		}
		else if (m_htTrack >= splitterIntersection1 &&
			m_htTrack <= splitterIntersection225)
		{
			// set row height and column width
			int row = (m_htTrack - splitterIntersection1) / 15;
			int col = (m_htTrack - splitterIntersection1) % 15;

			TrackRowSize(m_rectTracker.top, row);
			TrackColumnSize(m_rectTracker2.left, col);
			RecalcLayout();
		}
		else if (m_htTrack == bothSplitterBox)
		{
			// rectTracker is vSplitter (splits rows)
			// rectTracker2 is hSplitter (splits cols)
			SplitRow(m_rectTracker.top);
			SplitColumn(m_rectTracker2.left);
		}
	}

	if ( (pOldActiveView == GetActivePane()) &&
	     (pOldActiveView != NULL)
	   )
			SetActivePane(-1, -1, pOldActiveView); // re-activate
}


BEGIN_MESSAGE_MAP(CDynSplitterWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CDynSplitterWnd)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDynSplitterWnd message handlers


void CDynSplitterWnd::OnMouseMove(UINT nFlags, CPoint pt) 
{
	if(!m_bDynSplit)
	{
		CSplitterWnd::OnMouseMove(nFlags, pt);
		return;
	}

	if (GetCapture() != this)
		StopTracking(FALSE);

	if (m_bTracking)
	{
		// move tracker to current cursor position
		pt.Offset(m_ptTrackOffset); // pt is the upper right of hit detect
		// limit the point to the valid split range
		if (pt.y < m_rectLimit.top)
			pt.y = m_rectLimit.top;
		else if (pt.y > m_rectLimit.bottom)
			pt.y = m_rectLimit.bottom;
		if (pt.x < m_rectLimit.left)
			pt.x = m_rectLimit.left;
		else if (pt.x > m_rectLimit.right)
			pt.x = m_rectLimit.right;

		if (m_htTrack == vSplitterBox ||
			m_htTrack >= vSplitterBar1 && m_htTrack <= vSplitterBar15)
		{
			if (m_rectTracker.top != pt.y)
			{
				OnInvertTracker(m_rectTracker);
				m_rectTracker.OffsetRect(0, pt.y - m_rectTracker.top);
				OnInvertTracker(m_rectTracker);
			}
		}
		else if (m_htTrack == hSplitterBox ||
			m_htTrack >= hSplitterBar1 && m_htTrack <= hSplitterBar15)
		{
			if (m_rectTracker.left != pt.x)
			{
				OnInvertTracker(m_rectTracker);
				m_rectTracker.OffsetRect(pt.x - m_rectTracker.left, 0);
				OnInvertTracker(m_rectTracker);
			}
		}
		else if (m_htTrack == bothSplitterBox ||
		   (m_htTrack >= splitterIntersection1 &&
			m_htTrack <= splitterIntersection225))
		{
			if (m_rectTracker.top != pt.y)
			{
				OnInvertTracker(m_rectTracker);
				m_rectTracker.OffsetRect(0, pt.y - m_rectTracker.top);
				OnInvertTracker(m_rectTracker);
			}
			if (m_rectTracker2.left != pt.x)
			{
				OnInvertTracker(m_rectTracker2);
				m_rectTracker2.OffsetRect(pt.x - m_rectTracker2.left, 0);
				OnInvertTracker(m_rectTracker2);
			}
		}

		OnLButtonUp(MK_LBUTTON,pt);
		OnLButtonDown(MK_LBUTTON,pt);
		if(m_OldPoint != pt)
		{
			RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);
			m_OldPoint = pt;
		}
	}
	else
	{
		// simply hit-test and set appropriate cursor
		int ht = HitTest(pt);
		SetSplitCursor(ht);
	}
}

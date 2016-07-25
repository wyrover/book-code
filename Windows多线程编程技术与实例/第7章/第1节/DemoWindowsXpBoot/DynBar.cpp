// DynBar.cpp : implementation file
//

#include "stdafx.h"
#include "DemoWindowsXpBoot.h"
#include "DynBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynBar

long	g_isRun = 0;


UINT RunThread(LPVOID lpParam)
{
	CDynBar* pWnd = (CDynBar*)lpParam;
	CClientDC dcClient(pWnd);
	CDC		dcMem;
	dcMem.CreateCompatibleDC(&dcClient);
	BITMAP	bits;
	pWnd->m_bmp.GetBitmap(&bits);
	CBitmap *pOldBitmap = (CBitmap*)dcMem.SelectObject(&pWnd->m_bmp );

	CSize	size(0,0);

	while(g_isRun)
	{
		if( size.cx<bits.bmWidth-1)
			size.cx +=1;
		else
			size.cx = 1;
		dcClient.BitBlt( 1,0,size.cx,bits.bmHeight,&dcMem,(bits.bmWidth-size.cx-1),0,SRCCOPY);
		dcClient.BitBlt( size.cx,0,(bits.bmWidth-size.cx-2),bits.bmHeight,&dcMem,1,0,SRCCOPY);

		Sleep( 20 );
	}
    dcMem.SelectObject(pOldBitmap);
	return 1;
}
/////////////////////////////////////////////////////////////////////////////
// CDynBar

CDynBar::CDynBar()
{
	m_bmp.LoadBitmap(IDB_BOOTBAR);
}

CDynBar::~CDynBar()
{
	if( g_isRun )	
		Stop();
}


BEGIN_MESSAGE_MAP(CDynBar, CStatic)
	//{{AFX_MSG_MAP(CDynBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynBar message handlers


void CDynBar::Start()
{
	InterlockedIncrement(&g_isRun);
	AfxBeginThread(RunThread,this);

}

void CDynBar::Stop()
{
	InterlockedDecrement(&g_isRun);

}


/////////////////////////////////////////////////////////////////////////////
// CDynBar message handlers

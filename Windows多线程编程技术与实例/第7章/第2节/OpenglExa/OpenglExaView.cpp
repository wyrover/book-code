// OpenglExaView.cpp : implementation of the COpenglExaView class
//

#include "stdafx.h"
#include "OpenglExa.h"

#include "OpenglExaDoc.h"
#include "OpenglExaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenglExaView

IMPLEMENT_DYNCREATE(COpenglExaView, CView)

BEGIN_MESSAGE_MAP(COpenglExaView, CView)
	//{{AFX_MSG_MAP(COpenglExaView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenglExaView construction/destruction

COpenglExaView::COpenglExaView()
{
	// TODO: add construction code here
m_hGLContext = NULL;
m_GLPixelIndex = 0;

}

COpenglExaView::~COpenglExaView()
{
}

BOOL COpenglExaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// COpenglExaView drawing

void COpenglExaView::OnDraw(CDC* pDC)
{
	COpenglExaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// COpenglExaView diagnostics

#ifdef _DEBUG
void COpenglExaView::AssertValid() const
{
	CView::AssertValid();
}

void COpenglExaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenglExaDoc* COpenglExaView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenglExaDoc)));
	return (COpenglExaDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COpenglExaView message handlers

BOOL COpenglExaView::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;

	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW |PFD_SUPPORT_OPENGL |PFD_DOUBLEBUFFER |PFD_STEREO_DONTCARE;
	
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;
	m_GLPixelIndex = ChoosePixelFormat( hDC, &pixelDesc);
	if (m_GLPixelIndex==0) // Let's choose a default index.
	{
		m_GLPixelIndex = 1;
		if (DescribePixelFormat(hDC, m_GLPixelIndex,
			sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc)==0)
			return FALSE;
	}

	if (SetPixelFormat( hDC, m_GLPixelIndex, &pixelDesc)==FALSE)
	{ return FALSE;
	}
	return TRUE;
}




int COpenglExaView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;	
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	SetWindowPixelFormat (hDC);
	CreateViewGLContext (hDC);
	
	return 0;
}

BOOL COpenglExaView::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);//用当前DC产生绘制环境(RC)
	if (m_hGLContext == NULL)
	{ return FALSE;
	}
	if (wglMakeCurrent(hDC, m_hGLContext)==FALSE)
	{ return FALSE;
	}
	return TRUE;
}


void COpenglExaView::OnDestroy() 
{
	if(wglGetCurrentContext()!=NULL)
	{ // make the rendering context not current
		wglMakeCurrent(NULL, NULL) ;
	}
	if (m_hGLContext!=NULL)
	{ wglDeleteContext(m_hGLContext);
	m_hGLContext = NULL;
	}
	
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void COpenglExaView::OnPaint() 
{
	CPaintDC DC(this); // device context for painting (added by ClassWizard)
// 	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex2f(100.0f, 50.0f);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex2f(100.0f, 400.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex2f(450.0f, 400.0f);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex2f(450.0f, 50.0f);

	glEnd();
	glFlush();
 	SwapBuffers(DC.m_ps.hdc);
}

void COpenglExaView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	GLsizei width, height;
	GLdouble aspect;
	width = cx;
	height = cy;
	if (cy==0)
		aspect = (GLdouble)width;
	else
		aspect = (GLdouble)width/(GLdouble)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
 	glLoadIdentity();
 	gluOrtho2D(0.0, 500.0*aspect, 0.0, 500.0);
//// 	gluPerspective(60, aspect, 1, 10.0);
// 	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glDrawBuffer(GL_BACK);



}

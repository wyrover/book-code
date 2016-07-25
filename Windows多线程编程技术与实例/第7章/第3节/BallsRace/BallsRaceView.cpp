// BallsRaceView.cpp : implementation of the CBallsRaceView class
//

#include "stdafx.h"
#include "BallsRace.h"

#include "BallsRaceDoc.h"
#include "BallsRaceView.h"


#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define HIGHEST_THREAD	 0
#define ABOVE_AVE_THREAD 1
#define NORMAL_THREAD	 2
#define BELOW_AVE_THEAD  3
#define LOWEST_THREAD	 4
/////////////////////////////////////////////////////////////////////////////
// CBallsRaceView

IMPLEMENT_DYNCREATE(CBallsRaceView, CView)

BEGIN_MESSAGE_MAP(CBallsRaceView, CView)
	//{{AFX_MSG_MAP(CBallsRaceView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_COMMAND(ID_BALL1WAIT, OnBall1wait)
	ON_COMMAND(ID_BALL0WAIT, OnBall0wait)
	ON_COMMAND(ID_BALL2WAIT, OnBall2wait)
	ON_COMMAND(ID_BALL3WAIT, OnBall3wait)
	ON_COMMAND(ID_BALL4WAIT, OnBall4wait)
	ON_COMMAND(ID_TERMINATEALL, OnTerminateall)
	ON_COMMAND(ID_BALL0CONTINUE, OnBall0continue)
	ON_COMMAND(ID_BALL1CONTINUE, OnBall1continue)
	ON_COMMAND(ID_BALL2CONTINUE, OnBall2continue)
	ON_COMMAND(ID_BALL3CONTINUE, OnBall3continue)
	ON_COMMAND(ID_BALL4CONTINUE, OnBall4continue)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//全局变量
HANDLE _hThread[5];
HANDLE _hEvent[5];

int g_yPosition[5];

void ThreadProc(DWORD *ThreadArg);

/////////////////////////////////////////////////////////////////////////////
// CBallsRaceView construction/destruction

CBallsRaceView::CBallsRaceView()
{
	// TODO: add construction code here
	isTerminate = FALSE;

}

CBallsRaceView::~CBallsRaceView()
{
}

BOOL CBallsRaceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBallsRaceView drawing

void CBallsRaceView::OnDraw(CDC* pDC)
{
	CBallsRaceDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDC->TextOut(0,0,"this is a test");
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CBallsRaceView diagnostics

#ifdef _DEBUG
void CBallsRaceView::AssertValid() const
{
	CView::AssertValid();
}

void CBallsRaceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBallsRaceDoc* CBallsRaceView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBallsRaceDoc)));
	return (CBallsRaceDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBallsRaceView message handlers

int CBallsRaceView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	int i = 0;
	DWORD ThreadID[5];
	static DWORD ThreadArg[5] = {HIGHEST_THREAD,ABOVE_AVE_THREAD,NORMAL_THREAD,
		BELOW_AVE_THEAD,LOWEST_THREAD};
	for(i=0;i<5;i++)
	{
		_hThread[i] = ::CreateThread(NULL,0,
			(LPTHREAD_START_ROUTINE)ThreadProc,
			&ThreadArg[i],
			CREATE_SUSPENDED,
			&ThreadID[i]);
	}
	for(i=0;i<5;i++)
	{
		_hEvent[i] = ::CreateEvent(NULL,TRUE,TRUE,NULL);

	}
	SetThreadPriority(_hThread[0],THREAD_PRIORITY_HIGHEST);
	SetThreadPriority(_hThread[1],THREAD_PRIORITY_ABOVE_NORMAL);
	SetThreadPriority(_hThread[2],THREAD_PRIORITY_NORMAL);
	SetThreadPriority(_hThread[3],THREAD_PRIORITY_BELOW_NORMAL);
	SetThreadPriority(_hThread[4],THREAD_PRIORITY_LOWEST);
				
	// TODO: Add your specialized creation code here
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

	
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	m_GLPixelIndex = ChoosePixelFormat( hDC, &pixelDesc);
	if (SetPixelFormat( hDC, m_GLPixelIndex, &pixelDesc)==FALSE)
	{ 
		return -1;
	}
	m_hGLContext = wglCreateContext(hDC);
	
//	this->SetTimer(1,50,NULL);


	return 0;
}

void CBallsRaceView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);
	wglMakeCurrent(hDC,m_hGLContext);
	
	glEnable(GL_DEPTH_TEST);//允许进行深度缓存校验
	//光源属性设置
	GLfloat ambientProperties[] = {0.3f,0.4f,0.8f,1.0f};//环境光强度值
	GLfloat diffuseProperties[] = {0.3f,0.5f,0.8f,1.0f};//漫反射光强度值
	GLfloat specularProperties[] = {1.0f,1.0f,1.0f,1.0f};//镜面光强度值
	GLfloat lmodel_ambient[] = {0.2f,0.2f,0.2f,1.0f};
	
	GLfloat light_position[] = {0.0f,0.0f,-0.8f,1.0f};//光源位置值
	
	//材质属性设置
	GLfloat no_mat[] = {0.1f,0.1f,0.1f,1.0f}; // 黑色材质
	GLfloat mat_ambient[] = {0.8f,0.7f,0.3f,1.0f};//物体对反射光的反射因素
	GLfloat mat_diffuse[] = {0.3f,0.5f,0.8f,1.0f};
	GLfloat mat_specular[] = {1.0f,1.0f,1.0f,1.0f};
	GLfloat mat_emission[] = {0.3f,0.4f,0.8f,1.0f};
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient);//设置全局环境光
	
	//初始化光源
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientProperties);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseProperties);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specularProperties);
	float ff = 1.0;
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE,&ff);
	glEnable(GL_LIGHT0);//允许GL_LIGHT0光源
	glEnable(GL_LIGHTING);//允许光照
	
	glClearColor( 0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION); //对投影矩阵进行操作
	glLoadIdentity();
	glFrustum(-1,1,-1,1,2,5);
	//
	//	//对将要绘制的设置材质
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse);
	//
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//	
	glTranslatef(0,0,-2);
	auxSolidTorus(0.1,0.2);

	glViewport(350,g_yPosition[0],50,50);	
	auxSolidSphere(0.6);
		
	glViewport(450,g_yPosition[1],50,50);
	auxSolidSphere(0.6);
	
	
	glLightfv(GL_LIGHT0,GL_PROJECTION,light_position);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,no_mat);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,no_mat);
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,mat_emission);
	
	glViewport(550,g_yPosition[2],50,50);
	auxSolidSphere(0.6);

	glViewport(650,g_yPosition[3],50,50);
	auxSolidSphere(0.6);
				
	glViewport(750,g_yPosition[4],50,50);
	auxSolidSphere(0.6);
				

	glFlush();
				
	//为避免画面闪动，采用双缓存
	SwapBuffers(hDC);
				
	// Do not call CView::OnPaint() for painting messages
}

void CBallsRaceView::OnDestroy() 
{
	CView::OnDestroy();	
	// TODO: Add your message handler code here
	if(wglGetCurrentContext()!=NULL)
	{ // make the rendering context not current
		wglMakeCurrent(NULL, NULL) ;
	}
	if (m_hGLContext!=NULL)
	{ 
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}

	if (!isTerminate) {
		OnTerminateall();
	}
	
}

void CBallsRaceView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	InvalidateRect(NULL,FALSE);
	
	CView::OnTimer(nIDEvent);
}

void CBallsRaceView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
 	this->SetTimer(1,10,NULL);
	int i=0;

	for(i=0;i<5;i++)
	{
		ResumeThread(_hThread[i]);
	}

	CView::OnLButtonDown(nFlags, point);
}






void ThreadProc(DWORD *ThreadArg)
{
	DWORD dwThreadHits = 0;
	int i = 0;
	
	switch(*ThreadArg) {
	case HIGHEST_THREAD:
		do {
			::WaitForSingleObject(_hEvent[0],INFINITE);
			dwThreadHits++;
			g_yPosition[0] += 1;
			Sleep(5);
			for(i=0;i<300000;i++);
		} while(dwThreadHits<600);
		
		break;
	case ABOVE_AVE_THREAD:
		do {
			::WaitForSingleObject(_hEvent[1],INFINITE);
			dwThreadHits++;
			g_yPosition[1] += 1;
			for(i=0;i<300000;i++);
			 	Sleep(5);
		} while(dwThreadHits<600);
	
		break;
	case NORMAL_THREAD:
		do {
			::WaitForSingleObject(_hEvent[2],INFINITE);
			dwThreadHits++;
			g_yPosition[2] += 1;
			for(i=0;i<300000;i++);
	 		Sleep(5);
		} while(dwThreadHits<600);
		
		
		break;
	case BELOW_AVE_THEAD:
		do {
			::WaitForSingleObject(_hEvent[3],INFINITE);
			dwThreadHits++;
			g_yPosition[3] += 1;
			for(i=0;i<300000;i++);
	 			Sleep(5);
		} while(dwThreadHits<600);
		
		
		break;
		
	default:
		do {
			::WaitForSingleObject(_hEvent[4],INFINITE);
			dwThreadHits++;
			g_yPosition[4] += 1;
			for(i=0;i<300000;i++);
		 		Sleep(5);
		} while(dwThreadHits<600);						
	}			
}

void CBallsRaceView::OnBall0wait() 
{
	// TODO: Add your command handler code here
	ResetEvent(_hEvent[0]);

}


void CBallsRaceView::OnBall1wait() 
{
	// TODO: Add your command handler code here
	ResetEvent(_hEvent[1]);

}


void CBallsRaceView::OnBall2wait() 
{
	// TODO: Add your command handler code here

	ResetEvent(_hEvent[2]);

}

void CBallsRaceView::OnBall3wait() 
{
	// TODO: Add your command handler code here

	ResetEvent(_hEvent[3]);

}

void CBallsRaceView::OnBall4wait() 
{
	// TODO: Add your command handler code here

	ResetEvent(_hEvent[4]);

}

void CBallsRaceView::OnTerminateall() 
{
	// TODO: Add your command handler code here
 	this->KillTimer(1);

	for(int i=0; i<5; i++)
	{
		::CloseHandle(_hThread[i]);
		::CloseHandle(_hEvent[i]);
		TerminateThread(_hThread[i], 0); 
	}


	isTerminate = TRUE;

	
}

void CBallsRaceView::OnBall0continue() 
{
	// TODO: Add your command handler code here
	::SetEvent(_hEvent[0]);
	
}

void CBallsRaceView::OnBall1continue() 
{
	// TODO: Add your command handler code here
	::SetEvent(_hEvent[1]);
	
}

void CBallsRaceView::OnBall2continue() 
{
	// TODO: Add your command handler code here
	::SetEvent(_hEvent[2]);
}

void CBallsRaceView::OnBall3continue() 
{
	// TODO: Add your command handler code here
	::SetEvent(_hEvent[3]);
}

void CBallsRaceView::OnBall4continue() 
{
	// TODO: Add your command handler code here
	::SetEvent(_hEvent[4]);
}

void CBallsRaceView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CMenu menu;
	menu.LoadMenu(IDR_POPMENU);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
	
}










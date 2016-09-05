// ImageProcessingView.cpp : implementation of the CImageProcessingView class
//

#include "stdafx.h"
#include "ImageProcessing.h"

#include "ImageProcessingDoc.h"
#include "ImageProcessingView.h"
#include  "GlobalApi.h"

#include <complex>
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView

IMPLEMENT_DYNCREATE(CImageProcessingView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProcessingView, CScrollView)
	//{{AFX_MSG_MAP(CImageProcessingView)
	ON_COMMAND(ID_DEGENERATION_INVERSE, OnDegenerationInverse)
	ON_COMMAND(ID_DEGENERATION_MOTION, OnDegenerationMotion)
	ON_COMMAND(ID_RESTORE_INVERSE, OnRestoreInverse)
	ON_COMMAND(ID_RESTORE_MOTION, OnRestoreMotion)
	ON_COMMAND(ID_RESTORE_WINNER, OnRestoreWinner)
	ON_COMMAND(ID_RESTORE_MINDBL, OnRestoreMindbl)
	ON_COMMAND(ID_RESTORE_MAXPRO, OnRestoreMaxpro)
	ON_COMMAND(ID_RESTORE_PROJECT, OnRestoreProject)
	ON_COMMAND(ID_RESTORE_BLIND, OnRestoreBlind)
	ON_COMMAND(ID_RESTORE_GEO, OnRestoreGeo)
	ON_COMMAND(ID_RESTORE_MAXENTR, OnRestoreMaxentr)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView construction/destruction

CImageProcessingView::CImageProcessingView()
{
	// 为小波变换设置的参数
	// 临时存放小波变换系数内存
	m_pDbImage = NULL;	
	
	// 设置当前层数
	m_nDWTCurDepth = 0;

	// 设置小波基紧支集长度
	m_nSupp = 1;
}

CImageProcessingView::~CImageProcessingView()
{
	// 释放已分配内存
	if(m_pDbImage){
		delete[]m_pDbImage;
		m_pDbImage = NULL;
	}
}

BOOL CImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView drawing

void CImageProcessingView::OnDraw(CDC* pDC)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CSize sizeDibDisplay;		
	

	
	if(!pDoc->m_pDibInit->IsEmpty()){	
		sizeDibDisplay = pDoc->m_pDibInit->GetDimensions();
		pDoc->m_pDibInit->Draw(pDC,CPoint(0,0),sizeDibDisplay);	
	}	

}

void CImageProcessingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CSize sizeTotal = pDoc->m_pDibInit->GetDimensions();
	SetScrollSizes(MM_TEXT, sizeTotal);

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView printing

BOOL CImageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView diagnostics

#ifdef _DEBUG
void CImageProcessingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProcessingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProcessingDoc* CImageProcessingView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessingDoc)));
	return (CImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView message handlers




void CImageProcessingView::OnDegenerationInverse() 
{
	// 图象的模糊
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的图象模糊）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象模糊！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBNoRestriction(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreInverse() 
{
	// 图象的逆滤波
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的逆滤波）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的逆滤波！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBInverseFilter(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnDegenerationMotion() 
{
	// 图象的运动模糊
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的运动模糊）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运动模糊！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBMotionDegeneration(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreMotion() 
{
	// 运动模糊图象复原
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的运动模糊复原）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的运动模糊复原！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBMotionRestore(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreWinner() 
{
	// 图象的维纳滤波
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的维纳滤波）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的维纳滤波！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBWinnerFilter(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreMindbl() 
{
	// 图象的最小平方滤波
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的维纳滤波）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的维纳滤波！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBMinDblFilter(pDib);
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreMaxpro() 
{
	// 图象的最大后验滤波
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的维纳滤波）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的维纳滤波！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBMaxProFilter(pDib);
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreProject() 
{
	// 图象的投影滤波
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的维纳滤波）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的维纳滤波！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBProjectFilter(pDib);
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnRestoreBlind() 
{
	// 图象的迭代盲目滤波
		
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的维纳滤波）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的维纳滤波！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBBlindFilter(pDib);
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();	
}



void CImageProcessingView::OnRestoreGeo() //几何复原
{
	// 更改光标形状
	BeginWaitCursor();
	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();
	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;
	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的维纳滤波）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的维纳滤波！", "系统提示" ,MB_ICONINFORMATION | MB_OK);		
		// 返回
		return;
	}	
	::DIBGeoFilter(pDib);
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);		
	// 更新视图
	pDoc->UpdateAllViews(NULL);
    // 恢复光标
	EndWaitCursor();	
}

void CImageProcessingView::OnRestoreMaxentr() 
{
	// 更改光标形状
	BeginWaitCursor();

	// 获取文档
	CImageProcessingDoc* pDoc = GetDocument();

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 获得图象的头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的维纳滤波）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的维纳滤波！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	::DIBMaxEntrFilter(pDib);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);

    // 恢复光标
	EndWaitCursor();
	
}

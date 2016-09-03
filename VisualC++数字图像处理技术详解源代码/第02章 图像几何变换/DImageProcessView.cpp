// DImageProcessView.cpp : implementation of the CDImageProcessView class
//

#include "stdafx.h"
#include "DImageProcess.h"
#include "DlgTran.h"
#include "DlgRot.h"
#include "DlgZoom.h"
#include "Dlggeo.h"

#include "matlib.h"
///#include "DlgHistogram.h"
//#include "DlgLiner.h"
//#include "DlgStretch.h"
//#include "DlgThreshold.h"
//#include "DlgWindow.h"

//#include "GrayEnhance.h"
//#include "MedianFiltDlg.h"
//#include "LaplacSharp.h"
//#include "lowpassPra.h"
//#include "higpassPara.h"
//#include "ErosionPara.h"
//#include "DilationPara.h"
//#include "OpenclosePara.h"
//#include "EdgeDetection.h"
//#include "ThresholdDivide.h"
//#include "RegionGrow.h"
//#include "Projection.h"
//#include "HuffmanCoding.h"
//#include "ShannonCoding.h"

#include "function.h"

#include "DImageProcessView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDImageProcessView* pView;
/////////////////////////////////////////////////////////////////////////////
// CDImageProcessView

IMPLEMENT_DYNCREATE(CDImageProcessView, CView)

BEGIN_MESSAGE_MAP(CDImageProcessView, CView)
	//{{AFX_MSG_MAP(CDImageProcessView)
	ON_COMMAND(ID_TRANSPOSE, OnTranspose)
	ON_COMMAND(ID_MIRROR, OnMirror)
	//ON_COMMAND(ID_HISTOGRAM, OnHistogram)
	//ON_COMMAND(ID_gray_equal, OnGrayEqual)
	ON_COMMAND(ID_geo_rotation, OnRotation)
	ON_COMMAND(ID_geo_translation, OnTranslation)
	ON_COMMAND(ID_geo_zoom, OnZoom)
	//ON_COMMAND(ID_pixel_liner, OnLiner)
	//ON_COMMAND(ID_pixel_stretch, OnStretch)
	//ON_COMMAND(ID_pixel_threshold, OnThreshold)
	//ON_COMMAND(ID_pixel_window, OnWindow)
	ON_COMMAND(ID_dct, OnDct)
	ON_COMMAND(ID_walh_har, OnWalhHar)
	//ON_COMMAND(ID_pixel_log, OnLog)
	ON_COMMAND(ID_fourier, OnFourier)
	ON_COMMAND(ID_kl, OnKL)
	//**ON_COMMAND(ID_inverRestore, OnInverRestore)
	//ON_COMMAND(ID_winnerRestore, OnWinnerRestore)
	//ON_COMMAND(ID_GRAY_ENHANCE, OnGrayEnhance)
	//ON_COMMAND(ID_HIST_ENHANCE, OnHistEnhance)
	//ON_COMMAND(ID_SMOOTH_ENHANCE, OnSmoothEnhance)
	//ON_COMMAND(ID_MEDIAN_FILTER, OnMedianFilter)
	//ON_COMMAND(ID_LAPLAS_SHARP, OnLaplasSharp)
	//ON_COMMAND(ID_LOWPASS_FILER, OnLowpassFiler)
	//ON_COMMAND(ID_HIGHPASS_FILER, OnHighpassFiler)
	//ON_COMMAND(ID_EROSION_CALCULATION, OnErosionCalculation)
	//ON_COMMAND(ID_DILATION_CALCULATION, OnDilationCalculation)
	//ON_COMMAND(ID_OPENCLOSE_CALCULATION, OnOpencloseCalculation)
	//ON_COMMAND(ID_THINING_CALCULATION, OnThiningCalculation)
	//ON_COMMAND(_EDGE_DETECTION, OnEdgeDetection)
	//ON_COMMAND(_HOUGH_TRANS, OnHoughTrans)
	//ON_COMMAND(ID_OUTLINE, OnOutline)
	//ON_COMMAND(ID_CONTOURTRACE, OnContourtrace)
	//ON_COMMAND(ID_THRESHOLD_DIVIDE, OnThresholdDivide)
	//ON_COMMAND(ID_REGION_GROW, OnRegionGrow)
	//ON_COMMAND(ID_PROJECTION, OnProjection)
	//ON_COMMAND(ID_SUBTRACT, OnSubtract)
	//ON_COMMAND(ID_TEMPLETEMATCH, OnTempletematch)
	//ON_COMMAND(ID_HUFFMANCODING, OnHuffmancoding)
	//ON_COMMAND(ID_SHANNONCODING, OnShannoncoding)
	//ON_COMMAND(ID_RLECODING, OnRlecoding)
	//ON_COMMAND(ID_GIFLZW, OnGiflzw)
	//ON_COMMAND(ID_GRAY_ENHANCE, OnGrayEnhance)
	ON_COMMAND(ID_WL, OnWl)
	//}}AFX_MSG_MAP
	//ON_COMMAND_RANGE(ID_SCAN_UPDOWN, ID_INSERT_LEFT, OnImageStunt)
	ON_COMMAND(ID_MIRROR2, &CDImageProcessView::OnMirror2)
		ON_COMMAND(ID_GEOM_ROTA, &CDImageProcessView::OnGeomRota)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDImageProcessView construction/destruction

CDImageProcessView::CDImageProcessView()
{
	// TODO: add construction code here
	m_pDbImage=NULL;
	m_nDWTCurDepth=0;

}

CDImageProcessView::~CDImageProcessView()
{
}

BOOL CDImageProcessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDImageProcessView drawing

void CDImageProcessView::OnDraw(CDC* pDC)
{
	// 获取文档
	CDImageProcessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	HGLOBAL hDIB = pDoc->GetHObject();

	// 判断DIB是否为空
	if (hDIB != NULL)
	{
		LPSTR lpDibSection = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);

		// 获取DIB宽度
		int cxDIB = (int) pDoc->m_dib.GetWidth(lpDibSection);

		// 获取DIB高度
		int cyDIB = (int) pDoc->m_dib.GetHeight(lpDibSection);

		::GlobalUnlock((HGLOBAL) hDIB);

		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;

		CRect rcDest= rcDIB;	
		// 输出DIB
		pDoc->m_dib.DrawDib(pDC->m_hDC, &rcDest, pDoc->GetHObject(),
			&rcDIB, pDoc->GetDocPal());
	}

	/*	long	lSrcWidth;      //图像的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//指向源图像的指针
	LPSTR	lpSrcStartBits;		//指向源图像起始的指针
	LPBITMAPINFO lpbmi;// 指向BITMAPINFO结构的指针
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// 锁定DIB		
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);// 获取图像的宽度		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);// 获取图像的高度
	lpbmi = (LPBITMAPINFO)lpSrcDib;// 获取指针
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);// 找到DIB图像像素起始位置
	/////////////////////////////////////////////////////////////////////////////////////////////////
	CPalette * pPal=new CPalette;// 创建新调色板
	pDoc->m_dib.ConstructPalette(pDoc->m_dib.m_hDib,pPal);

	CPalette * pOldPal=pDC->SelectPalette(pPal, TRUE);
	pDC->RealizePalette();
	::StretchDIBits(pDC->GetSafeHdc(),0,0, lSrcWidth, lSrcHeight,
	0, 0, lSrcWidth , lSrcHeight ,
	lpSrcStartBits, lpbmi,
	DIB_RGB_COLORS, SRCCOPY);
	pDC->SelectPalette(pOldPal, TRUE);
	::DeleteObject(pPal);*/
}

/////////////////////////////////////////////////////////////////////////////
// CDImageProcessView diagnostics

#ifdef _DEBUG
void CDImageProcessView::AssertValid() const
{
	CView::AssertValid();
}

void CDImageProcessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDImageProcessDoc* CDImageProcessView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDImageProcessDoc)));
	return (CDImageProcessDoc*)m_pDocument;
}
#endif //_DEBUG



void CDImageProcessView::OnTranspose() 
{
	// TODO: Add your command handler code here

	CDImageProcessDoc* pDoc = GetDocument();

	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图像每行的字节数
	long	lSrcWidth;      //图像的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//指向源图像的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	long lDstLineBytes;		//新图像每行的字节数
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// 锁定DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{		
		AfxMessageBox(_T("对不起，不是256色位图！"));// 警告				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}											//判断是否是8-bpp位图,不是则返回	
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图像像素起始位置	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图像的宽度		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图像的高度		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// 计算图像每行的字节数

	/////////////////////////////////////////////////////////////////////////////////////////////////

	lDstLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcHeight * 8);	// 计算新图像每行的字节数
	if (Transpose(lpSrcDib,lpSrcStartBits,lSrcWidth,
		lSrcHeight,lSrcLineBytes,lDstLineBytes))	// 调用Transpose()函数转置DIB
	{		
		pDoc->SetDib();									// 更新DIB大小和调色板		
		pDoc->SetModifiedFlag(TRUE);								// 设置脏标记		
		pDoc->UpdateAllViews(NULL);									// 更新视图		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// 解除锁定
	}
	else		
	{
		AfxMessageBox(_T("分配内存失败！")); // 警告
	}
}

void CDImageProcessView::OnMirror() 	
{	
	CDImageProcessDoc* pDoc = GetDocument();						// 获取文档											
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图像每行的字节数
	long	lSrcWidth;      //图像的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//指向源图像的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// 锁定DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{		
		AfxMessageBox(_T("对不起，不是256色位图！"));// 警告				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}											//判断是否是8-bpp位图,不是则返回	
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图像像素起始位置	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图像的宽度		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图像的高度		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// 计算图像每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	if (Mirror(lpSrcStartBits,lSrcWidth, lSrcHeight,lSrcLineBytes))// 调用Mirror()函数水平镜像DIB
	{
		pDoc->SetModifiedFlag(TRUE);								// 设置脏标记		
		pDoc->UpdateAllViews(NULL);									// 更新视图		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// 解除锁定
	}
	else
	{	
		AfxMessageBox(_T("分配内存失败！"));
	}// 警告	
}



void CDImageProcessView::OnTranslation() 
{
	// TODO: Add your command handler code here
	CDImageProcessDoc* pDoc = GetDocument();		
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图像每行的字节数
	long	lSrcWidth;      //图像的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//指向源图像的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	long lDstLineBytes;		//新图像每行的字节数
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// 锁定DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{		
		AfxMessageBox(_T("对不起，不是256色位图！"));// 警告				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}											//判断是否是8-bpp位图,不是则返回	
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图像像素起始位置	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图像的宽度		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图像的高度		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// 计算图像每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////	
	lDstLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcHeight * 8);	// 计算新图像每行的字节数	


	CDlgTran TranPara;// 创建对话框	
	if (TranPara.DoModal() != IDOK)// 显示对话框，提示用户设定量
		return;	
	int temver=TranPara.m_verOff;
	int temhor=TranPara.m_horOff;

	if (Translation(lpSrcStartBits, lSrcWidth,lSrcHeight,// 调用Translation()函数平移DIB
		temver,temhor,lSrcLineBytes,lDstLineBytes))		
	{
		pDoc->SetModifiedFlag(TRUE);								// 设置脏标记		
		pDoc->UpdateAllViews(NULL);									// 更新视图		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// 解除锁定
	}
	else
	{
		AfxMessageBox(_T("分配内存失败！"));
	}//警告	
}
void CDImageProcessView::OnRotation() 
{
	// TODO: Add your command handler code here
	CDImageProcessDoc* pDoc = GetDocument();	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图像每行的字节数
	long	lSrcWidth;      //图像的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//指向源图像的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	long	lDstWidth;      //临时图像的宽度和高度
	long	lDstHeight;
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// 锁定DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{		
		AfxMessageBox(_T("对不起，不是256色位图！"));// 警告				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}										//判断是否是8-bpp位图,不是则返回	
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图像像素起始位置	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图像的宽度		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图像的高度		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// 计算图像每行的字节数
	long lDstLineBytes;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	CDlgRot RotPara;// 创建对话框	
	if(RotPara.DoModal() != IDOK)// 显示对话框，设定旋转角度
	{
		return;	
	}
	DWORD palSize=pDoc->m_dib.GetPalSize(lpSrcDib);	

	float fRotateAngle = (float) AngleToRadian(RotPara.m_rotAngle);// 将旋转角度从度转换到弧度		
	float fSina = (float) sin((double)fRotateAngle);// 计算旋转角度的正余弦	
	float fCosa = (float) cos((double)fRotateAngle);	
	float	fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;// 旋转前四个角的坐标（以图像中心为坐标系原点）		
	float	fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;// 旋转后四个角的坐标（以图像中心为坐标系原点）

	fSrcX1 = (float) (- (lSrcWidth  - 1) / 2);// 计算原图的四个角的坐标
	fSrcY1 = (float) (  (lSrcHeight - 1) / 2);
	fSrcX2 = (float) (  (lSrcWidth  - 1) / 2);
	fSrcY2 = (float) (  (lSrcHeight - 1) / 2);
	fSrcX3 = (float) (- (lSrcWidth  - 1) / 2);
	fSrcY3 = (float) (- (lSrcHeight - 1) / 2);
	fSrcX4 = (float) (  (lSrcWidth  - 1) / 2);
	fSrcY4 = (float) (- (lSrcHeight - 1) / 2);

	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;// 计算新图四个角的坐标
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

	lDstWidth= (long) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);// 计算旋转后的图像实际宽度
	lDstLineBytes=pDoc->m_dib.GetReqByteWidth(lDstWidth * 8);// 计算新图像每行的字节数
	lDstHeight= (long) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) )  + 0.5);// 计算旋转后的图像高度

	HGLOBAL hDstDIB = NULL;// 创建新DIB		
	hDstDIB = (HGLOBAL) Rotate(lpSrcDib,lpSrcStartBits,lSrcWidth,lSrcHeight,lSrcLineBytes,
		palSize,lDstWidth,lDstHeight,lDstLineBytes,fSina,fCosa);// 调用Rotate()函数旋转DIB		
	if(hDstDIB != NULL)// 判断旋转是否成功
	{				
		pDoc->UpdateObject(hDstDIB);// 替换DIB，同时释放旧DIB对象		
		pDoc->SetDib();// 更新DIB大小和调色板				
		pDoc->SetModifiedFlag(TRUE);								// 设置脏标记		
		pDoc->UpdateAllViews(NULL);									// 更新视图		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// 解除锁定
	}
	else
	{		
		AfxMessageBox(_T("分配内存失败！"));
	}// 警告		
}
void CDImageProcessView::OnZoom() 
{
	// TODO: Add your command handler code here
	CDImageProcessDoc* pDoc = GetDocument();		
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图像每行的字节数
	long	lSrcWidth;      //图像的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//指向源图像的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	long	lDstWidth;      //临时图像的宽度和高度
	long	lDstHeight;
	long lDstLineBytes;
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// 锁定DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{		
		AfxMessageBox(_T("对不起，不是256色位图！"));// 警告				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}						
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图像像素起始位置	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图像的宽度		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图像的高度		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// 计算图像每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	DWORD palSize=pDoc->m_dib.GetPalSize(lpSrcDib);	

	CDlgZoom ZoomPara;// 创建对话框,设定平移量
	if (ZoomPara.DoModal() != IDOK)
	{
		return;	
	}
	float fX = ZoomPara.m_horZoom;// 获取设定的平移量,缩放比率
	float fY = ZoomPara.m_verZoom;

	lDstWidth= (long) (lSrcWidth*fX + 0.5);// 计算缩放后的图像实际宽度,加0.5是由于强制类型转换时不四舍五入，而是直接截去小数部分
	lDstLineBytes=pDoc->m_dib.GetReqByteWidth(lDstWidth * 8);	//转换后图像应有的行字节数，为4的倍数
	lDstHeight= (long) (lSrcHeight * fY + 0.5);// 计算缩放后的图像高度

	HGLOBAL hDstDIB = NULL;// 创建新DIB	
	hDstDIB = (HGLOBAL) Zoom(lpSrcDib,lpSrcStartBits,lSrcWidth,lSrcHeight,
		lSrcLineBytes,palSize,lDstWidth,lDstLineBytes,lDstHeight,fX, fY);// 调用Zoom()函数转置DIB		

	if(hDstDIB != NULL)// 判断旋转是否成功
	{				
		pDoc->UpdateObject(hDstDIB);// 替换DIB，同时释放旧DIB对象		
		pDoc->SetDib();// 更新DIB大小和调色板				
		pDoc->SetModifiedFlag(TRUE);								// 设置脏标记		
		pDoc->UpdateAllViews(NULL);									// 更新视图		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// 解除锁定
	}
	else
	{		
		AfxMessageBox(_T("分配内存失败！"));
	}
	// 警告	
}




void CDImageProcessView::OnDct() 
{
	// TODO: Add your command handler code here
	CDImageProcessDoc* pDoc = GetDocument();	
	SetCursor(LoadCursor(NULL, IDC_WAIT));//光标显示为处理状态
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图像每行的字节数
	long	lSrcWidth;      //图像的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//指向源图像的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// 锁定DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{		
		AfxMessageBox(_T("对不起，不是256色位图！"));// 警告				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}		
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图像像素起始位置	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图像的宽度		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图像的高度		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// 计算图像每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////		
	// 调用Dct()函数进行离散余弦变换
	if (CosTran(lpSrcStartBits, lSrcWidth, lSrcHeight,lSrcLineBytes))
	{
		pDoc->SetModifiedFlag(TRUE);								// 设置脏标记		
		pDoc->UpdateAllViews(NULL);									// 更新视图		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// 解除锁定
	}
	else
	{		
		AfxMessageBox(_T("分配内存失败！"));
	}// 警告
	SetCursor(LoadCursor(NULL, IDC_ARROW));//光标显示为处理状态
}

void CDImageProcessView::OnWalhHar() 
{
	// TODO: Add your command handler code here
	CDImageProcessDoc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图像每行的字节数
	long	lSrcWidth;      //图像的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//指向源图像的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// 锁定DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{		
		AfxMessageBox(_T("对不起，不是256色位图！"));// 警告				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}		
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图像像素起始位置	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图像的宽度		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图像的高度		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// 计算图像每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////			
	// 调用Walsh()进行变换
	if (Walsh_HarTran(lpSrcStartBits, lSrcWidth, lSrcHeight,lSrcLineBytes))
	{
		pDoc->SetModifiedFlag(TRUE);								// 设置脏标记		
		pDoc->UpdateAllViews(NULL);									// 更新视图		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// 解除锁定
	}
	else
	{		
		AfxMessageBox(_T("分配内存失败！"));
	}// 警告
}

void CDImageProcessView::OnLog() 
{
	// TODO: Add your command handler code here
	CDImageProcessDoc* pDoc = GetDocument();// 获取文档				
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图像每行的字节数
	long	lSrcWidth;      //图像的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//指向源图像的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// 锁定DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{		
		AfxMessageBox(_T("对不起，不是256色位图！"));// 警告				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}		
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图像像素起始位置	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图像的宽度		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图像的高度		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// 计算图像每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////	
	LogTranslation(lpSrcStartBits, lSrcWidth, lSrcHeight,lSrcLineBytes);	
	pDoc->SetModifiedFlag(TRUE);								// 设置脏标记		
	pDoc->UpdateAllViews(NULL);									// 更新视图		
	::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// 解除锁定
}

void CDImageProcessView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	pView=this;
}


void CDImageProcessView::OnFourier() 
{
	// TODO: Add your command handler code here
	CDImageProcessDoc* pDoc = GetDocument();
	SetCursor(LoadCursor(NULL, IDC_WAIT));//光标显示为处理状态
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图像每行的字节数
	long	lSrcWidth;      //图像的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//指向源图像的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// 锁定DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{		
		AfxMessageBox(_T("对不起，不是256色位图！"));// 警告				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}		
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图像像素起始位置	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图像的宽度		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图像的高度		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// 计算图像每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////		
	double dTmp;// 临时变量
	long	wid=1,hei=1;// 进行付立叶变换的宽度和高度，初始化为1		
	int		widpor=0,heiPor=0;//2的幂数	
	unsigned char*	lpSrcUnChr;	//指向像素的指针
	long i;                 //行循环变量
	long j;                 //列循环变量	
	while(wid * 2 <= lSrcWidth)// 计算进行付立叶变换的宽度和高度（2的整数次方）
	{
		wid *= 2;
		widpor++;
	}	
	while(hei * 2 <= lSrcHeight)
	{
		hei *= 2;
		heiPor++;
	}
	CplexNum *pTd = new CplexNum[sizeof(CplexNum)*wid * hei];// 分配内存
	CplexNum *pFd = new CplexNum[sizeof(CplexNum)*wid * hei];
	// 初始化
	// 图像数据的宽和高不一定是2的整数次幂，所以pCTData
	// 有一部分数据需要补0
	for(i=0; i<hei; i++)
	{
		for(j=0; j<wid; j++)
		{
			pTd[i*wid + j].re=0;
			pTd[i*wid + j].im=0;
		}
	}
	// 把图像数据传给pCTData
	for(i=0; i<lSrcHeight; i++)
	{
		for(j=0; j<lSrcWidth; j++)
		{
			lpSrcUnChr=(unsigned char*)lpSrcStartBits+lSrcLineBytes*(lSrcHeight-1-i)+j;
			pTd[i*wid + j].re=*lpSrcUnChr;//complex<double>(unchValue,0);
			pTd[i*wid + j].im=0;
		}
	}
	Fourier(pTd, lSrcWidth, lSrcHeight, pFd) ;

	for(i=0; i<lSrcHeight; i++)
	{
		for(j=0; j<lSrcWidth; j++)
		{
			dTmp = pFd[i * wid + j].re* pFd[i * wid + j].re
				+ pFd[i * wid + j].im* pFd[i * wid + j].im;			
			dTmp = sqrt(dTmp) ;

			// 为了显示，需要对幅度的大小进行伸缩
			dTmp /= 100  ;
			// 限制图像数据的大小
//			dTmp = min(dTmp, 255) ;
			lpSrcStartBits[i*lSrcLineBytes +j] = (unsigned char)(int)dTmp;
		}
	}
	// 为了在屏幕上显示，我们把幅度值大的部分用黑色显示
	for(i=0; i<lSrcHeight; i++)
	{
		for(j=0; j<lSrcWidth; j++)
		{
			lpSrcStartBits[i*lSrcLineBytes +j] = 255 - lpSrcStartBits[i*lSrcLineBytes +j];
		}
	}			
	delete pTd;// 释放内存
	delete pFd;	
	pDoc->SetModifiedFlag(TRUE);// 设置脏标记			
	pDoc->UpdateAllViews(NULL);// 更新视图
	SetCursor(LoadCursor(NULL, IDC_ARROW));//光标显示为处理状态
}

void CDImageProcessView::OnKL() 
{
	// TODO: Add your command handler code here
	// 获取文档
	CDImageProcessDoc* pDoc = GetDocument();
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图像每行的字节数
	long	lSrcWidth;      //图像的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//指向源图像的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// 锁定DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{		
		AfxMessageBox(_T("对不起，不是256色位图！"));// 警告				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}		
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图像像素起始位置	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图像的宽度		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图像的高度		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// 计算图像每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//调用DisK_L变换
	if (DisK_L(lpSrcStartBits,lSrcWidth,lSrcHeight,lSrcLineBytes))
	{
		pDoc->SetModifiedFlag(TRUE);								// 设置脏标记		
		pDoc->UpdateAllViews(NULL);									// 更新视图		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// 解除锁定
	}
	else
	{		
		AfxMessageBox(_T("分配内存失败！"));// 警告
	}
}







void CDImageProcessView::OnWl() 
{
	CDImageProcessDoc* pDoc = GetDocument();					//获取文档
	LPSTR lpDIB;											//指向源图像的指针
	LPSTR    lpDIBBits;									//指向DIB像素指针
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());		//锁定DIB
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);							//找到DIB图像像素起始位置
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)							//判断是否是256色位图
	{
		//提示用户
		//MessageBox("目前只支持256色位图！", "系统提示" , MB_ICONINFORMATION | MB_OK);	
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());				//解除锁定
		return;											//返回
	}
	int nWidth;												//图像宽度
	int nHeight;												//图像高度
	nWidth=(int)pDoc->m_dib.GetWidth(lpDIB);									//获取图像宽度
	nHeight=(int)pDoc->m_dib.GetHeight(lpDIB);									//获取图像高度

	DIBDWTStep(lpDIBBits,m_pDbImage, nWidth,nHeight, 0, m_nDWTCurDepth,1);
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL) pDoc->GetHObject());

}

void CDImageProcessView::OnMirror2()
{
	// TODO: 在此添加命令处理程序代码
	CDImageProcessDoc* pDoc = GetDocument();						// 获取文档											
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图像每行的字节数
	long	lSrcWidth;      //图像的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcDib;		//指向源图像的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib= (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());// 锁定DIB
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{		
		AfxMessageBox(_T("对不起，不是256色位图！"));// 警告				
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}											//判断是否是8-bpp位图,不是则返回	
	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图像像素起始位置	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图像的宽度		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图像的高度		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// 计算图像每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	if (Mirror2(lpSrcStartBits,lSrcWidth, lSrcHeight,lSrcLineBytes))// 调用Mirror2()函数水平镜像DIB
	{
		pDoc->SetModifiedFlag(TRUE);								// 设置脏标记		
		pDoc->UpdateAllViews(NULL);									// 更新视图		
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());					// 解除锁定
	}
	else
	{	
		AfxMessageBox(_T("分配内存失败！"));
	}// 警告
} 



void CDImageProcessView::OnGeomRota()
{
	// TODO: 在此添加命令处理程序代码
	// 图像旋转

	CDImageProcessDoc* pDoc = GetDocument();
	long lSrcLineBytes;		                       //图像每行的字节数
	long	lSrcWidth;                                                     //图像的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcDib;		                                      //指向源图像的指针
	LPSTR	lpSrcStartBits;	                                     //指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHObject());//lpSrcDib=lpDIB GetHDIB=GetHObject
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的旋转，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpSrcDib)  != 256)
	{
		 AfxMessageBox (_T("对不起，不是色位图！"));                           // 警告
		::GlobalUnlock((HGLOBAL) pDoc->GetHObject());                     // 解除锁定
		return;						             //返回
	}

	lpSrcStartBits=pDoc->m_dib.GetBits(lpSrcDib);		// 找到DIB图像像素起始位置	
	lSrcWidth= pDoc->m_dib.GetWidth(lpSrcDib);		// 获取图像的宽度		
	lSrcHeight= pDoc->m_dib.GetHeight(lpSrcDib);		// 获取图像的高度		
	lSrcLineBytes=pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);	   // 计算图像每行的字节数

	CDlggeo RotPara;//CGeoRotaDlg dlgPara;
	//dlgPara.m_iRotateAngle = 90;	
	// 显示对话框，提示用户设定旋转角度
	if (RotPara.DoModal() != IDOK)
	{
		return;
	}
	float fRotateAngle = (float) AngleToRadian(RotPara.m_rotAngle);
	
	delete RotPara;	
	DWORD palSize=pDoc->m_dib.GetPalSize(lpSrcDib);

	HGLOBAL hNewDIB = NULL;//HDIB
	BeginWaitCursor();

	hNewDIB = (HGLOBAL) RotateDIB2(lpSrcDib, fRotateAngle,lpSrcStartBits,lSrcWidth,lSrcHeight,palSize);
	if (hNewDIB != NULL)
	{		
		pDoc->UpdateObject(hNewDIB);			// 替换DIB，同时释放旧DIB对象		
		pDoc->SetDib();				           // 更新DIB大小和调色板		
		pDoc->SetModifiedFlag(TRUE);		   // 设置脏标记		
		//SetScrollSizes(MM_TEXT, pDoc->GetDocSize());// 重新设置滚动视图大小	
		pDoc->UpdateAllViews(NULL);			// 更新视图
		
	}
	else
	{
		AfxMessageBox(_T("分配内存失败！"));//MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	::GlobalUnlock((HGLOBAL) pDoc->GetHObject());  // 解除锁定//::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	EndWaitCursor();
}

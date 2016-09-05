// MagicHouseView.cpp : CMagicHouseView 类的实现
//

#include "stdafx.h"
#include "MagicHouse.h"

#include "MagicHouseDoc.h"
#include "MagicHouseView.h"

#include "MagicHouseDoc.h"
#include "mainfrm.h"

#include "EffectDisplay.h"
#include "myDib.h"
#include "HistogramDlg.h"

#include "JpegDecoder.h"
#include "PreviewDlg.h"
#include "LineTranDlg.h"
#include "StretchDlg.h"
#include "ThresholdDlg.h"
#include "GrayOperator.h"
#include "LogTranDlg.h"
#include "PowerTranDlg.h"
#include "ExpTranDlg.h"
#include "MoveTranDlg.h"
#include "MirTranDlg.h"
#include "ZoomDlg.h"
#include "RotateDlg.h"
#include "FogDlg.h"

#include "GeoOperator.h"
#include "improve.h"
#include "Filter.h"

#include <iostream>
#include <string>
using namespace std;

#pragma warning ( disable : 4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 图像显示类型枚举
enum ShowType
{
	ST_NORMAL		= 0,	// 原始大小
	ST_FIXWIDTH		= 1,	// 适合宽度
	ST_FIXHEIGHT	= 2,	// 适合高度
	ST_FIXSCREEN	= 3		// 适合屏幕
};

typedef struct s_BM_header {
	WORD BMP_id ; // 'B''M'
	DWORD size; // size in bytes of the BMP file
	DWORD zero_res; // 0
	DWORD offbits; // 54
	DWORD biSize; // 0x28
	DWORD Width;  // X
	DWORD Height;  // Y
	WORD  biPlanes; // 1
	WORD  biBitCount ; // 24
	DWORD biCompression; // 0 = BI_RGB
	DWORD biSizeImage; // 0
	DWORD biXPelsPerMeter; // 0xB40
	DWORD biYPelsPerMeter; // 0xB40
	DWORD biClrUsed; //0
	DWORD biClrImportant; //0
} BM_header;

typedef struct s_RGB {
	BYTE B;
	BYTE G;
	BYTE R;
} RGB;

// CMagicHouseView

IMPLEMENT_DYNCREATE(CMagicHouseView, CView)

BEGIN_MESSAGE_MAP(CMagicHouseView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_MODE_NORMAL, &CMagicHouseView::OnModeNormal)
	ON_COMMAND(ID_MODE_WIDTH, &CMagicHouseView::OnModeWidth)
	ON_COMMAND(ID_MODE_HEIGHT, &CMagicHouseView::OnModeHeight)
	ON_COMMAND(ID_MODE_SCREEN, &CMagicHouseView::OnModeScreen)
	ON_UPDATE_COMMAND_UI(ID_MODE_NORMAL, &CMagicHouseView::OnUpdateModeNormal)
	ON_UPDATE_COMMAND_UI(ID_MODE_WIDTH, &CMagicHouseView::OnUpdateModeWidth)
	ON_UPDATE_COMMAND_UI(ID_MODE_HEIGHT, &CMagicHouseView::OnUpdateModeHeight)
	ON_UPDATE_COMMAND_UI(ID_MODE_SCREEN, &CMagicHouseView::OnUpdateModeScreen)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_IMAGE_PREV, &CMagicHouseView::OnImagePrev)
	ON_COMMAND(ID_IMAGE_NEXT, &CMagicHouseView::OnImageNext)
	ON_COMMAND(ID_IMAGE_FIRST, &CMagicHouseView::OnImageFirst)
	ON_COMMAND(ID_IMAGE_LAST, &CMagicHouseView::OnImageLast)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_PREV, &CMagicHouseView::OnUpdateImagePrev)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_NEXT, &CMagicHouseView::OnUpdateImageNext)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_FIRST, &CMagicHouseView::OnUpdateImageFirst)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_LAST, &CMagicHouseView::OnUpdateImageLast)
	ON_COMMAND(ID_EFFECT_NONE, &CMagicHouseView::OnEffectNone)
	ON_COMMAND(ID_EFFECT_RAND, &CMagicHouseView::OnEffectRand)
	ON_COMMAND(ID_EFFECT_SCANDOWN, &CMagicHouseView::OnEffectScan)
	ON_COMMAND(ID_EFFECT_VSCAN, &CMagicHouseView::OnEffectDscan)
	ON_COMMAND(ID_EFFECT_MOVERIGHT, &CMagicHouseView::OnEffectMove)
	ON_COMMAND(ID_EFFECT_VBLIND, &CMagicHouseView::OnEffectVBlind)
	ON_COMMAND(ID_EFFECT_HRASTER, &CMagicHouseView::OnEffectHRaster)
	ON_COMMAND(ID_EFFECT_MOSAIC, &CMagicHouseView::OnEffectMosaic)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_NONE, &CMagicHouseView::OnUpdateEffectNone)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_RAND, &CMagicHouseView::OnUpdateEffectRand)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_SCANDOWN, &CMagicHouseView::OnUpdateEffectScan)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_VSCAN, &CMagicHouseView::OnUpdateEffectDscan)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_MOVERIGHT, &CMagicHouseView::OnUpdateEffectMove)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_VBLIND, &CMagicHouseView::OnUpdateEffectBlind)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_HRASTER, &CMagicHouseView::OnUpdateEffectRaster)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_MOSAIC, &CMagicHouseView::OnUpdateEffectMosaic)
	ON_COMMAND(ID_EFFECT_HSMOVE, &CMagicHouseView::OnEffectHsmove)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_HSMOVE, &CMagicHouseView::OnUpdateEffectHsmove)
	ON_COMMAND(ID_EFFECT_SBLIND, &CMagicHouseView::OnEffectSblind)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_SBLIND, &CMagicHouseView::OnUpdateEffectSblind)
	ON_COMMAND(ID_EFFECT_VRASTER, &CMagicHouseView::OnEffectVraster)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_VRASTER, &CMagicHouseView::OnUpdateEffectVraster)
	ON_COMMAND(ID_EFFECT_RAINDROP, &CMagicHouseView::OnEffectRaindrop)
	ON_UPDATE_COMMAND_UI(ID_EFFECT_RAINDROP, &CMagicHouseView::OnUpdateEffectRaindrop)
	ON_COMMAND(ID_FILE_OPEN, &CMagicHouseView::OnFileOpen)
	ON_COMMAND(ID_MODE_BROWSE, &CMagicHouseView::OnModeBrowse)
	ON_COMMAND(ID_MODE_EDIT, &CMagicHouseView::OnModeEdit)
	ON_COMMAND(ID_FILE_SAVE, &CMagicHouseView::OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_MODE_BROWSE, &CMagicHouseView::OnUpdateModeBrowse)
	ON_UPDATE_COMMAND_UI(ID_MODE_EDIT, &CMagicHouseView::OnUpdateModeEdit)
	ON_COMMAND(ID_FILTER_NEGATIVE, &CMagicHouseView::OnFilterNegative)
	ON_COMMAND(ID_FILTER_EMBOSS, &CMagicHouseView::OnFilterEmboss)
	ON_COMMAND(ID_FILTER_BW, &CMagicHouseView::OnFilterBw)
	ON_COMMAND(ID_FILTER_FOG, &CMagicHouseView::OnFilterFog)
	ON_COMMAND(ID_FILTER_MOSAIC, &CMagicHouseView::OnFilterMosaic)
	ON_COMMAND(ID_FILTER_SKETCH, &CMagicHouseView::OnFilterSketch)
END_MESSAGE_MAP()

// CMagicHouseView 构造/析构

CMagicHouseView::CMagicHouseView()
{
	m_bIsEditMode = false;
	m_nShowType = ST_NORMAL;
	m_bHeightOut = false;
	m_bWidhtOut = false;
	m_nXX = 0;
	m_nYY = 0;
	m_nXXMax = 0;
	m_nYYMax = 0;
	m_nPos = -1;

	m_pBitmap = NULL;
	m_bEffectDraw = false;
	m_nEffectDisplayType = EDT_RAND;

	m_pImageBuffer = NULL;
	m_pImageTempBuffer = NULL;
	m_nPicWidth = m_nPicHeight = 0;
}

CMagicHouseView::~CMagicHouseView()
{
	::delete m_pBitmap;

	FreeImageBuffer();
}

//--------------------------------------------------------------
//	作用:		视图重绘函数
//--------------------------------------------------------------
void CMagicHouseView::OnDraw(CDC* pDC)
{
	CMagicHouseDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(rect);

	CDC memDC;
	CBitmap MemBitmap;

	// 设备描述表初始化
	memDC.CreateCompatibleDC(NULL);

	// 建立与屏幕显示兼容的内存显示设备
	MemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

	// 选取空白位图
	memDC.SelectObject(MemBitmap);
	memDC.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255,255,255));

	if (m_nPos >= 0)
		ShowPicture(&memDC, *m_pBitmap, m_nShowType);

	if (m_bEffectDraw && m_nEffectDisplayType != EDT_NONE)
	{
		int nHeight = rect.Height() - m_nShowPicHeight;
		int nWidth = rect.Width() - m_nShowPicWidth;

		if (nHeight < 0)	
			nHeight = 0;
		if (nWidth < 0)	
			nWidth = 0;

		pDC->FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255,255,255));

		EffectDisplay::s_nOffsetX = nWidth / 2;
		EffectDisplay::s_nOffsetY = nHeight / 2;
		EffectDisplay::s_nPicWidth = m_nShowPicWidth;
		EffectDisplay::s_nPicHeight = m_nShowPicHeight;
		EffectDisplay::s_nCDCWidth = rect.Width();
		EffectDisplay::s_nCDCHeight = rect.Height();

		EffectDisplayImage(pDC, &memDC);

		m_bEffectDraw = false;
	}
	else
		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	MemBitmap.DeleteObject();
	memDC.DeleteDC();
}

//--------------------------------------------------------------
//	作用:		打开指定路径的文件
//--------------------------------------------------------------
void CMagicHouseView::OpenFile(const CString& strPath)
{	
	FreeImageBuffer();
	m_FilesNameAry.RemoveAll();

	int nIndex = strPath.ReverseFind('\\');
	m_strPath = strPath.Left(nIndex);
	CString strFileName = strPath.Right(strPath.GetLength() - nIndex - 1);

	CMainFrame::GetAllFileNames(m_strPath, m_FilesNameAry);
	m_nPicNum = (int)m_FilesNameAry.GetCount();

	m_nPos = 0;

	while( m_nPos < m_nPicNum && m_FilesNameAry[m_nPos] != strFileName)
		m_nPos++;

	PreDrawImage();
	m_bEffectDraw = true;
	Invalidate();

	if (m_bIsEditMode)
	{
		CString strPath = GetFilePath(m_nPos);
		GetImageBuffer(strPath);
	}
}

void CMagicHouseView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
}

// CMagicHouseView 打印

BOOL CMagicHouseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMagicHouseView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMagicHouseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CMagicHouseView 诊断

#ifdef _DEBUG
void CMagicHouseView::AssertValid() const
{
	CView::AssertValid();
}

void CMagicHouseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMagicHouseDoc* CMagicHouseView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMagicHouseDoc)));
	return (CMagicHouseDoc*)m_pDocument;
}
#endif //_DEBUG

//----------------------------------
// 作用:		"打开"的命令响应函数
//----------------------------------
void CMagicHouseView::OnFileOpen()
{
	CFileDialog dlg(true, NULL, L"", 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, 
		L"所有图像文件(*.bmp; *.jpg)|*.bmp; *.jpg|所有文件(*.*)|*.*||");

	if (dlg.DoModal() == IDOK)
	{
		OpenFile(dlg.GetPathName());
		FixOffset();
		RefreshIndicator();
		Invalidate();
	}
}

//----------------------------------
//	作用:		禁止重绘背景
//----------------------------------
BOOL CMagicHouseView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

//----------------------------------
//	作用:			以指定模式显示图像
//	参数:		
//  	pDC			目标设备上下文指针
//		image		显示图像
//		nShowType	显示模式
//----------------------------------
void CMagicHouseView::ShowPicture(CDC* pDC, Image& image, int nShowType)
{
	Graphics graph(pDC->GetSafeHdc());

	CRect winRect;
	GetClientRect(winRect);

	REAL x = 0.0;
	REAL y = 0.0;
	CSize showPicSize = GetShowPicSize(image, m_nShowType);
	REAL width  = (REAL)showPicSize.cx;
	REAL height = (REAL)showPicSize.cy;

	// 使图像居中
	if ((UINT)winRect.Width() > width)
		x = (winRect.Width() - width) / 2.0f;

	if ( (UINT)winRect.Height() > height )
		y = ((UINT)winRect.Height() - height) / 2.0f;

	if (m_bWidhtOut)
		x += m_nXX;

	if (m_bHeightOut)
		y += m_nYY;

	m_nShowPicHeight = (int)height;
	m_nShowPicWidth = (int)width;

	graph.DrawImage(&image, x, y, width, height);
}

//----------------------------------
//	作用:		设置显示模式
//	参数:		
//		nType	新的显示模式
//----------------------------------
void CMagicHouseView::SetShowType(int nType)
{
	m_nShowType = nType;
}

//----------------------------------
//	作用:			获得指定模式下图像的大小
//	参数:		
//		image		图像对象
//		nShowType	显示模式
//	返回值:			在指定模式下图像的显示大小
//----------------------------------
CSize CMagicHouseView::GetShowPicSize(Image& image, int nShowType)
{
	REAL width  = (REAL)image.GetWidth();
	REAL height = (REAL)image.GetHeight();
	REAL c		= 0.0f;

	CRect winRect;
	GetClientRect(winRect);

	switch (nShowType)
	{
	case ST_FIXWIDTH:	// 适合宽度模式
		width = (REAL)winRect.Width();
		c = width / image.GetWidth();
		height = c * image.GetHeight();
		break;

	case ST_FIXHEIGHT:	// 适合高度模式
		height = (REAL)winRect.Height();
		c = height / image.GetHeight();
		width = c * image.GetWidth();

		break;

	case ST_FIXSCREEN:	// 适合大小模式

		// 按适合宽度模式处理
		width = (REAL)winRect.Width();
		c = width / image.GetWidth();

		// 如果没有超界
		if (c * image.GetHeight() < winRect.Height())
			height = c * image.GetHeight();
		// 超界则采用适合高度模式
		else
		{
			height = (REAL)winRect.Height();
			c = height / image.GetHeight();
			width = c * image.GetWidth();
		}

		break;
	}

	return CSize((int)width, (int)height);
}


//----------------------------------
//	作用:		鼠标按下的响应函数
//----------------------------------
void CMagicHouseView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_psMove = point;

	CView::OnLButtonDown(nFlags, point);
}

//----------------------------------
//	作用:		修正图像的偏移坐标等信息
//	备注:		以显示模式为依据
//----------------------------------
void CMagicHouseView::FixOffset()
{
	if (m_nPos < 0 || m_nPos >= m_nPicNum)
		return;

	Image image(GetFilePath(m_nPos));

	if ( image.GetLastStatus() != Ok )
		return;

	m_bHeightOut = false;
	m_bWidhtOut = false;

	CSize imgSize = GetShowPicSize(image, m_nShowType);
	CRect rect;

	GetClientRect(rect);

	switch (m_nShowType)
	{
	case ST_NORMAL:
		if (imgSize.cx > rect.Width())
		{
			m_nXXMax = imgSize.cx - rect.Width();
			m_bWidhtOut = true;
		}

		if (imgSize.cy > rect.Height())
		{
			m_nYYMax = imgSize.cy - rect.Height();
			m_bHeightOut = true;
		}
		break;

	case ST_FIXWIDTH:
		if (imgSize.cy > rect.Height())
		{
			m_nYYMax = imgSize.cy - rect.Height();
			m_bHeightOut = true;
		}
		break;

	case ST_FIXHEIGHT:
		if (imgSize.cx > rect.Width())
		{
			m_nXXMax = imgSize.cx - rect.Width();
			m_bWidhtOut = true;
		}
		break;

	case ST_FIXSCREEN:
	default:;
	}
}

//----------------------------------
//	作用:		鼠标移动的响应函数
//	备注:		如果图像为拖动状态,则自动修正图像的偏移坐标
//----------------------------------
void CMagicHouseView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON)
	{
		if (m_bWidhtOut)
		{
			m_nXX += point.x - m_psMove.x;
			if (m_nXX < 0 && abs(m_nXX) > m_nXXMax)
				m_nXX = -m_nXXMax;
			if (m_nXX > 0)
				m_nXX = 0;
		}

		if (m_bHeightOut)
		{
			m_nYY += point.y - m_psMove.y;
			if (m_nYY < 0 && abs(m_nYY) > m_nYYMax)
				m_nYY = -m_nYYMax;
			if (m_nYY > 0)
				m_nYY = 0;
		}

		m_psMove = point;

		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}

//------------------------------------------------------------
//	作用:		根据情况改变鼠标光标
//------------------------------------------------------------
BOOL CMagicHouseView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_bHeightOut || m_bWidhtOut)
	{
		SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		return TRUE;
	}
	else
		return CView::OnSetCursor(pWnd, nHitTest, message);
}

//--------------------------------------------------------------
//	作用:		在内存中预先绘制图像
//--------------------------------------------------------------
void CMagicHouseView::PreDrawImage(void)
{
	Image image(GetFilePath(m_nPos));

	::delete m_pBitmap;

	m_pBitmap = ::new Bitmap(image.GetWidth(), image.GetHeight(), PixelFormat32bppARGB);
	Graphics* graph = Graphics::FromImage(m_pBitmap);

	graph->DrawImage(&image, 0, 0, image.GetWidth(), image.GetHeight());
	SAFE_DELETE (graph);
}

//--------------------------------------------------------------
//	作用:		更新状态栏的指示器信息
//--------------------------------------------------------------
void CMagicHouseView::RefreshIndicator()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	CString strTemp;
	strTemp.Format(L"%d/%d", m_nPos + 1, m_nPicNum);
	pMain->UpdateIndicator(ID_INDICATOR_PICNUM, strTemp);
	pMain->UpdateIndicator(ID_INDICATOR_PICNAME, m_FilesNameAry[m_nPos]);
}

//--------------------------------------------------------------
//	作用:		使用特效显示图像
//	参数:
//		pDC		目标设备上下文
//		pMemDC	内存设备上下文
//	备注:	将内存设备上下文中已经绘制的图像以特效形式显示在目标设备上下文中
//--------------------------------------------------------------
void CMagicHouseView::EffectDisplayImage(CDC* pDC, CDC* pMemDC)
{
	int nType = m_nEffectDisplayType;

	if (nType == EDT_RAND)
	{
		LARGE_INTEGER seed;
		QueryPerformanceFrequency(&seed);
		QueryPerformanceCounter(&seed);

		//初始化一个以微秒为单位的时间种子
		srand((int)seed.QuadPart);

		nType = rand() % 10;
	}

	switch (nType)
	{
	case EDT_SCANDOWN:
		EffectDisplay::ScanDownDisplay(pDC, pMemDC);
		break;

	case EDT_VSCAN:
		EffectDisplay::VSScanDisplay(pDC, pMemDC);
		break;

	case EDT_MOVERIGHT:
		EffectDisplay::MoveRightDisplay(pDC, pMemDC);
		break;

	case EDT_HSMOVE:
		EffectDisplay::HSMoveDisplay(pDC, pMemDC);
		break;

	case EDT_VBLIND:
		EffectDisplay::VBlindDisplay(pDC, pMemDC);
		break;

	case EDT_HBLIND:
		EffectDisplay::HBlindDisplay(pDC, pMemDC);
		break;

	case EDT_VRASTER:
		EffectDisplay::VRasterDisplay(pDC, pMemDC);
		break;

	case EDT_HRASTER:
		EffectDisplay::HRasterDisplay(pDC, pMemDC);
		break;

	case EDT_MOSAIC:
		EffectDisplay::MosaicDisplay(pDC, pMemDC);
		break;

	default:
		EffectDisplay::RaindropDisplay(pDC, pMemDC);
	}
}

void CMagicHouseView::OnModeNormal()
{
	if (m_nShowType != ST_NORMAL)
	{
		m_nShowType = ST_NORMAL;
		FixOffset();
		Invalidate();
	}
}

void CMagicHouseView::OnModeWidth()
{
	if (m_nShowType != ST_FIXWIDTH)
	{
		m_nShowType = ST_FIXWIDTH;
		FixOffset();
		Invalidate();
	}
}

void CMagicHouseView::OnModeHeight()
{
	if (m_nShowType != ST_FIXHEIGHT)
	{
		m_nShowType = ST_FIXHEIGHT;
		FixOffset();
		Invalidate();
	}
}

void CMagicHouseView::OnModeScreen()
{
	if (m_nShowType != ST_FIXSCREEN)
	{
		m_nShowType = ST_FIXSCREEN;
		FixOffset();
		Invalidate();
	}
}

void CMagicHouseView::OnUpdateModeNormal(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nShowType == ST_NORMAL);
}

void CMagicHouseView::OnUpdateModeWidth(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nShowType == ST_FIXWIDTH);
}

void CMagicHouseView::OnUpdateModeHeight(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nShowType == ST_FIXHEIGHT);
}

void CMagicHouseView::OnUpdateModeScreen(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nShowType == ST_FIXSCREEN);
}

void CMagicHouseView::OnImagePrev()
{
	if (m_nPos <= 0)
		return;

	m_nPos--;

	PreDrawImage();
	ClearOffset();
	FixOffset();
	RefreshIndicator();
	m_bEffectDraw = true;
	Invalidate();
}

void CMagicHouseView::OnImageNext()
{
	if (m_nPos >= m_nPicNum - 1)
		return;

	m_nPos++;

	PreDrawImage();
	ClearOffset();
	FixOffset();
	RefreshIndicator();
	m_bEffectDraw = true;
	Invalidate();
}

void CMagicHouseView::OnImageFirst()
{
	m_nPos = 0;

	PreDrawImage();
	ClearOffset();
	FixOffset();
	RefreshIndicator();
	m_bEffectDraw = true;
	Invalidate();
}

void CMagicHouseView::OnImageLast()
{
	m_nPos = m_nPicNum - 1;

	PreDrawImage();
	ClearOffset();
	FixOffset();
	RefreshIndicator();
	m_bEffectDraw = true;
	Invalidate();
}

void CMagicHouseView::OnUpdateImagePrev(CCmdUI *pCmdUI)
{
	if (m_nPos <= 0 || m_bIsEditMode)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CMagicHouseView::OnUpdateImageNext(CCmdUI *pCmdUI)
{
	if ((m_nPos < 0) || m_nPos >= m_nPicNum - 1 || m_bIsEditMode)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CMagicHouseView::OnUpdateImageFirst(CCmdUI *pCmdUI)
{
	if (m_nPos < 0 || m_bIsEditMode)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CMagicHouseView::OnUpdateImageLast(CCmdUI *pCmdUI)
{
	if (m_nPos < 0 || m_bIsEditMode)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CMagicHouseView::OnEffectNone()
{
	m_nEffectDisplayType = EDT_NONE;
}

void CMagicHouseView::OnEffectRand()
{
	m_nEffectDisplayType = EDT_RAND;
}

void CMagicHouseView::OnEffectScan()
{
	m_nEffectDisplayType = EDT_SCANDOWN;
}

void CMagicHouseView::OnEffectDscan()
{
	m_nEffectDisplayType = EDT_VSCAN;
}

void CMagicHouseView::OnEffectMove()
{
	m_nEffectDisplayType = EDT_MOVERIGHT;
}

void CMagicHouseView::OnEffectHsmove()
{
	m_nEffectDisplayType = EDT_HSMOVE;
}

void CMagicHouseView::OnEffectVBlind()
{
	m_nEffectDisplayType = EDT_VBLIND;
}

void CMagicHouseView::OnEffectSblind()
{
	m_nEffectDisplayType = EDT_HBLIND;
}

void CMagicHouseView::OnEffectVraster()
{
	m_nEffectDisplayType = EDT_VRASTER;
}

void CMagicHouseView::OnEffectHRaster()
{
	m_nEffectDisplayType = EDT_HRASTER;
}

void CMagicHouseView::OnEffectMosaic()
{
	m_nEffectDisplayType = EDT_MOSAIC;
}

void CMagicHouseView::OnEffectRaindrop()
{
	m_nEffectDisplayType = EDT_RAINDROP;
}

void CMagicHouseView::OnUpdateEffectNone(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_NONE);
}

void CMagicHouseView::OnUpdateEffectRand(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_RAND);
}

void CMagicHouseView::OnUpdateEffectScan(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_SCANDOWN);
}

void CMagicHouseView::OnUpdateEffectDscan(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_VSCAN);
}

void CMagicHouseView::OnUpdateEffectMove(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_MOVERIGHT);
}

void CMagicHouseView::OnUpdateEffectBlind(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_VBLIND);
}

void CMagicHouseView::OnUpdateEffectRaster(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_HRASTER);
}

void CMagicHouseView::OnUpdateEffectMosaic(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_MOSAIC);
}

void CMagicHouseView::OnUpdateEffectHsmove(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_HSMOVE);
}

void CMagicHouseView::OnUpdateEffectSblind(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_HBLIND);
}

void CMagicHouseView::OnUpdateEffectVraster(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_VRASTER);
}

void CMagicHouseView::OnUpdateEffectRaindrop(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nEffectDisplayType == EDT_RAINDROP);
}

void CMagicHouseView::OnModeBrowse()
{
	if (!m_bIsEditMode)
		return;

	FreeImageBuffer();
	m_bIsEditMode = false;
}

void CMagicHouseView::OnModeEdit()
{
	if (m_bIsEditMode)
		return;

	m_bIsEditMode = true;

	if (m_nPos != -1)
	{
		CString strPath = GetFilePath(m_nPos);
		GetImageBuffer(strPath);
	}
}

//--------------------------------------------------------------
//	作用:			将指定路径的图像信息提取到数组中
//	参数:			
//		strPath		图像文件地址
//--------------------------------------------------------------
bool CMagicHouseView::GetImageBuffer(CString& strPath)
{
	CString fileNameExt = strPath.Right(3);
	string filePath = (LPCSTR)CStringA(strPath);

	if (fileNameExt == "jpg" || fileNameExt == "JPG")
	{
		FILE *fp;

		fp = fopen(filePath.c_str(), "rb");

		JpegDecoder dec(fp);
		fclose(fp);

		m_nPicWidth = m_nTempWidth = dec.GetWidth();
		m_nPicHeight = m_nTempHeight = dec.GetHeight();
		dec.GetJPEGBuffer(m_nPicWidth, m_nPicHeight,&m_pImageBuffer);
		int sizeb = m_nPicWidth * m_nPicHeight * 4;

		m_pImageTempBuffer = (BYTE*)malloc(sizeb);
		memcpy(m_pImageTempBuffer, m_pImageBuffer, sizeb);
	}

	if (fileNameExt == "bmp" || fileNameExt == "BMP")
	{
		MyDib* dib = new MyDib(strPath);

		int b = dib->GetBitsPerPixel();

		m_nPicWidth = m_nTempWidth = dib->GetWidth();
		m_nPicHeight = m_nTempHeight = dib->GetHeight();
		int sizeofbuffer = m_nPicWidth * m_nPicHeight * 4;
		m_pImageTempBuffer = (BYTE *)malloc(sizeofbuffer);

		m_pImageBuffer = (BYTE *)malloc(dib->GetBodySize());
		memcpy(m_pImageBuffer, dib->GetBits(), dib->GetBodySize());	

		SAFE_DELETE (dib);

		if ( b != 24 && b != 32 && b != 8 ) 
		{
			free(m_pImageTempBuffer);
			free(m_pImageBuffer);
			MessageBox(L"本程序仅能处理24位与32位彩色图和8位灰度图!");
			return false;
		}

		if (b == 32) 
		{
			for(unsigned int i = 0; i < m_nPicWidth * 4; i += 4) 
			{
				for (unsigned int j = 0; j < m_nPicHeight / 2; j++) 
				{
					swap(m_pImageBuffer[j*m_nPicWidth*4+i], m_pImageBuffer[(m_nPicHeight-j-1)*m_nPicWidth*4+i]);
					swap(m_pImageBuffer[j*m_nPicWidth*4+i+1], m_pImageBuffer[(m_nPicHeight-j-1)*m_nPicWidth*4+i+1]);
					swap(m_pImageBuffer[j*m_nPicWidth*4+i+2], m_pImageBuffer[(m_nPicHeight-j-1)*m_nPicWidth*4+i+2]);
					swap(m_pImageBuffer[j*m_nPicWidth*4+i+3], m_pImageBuffer[(m_nPicHeight-j-1)*m_nPicWidth*4+i+3]);
				}
			}
			memcpy(m_pImageTempBuffer, m_pImageBuffer, sizeofbuffer);
		}

		if (b == 24) 
		{
			int externWidth;
			externWidth = m_nPicWidth * 3;
			if(externWidth % 4 != 0)
				externWidth = 4 - externWidth % 4;
			else
				externWidth = 0;

			int k = 0;

			for (int n = m_nPicHeight - 1; n >= 0; n--) 
			{
				for (UINT m = 0; m < m_nPicWidth * 3; m += 3) 
				{
					m_pImageTempBuffer[k]   = m_pImageBuffer[n*(m_nPicWidth*3+externWidth)+m];	//blue
					m_pImageTempBuffer[k+1] = m_pImageBuffer[n*(m_nPicWidth*3+externWidth)+m+1];//green
					m_pImageTempBuffer[k+2] = m_pImageBuffer[n*(m_nPicWidth*3+externWidth)+m+2];//red
					m_pImageTempBuffer[k+3] = 255;
					k += 4;
				}
			}

			free(m_pImageBuffer);
			m_pImageBuffer = (BYTE *)malloc(sizeofbuffer);
			memcpy(m_pImageBuffer, m_pImageTempBuffer, sizeofbuffer);
		}

		if (b==8) {

			int externWidth;
			externWidth = m_nPicWidth;
			if(externWidth%4!=0)
				externWidth=4-externWidth%4;
			else
				externWidth=0;

			int k = 0;

			for (int n=m_nPicHeight-1; n>=0; n--) {
				for (UINT m=0; m<m_nPicWidth; m+=1) {

					m_pImageTempBuffer[k]  =m_pImageBuffer[n*(m_nPicWidth+externWidth)+m];//blue
					m_pImageTempBuffer[k+1]=m_pImageBuffer[n*(m_nPicWidth+externWidth)+m];//green
					m_pImageTempBuffer[k+2]=m_pImageBuffer[n*(m_nPicWidth+externWidth)+m];//red
					m_pImageTempBuffer[k+3]=0;
					k+=4;
				}
			}

			free(m_pImageBuffer);
			m_pImageBuffer = (BYTE*)malloc(sizeofbuffer);
			memcpy(m_pImageBuffer, m_pImageTempBuffer, sizeofbuffer);
		}
	}

	return true;
}

//--------------------------------------------------------------
//	作用:		释放图像信息占用的内存
//--------------------------------------------------------------
void CMagicHouseView::FreeImageBuffer(void)
{
	if (m_pImageBuffer != NULL)
	{
		free(m_pImageBuffer);
		m_pImageBuffer = NULL;
	}

	if (m_pImageTempBuffer != NULL)
	{
		free(m_pImageTempBuffer);
		m_pImageTempBuffer = NULL;
	}
}

//--------------------------------------------------------------
//	作用:		"保存"的命令相应函数
//--------------------------------------------------------------
void CMagicHouseView::OnFileSave()
{
	if (m_nPos == -1) 
	{
		MessageBox(L"工作区中未加载图片文件!", L"Magic House", MB_ICONWARNING);
		return;
	}
	if (m_pImageTempBuffer == NULL) 
	{
		MessageBox(L"没有可以保存的文件!", L"Magic House", MB_ICONWARNING);
		return;
	}
	CFileDialog SaveDlg( FALSE, L"bmp", L"未命名", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, L"图像文件格式(*.bmp) |*.bmp||", NULL);
	if(SaveDlg.DoModal() == IDOK)
	{
		CString tempname = SaveDlg.GetPathName();
		string savePath = (LPCSTR)CStringA(tempname);
		WriteBufferToBMP(m_pImageTempBuffer, m_nTempWidth, m_nTempHeight, savePath.c_str());
		tempname.ReleaseBuffer();
		MessageBox(L"文件已成功保存!");
	}
}

//--------------------------------------------------------------
//	作用:			将图像信息写入bmp文件中
//	参数:		
//		im_buffer	图像信息
//	    X_bitmap	宽度
//		Y_bitmap	高度
//		BMPname		保存bmp文件路径
//--------------------------------------------------------------
void CMagicHouseView::WriteBufferToBMP(BYTE *im_buffer, WORD X_bitmap, WORD Y_bitmap, const char* BMPname)
{
	SWORD x,y;
	RGB *pixel;
	BM_header BH;
	FILE *fp_bitmap;
	DWORD im_loc_bytes;
	BYTE nr_fillingbytes, i;
	BYTE zero_byte=0;

	fp_bitmap=fopen(BMPname,"wb");
	if (fp_bitmap==NULL) 
	{
			AfxMessageBox(L"错误! 文件无法创建.", MB_ICONWARNING);
			return;
	}

	if (X_bitmap%4!=0) nr_fillingbytes=4-((X_bitmap*3L)%4);
	else nr_fillingbytes=0;

	BH.BMP_id = 'M'*256+'B';     fwrite(&BH.BMP_id,2,1,fp_bitmap);
	BH.size=54+Y_bitmap*(X_bitmap*3+nr_fillingbytes);fwrite(&BH.size,4,1,fp_bitmap);
	BH.zero_res = 0;             fwrite(&BH.zero_res,4,1,fp_bitmap);
	BH.offbits = 54;             fwrite(&BH.offbits,4,1,fp_bitmap);
	BH.biSize = 0x28;            fwrite(&BH.biSize,4,1,fp_bitmap);
	BH.Width = X_bitmap;	      fwrite(&BH.Width,4,1,fp_bitmap);
	BH.Height = Y_bitmap;	      fwrite(&BH.Height,4,1,fp_bitmap);
	BH.biPlanes = 1;             fwrite(&BH.biPlanes,2,1,fp_bitmap);
	BH.biBitCount = 24;          fwrite(&BH.biBitCount,2,1,fp_bitmap);
	BH.biCompression = 0;        fwrite(&BH.biCompression,4,1,fp_bitmap);
	BH.biSizeImage = 0;          fwrite(&BH.biSizeImage,4,1,fp_bitmap);
	BH.biXPelsPerMeter = 0xB40;  fwrite(&BH.biXPelsPerMeter,4,1,fp_bitmap);
	BH.biYPelsPerMeter = 0xB40;  fwrite(&BH.biYPelsPerMeter,4,1,fp_bitmap);
	BH.biClrUsed = 0;	          fwrite(&BH.biClrUsed,4,1,fp_bitmap);
	BH.biClrImportant = 0;	      fwrite(&BH.biClrImportant,4,1,fp_bitmap);

	im_loc_bytes=(DWORD)im_buffer+((DWORD)Y_bitmap-1)*X_bitmap*4;

	for (y=0;y<Y_bitmap;y++)
	{
		for (x=0;x<X_bitmap;x++)
		{
			pixel=(RGB *)im_loc_bytes;
			fwrite(pixel, 3, 1, fp_bitmap);
			im_loc_bytes+=4;
		}
		for (i=0;i<nr_fillingbytes;i++)
			fwrite(&zero_byte,1,1,fp_bitmap);
		im_loc_bytes-=2L*X_bitmap*4;
	}
	fclose(fp_bitmap);
}
void CMagicHouseView::OnUpdateModeBrowse(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!m_bIsEditMode);
}

void CMagicHouseView::OnUpdateModeEdit(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bIsEditMode);
}

void CMagicHouseView::ResetImage()
{
	m_nTempWidth = m_nPicWidth;
	m_nTempHeight = m_nPicHeight;

	int sizeofbuffer = m_nTempWidth * m_nTempHeight * 4;
	SAFE_DELETE (m_pImageTempBuffer);

	m_pImageTempBuffer = new BYTE[sizeofbuffer];
	memcpy(m_pImageTempBuffer, m_pImageBuffer, sizeofbuffer);
}



void CMagicHouseView::OnFilterNegative()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	ResetImage();
	Negative(m_pImageBuffer, m_pImageTempBuffer, m_nPicWidth, m_nPicHeight);

	// 创建预览对话框
	CPreviewDlg dlg;

	dlg.m_nType = 2;	// 对话框在关闭时提示是否保存
	dlg.DoModal();
}
void CMagicHouseView::OnFilterEmboss()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	ResetImage();
	Emboss(m_pImageBuffer, m_pImageTempBuffer, m_nPicWidth, m_nPicHeight);

	// 创建预览对话框
	CPreviewDlg dlg;

	dlg.m_nType = 2;	// 对话框在关闭时提示是否保存
	dlg.DoModal();
}
void CMagicHouseView::OnFilterBw()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	ResetImage();
	ColorToBW(m_pImageBuffer, m_pImageTempBuffer, m_nPicWidth, m_nPicHeight);

	// 创建预览对话框
	CPreviewDlg dlg;

	dlg.m_nType = 2;	// 对话框在关闭时提示是否保存
	dlg.DoModal();
}
void CMagicHouseView::OnFilterFog()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}
	CFogDlg dlg;
	ResetImage();
	dlg.DoModal();
}

void CMagicHouseView::OnFilterMosaic()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	//计算像素数组的长度
	int sum = m_nPicWidth * m_nPicHeight * 4;
	memcpy(m_pImageTempBuffer, m_pImageBuffer, sum);

	//通过degree来控制马赛克化的程度
	unsigned int degree = 3;
	//调用马赛克处理函数对像素数组进行处理
	Mosaic(m_pImageBuffer, m_pImageTempBuffer, m_nPicWidth, m_nPicHeight, degree);

	// 创建预览对话框
	CPreviewDlg dlg;

	dlg.m_nType = 2;	// 对话框在关闭时提示是否保存
	dlg.DoModal();
}

void CMagicHouseView::OnFilterSketch()
{
	if (!m_bIsEditMode || m_nPos == -1)
	{
		MessageBox(L"请先打开图像文件，然后选择编辑模式！");
		return;
	}

	ResetImage();
	LaplacianB(m_pImageBuffer, m_pImageTempBuffer, m_nPicWidth, m_nPicHeight);

	// 创建预览对话框
	CPreviewDlg dlg;

	dlg.m_nType = 2;	// 对话框在关闭时提示是否保存
	dlg.DoModal();
}


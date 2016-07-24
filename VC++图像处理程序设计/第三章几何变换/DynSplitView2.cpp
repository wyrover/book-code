// DynSplitView2.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DynSplitView2.h"
#include "DlgPingYi.h"
#include "DlgSuoFang.h"
#include "math.h"
#include "DlgXuanZhuan.h"
#include "JiHeBianHuanDib.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynSplitView2

IMPLEMENT_DYNCREATE(CDynSplitView2, CView)

CDynSplitView2::CDynSplitView2()
{
	state2=0;
	state3=1;
}

/*初始化图像数据*/
void CDynSplitView2::clearmem()
{
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc ->statedoc=0;
    state2=1;
	CDibNew1=&pDoc->CDibNew;
    CDib1=&pDoc->CDib;
    long int  size;
	if(CDibNew1->m_pBitmapInfoHeader->biBitCount<9)	
		size=CDib1->GetHeight()*CDib1->GetWidth();
	else
		size=CDib1->GetHeight()*CDib1->GetWidth()*3;

    memcpy(CDibNew1->m_pData,CDib1->m_pData,size);
	CDibNew1->SetHeight(CDib1->GetHeight());
    CDibNew1->SetWidth(CDib1->GetWidth());
}

CPalette * CDynSplitView2::CreateBitmapPalette(JiHeBianHuanDib * pBitmap)
{
	struct
	{
		WORD Version;
		WORD NumberOfEntries;
		PALETTEENTRY aEntries[256];
	} palette = { 0x300, 256 };
	
	LPRGBQUAD pRGBTable = pBitmap->GetRGB();
	UINT numberOfColors = pBitmap->GetNumberOfColors();
	
	for(UINT x=0; x<numberOfColors; ++x)
	{
		palette.aEntries[x].peRed =
			pRGBTable[x].rgbRed;
		palette.aEntries[x].peGreen =
			pRGBTable[x].rgbGreen;
		palette.aEntries[x].peBlue =
			pRGBTable[x].rgbBlue;
		palette.aEntries[x].peFlags = 0;
	}
	
	hPalette.CreatePalette((LPLOGPALETTE)&palette);
	return &hPalette;
}

CDynSplitView2::~CDynSplitView2()
{
}

CDSplitDoc* CDynSplitView2::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDSplitDoc)));
	return (CDSplitDoc*)m_pDocument;
}


BEGIN_MESSAGE_MAP(CDynSplitView2, CView)
	//{{AFX_MSG_MAP(CDynSplitView2)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_FILESAVE, OnFilesave)
	ON_COMMAND(ID_Tuxiangpingyi, OnTuxiangpingyi)
	ON_COMMAND(ID_Shuipingjingxiang, OnShuipingjingxiang)
	ON_COMMAND(ID_Chuizhijingxiang, OnChuizhijingxiang)
	ON_COMMAND(ID_Zhuanzhi, OnZhuanzhi)
	ON_COMMAND(ID_Xuanzhuan, OnXuanzhuan)
	ON_COMMAND(ID_Fangda, OnFangda)
	ON_COMMAND(ID_SuoXiao, OnSuoXiao)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynSplitView2 drawing

void CDynSplitView2::OnDraw(CDC* pDC)
{	
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if(!pDoc ->statedoc&&state2==1)
	{
        BYTE* pBitmapData = CDibNew1->GetData();
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
        int bitmapHeight = CDibNew1->GetHeight();
        int bitmapWidth = CDibNew1->GetWidth();
		int scaledWidth = (int)(bitmapWidth * 1);
		int scaledHeight = (int)(bitmapHeight * 1);
		if (CDibNew1->GetRGB()) // Has a color table
		{
			CPalette * hPalette1=CreateBitmapPalette(CDibNew1);
            CPalette * hOldPalette =
                pDC->SelectPalette(hPalette1, true);
            pDC->RealizePalette();
			::StretchDIBits(pDC->GetSafeHdc(),0,0, bitmapWidth, bitmapHeight,
				0, 0, bitmapWidth , bitmapHeight ,
				pBitmapData, pBitmapInfo,
				DIB_RGB_COLORS, SRCCOPY);
            pDC->SelectPalette(hOldPalette, true);
            hPalette.DeleteObject();
			CDibNew1->SetWidth(bitmapWidth);
			CDibNew1->SetHeight(bitmapHeight);
		}
		else
		{
			
            ::StretchDIBits(pDC->GetSafeHdc(),
                0, 0, scaledWidth, scaledHeight,
                0, 0, bitmapWidth, bitmapHeight,
				pBitmapData, pBitmapInfo,
				DIB_RGB_COLORS, SRCCOPY);
			CDibNew1->SetWidth(bitmapWidth);
			CDibNew1->SetHeight(bitmapHeight);
		}
	}
	
    
}


/////////////////////////////////////////////////////////////////////////////
// CDynSplitView2 diagnostics

#ifdef _DEBUG
void CDynSplitView2::AssertValid() const
{
	CView::AssertValid();
}

void CDynSplitView2::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDynSplitView2 message handlers

BOOL CDynSplitView2::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(&rect,::GetSysColor(COLOR_WINDOW));
	return TRUE;
}
 
void CDynSplitView2::OnFilesave() 
{
	CFileDialog dlg(FALSE,_T("BMP"),_T("*.BMP"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("位图文件(*.BMP)|*.BMP|"));	
    if(IDOK==dlg.DoModal())
		CString  filename;
    filename.Format ("%s",dlg.GetPathName() );    
    CDibNew1->SaveFile(filename);
	state2=1;
	Invalidate();
}

/*平移消息映射*/
void CDynSplitView2::OnTuxiangpingyi() 
{
	int x,y;
	if(state3==0)
		CDibNew1->clearmem2(); 
	MessageBox("正数表示右移量或下移量，负数表示左移量或上移量");
	//初始化图像数据
	clearmem();
	// 创建对话框
	CDlgPingYi  dlg;
	dlg.m_Xmove=10;
	dlg.m_Ymove=10;

	// 显示对话框，提示用户设定窗口上下限
	if (dlg.DoModal() != IDOK)
		return;// 返回

	x=dlg.m_Xmove;
	y=dlg.m_Ymove;
	
	// 删除对话框
	delete dlg;	
	CDibNew1->PingYi(x,y); 
	Invalidate();           //调用刷新函数
	state3=1;
}

/*水平镜像消息映射*/
void CDynSplitView2::OnShuipingjingxiang() 
{
	clearmem();
	CDibNew1->JingXiang(TRUE);             //调用水平镜像处理函数
    Invalidate();           //调用刷新函数
}

/*垂直镜像消息映射*/
void CDynSplitView2::OnChuizhijingxiang() 
{
	clearmem();
	CDibNew1->JingXiang(FALSE);             //调用垂直镜像处理函数
    Invalidate();           //调用刷新函数
}

/*转置消息映射*/
void CDynSplitView2::OnZhuanzhi() 
{
	clearmem();	
	CDibNew1->Zhuanzhi();             //调用转置处理函数
    Invalidate(); //调用刷新函数
    state3=0;
}
 



void CDynSplitView2::OnXuanzhuan() 
{
	MessageBox("角度应为90度的整数倍，且图像的高和宽需相同");
	clearmem();
	
	int iRotateAngle;			// 旋转角度
	CDlgXuanZhuan dlg;			// 创建对话框
	dlg.m_iRotateAngle = 90;	// 初始化变量值
	
	// 显示对话框，提示用户设定旋转角度
	if (dlg.DoModal() != IDOK)
	{
		return;// 返回
	}
	
	// 获取用户设定的旋转量
	iRotateAngle = dlg.m_iRotateAngle;
	CDibNew1->Xuanzhuan(iRotateAngle);
	this->Invalidate();
}

//图像放大消息映射
void CDynSplitView2::OnFangda() 
{
	if(state3==0)
		CDibNew1->clearmem2(); 
	clearmem();	
	float xzoom,yzoom;
	CDlgSuoFang  dlg;		// 定义对话框
	dlg.m_XZoom=2.0;
	dlg.m_YZoom=2.0;

	// 显示对话框，提示用户设定窗口上下限
	if (dlg.DoModal() != IDOK)
	{
		return;				// 返回
	}
    xzoom=dlg.m_XZoom;
    yzoom=dlg.m_YZoom;
	delete dlg;				// 删除对话框

	// 源图像的宽度和高度
	LONG	wide;
	LONG	height;
	
	// 缩放后图像的宽度和高度
	LONG	newwide;
	LONG	newheight;
	LONG	newLineBytes;
	LPBYTE	temp;
	
	// 获取图像的宽度和高度
	wide = CDibNew1->GetWidth();
	height = CDibNew1->GetHeight();

	if(CDibNew1->m_pBitmapInfoHeader->biBitCount<9)
	{
		// 计算缩放后的图像宽度和高度
		newwide = (LONG) (wide * xzoom +0.5);
		newheight = (LONG) (height * yzoom+0.5);
		newLineBytes=(newwide*8+31)/32*4;

		// 分配内存，以保存新DIB
		temp = new BYTE[ newLineBytes * newheight ];
		memset(temp,0,newLineBytes * newheight);
		CDibNew1->m_pData=CDibNew1->FangDa(temp,xzoom,yzoom,wide,height,newLineBytes,newheight);  
		CDibNew1->SetWidth(newwide);
		CDibNew1->SetHeight(newheight);
		Invalidate();           //调用刷新函数
	}
	else
	{
		// 计算缩放后的图像宽度和高度
		newwide = (LONG) (wide * xzoom*3);
		newheight = (LONG) (height * yzoom);
		
		// 分配内存，以保存新DIB
		temp = new BYTE[ newwide * newheight];
		memset(temp,0,newwide * newheight);
		CDibNew1->m_pData=CDibNew1->FangDa(temp,xzoom,yzoom,wide,height,newwide,newheight); 
		CDibNew1->SetWidth(newwide/3);
		CDibNew1->SetHeight(newheight);
		Invalidate();           //调用刷新函数
	}
}

//图像缩小消息映射
void CDynSplitView2::OnSuoXiao() 
{
	if(state3==0)
		CDibNew1->clearmem2(); 

	float x,y;					//缩小倍数
	clearmem();	
	CDlgSuoFang  dlg;			//创建对话框
	dlg.m_XZoom=0.5;
	dlg.m_YZoom=0.5;

	// 显示对话框，提示用户设定窗口上下限
	if (dlg.DoModal() != IDOK)
	{
		return;					//返回
	}
	x=dlg.m_XZoom;
	y=dlg.m_YZoom;

	delete dlg;					//删除对话框	 
	CDibNew1->SuoXiao( x, y );	//调用缩小处理函数
    Invalidate();				//调用刷新函数
	state3=1;
}

void CDynSplitView2::clearmem(float xzoom, float yzoom)
{
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->statedoc=0;
    state2=1;
	CDibNew1=&pDoc->CDibNew;
    CDib1=&pDoc->CDib;
	long int  size=CDib1->GetHeight()*CDib1->GetDibWidthBytes()*xzoom;
	memcpy(CDibNew1->m_pData,CDib1->m_pData,size);
}

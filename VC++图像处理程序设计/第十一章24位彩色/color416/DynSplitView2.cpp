// DynSplitView2.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DynSplitView2.h"
#include "LightDlg.h"
#include "ontrastDlg.h"
#include "PaintColorDlg.h"
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
   	m_byRed = m_byGreen = m_byBlue = 255;
	m_GaoTong=0;
}

void CDynSplitView2::clearmem()
{
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->statedoc=0;
    state2=1;
	CDibNew1=&pDoc->CDibNew;
    CDib1=&pDoc->CDib;
long int  size=CDib1->GetHeight()*CDib1->GetDibWidthBytes();

 // for(int i=0;i<CDib1->GetHeight();i++)
//		for(int j=0;j<CDib1->GetDibWidthBytes();j++)
//		{
//			*(CDibNew1->m_pData)++=*(CDib1->m_pData)++;
//		}
memcpy(CDibNew1->m_pData,CDib1->m_pData,size);

//memset(CDibNew1->m_pData,255,CDibNew1->GetWidth()*CDibNew1->GetHeight());

}

CPalette * CDynSplitView2::CreateBitmapPalette(MakeColorDib * pBitmap)
{
		struct
		{
			WORD Version;
			WORD NumberOfEntries;
			PALETTEENTRY aEntries[256];
		} palette = { 0x300, 256 };
		
		LPRGBQUAD pRGBTable = pBitmap->GetRGB();
		UINT numberOfColors = pBitmap->GetNumberOfColors();

	if(numberOfColors!=24)
	{
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
	}
		
			hPalette.CreatePalette((LPLOGPALETTE)&palette);
		//CPalette hPale=&hPalette;
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
	ON_COMMAND(ID_MakegGray, OnMakegGray)
	ON_COMMAND(ID_LightAlter, OnLightAlter)
	ON_COMMAND(ID_LightReverse, OnLightReverse)
	ON_COMMAND(ID_ContrastAlter, OnContrastAlter)
	ON_COMMAND(ID_Exposal, OnExposal)
	ON_COMMAND(ID_PaintColor, OnPaintColor)
	ON_COMMAND(ID_Smoothness, OnSmoothness)
	ON_COMMAND(ID_NeonLight, OnNeonLight)
	ON_COMMAND(ID_Embossment, OnEmbossment)
	ON_COMMAND(ID_Spread, OnSpread)
	ON_COMMAND(ID_Sharp, OnSharp)
	ON_COMMAND(ID_High_LVBO, OnHighLVBO)
	ON_COMMAND(ID_Low_LVBO, OnLowLVBO)
	ON_COMMAND(ID_ShuiPing_GROW, OnShuiPingGROW)
	ON_COMMAND(ID_ChuiZhi_GROW, OnChuiZhiGROW)
	ON_COMMAND(ID_ShuangXiang_GROW, OnShuangXiangGROW)
	ON_COMMAND(ID_High_LVBOnormal, OnHighLVBOnormal)
	ON_COMMAND(ID_High_LVBOexcessive, OnHighLVBOexcessive)
	ON_COMMAND(ID_Low_LVBObig, OnLowLVBObig)
	ON_COMMAND(ID_Mosaic, OnMosaic)
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
 	    int m_scale=1;
        BYTE* pBitmapData = CDibNew1->GetData();
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
        int bitmapHeight = CDibNew1->GetHeight();
        int bitmapWidth = CDibNew1->GetWidth();
		int scaledWidth = (int)(bitmapWidth * m_scale);
		int scaledHeight = (int)(bitmapHeight * m_scale);
		if (CDibNew1->GetRGB()) // Has a color table
		{
	  		CPalette * hPalette=CreateBitmapPalette(CDibNew1);
            CPalette * hOldPalette =
                pDC->SelectPalette(hPalette, true);
            pDC->RealizePalette();
			::StretchDIBits(pDC->GetSafeHdc(),0,0,scaledWidth,scaledHeight,
               0,0,bitmapWidth,bitmapHeight,
				pBitmapData,pBitmapInfo,
				DIB_RGB_COLORS, SRCCOPY);
            pDC->SelectPalette(hOldPalette, true);
            ::DeleteObject(hPalette);
		}
		else
		{
            StretchDIBits(pDC->GetSafeHdc(),
                0, 0, scaledWidth, scaledHeight,
                0, 0, bitmapWidth, bitmapHeight,
				pBitmapData, pBitmapInfo,
				DIB_RGB_COLORS, SRCCOPY);
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


void CDynSplitView2::OnMakegGray()   //灰度调整 
{
	clearmem();   //取得原始图像的拷贝文件
	CDibNew1->MakegGray();   //调用灰度调整函数
 	Invalidate();   //调用刷新函数
}

void CDynSplitView2::OnLightAlter()   //亮度调整
{
	LightDlg dlg;        // 创建对话框
	dlg.m_Light=0;   //提示用户设定窗口亮度调整值
	dlg.DoModal();      // 显示对话框
    clearmem();     //取得原始图像的拷贝文件
	CDibNew1->LightAlter(dlg.m_Light);  //调用亮度调整函数
	Invalidate();	   //调用刷新函数	
}

void CDynSplitView2::OnLightReverse()  //亮度取反
{
    clearmem();   //取得原始图像的拷贝文件
	CDibNew1->LightReverse();     //调用亮度取反函数
	Invalidate();   //调用刷新函数
}

void CDynSplitView2::OnContrastAlter()   //对比度调整
{
	ContrastDlg dlg;   // 创建对话框
	dlg.m_Increment=0;  //设置初始值
	dlg.DoModal();   // 显示对话框
    clearmem();       //取得原始图像的拷贝文件
	CDibNew1->ContrastAlter(dlg.m_Increment);  //取到输入值，调用对比度调整函数
	Invalidate();		   //调用刷新函数	
}

void CDynSplitView2::OnExposal()   //图像曝光
{
    clearmem();   //取得原始图像的拷贝文件
	CDibNew1->Exposal();   //调用图像曝光函数
	Invalidate();	   //调用刷新函数
}

void CDynSplitView2::OnPaintColor()  //图像着色
{
	PaintColorDlg Dlg;    // 创建对话框
	Dlg.m_Red = m_byRed;       //设置初始值
	Dlg.m_Green = m_byGreen;     //设置初始值
	Dlg.m_Blue = m_byBlue;     //设置初始值
	int responeDlg = Dlg.DoModal();
	if(responeDlg == IDOK)      // 显示对话框
	{
	 m_byRed=Dlg.m_Red;    //取到窗口输入值
	 m_byGreen=Dlg.m_Green;  //取到窗口输入值
	 m_byBlue=Dlg.m_Blue ;	  //取到窗口输入值
	}
    clearmem();   //取得原始图像的拷贝文件
	CDibNew1->PaintColor(m_byRed,m_byGreen,m_byBlue);  //调用图像着色函数
	Invalidate();		   //调用刷新函数	
}

void CDynSplitView2::OnSmoothness()  //图像平滑
{
    clearmem();   //取得原始图像的拷贝文件
	CDibNew1->Smoothness();    //调用图像平滑函数
	Invalidate();		   //调用刷新函数
}

void CDynSplitView2::OnNeonLight() //图像霓虹
{
    clearmem();   //取得原始图像的拷贝文件
	CDibNew1->NeonLight();   //调用图像霓虹函数
	Invalidate();		   //调用刷新函数	
}

void CDynSplitView2::OnEmbossment()   //图像浮雕
{
    clearmem();   //取得原始图像的拷贝文件
	CDibNew1->Embossment();   //调用图像浮雕函数
	Invalidate();		   //调用刷新函数	
}

void CDynSplitView2::OnSpread() //图像扩散
{
    clearmem();   //取得原始图像的拷贝文件
	CDibNew1->Spread();  //调用图像扩散函数
	Invalidate();	   //调用刷新函数
}

void CDynSplitView2::OnSharp() //图像锐化
{
    clearmem();   //取得原始图像的拷贝文件
	CDibNew1->Sharp();  //调用图像锐化函数
	Invalidate();	   //调用刷新函数	
}

void CDynSplitView2::OnHighLVBO() ///高通滤波(基本)
{  
	m_GaoTong=1;   //设置标志
    clearmem();   //取得原始图像的拷贝文件
	CDibNew1->HighLVBO(m_GaoTong);  //调用高通滤波(基本)函数
	Invalidate();		   //调用刷新函数
}
void CDynSplitView2::OnHighLVBOnormal() //高通滤波（中等）
{
	m_GaoTong=2;  //设置标志
    clearmem();   //取得原始图像的拷贝文件
	CDibNew1->HighLVBO(m_GaoTong);  //调用高通滤波（中等）函数
	Invalidate();	   //调用刷新函数
}

void CDynSplitView2::OnHighLVBOexcessive() ///高通滤波（过量）
{ 
	m_GaoTong=3;    //设置标志
    clearmem();   //取得原始图像的拷贝文件
	CDibNew1->HighLVBO(m_GaoTong);  //调用高通滤波（过量）函数
	Invalidate();	   //调用刷新函数
}

void CDynSplitView2::OnLowLVBO() ///低通滤波(3x3)
{
    clearmem();   //取得原始图像的拷贝文件
	CDibNew1->LowLVBO();  //调用低通滤波(3x3)函数
	Invalidate();		   //调用刷新函数
}

void CDynSplitView2::OnLowLVBObig()   //低通滤波(5x5)
{
    clearmem();   //取得原始图像的拷贝文件
	CDibNew1->LowLVBObig();   //调用低通滤波(5x5)函数
	Invalidate();		   //调用刷新函数
}

void CDynSplitView2::OnShuiPingGROW() ///水平增强
{
    clearmem();   //取得原始图像的拷贝文件
	CDibNew1->ShuiPingGROW();  //调用水平增强函数
	Invalidate();	   //调用刷新函数
}

void CDynSplitView2::OnChuiZhiGROW() ///垂直增强
{
    clearmem();   //取得原始图像的拷贝文件
	CDibNew1->ChuiZhiGROW();   //调用垂直增强函数
	Invalidate();	   //调用刷新函数
}

void CDynSplitView2::OnShuangXiangGROW() ///双向增强
{
    clearmem();   //取得原始图像的拷贝文件
	CDibNew1->ShuangXiangGROW();  //调用双向增强函数
	Invalidate();		   //调用刷新函数
}

void CDynSplitView2::OnMosaic() // 马赛克
{
    clearmem();   //取得原始图像的拷贝文件
	CDibNew1->Mosaic();  //调用马赛克处理函数
	Invalidate();    //调用刷新函数
}

// DynSplitView2.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DynSplitView2.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynSplitView2

IMPLEMENT_DYNCREATE(CDynSplitView2, CView)

CDynSplitView2::CDynSplitView2()
{state2=0;
for(int i=0;i<8;i++)
start[i]=0;
}


/*初始化图像数据*/
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

CPalette * CDynSplitView2::CreateBitmapPalette(FuShiYuPengZhangDib * pBitmap)
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
	ON_COMMAND(ID_Chuizhifushi, OnChuizhifushi)
	ON_COMMAND(ID_Shuipingfushi, OnShuipingfushi)
	ON_COMMAND(ID_Quanfangwei, OnQuanfangwei)
	ON_COMMAND(ID_Shuipingpengzhang, OnShuipingpengzhang)
	ON_COMMAND(ID_Chuizhipengzhang, OnChuizhipengzhang)
	ON_COMMAND(ID_Quanfangxiangpengzhang, OnQuanfangxiangpengzhang)
	ON_COMMAND(ID_Kaiqi, OnKaiqi)
	ON_COMMAND(ID_Bihe, OnBihe)
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
	// TODO: Add your command handler code here
	CFileDialog dlg(FALSE,_T("BMP"),_T("*.BMP"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("位图文件(*.BMP)|*.BMP|"));	
    if(IDOK==dlg.DoModal())
	CString  filename;
    filename.Format ("%s",dlg.GetPathName() );    
    CDibNew1->SaveFile(filename);
	state2=1;
	Invalidate();

}



/*垂直腐蚀消息映射*/
void CDynSplitView2::OnChuizhifushi() 
{
	// TODO: Add your command handler code here
	clearmem();
	CDibNew1->ChuiZhiFuShi();    //调用垂直腐蚀函数
 	Invalidate();
}


/*水平腐蚀消息映射*/
void CDynSplitView2::OnShuipingfushi() 
{

//	 TODO: Add your command handler code here
	 clearmem();
	CDibNew1->Shuipingfushi();             //调用水平腐蚀函数
 	Invalidate();
}


/*全方向腐蚀消息映射*/
void CDynSplitView2::OnQuanfangwei() 
{
	// TODO: Add your command handler code here
	clearmem();
	CDibNew1->Quanfangxiangfushi();             //调用全方位腐蚀函数
 	Invalidate();
}


/*水平膨胀消息映射*/
void CDynSplitView2::OnShuipingpengzhang() 
{
	// TODO: Add your command handler code here
    clearmem();
	CDibNew1->Shuipingpengzhang();             //调用水平膨胀函数
 	Invalidate();
}


/*垂直膨胀消息映射*/
void CDynSplitView2::OnChuizhipengzhang() 
{
	 //TODO: Add your command handler code here
	
	//初始化图像数据
	 clearmem();
	CDibNew1->Chuizhipengzhang();             //调用垂直膨胀函数
 	Invalidate();
}


/*全方向膨胀消息映射*/
void CDynSplitView2::OnQuanfangxiangpengzhang() 
{
	// TODO: Add your command handler code here
	clearmem();
	CDibNew1->Quanfangxiangpengzhang();             //调用全方向膨胀函数
 	Invalidate();
}



/*开运算消息映射*/
void CDynSplitView2::OnKaiqi() 
{
	// TODO: Add your command handler code here
    clearmem();
    CDibNew1->Quanfangxiangfushi(); 
	CDibNew1->Quanfangxiangpengzhang();             //调用全方向开启函数
 	Invalidate();
}



/*闭运算消映射*/
void CDynSplitView2::OnBihe() 
{
	// TODO: Add your command handler code here
    clearmem();
	CDibNew1->Quanfangxiangpengzhang(); 
    CDibNew1->Quanfangxiangfushi();         //调用全方向开启函数
 	Invalidate();
}



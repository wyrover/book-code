// DynSplitView2.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DynSplitView2.h"
#include "XiaoChuHeiDian.h"
#include "THREETHREEJUNZHI.h"
#include "NNJUNZHI.h"
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
}

void CDynSplitView2::clearmem()
{
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc ->statedoc=0;
    state2=1;
	CDibNew1=&pDoc->CDibNew;
    CDib1=&pDoc->CDib;
	long int  size=CDib1->GetHeight()*CDib1->GetDibWidthBytes();
	memcpy(CDibNew1->m_pData,CDib1->m_pData,size);

}

CPalette * CDynSplitView2::CreateBitmapPalette(ZaoShengXiaoChuDib * pBitmap)
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
	ON_COMMAND(ID_XIAOCHUHEIDIAN, OnXiaochuheidian)
	ON_COMMAND(ID_33JUNZHI, On33junzhi)
	ON_COMMAND(ID_NNJUNZHI, OnNnjunzhi)
	ON_COMMAND(ID_JUBUPINGJUN, OnJubupingjun)
	ON_COMMAND(ID_NNZHONGZHI, OnNnzhongzhi)
	ON_COMMAND(ID_SHIZI, OnShizi)
	ON_COMMAND(ID_NNZUIDA, OnNnzuida)
	ON_COMMAND(ID_SUIJIZAOSHENG, OnSuijizaosheng)
	ON_COMMAND(ID_JIAOYANZAOSHENG, OnJiaoyanzaosheng)
	ON_COMMAND(ID_FANZHUAN, OnFanzhuan)
	ON_COMMAND(ID_CHAOXIAN, OnChaoxian)
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

void CDynSplitView2::OnFanzhuan() //二值图像的黑白点噪
{
	clearmem();
 	CDibNew1->HeiBaiFanZhuan();//二值图像的黑白点噪
	Invalidate();	
}
void CDynSplitView2::OnXiaochuheidian() 
{
	clearmem();
    XiaoChuHeiDian dlg;
    if(IDOK==dlg.DoModal())
	{
		int connec=dlg.n;
	    CDibNew1->black(connec);//消除图像的孤立黑点
		Invalidate();	
	}
}
void CDynSplitView2::On33junzhi() 
{
	clearmem();
 	CDibNew1->threethree();//对图像进行3*3均值滤波
	Invalidate();	

}

void CDynSplitView2::OnChaoxian() 
{
	clearmem();
    THREETHREEJUNZHI dlg;
    if(IDOK==dlg.DoModal())
	{
		int t=dlg.m_T;
	    CDibNew1->Chaoxian(t);//超限邻域平均法
		Invalidate();	
	}	
}

void CDynSplitView2::OnNnjunzhi() 
{
	clearmem();
    NNJUNZHI dlg;
    if(IDOK==dlg.DoModal())
    {
		CDibNew1->nn(dlg.m_n);//对图像进行n*n均值滤波
		Invalidate();	
	}
}

void CDynSplitView2::OnJubupingjun() 
{
	clearmem();
    CDibNew1->jubupingjun();//对图像进行有选择局部平均化
	Invalidate();	
}

void CDynSplitView2::OnNnzhongzhi() 
{
	clearmem();
    NNJUNZHI dlg;
    if(IDOK==dlg.DoModal())
    {
		CDibNew1->nnzhong(dlg.m_n);//对图像进行N*N中值滤波
		Invalidate();	
	}
}

void CDynSplitView2::OnShizi() 
{
	clearmem();
    NNJUNZHI dlg;
    if(IDOK==dlg.DoModal())
    {
		CDibNew1->shizi(dlg.m_n);//对图像进行十字型中值滤波
		Invalidate();	
	}
}

void CDynSplitView2::OnNnzuida() 
{
	clearmem();
    NNJUNZHI dlg;
    if(IDOK==dlg.DoModal())
    {
		CDibNew1->nnzuida(dlg.m_n);//对图像进行N*N最大滤波
		Invalidate();	
	}
}

void CDynSplitView2::OnSuijizaosheng() 
{
	clearmem();
    CDibNew1->suijizaosheng();//对图像产生随机噪声
	Invalidate();	
}

void CDynSplitView2::OnJiaoyanzaosheng() 
{
	clearmem();
    CDibNew1->jiaoyanzaosheng();//对图像产生椒盐噪声
	Invalidate();	
}


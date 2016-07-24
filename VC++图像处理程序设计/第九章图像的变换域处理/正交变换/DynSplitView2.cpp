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
}


//8位彩色图像初始化
void CDynSplitView2::clearmem()
{
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc ->statedoc=0;
    state2=1;
	CDibNew1=&pDoc->CDibNew;
    CDib1=&pDoc->CDib;
long int  size=CDib1->GetHeight()*CDib1->GetWidth();
memcpy(CDibNew1->m_pData,CDib1->m_pData,size);
//memset(CDibNew1->m_pData,0,CDibNew1->GetWidth()*CDibNew1->GetHeight());

}

CPalette * CDynSplitView2::CreateBitmapPalette(ZhengJiaoBianHuanDib * pBitmap)
	{
		struct
		{
			WORD Version;
			WORD NumberOfEntries;
			PALETTEENTRY aEntries[256];
		} palette = { 0x300, 256 };
		
		RGBQUAD buf[256];
		for (int k=0;k<256;k++)
			
		{
			buf[k].rgbBlue=k;
			buf[k].rgbGreen=k;
            buf[k].rgbRed=k;
			buf[k].rgbReserved=0;
			
		}
	
		UINT numberOfColors = 256;
		
		for(UINT x=0; x<numberOfColors; ++x)
		{
			palette.aEntries[x].peRed =buf[x].rgbBlue;
			     
			palette.aEntries[x].peGreen =buf[x].rgbGreen;
				
			palette.aEntries[x].peBlue =buf[x].rgbRed;
			
			palette.aEntries[x].peFlags = buf[x].rgbReserved;
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
	ON_COMMAND(ID_QuickForuier, OnQuickForuier)
	ON_COMMAND(ID_LISANYUXUAN, OnLisanyuxuan)
	ON_COMMAND(ID_WALSH, OnWalsh)
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
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
	  RGBQUAD* m_pRGB=CDibNew1->GetRGB();
	   BYTE* pBitmapData;
		if(CDibNew1->byBitCount==8) //灰度图像
		  pBitmapData = CDibNew1->GetData();
        else//24位真彩色
		{
			pBitmapData = CDibNew1->GetData2();
	      pBitmapInfo->bmiHeader.biBitCount=8;
		 }
        int bitmapHeight = CDibNew1->GetHeight();
        int bitmapWidth = CDibNew1->GetWidth();

		pBitmapInfo->bmiHeader.biSizeImage=bitmapHeight*bitmapWidth;
	    unsigned char *pBit;
		char buf[256][3];
		
		for (int k=0;k<256;k++)
			for(int kk=0;kk<3;kk++)
		{
			buf[k][kk]=k;
			
		}
		LPBITMAPINFO p_bihed;
		p_bihed=(LPBITMAPINFO)malloc(sizeof(BITMAPINFO)+256*3+1024);
		memcpy(p_bihed,pBitmapInfo,sizeof(BITMAPINFO));
		pBit=(unsigned char*)p_bihed+sizeof(BITMAPINFO);
		m_pRGB = (RGBQUAD*)pBit;
		memcpy(pBit,buf,256*3);
		p_bihed->bmiHeader.biBitCount=8;
		p_bihed->bmiHeader.biSizeImage=bitmapHeight*bitmapWidth;
		

		pBitmapInfo=p_bihed;
//		if (CDibNew1->GetRGB()) // Has a color table
		{
			CPalette * hPalette=CreateBitmapPalette(CDibNew1);
            CPalette * hOldPalette =
                pDC->SelectPalette(hPalette, true);
            pDC->RealizePalette();
			::StretchDIBits(pDC->GetSafeHdc(),0, 0, bitmapWidth, bitmapHeight,
               0, 0, bitmapWidth, bitmapHeight,
				pBitmapData, pBitmapInfo,
				DIB_RGB_COLORS, SRCCOPY);
            pDC->SelectPalette(hOldPalette, true);
            ::DeleteObject(hPalette);
		}
			if(CDibNew1->byBitCount==24) 
		pBitmapInfo->bmiHeader.biBitCount=24;
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

void CDynSplitView2::OnQuickForuier() //对图像进行傅立业变换
{
	// TODO: Add your command handler code here
    clearmem();
	if(CDibNew1->byBitCount==24) 
		 clearmem2();
	CDibNew1->QuickFourier();
	Invalidate();	
}

void CDynSplitView2::OnLisanyuxuan() //对图像进行离散余弦变换
{
	// TODO: Add your command handler code here
    clearmem();
		
	// 图像离散余弦变换
	
	// 指向DIB象素指针
	LPBYTE    lpDIBBits;
	
	lpDIBBits = CDibNew1->GetData();
	// 找到DIB图像象素起始位置
    if(CDibNew1->byBitCount==24) //24位真彩色
	{
		clearmem2();
	lpDIBBits = CDibNew1->GetData2();
    }
	long width=CDibNew1->GetWidth();
	long height=CDibNew1->GetHeight();
	
	// 调用DIBDct()函数进行离散余弦变换
	if (CDibNew1->DIBLiSanYuXuan(lpDIBBits, width, height))
	{
		// 更新视图
	    Invalidate();
	}

}

void CDynSplitView2::OnWalsh() //对图像进行沃尔什变换
{
	// TODO: Add your command handler code here
    clearmem();
		
	// 图像离散余弦变换
	
	// 指向DIB象素指针
	LPBYTE    lpDIBBits;
	lpDIBBits = CDibNew1->GetData();
	
	// 找到DIB图像象素起始位置
	if(CDibNew1->byBitCount==24) //24位真彩色
	{	 clearmem2();

	lpDIBBits = CDibNew1->GetData2();
	}
	long width=CDibNew1->GetWidth();
	long height=CDibNew1->GetHeight();
	
	// 调用DIBDct()函数进行离散余弦变换
	if (CDibNew1->DIBWalsh(lpDIBBits, width, height))
	{
		// 更新视图
	    Invalidate();
	}
	
}


//24位彩色图像初始化
void CDynSplitView2::clearmem2()
{
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc ->statedoc=0;
    state2=1;
	CDibNew1=&pDoc->CDibNew;
    CDib1=&pDoc->CDib;
	long int  size=CDib1->GetHeight()*CDib1->GetWidth();
	memcpy(CDibNew1->m_pData1,CDib1->m_pData1,size);
}

// DynSplitView2.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DynSplitView2.h"
#include "PinYuLuBoDib.h"
#include "Dialog1.h"
#include "Dialog2.h"
#include "Dialog3.h"
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

//8位图像初始化
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

CPalette * CDynSplitView2::CreateBitmapPalette(PinYuLuBoDib * pBitmap)
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
	ON_COMMAND(ID_BUTTERWORTH_L, OnButterworthL)
	ON_COMMAND(ID_BUTTERWORTH_H, OnButterworthH)
	ON_COMMAND(ID_QuickForuier, OnQuickForuier)
	ON_COMMAND(ID_BOX, OnBox)
	ON_COMMAND(ID_PERFECTL, OnPerfectl)
	ON_COMMAND(ID_PERFECTH, OnPerfecth)
	ON_COMMAND(ID_TL, OnTl)
	ON_COMMAND(ID_Th, OnTh)
	ON_COMMAND(ID_ZL, OnZl)
	ON_COMMAND(ID_ZH, OnZh)
	ON_COMMAND(ID_FirstQuickForuier, OnFirstQuickForuier)
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
		if(CDibNew1->byBitCount==8) 
		  pBitmapData = CDibNew1->GetData();
        else
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
		
	//	if (CDibNew1->GetRGB()) // Has a color table
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

//低通布特波斯滤波消息函数
void CDynSplitView2::OnButterworthL() 
{
	// TODO: Add your command handler code here
	clearmem();
	if(CDibNew1->byBitCount==24) 
		 clearmem2();
    CDialog1 dlg;
	if(IDOK==dlg.DoModal())
    {CDibNew1->BWFilterL(dlg.m_x,dlg.m_y,dlg.m_n);
	Invalidate();
	}
	
}


//高通布特波斯滤波消息函数
void CDynSplitView2::OnButterworthH() 
{
	// TODO: Add your command handler code here
      clearmem();
	if(CDibNew1->byBitCount==24) 
		 clearmem2();
    CDialog1 dlg;
	if(IDOK==dlg.DoModal())
    {CDibNew1->BWFilterH(dlg.m_x,dlg.m_y,dlg.m_n);
	Invalidate();	
	}
}


//快速傅立叶消息函数
void CDynSplitView2::OnQuickForuier() 
{
	// TODO: Add your command handler code here
       clearmem();
	if(CDibNew1->byBitCount==24) 
		 clearmem2();
	
	CDibNew1->QuickFourier();
	Invalidate();

}

void CDynSplitView2::OnBox() 
{
	// TODO: Add your command handler code here
  AfxMessageBox("打开的图像宽高尺寸要符合如下数据2、4、8、16、32、64、128、256");
}


//理想低通滤波消息函数
void CDynSplitView2::OnPerfectl() 
{
	// TODO: Add your command handler code here
	  clearmem();
	if(CDibNew1->byBitCount==24) 
		 clearmem2();
    CDialog2 dlg;
	if(IDOK==dlg.DoModal())
    {CDibNew1->PerfectFilterL(dlg.m_x,dlg.m_y);
	Invalidate();	
	}	
}


//理想高通滤波消息函数
void CDynSplitView2::OnPerfecth() 
{
	// TODO: Add your command handler code here
	    clearmem();
	if(CDibNew1->byBitCount==24) 
		 clearmem2();
    CDialog2 dlg;
	if(IDOK==dlg.DoModal())
    {CDibNew1->PerfectFilterH(dlg.m_x,dlg.m_y);
	Invalidate();
	}
}

//梯形低通滤波消息函数
void CDynSplitView2::OnTl() 
{
	// TODO: Add your command handler code here
	   clearmem();
	if(CDibNew1->byBitCount==24) 
		 clearmem2();
    Dialog3 dlg;
	if(IDOK==dlg.DoModal())
    {CDibNew1->TLFilter(dlg.m_x,dlg.m_y,dlg.m_x1,dlg.m_y1);
	Invalidate();
	}
}


//梯形高通滤波消息函数
void CDynSplitView2::OnTh() 
{
	// TODO: Add your command handler code here
	   clearmem();
	if(CDibNew1->byBitCount==24) 
		 clearmem2();
    Dialog3 dlg;
	if(IDOK==dlg.DoModal())
    {CDibNew1->THFilter(dlg.m_x,dlg.m_y,dlg.m_x1,dlg.m_y1);
	Invalidate();
	}	
}

//指数低通滤波消息函数
void CDynSplitView2::OnZl() 
{
	// TODO: Add your command handler code here
	   clearmem();
	if(CDibNew1->byBitCount==24) 
		 clearmem2();
    CDialog1 dlg;
	if(IDOK==dlg.DoModal())
    {CDibNew1->ZLFilter(dlg.m_x,dlg.m_y,dlg.m_n);
	Invalidate();	
	}
}

//指数高通滤波消息函数
void CDynSplitView2::OnZh() 
{
	// TODO: Add your command handler code here
	  clearmem();
	if(CDibNew1->byBitCount==24) 
		 clearmem2();
    CDialog1 dlg;
	if(IDOK==dlg.DoModal())
    {CDibNew1->ZHFilter(dlg.m_x,dlg.m_y,dlg.m_n);
	Invalidate();	
	}	
}


//未平移的傅立叶消息函数
void CDynSplitView2::OnFirstQuickForuier() 
{
	// TODO: Add your command handler code here
	   
	
		clearmem();
	if(CDibNew1->byBitCount==24) 
		 clearmem2();
	CDibNew1->FirstQuickFourier();
	Invalidate();
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

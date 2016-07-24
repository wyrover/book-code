// DynSplitView2.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DynSplitView2.h"
#include "Num.h"

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
n=0;
}


/*初始化图像数据*/
void CDynSplitView2::clearmem()
{
/*	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc ->statedoc=0;
    state2=1;

	CDibNew1=&pDoc->CDibNew;
    CDib1=&pDoc->CDib;
    long int  size=CDib1->GetHeight()*CDib1->GetWidth();
     memcpy(CDibNew1->m_pData,CDib1->m_pData,size);//复制原图像到处理区
*/
}

CPalette * CDynSplitView2::CreateBitmapPalette(CWvltTransDib * pBitmap)
{
	struct
	{
		WORD Version;
		WORD NumberOfEntries;
		PALETTEENTRY aEntries[256];
	} palette = { 0x300, 256 };
  
	for(UINT x=0; x<256; ++x)
	{
		palette.aEntries[x].peRed =x;
		
		palette.aEntries[x].peGreen =x;
		
		palette.aEntries[x].peBlue =x;
			
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
	ON_COMMAND(ID_HANGBIANHUAN, OnHangbianhuan)
	ON_COMMAND(ID_LIEBIANHUAN, OnLiebianhuan)
	ON_COMMAND(ID_ONCE, OnOnce)
	ON_COMMAND(ID_TWICE, OnTwice)
	ON_COMMAND(ID_THRICE, OnThrice)
	ON_COMMAND(ID_LOWFILTER, OnLowfilter)
	ON_COMMAND(ID_IDWT, OnIdwt)
	ON_COMMAND(ID_HIGHFILTER, OnHighfilter)
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
		}
		else
		{
			
            ::StretchDIBits(pDC->GetSafeHdc(),
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
/*	CFileDialog dlg(FALSE,_T("BMP"),_T("*.BMP"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("位图文件(*.BMP)|*.BMP|"));	
    if(IDOK==dlg.DoModal())
	CString  filename;
    filename.Format ("%s",dlg.GetPathName() );    
    CDibNew1->SaveFile(filename);
	state2=1;
	Invalidate();
*/
}

void CDynSplitView2::clearmem2()
{
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc ->statedoc=0;
    state2=1;

	CDibNew1=&pDoc->CDibNew;
    CDib1=&pDoc->CDib;
    long int  size=CDib1->GetHeight()*CDib1->GetWidth();
   memcpy(CDibNew1->m_pData2,CDib1->m_pData2,size);//复制原图像到处理区

}

void CDynSplitView2::OnHangbianhuan() 
{
	clearmem2();
	CDibNew1->Hangbianhuan();
	CDibNew1->GradetoRGB();

	Invalidate();
}

void CDynSplitView2::OnLiebianhuan() 
{
	clearmem2();
	CDibNew1->Liebianhuan();
	CDibNew1->GradetoRGB();
	Invalidate();	
}

void CDynSplitView2::OnOnce() 
{
	clearmem2();
	n=1;
	CDibNew1->Once(n);
	CDibNew1->GradetoRGB();
	Invalidate();		
}

void CDynSplitView2::OnTwice() 
{
	clearmem2();
	n=2;
    CDibNew1->Once(n);
	CDibNew1->GradetoRGB();
	Invalidate();	
}

void CDynSplitView2::OnThrice() 
{
	clearmem2();
	n=3;
    CDibNew1->Once(n);
	CDibNew1->GradetoRGB();
	Invalidate();
}

void CDynSplitView2::OnLowfilter() 
{
	clearmem2();
	CNum dlg;
	dlg.m_Num=1;
	if(IDOK==dlg.DoModal())
		n=dlg.m_Num ;
	CDibNew1->LowFilter(n);
	CDibNew1->GradetoRGB();
	Invalidate();
	

}

void CDynSplitView2::OnIdwt() 
{

	CDibNew1->IDWT(n);
	CDibNew1->GradetoRGB();
	Invalidate();
}

void CDynSplitView2::OnHighfilter() 
{
	clearmem2();
	CNum dlg;
	dlg.m_Num=1;
	if(IDOK==dlg.DoModal())
		n=dlg.m_Num ;
	CDibNew1->HighFilter(n);
	CDibNew1->GradetoRGB();
	Invalidate();	
}

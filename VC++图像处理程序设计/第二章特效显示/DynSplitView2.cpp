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
{
	state2=0;
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
	long int  size=CDib1->GetHeight()*CDib1->GetWidth();
	memcpy(CDibNew1->m_pData,CDib1->m_pData,size);//复制原图像到处理区
	

}

CPalette * CDynSplitView2::CreateBitmapPalette(TeXiaoXianShiDib * pBitmap)
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
	ON_COMMAND(ID_Xiangxia, OnXiangxia)
	ON_COMMAND(ID_Xiangshang, OnXiangshang)
	ON_COMMAND(ID_Xiangyou, OnXiangyou)
	ON_COMMAND(ID_Xiangzuo, OnXiangzuo)
	ON_COMMAND(ID_Shuiping, OnShuiping)
	ON_COMMAND(ID_Chuizhi, OnChuizhi)
	ON_COMMAND(ID_Masaike, OnMasaike)
	ON_COMMAND(ID_Shuipingshantiao, OnShuipingshantiao)
	ON_COMMAND(ID_Chuizhishantiao, OnChuizhishantiao)
	ON_COMMAND(ID_Jianxian, OnJianxian)
	ON_COMMAND(ID_Youyi, OnYouyi)
	ON_COMMAND(ID_ZhongJianKuoZhang, OnZhongJianKuoZhang)
	ON_COMMAND(ID_Shangyi, OnShangyi)
	ON_COMMAND(ID_JiaoChaFeiRu, OnJiaoChaFeiRu)
	ON_COMMAND(ID_ZhongJianShouSuo, OnZhongJianShouSuo)
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
	// TODO: Add your command handler code here
	CFileDialog dlg(FALSE,_T("BMP"),_T("*.BMP"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("位图文件(*.BMP)|*.BMP|"));	
    if(IDOK==dlg.DoModal())
		CString  filename;
    filename.Format ("%s",dlg.GetPathName() );    
    CDibNew1->SaveFile(filename);
	state2=1;
	Invalidate();
}

/*向下扫描特效显示*/
void CDynSplitView2::OnXiangxia() 
{	 
	//刷新屏幕
	CDC *pDC=GetDC();
	CRect rect(0,0,1000,1000);
	CBrush brush(RGB(255,255,255));		
	pDC->FillRect(&rect,&brush);
	//复制图像数据
	clearmem();	 
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc ->statedoc&&state2==1)
	{
        BYTE* pBitmapData = CDibNew1->GetData();
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
        int bitmapHeight = CDibNew1->GetHeight();
        int bitmapWidth = CDibNew1->GetWidth();
		if (CDibNew1->GetRGB()) // Has a color table
		{
			CPalette * hPalette=CreateBitmapPalette(CDibNew1);
            CPalette * hOldPalette =
                pDC->SelectPalette(hPalette, true);
            pDC->RealizePalette();
			for(int j=0;j<bitmapHeight;j++)
			{
				::StretchDIBits(pDC->GetSafeHdc(),0, j, bitmapWidth, 1,
					0, bitmapHeight-j, bitmapWidth, 1,
					pBitmapData, pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY);
				Sleep(5);
			}
            pDC->SelectPalette(hOldPalette, true);
            ::DeleteObject(hPalette);
		}
		else
		{
			for(int j=0;j<bitmapHeight;j++)
			{
					::StretchDIBits(pDC->GetSafeHdc(),0, j, bitmapWidth, 1,
					0, bitmapHeight-j, bitmapWidth, 1,
					pBitmapData, pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY);
				Sleep(5);
			}
		}
	}
	//Invalidate(); 	
}


/*向上扫描特效显示*/
void CDynSplitView2::OnXiangshang() 
{
	//刷新屏幕
	CDC *pDC=GetDC();
	CRect rect(0,0,1000,1000);
	CBrush brush(RGB(255,255,255));	
	pDC->FillRect(&rect,&brush);
	//复制图像数据
	clearmem();
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	//向上扫描处理
	if(!pDoc ->statedoc&&state2==1)
	{
        BYTE* pBitmapData = CDibNew1->GetData();
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
        int bitmapHeight = CDibNew1->GetHeight();
        int bitmapWidth = CDibNew1->GetWidth();
		if (CDibNew1->GetRGB()) // Has a color table
		{
			CPalette * hPalette=CreateBitmapPalette(CDibNew1);
            CPalette * hOldPalette =
                pDC->SelectPalette(hPalette, true);
            pDC->RealizePalette();
			for(int j=0;j<bitmapHeight;j++)
			{
				::StretchDIBits(pDC->GetSafeHdc(),0, bitmapHeight-j, bitmapWidth, 1,
					0,  j, bitmapWidth, 1,
					pBitmapData, pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY);
				Sleep(5);
			}
            pDC->SelectPalette(hOldPalette, true);
            ::DeleteObject(hPalette);
		}
		else
		{
			for(int j=0;j<bitmapHeight;j++)
			{
				::StretchDIBits(pDC->GetSafeHdc(),0, bitmapHeight-j, bitmapWidth, 1,
					0,  j, bitmapWidth, 1,
					pBitmapData, pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY);
				Sleep(5);
			}
		}
	}
    Invalidate();
}

/*向右扫描特效显示*/
void CDynSplitView2::OnXiangyou() 
{
	//刷新屏幕
	CDC *pDC=GetDC();
	CRect rect(0,0,1000,1000);
	CBrush brush(RGB(255,255,255));
	pDC->FillRect(&rect,&brush);
    //复制图像数据
	clearmem();
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc ->statedoc&&state2==1)
	{
        BYTE* pBitmapData = CDibNew1->GetData();
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
        int bitmapHeight = CDibNew1->GetHeight();
        int bitmapWidth = CDibNew1->GetWidth();
		if (CDibNew1->GetRGB()) // Has a color table
		{
			CPalette * hPalette=CreateBitmapPalette(CDibNew1);
            CPalette * hOldPalette =
                pDC->SelectPalette(hPalette, true);
            pDC->RealizePalette();
			for(int i=0;i<bitmapWidth;i++)
			{
				::StretchDIBits(pDC->GetSafeHdc(),   i, 0, 1, bitmapHeight,
					i,  0, 1, bitmapHeight,
					pBitmapData, pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY);
				Sleep(5);
			}
            pDC->SelectPalette(hOldPalette, true);
            ::DeleteObject(hPalette);
		}
		else
		{
			for(int i=0;i<bitmapWidth;i++)
			{
				::StretchDIBits(pDC->GetSafeHdc(),   i, 0, 1, bitmapHeight,
					i,  0, 1, bitmapHeight,
					pBitmapData, pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY);
				Sleep(5);
			}
		}
	}
	Invalidate();
}

/*向左扫描特效显示*/
void CDynSplitView2::OnXiangzuo() 
{
	//刷新屏幕
	CDC *pDC=GetDC();
	CRect rect(0,0,1000,1000);
	CBrush brush(RGB(255,255,255));	
	pDC->FillRect(&rect,&brush);
    //复制图像数据
	clearmem();
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc ->statedoc&&state2==1)
	{
        BYTE* pBitmapData = CDibNew1->GetData();
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
        int bitmapHeight = CDibNew1->GetHeight();
        int bitmapWidth = CDibNew1->GetWidth();
		if (CDibNew1->GetRGB()) // Has a color table
		{
			CPalette * hPalette=CreateBitmapPalette(CDibNew1);
            CPalette * hOldPalette =
                pDC->SelectPalette(hPalette, true);
            pDC->RealizePalette();
			for(int i=0;i<bitmapWidth;i++)
			{
				::StretchDIBits(pDC->GetSafeHdc(),  bitmapWidth-i, 0, 1, bitmapHeight,
					bitmapWidth-i,  0, 1, bitmapHeight,
					pBitmapData, pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY);
				Sleep(5);
			}
            pDC->SelectPalette(hOldPalette, true);
            ::DeleteObject(hPalette);			
		}
		else
		{
			for(int i=0;i<bitmapWidth;i++)
			{
				::StretchDIBits(pDC->GetSafeHdc(),  bitmapWidth-i, 0, 1, bitmapHeight,
					bitmapWidth-i,  0, 1, bitmapHeight,
					pBitmapData, pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY);
				Sleep(5);
			}
		}
	}
	Invalidate();
}

/*水平百叶窗特效显示*/
void CDynSplitView2::OnShuiping() 
{
	//刷新屏幕
	CDC *pDC=GetDC();
	CRect rect(0,0,1000,1000);
	CBrush brush(RGB(255,255,255));	
	pDC->FillRect(&rect,&brush);
    //复制图像数据
	clearmem();
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	//进行水平百叶窗特效处理
	if(!pDoc ->statedoc&&state2==1)
	{
        BYTE* pBitmapData = CDibNew1->GetData();
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
        int bitmapHeight = CDibNew1->GetHeight();
        int bitmapWidth = CDibNew1->GetWidth();
		if (CDibNew1->GetRGB()) // Has a color table
		{
			CPalette * hPalette=CreateBitmapPalette(CDibNew1);
            CPalette * hOldPalette =
                pDC->SelectPalette(hPalette, true);
            pDC->RealizePalette();
			for(int i=0;i<=bitmapWidth/10;i++)
				for(int j=0;j<=10;j++)
				{
					::StretchDIBits(pDC->GetSafeHdc(),j*bitmapWidth/10, 0, i, bitmapHeight,
						j*bitmapWidth/10, 0, i, bitmapHeight,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY);
					Sleep(15);
				}
			pDC->SelectPalette(hOldPalette, true);
			::DeleteObject(hPalette);
		}
		else
		{
			for(int i=0;i<=bitmapWidth/10;i++)
				for(int j=0;j<=10;j++)
				{
					::StretchDIBits(pDC->GetSafeHdc(),j*bitmapWidth/10, 0, i, bitmapHeight,
						j*bitmapWidth/10, 0, i, bitmapHeight,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY);
					Sleep(15);
				}
		}
		Invalidate();
	}
}

/*垂直百叶窗特效*/
void CDynSplitView2::OnChuizhi() 
{
	//刷新屏幕    
	CDC *pDC=GetDC();
	CRect rect(0,0,1000,1000);
	CBrush brush(RGB(255,255,255));		
	pDC->FillRect(&rect,&brush);
	//复制图像数据
	clearmem();
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	//进行特效处理
	if(!pDoc ->statedoc&&state2==1)
	{
        BYTE* pBitmapData = CDibNew1->GetData();
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
        int bitmapHeight = CDibNew1->GetHeight();
        int bitmapWidth = CDibNew1->GetWidth();
		if (CDibNew1->GetRGB()) // Has a color table
		{
			CPalette * hPalette=CreateBitmapPalette(CDibNew1);
            CPalette * hOldPalette =
                pDC->SelectPalette(hPalette, true);
            pDC->RealizePalette();
			for(int j=0;j<=bitmapHeight/10;j++)
				for(int i=0;i<=10;i++)
				{
					::StretchDIBits(pDC->GetSafeHdc(),0, i*bitmapHeight/10, bitmapWidth,j,
						0, (9-i)*bitmapHeight/10, bitmapWidth,j,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY);
					Sleep(10);
				}
			pDC->SelectPalette(hOldPalette, true);
			::DeleteObject(hPalette);
		}
		else
		{
			for(int j=0;j<=bitmapHeight/10;j++)
				for(int i=0;i<=10;i++)
				{
					::StretchDIBits(pDC->GetSafeHdc(),0, i*bitmapHeight/10, bitmapWidth,j,
						0, (9-i)*bitmapHeight/10, bitmapWidth,j,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY);
					Sleep(10);
				}
		}
		Invalidate();
	}
}

/*马赛克特效显示*/
void CDynSplitView2::OnMasaike() 
{
	//刷新屏幕
	CDC *pDC=GetDC();
	CRect rect(0,0,1000,1000);
	CBrush brush(RGB(255,255,255));	
	pDC->FillRect(&rect,&brush);
    //复制图像数据
	clearmem();
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	long mx=0;
	long my=0;
	int nscanline=8;  //设置显示定位量
	int k=0;
	if(!pDoc ->statedoc&&state2==1)
	{
        BYTE* pBitmapData = CDibNew1->GetData();
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
        int bitmapHeight = CDibNew1->GetHeight();
        int bitmapWidth = CDibNew1->GetWidth();
		int nrw=0; 
		int nrh=0;
		if((bitmapWidth%12)!=0)nrw=1;
		if((bitmapHeight%12)!=0)nrh=1;
		long ltilenum=(bitmapWidth/12+nrw)*(bitmapHeight/12+nrh);
		POINT* point=new POINT[ltilenum];
		if (CDibNew1->GetRGB()) // Has a color table
		{
			CPalette * hPalette=CreateBitmapPalette(CDibNew1);
            CPalette * hOldPalette =
				pDC->SelectPalette(hPalette, true);
            pDC->RealizePalette();
			for(int mi=0;mi<ltilenum;mi++)
			{
				point[mi].x=mx;
				point[mi].y=my;
				mx=mx+12;
				if(mx>bitmapWidth)
				{mx=0;my=my+12;}
			}
			for(int i=0;i<=1;i++)
				for(mi=ltilenum-1;mi>0;mi--)
				{
					int n=(int)((double)ltilenum*rand()/RAND_MAX);
					mx=point[n].x;my=point[n].y;
					::StretchDIBits(pDC->GetSafeHdc(),mx, my, 12,12,
						mx,bitmapHeight-my-12, 12,12,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY);
					point[n].x=point[mi].x;
					point[n].y=point[mi].y;
					Sleep(1);
				}
			pDC->SelectPalette(hOldPalette, true);
			::DeleteObject(hPalette);
			delete point;	 
		}
		else
		{
			for(int mi=0;mi<ltilenum;mi++)
			{
				point[mi].x=mx;
				point[mi].y=my;
				mx=mx+12;
				if(mx>bitmapWidth)
				{mx=0;my=my+12;}
			}
			for(int i=0;i<=1;i++)
				for(mi=ltilenum-1;mi>0;mi--)
				{
					int n=(int)((double)ltilenum*rand()/RAND_MAX);
					mx=point[n].x;my=point[n].y;
					::StretchDIBits(pDC->GetSafeHdc(),mx, my, 12,12,
						mx,bitmapHeight-my-12, 12,12,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY);
					point[n].x=point[mi].x;
					point[n].y=point[mi].y;
					Sleep(1);
				}
			delete point;
		}
	}
	Invalidate();
}

/*水平栅条特效显示*/
void CDynSplitView2::OnShuipingshantiao() 
{
	//刷新屏幕
	CDC *pDC=GetDC();
	CRect rect(0,0,1000,1000);
	CBrush brush(RGB(255,255,255));	
	pDC->FillRect(&rect,&brush);
    //复制图像数据
	clearmem();
	int nscanline=8;  //设定显示定位量
	int k=0;
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    //进行水平栅条特效处理
	if(!pDoc ->statedoc&&state2==1)
	{
        BYTE* pBitmapData = CDibNew1->GetData();
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
        int bitmapHeight = CDibNew1->GetHeight();
        int bitmapWidth = CDibNew1->GetWidth();
		if (CDibNew1->GetRGB()) // Has a color table
		{
			CPalette * hPalette=CreateBitmapPalette(CDibNew1);
            CPalette * hOldPalette =
                pDC->SelectPalette(hPalette, true);
            pDC->RealizePalette();
			for(int i=0;i<=bitmapWidth;i++)
			{
				for(int j=0;j<=bitmapHeight;j=j+2*nscanline)
				{
					::StretchDIBits(pDC->GetSafeHdc(),0,  j  ,  i+1, nscanline,
						bitmapWidth-i, bitmapHeight-j, i+1, nscanline,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY);
					k=j+nscanline ;
					::StretchDIBits(pDC->GetSafeHdc(), bitmapWidth-i, k ,  i+1,  nscanline,
						0,bitmapHeight-k, i+1, nscanline,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY);
				}
			}
            pDC->SelectPalette(hOldPalette, true);
            ::DeleteObject(hPalette);
		}
		else
		{
			for(int i=0;i<=bitmapWidth;i++)
			{
				for(int j=0;j<=bitmapHeight;j=j+2*nscanline)
				{
					::StretchDIBits(pDC->GetSafeHdc(),0,  j  ,  i+1, nscanline,
						bitmapWidth-i, bitmapHeight-j, i+1, nscanline,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY);
					k=j+nscanline ;
					::StretchDIBits(pDC->GetSafeHdc(), bitmapWidth-i, k ,  i+1,  nscanline,
						0,bitmapHeight-k, i+1, nscanline,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY);
				}
			}
		}
	}
	Invalidate();
}

/*垂直栅条特效显示*/
void CDynSplitView2::OnChuizhishantiao() 
{
	//刷新屏幕
	CDC *pDC=GetDC();
	CRect rect(0,0,1000,1000);
	CBrush brush(RGB(255,255,255));		
	pDC->FillRect(&rect,&brush);
	//复制图像数据 
	clearmem();
	int nscanline=8;  //设置图像显示定位量
	int k=0;
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    //进行垂直栅条特效处理
	if(!pDoc ->statedoc&&state2==1)
	{
        BYTE* pBitmapData = CDibNew1->GetData();
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
        int bitmapHeight = CDibNew1->GetHeight();
        int bitmapWidth = CDibNew1->GetWidth();
		if (CDibNew1->GetRGB()) // Has a color table
		{
			CPalette * hPalette=CreateBitmapPalette(CDibNew1);
            CPalette * hOldPalette =
                pDC->SelectPalette(hPalette, true);
            pDC->RealizePalette();
			for(int j=0;j<=bitmapHeight;j++)
			{
				for(int i=0;i<=bitmapWidth;i=i+2*nscanline)
				{
					::StretchDIBits(pDC->GetSafeHdc(),i,  0 ,  nscanline, j+1,
						i, bitmapHeight-j, nscanline, j+1,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY);
					k=i+nscanline;
					::StretchDIBits(pDC->GetSafeHdc(), k, bitmapHeight-j ,  nscanline,  j+1,
						k,0, nscanline, j+1,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY);
				}
			}
            pDC->SelectPalette(hOldPalette, true);
            ::DeleteObject(hPalette);
		}
		else
		{
			for(int j=0;j<=bitmapHeight;j++)
			{
				for(int i=0;i<=bitmapWidth;i=i+2*nscanline)
				{
					::StretchDIBits(pDC->GetSafeHdc(),i,  0 ,  nscanline, j+1,
						i, bitmapHeight-j, nscanline, j+1,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY);
					k=i+nscanline;
					::StretchDIBits(pDC->GetSafeHdc(), k, bitmapHeight-j ,  nscanline,  j+1,
						k,0, nscanline, j+1,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY);
				}
			}
		}
	}
	Invalidate();   
}

/*渐显特效显示*/
void CDynSplitView2::OnJianxian() 
{
	//刷新屏幕
	CDC *pDC=GetDC();
	CRect rect(0,0,1000,1000);
	CBrush brush(RGB(255,255,255));		
	pDC->FillRect(&rect,&brush);
	//复制图像数据
	clearmem();
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc ->statedoc&&state2==1 )
	{
		BYTE* pBitmapData = CDibNew1->GetData();
		LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
		int bitmapHeight= CDibNew1->GetHeight();
		int bitmapWidth = CDibNew1->GetWidth();
		if (CDibNew1->GetRGB()) // Has a color table
		{
			CPalette * hPalette=CreateBitmapPalette(CDibNew1);
			CPalette * hOldPalette = pDC->SelectPalette(hPalette, true);
			pDC->RealizePalette();
			LPBYTE temp,temp1,temp2;
			temp=new BYTE[CDibNew1->GetHeight()*CDibNew1->GetWidth()];
			memset (temp,0,CDibNew1->GetHeight()*CDibNew1->GetWidth());
			for(int n=0;n<=256;n++)
			{
				temp1=temp;
				temp2=pBitmapData;
				for(int j = 0; j < bitmapHeight; j++)
				{
					for(int i = 0; i < bitmapWidth; i ++)
					{
						*temp1=(*temp2)*(n)/256;
						temp1++;
						temp2++;
					}
				}
				::StretchDIBits(pDC->GetSafeHdc(),0, 0, bitmapWidth, bitmapHeight,
					0, 0, bitmapWidth, bitmapHeight,
					temp,pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY);
				Sleep(10);	
			}
			pDC->SelectPalette(hOldPalette, true);
			::DeleteObject(hPalette);
			delete temp;
		} 
		else
		{
			LPBYTE temp;
			temp=new BYTE[CDibNew1->GetHeight()*CDibNew1->GetWidth()*3];
			memset (temp,0,CDibNew1->GetHeight()*CDibNew1->GetWidth()*3);
			for(int m=0;m<256;m++)
			{
				for(int j = 0; j < bitmapHeight; j++)
				{
					for(int i = 0; i < bitmapWidth; i ++)
					{
						temp[j*bitmapWidth*3+i*3]=pBitmapData[j*bitmapWidth*3+i*3]*(m)/256;
						temp[j*bitmapWidth*3+i*3+1]=pBitmapData[j*bitmapWidth*3+i*3+1]*(m)/256;
						temp[j*bitmapWidth*3+i*3+2]=pBitmapData[j*bitmapWidth*3+i*3+2]*(m)/256;
					}
				}
				::StretchDIBits(pDC->GetSafeHdc(),
					0, 0, bitmapWidth, bitmapHeight,
					0, 0, bitmapWidth, bitmapHeight,
					temp, pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY);
				Sleep(20);
			}
			delete temp;
		}
	}
}

/*水平右移特效显示*/
void CDynSplitView2::OnYouyi() 
{
	//刷新屏幕
	CDC *pDC=GetDC();
	CRect rect(0,0,1000,1000);
	CBrush brush(RGB(255,255,255));	
	pDC->FillRect(&rect,&brush);
	//复制图像数据
	clearmem();
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	//进行右移处理
	if(!pDoc ->statedoc&&state2==1)
	{
        BYTE* pBitmapData = CDibNew1->GetData();
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
        int bitmapHeight = CDibNew1->GetHeight();
        int bitmapWidth = CDibNew1->GetWidth();
		if (CDibNew1->GetRGB()) // Has a color table
		{
			CPalette * hPalette=CreateBitmapPalette(CDibNew1);
            CPalette * hOldPalette =
                pDC->SelectPalette(hPalette, true);
            pDC->RealizePalette();
			for(int i=0;i<=bitmapWidth;i++)
			{
				for(int j=0;j<=bitmapHeight;j=j+8)
				{
					::StretchDIBits(pDC->GetSafeHdc(),0,  j-8 ,  i+1, 8,
						bitmapWidth-i, bitmapHeight-j, i+1, 8,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY); 	 
				}
			}
            pDC->SelectPalette(hOldPalette, true);
            ::DeleteObject(hPalette);
		}
		else
		{
			for(int i=0;i<=bitmapWidth;i++)
			{
				for(int j=0;j<=bitmapHeight;j=j+8)
				{
					::StretchDIBits(pDC->GetSafeHdc(),0,  j-8 ,  i+1, 8,
						bitmapWidth-i, bitmapHeight-j, i+1, 8,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY); 	 
				}
			}
		}
	}
	Invalidate();
}

/*中间扩张特效显示*/
void CDynSplitView2::OnZhongJianKuoZhang() 
{
	//刷新屏幕
	CDC *pDC=GetDC();
	CRect rect(0,0,1000,1000);
	CBrush brush(RGB(255,255,255));		
	pDC->FillRect(&rect,&brush);
	//复制图像数据
	clearmem(); 
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    //进行中间扩张处理
	if(!pDoc ->statedoc&&state2==1)
	{	     
        BYTE* pBitmapData = CDibNew1->GetData();
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
        int bitmapHeight = CDibNew1->GetHeight();
        int bitmapWidth = CDibNew1->GetWidth();
		if (CDibNew1->GetRGB()) // Has a color table
		{
			CPalette * hPalette=CreateBitmapPalette(CDibNew1);
            CPalette * hOldPalette =
				pDC->SelectPalette(hPalette, true);
            pDC->RealizePalette();
			for(int i=0;i<=bitmapWidth/2;i++)
			{		
				::StretchDIBits(pDC->GetSafeHdc(),0 ,  bitmapHeight/2+i  , bitmapWidth, 1   ,
					0  , bitmapHeight/2-i , bitmapWidth,1 ,
					pBitmapData, pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY); 	
				::StretchDIBits(pDC->GetSafeHdc(),0 ,  bitmapHeight/2-i  , bitmapWidth, 1   ,
					0  , bitmapHeight/2+i , bitmapWidth,1 ,
					pBitmapData, pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY); 
				Sleep(5);
			}
            pDC->SelectPalette(hOldPalette, true);
            ::DeleteObject(hPalette);
		}
		else
		{
			for(int i=0;i<=bitmapWidth/2;i++)
			{		
				::StretchDIBits(pDC->GetSafeHdc(),0 ,  bitmapHeight/2+i  , bitmapWidth, 1   ,
					0  , bitmapHeight/2-i , bitmapWidth,1 ,
					pBitmapData, pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY); 	
				::StretchDIBits(pDC->GetSafeHdc(),0 ,  bitmapHeight/2-i  , bitmapWidth, 1   ,
					0  , bitmapHeight/2+i , bitmapWidth,1 ,
					pBitmapData, pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY); 
				Sleep(5);
			}
		}
	}
	Invalidate();
}


/*图像上移特效显示*/
void CDynSplitView2::OnShangyi() 
{
	// 刷新屏幕
	CDC *pDC=GetDC();
	CRect rect(0,0,1000,1000);
	CBrush brush(RGB(255,255,255));		
	pDC->FillRect(&rect,&brush); 
	//复制图像数据
	clearmem();
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	//进行上移处理
	if(!pDoc ->statedoc&&state2==1)
	{
		BYTE* pBitmapData = CDibNew1->GetData();
		LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
		int bitmapHeight = CDibNew1->GetHeight();
		int bitmapWidth = CDibNew1->GetWidth();
		if (CDibNew1->GetRGB()) // Has a color table
		{
			CPalette * hPalette=CreateBitmapPalette(CDibNew1);
			CPalette * hOldPalette =
				pDC->SelectPalette(hPalette, true);
			pDC->RealizePalette();
			for(int i=0;i<=bitmapHeight;i++)
			{
				for(int j=0;j<=bitmapWidth;j=j++)
				{
					::StretchDIBits(pDC->GetSafeHdc(), j ,  bitmapHeight-i , 1, i  ,
						j , bitmapHeight-i , 1, i ,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY); 
				}
				Sleep(5);
			}
			pDC->SelectPalette(hOldPalette, true);
			::DeleteObject(hPalette);
		}
		else
		{
			for(int i=0;i<=bitmapHeight;i++)
			{
				for(int j=0;j<=bitmapWidth;j=j++)
				{
					::StretchDIBits(pDC->GetSafeHdc(), j ,  bitmapHeight-i , 1, i  ,
						j , bitmapHeight-i , 1, i ,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY); 
				}
				Sleep(5);
			}
		}
	}
	Invalidate();
}

/*交叉飞入特效显示*/
void CDynSplitView2::OnJiaoChaFeiRu() 
{
	//刷新屏幕
	CDC *pDC=GetDC();
	CRect rect(0,0,1000,1000);
	CBrush brush(RGB(255,255,255));		
	pDC->FillRect(&rect,&brush);
	//复制图像数据
	clearmem();
	int nscanline=8;//设置显示定位量
	int k=0;
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc ->statedoc&&state2==1)
	{
        BYTE* pBitmapData = CDibNew1->GetData();
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
        int bitmapHeight = CDibNew1->GetHeight();
        int bitmapWidth = CDibNew1->GetWidth();
		if (CDibNew1->GetRGB()) // 获得调色板
		{
			CPalette * hPalette=CreateBitmapPalette(CDibNew1);
            CPalette * hOldPalette =
				pDC->SelectPalette(hPalette, true);
            pDC->RealizePalette();
			for(int i=0;i<=bitmapWidth;i++)
			{
				for(int j=0;j<=bitmapHeight/2;j=j+nscanline)
				{
					::StretchDIBits(pDC->GetSafeHdc(),0,  j ,  i+1,nscanline,
						bitmapWidth-i, bitmapHeight-j-nscanline, i+1,nscanline,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY);
					k=j+nscanline+bitmapHeight/2;
					::StretchDIBits(pDC->GetSafeHdc(), bitmapWidth-i, k ,  i+1,  nscanline,
						0,bitmapHeight-k, i+1, nscanline,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY); 
				}
				Sleep(5 );
			}
            pDC->SelectPalette(hOldPalette, true);
            ::DeleteObject(hPalette);
		}
		else
		{
			for(int i=0;i<=bitmapWidth;i++)
			{
				for(int j=0;j<=bitmapHeight/2;j=j+nscanline)
				{
					::StretchDIBits(pDC->GetSafeHdc(),0,  j ,  i+1,nscanline,
						bitmapWidth-i, bitmapHeight-j-nscanline, i+1,nscanline,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY);
					k=j+nscanline+bitmapHeight/2;
					::StretchDIBits(pDC->GetSafeHdc(), bitmapWidth-i, k ,  i+1,  nscanline,
						0,bitmapHeight-k, i+1, nscanline,
						pBitmapData, pBitmapInfo,
						DIB_RGB_COLORS, SRCCOPY); 
				}
				Sleep(5 );
			}
		}
	}
	Invalidate();
}


/*中间收缩特效显示*/
void CDynSplitView2::OnZhongJianShouSuo() 
{
	//刷新屏幕
	CDC *pDC=GetDC();
	CRect rect(0,0,1000,1000);
	CBrush brush(RGB(255,255,255));		
	pDC->FillRect(&rect,&brush);
	//复制图像数据
	clearmem();	 
	CDSplitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc ->statedoc&&state2==1)
	{
        BYTE* pBitmapData = CDibNew1->GetData();
        LPBITMAPINFO pBitmapInfo = CDibNew1->GetInfo();
        int bitmapHeight = CDibNew1->GetHeight();
        int bitmapWidth = CDibNew1->GetWidth();
		if (CDibNew1->GetRGB()) // Has a color table
		{
			CPalette * hPalette=CreateBitmapPalette(CDibNew1);
            CPalette * hOldPalette =
                pDC->SelectPalette(hPalette, true);
            pDC->RealizePalette();
			for(int j=0;j<bitmapHeight/2;j++)
			{
				::StretchDIBits(pDC->GetSafeHdc(),0, j, bitmapWidth, 1,
					0, bitmapHeight-j, bitmapWidth, 1,
					pBitmapData, pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY);
				::StretchDIBits(pDC->GetSafeHdc(),0, bitmapHeight-j, bitmapWidth, 1,
					0,  j, bitmapWidth, 1,
					pBitmapData, pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY);
				Sleep(5);
			}
            pDC->SelectPalette(hOldPalette, true);
            ::DeleteObject(hPalette);			
		}
		else
		{
			for(int j=0;j<bitmapHeight/2;j++)
			{
				::StretchDIBits(pDC->GetSafeHdc(),0, j, bitmapWidth, 1,
					0, bitmapHeight-j, bitmapWidth, 1,
					pBitmapData, pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY);
				::StretchDIBits(pDC->GetSafeHdc(),0, bitmapHeight-j, bitmapWidth, 1,
					0,  j, bitmapWidth, 1,
					pBitmapData, pBitmapInfo,
					DIB_RGB_COLORS, SRCCOPY);
				Sleep(5);
			}
		}
	}
	Invalidate(); 	
}



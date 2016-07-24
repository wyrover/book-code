// DynSplitView2.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DynSplitView2.h"
#include "DlgYuZhiFenGe.h"
#include "DlgZhiFangTu.h"
 
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
	state=0;
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
    long int  size=CDib1->GetHeight()*CDib1->GetDibWidthBytes();
	memcpy(CDibNew1->m_pData,CDib1->m_pData,size);//复制原图像到处理区
}

CPalette * CDynSplitView2::CreateBitmapPalette(BingXingBianJieDib * pBitmap)
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
	ON_COMMAND(ID_Diedaifazhi, OnDiedaifazhi)
	ON_COMMAND(ID_LunKuoTiQu, OnLunKuoTiQu)
	ON_COMMAND(ID_Lunkuogenzong, OnLunkuogenzong)
	ON_COMMAND(ID_Zhongzitianchong, OnZhongzitianchong)
	ON_COMMAND(ID_Yuzhifenge, OnYuzhifenge)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_Quyushengzhang, OnQuyushengzhang)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_BanYuZhi, OnBanYuZhi)
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
			::StretchDIBits(pDC->GetSafeHdc(),0, 0, bitmapWidth, bitmapHeight,
				0, 0, bitmapWidth, bitmapHeight,
				pBitmapData, pBitmapInfo,
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

/*迭代阀值消息映射*/
void CDynSplitView2::OnDiedaifazhi() 
{
	clearmem();
	// 各颜色分量的灰度分布密度
	int tongji[256];
	//初始为0
	memset(tongji, 0, sizeof(tongji));
    CDibNew1->Fenbutongji(tongji);      //统计直方图灰度值
	CDibNew1->Diedaifazhi(tongji);             //调用迭代法值检测处理函数
	Invalidate();
}

/*轮廓提取消息映射*/
void CDynSplitView2::OnLunKuoTiQu() 
{
	clearmem();
	if(CDibNew1->m_pBitmapInfoHeader->biBitCount<9)
	{
		
		CDibNew1->Lunkuotiqu();             //调用轮廓提取处理函数
		Invalidate();
	}
	else
	{
		Invalidate();
		MessageBox("在右图单击鼠标左键选取种子点");
		state=3;
	}
}

/*轮廓跟踪消息映射*/
void CDynSplitView2::OnLunkuogenzong() 
{
	clearmem();
	if(CDibNew1->m_pBitmapInfoHeader->biBitCount<9)
	{
		
		CDibNew1->Lunkuogenzong();             //调用轮廓跟踪处理函数
		Invalidate();
	}
	else
	{
		Invalidate();
		MessageBox("在右图单击鼠标左键选取种子点");
		state=4;
	}
}

/*种子填充消息映射*/
void CDynSplitView2::OnZhongzitianchong() 
{
	clearmem();
	Invalidate();
	MessageBox("在右图单击鼠标左键选取种子点");
	state=1;
}

/*阈值分割消息映射*/ 
void CDynSplitView2::OnYuzhifenge() 
{
	int i;
	int Yuzhi;   //阈值变量
	// 各颜色分量的灰度分布密度
    float midu[256];
	
	// 计算灰度分布密度	 	  
	clearmem();
	CDibNew1->Zhifangtu(midu);             //调用灰度统计处理函数
	
	// 创建灰度直方图对话框
	CDlgZhiFangTu*    dlg;
	dlg=new CDlgZhiFangTu(this);
	dlg->Create(IDD_DIALOG_ZhiFangTu);
	// 传递灰度分布密度数据给面板类
	for (i = 0; i <256; i++)
		dlg->m_fIntensity[i] = midu[i];
	
	// 显示对话框，由用户进行灰度折线变换
	dlg->ShowWindow(SW_RESTORE);
	
    //创建阈值选择对话框
	CDlgYuZhiFenGe  dlg1;
	dlg1.m_Yuzhi=0;
	
	// 显示对话框，提示用户输入阈值
	if (dlg1.DoModal() != IDOK)
	{
		// 返回
		return;
	}
    Yuzhi=dlg1.m_Yuzhi;
	
	// 删除对话框
	delete dlg1;
	delete dlg;
	
	clearmem();
	CDibNew1->Yuzhifenge(Yuzhi);             //调用阈值分割处理函数
	Invalidate();
}

void CDynSplitView2::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (state==1)
	{
		
		SeedPoint.x=point.x;
		SeedPoint.y=point.y;
		
		clearmem();
		CDibNew1->Zhongzitianchong(SeedPoint);             //调用种子填充处理函数
		Invalidate();
		state=0;
	}
	else if(state==2)
	{
		SeedPoint.x=point.x;
		SeedPoint.y=point.y;
		clearmem();
		CDibNew1->Qiyuzengzhang(point);             //调用区域增长处理函数
		Invalidate();
		state=0;
	}
	else if(state==3)
	{
		SeedPoint.x=point.x;
		SeedPoint.y=point.y;
		clearmem();
		CDibNew1->Lunkuotiqu(SeedPoint);             //调用轮廓提取处理函数
		Invalidate();
		state=0;
	}
	else if(state==4)
	{
		SeedPoint.x=point.x;
		SeedPoint.y=point.y;
		clearmem();
		CDibNew1->Lunkuogenzong(SeedPoint);             //调用轮廓跟踪处理函数
		Invalidate();
		state=0;
	}
	CView::OnLButtonDown(nFlags, point);
}

/*区域生长消息映射*/
void CDynSplitView2::OnQuyushengzhang() 
{
	clearmem();
	Invalidate();
	MessageBox("在右图单击鼠标左键选取生长点");
	state=2;
}

void CDynSplitView2::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (state==2)
	{
		
		point.x=point.x;
		point.y=point.y;
		clearmem();
		CDibNew1->Qiyuzengzhang(point);             //调用种子填充处理函数
		Invalidate();
		state=0;
	}
	CView::OnRButtonDown(nFlags, point);
}

/*半阈值分割消息映射*/ 
void CDynSplitView2::OnBanYuZhi() 
{
	int i;
	int Yuzhi;   //阈值变量
	// 各颜色分量的灰度分布密度
    float midu[256];
	
	// 计算灰度分布密度	 	  
	clearmem();
	CDibNew1->Zhifangtu(midu);             //调用灰度统计处理函数
	
	// 创建灰度直方图对话框
	CDlgZhiFangTu*    dlg;
	dlg=new CDlgZhiFangTu(this);
	dlg->Create(IDD_DIALOG_ZhiFangTu);
	// 传递灰度分布密度数据给面板类
	for (i = 0; i <256; i++)
		dlg->m_fIntensity[i] = midu[i];
	
	// 显示对话框，由用户进行灰度折线变换
	dlg->ShowWindow(SW_RESTORE);
	
    //创建阈值选择对话框
	CDlgYuZhiFenGe  dlg1;
	dlg1.m_Yuzhi=0;
	
	// 显示对话框，提示用户输入阈值
	if (dlg1.DoModal() != IDOK)
	{
		// 返回
		return;
	}
    Yuzhi=dlg1.m_Yuzhi;
	
	// 删除对话框
	delete dlg1;
	delete dlg;
	
	clearmem();
	CDibNew1->BanYuZhi(Yuzhi);             //调用半阈值分割处理函数
	Invalidate();
}

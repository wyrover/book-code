// ImageProcessingView.cpp : implementation of the CImageProcessingView class
//

#include "stdafx.h"
#include "ImageProcessing.h"

#include "ImageProcessingDoc.h"
#include "ImageProcessingView.h"
#include  "GlobalApi.h"
#include  "DlgCoding.h"
#include <complex>
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CImageProcessingView

IMPLEMENT_DYNCREATE(CImageProcessingView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProcessingView, CScrollView)
	//{{AFX_MSG_MAP(CImageProcessingView)
	ON_COMMAND(ID_CODING_SHANFINO, OnCodingShanfino)
	ON_COMMAND(ID_CODING_ARITH, OnCodingArith)
	ON_COMMAND(ID_CODING_BITPLANE, OnCodingBitplane)
	ON_COMMAND(ID_CODING_HUFFMAN, OnCodingHuffman)
	ON_COMMAND(ID_CODING_LOADIMG, OnCodingLoadimg)
	ON_COMMAND(ID_CODING_WRITEIMG, OnCodingWriteimg)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CImageProcessingView construction/destruction

CImageProcessingView::CImageProcessingView()
{
	// 为小波变换设置的参数
	// 临时存放小波变换系数内存
	m_pDbImage = NULL;	
	
	// 设置当前层数
	m_nDWTCurDepth = 0;

	// 设置小波基紧支集长度
	m_nSupp = 1;
}

CImageProcessingView::~CImageProcessingView()
{
	// 释放已分配内存
	if(m_pDbImage){
		delete[]m_pDbImage;
		m_pDbImage = NULL;
	}
}

BOOL CImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CImageProcessingView drawing

void CImageProcessingView::OnDraw(CDC* pDC)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CSize sizeDibDisplay;		
	

	
	if(!pDoc->m_pDibInit->IsEmpty()){	
		sizeDibDisplay = pDoc->m_pDibInit->GetDimensions();
		pDoc->m_pDibInit->Draw(pDC,CPoint(0,0),sizeDibDisplay);	
	}	

}

void CImageProcessingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CSize sizeTotal = pDoc->m_pDibInit->GetDimensions();
	SetScrollSizes(MM_TEXT, sizeTotal);

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

// CImageProcessingView printing

BOOL CImageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

// CImageProcessingView diagnostics

#ifdef _DEBUG
void CImageProcessingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProcessingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProcessingDoc* CImageProcessingView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessingDoc)));
	return (CImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG

// CImageProcessingView message handlers






void CImageProcessingView::OnCodingHuffman() 
{
	// 哈夫曼编码表
	
	// 获取文档
	CImageProcessingDoc * pDoc = GetDocument();
	
	// 指向源图象象素的指针
	unsigned char *	lpSrc;
		
	// 图象的高度和宽度
	LONG	lHeight;
	LONG	lWidth;
	
	// 图象每行的字节数
	LONG	lLineBytes;
	
	// 图象象素总数
	LONG	lCountSum;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 数组指针用来保存各个灰度值出现概率
	double * dProba;
	
	// 当前图象颜色数目
	int		nColorNum;

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;
			
	//图象数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// 头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（只处理8-bpp位图的霍夫曼编码）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的霍夫曼编码！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
//---------------------------------------
//	   开始计算各个灰度级出现的概率	
//	   如果需要对指定的序列进行哈夫曼编码,
//	   只要将这一步改成给各个灰度级概率赋值即可
//---------------------------------------
	
	//  由头文件信息得到图象的比特数，从而得到颜色信息
	nColorNum = (int)pow(2.0f,lpBMIH->biBitCount);

	// 分配内存
	dProba = new double[nColorNum];
	
	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth = SizeDim.cx;
	lHeight = SizeDim.cy;

	// 计算图象象素总数
	lCountSum = lHeight * lWidth;
	
	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 计算图象每行的字节数
	lLineBytes = SizeRealDim.cx;
	
	// 赋零值
	for (i = 0; i < nColorNum; i ++)
	{
		dProba[i] = 0.0;
	}
	
	// 计算各个灰度值的计数
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			// 指向图象指针
			lpSrc = lpDIBBits + lLineBytes * i + j;
			
			// 计数加1
			dProba[*(lpSrc)] = dProba[*(lpSrc)] + 1;
		}
	}
		
	
	// 计算各个灰度值出现的概率
	for (i = 0; i < nColorNum; i ++)
	{
		dProba[i] = dProba[i] / (FLOAT)lCountSum;
	}
	
//---------------------------------------
//	 构建霍夫曼编码的码表
//	 并用对话框显示霍夫曼码表
//---------------------------------------
	
	// 创建对话框
	CDlgHuffman dlgCoding;
	
	// 初始化变量值
	dlgCoding.dProba = dProba;
	dlgCoding.nColorNum = nColorNum;
	
	// 显示对话框
	dlgCoding.DoModal();
		
	// 恢复光标
	EndWaitCursor();
	
}

void CImageProcessingView::OnCodingShanfino() 
{
	// 香农－弗诺编码表
	
	// 获取文档
	CImageProcessingDoc * pDoc = GetDocument();
	
	// 指向源图象象素的指针
	unsigned char *	lpSrc;
		
	// 图象的高度
	LONG	lHeight;
	LONG	lWidth;
	
	// 图象每行的字节数
	LONG	lLineBytes;
	
	// 获取当前DIB颜色数目
	int		nColorNum;
	
	// 图象象素总数
	LONG	lCountSum;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 保存各个灰度值出现概率的数组指针
	double * dProba;	

	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;
			
	//图象数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// 头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的香农－费诺编码）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的香农－费诺编码！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	//---------------------------------------
	// 开始计算各个灰度级出现的概率	
	//
	// 如果需要对指定的序列进行香农－弗诺编码,
	//只要将这一步改成给各个灰度级概率赋值即可
	//---------------------------------------
	//  灰度值总数的计算
	nColorNum = (int)pow(2.0,lpBMIH->biBitCount);

	// 分配内存
	dProba = new double[nColorNum];
	
	//得到图象的宽度和高度
	CSize   SizeDim;
	SizeDim = pDib->GetDimensions();
	lWidth = SizeDim.cx;
	lHeight = SizeDim.cy;

	// 计算图象象素总数
	lCountSum = lHeight * lWidth;
	
	//得到实际的Dib图象存储大小
	CSize   SizeRealDim;
	SizeRealDim = pDib->GetDibSaveDim();

	// 计算图象每行的字节数
	lLineBytes = SizeRealDim.cx;

	// 计算图象象素总数
	lCountSum = lHeight * lWidth;
	
	// 重置计数为0
	for (i = 0; i < nColorNum; i ++)
	{
		dProba[i] = 0.0;
	}
	
	// 计算各个灰度值的计数（对于非256色位图，此处给数组dProba赋值方法将不同）
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			// 指向图象指针
			lpSrc = lpDIBBits + lLineBytes * i + j;
			
			// 计数加1
			dProba[*(lpSrc)] = dProba[*(lpSrc)]+ 1;
		}
	}
	
	
	// 计算各个灰度值出现的概率
	for (i = 0; i < nColorNum; i ++)
	{
		dProba[i] /= (double)lCountSum;
	}
	
	//---------------------------------------
	// 构建香农－费诺编码的码表
	// 并用对话框显示香农－费诺码表
	//---------------------------------------
	
	// 创建对话框
	CDlgShannon dlgPara;
	
	// 初始化变量值
	dlgPara.m_dProba = dProba;
	dlgPara.m_nColorNum = nColorNum;
	
	// 显示对话框
	dlgPara.DoModal();

	//释放内存
	delete dProba;
		
	// 恢复光标
	EndWaitCursor();
}

void CImageProcessingView::OnCodingArith() 
{
	CDlgArith dlgCoding;
	
	// 显示对话框
	dlgCoding.DoModal();
	
}

void CImageProcessingView::OnCodingBitplane() 
{
	// 创建对话框
	CDlgBitPlane dlgCoding;	
	
	// 显示对话框
	dlgCoding.DoModal();	
	
	BYTE bBitNum = dlgCoding.m_BItNumber;

	// 获取文档
	CImageProcessingDoc * pDoc = GetDocument();
		
	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;
	
	// 头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的位平面分解）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的位平面分解！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	DIBBITPLANE(pDib,bBitNum);

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
		
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
}

void CImageProcessingView::OnCodingWriteimg() 
{
	// 对当前图象进行DPCM编码（存为IMG格式文件）
	
	// 获取文档
	CImageProcessingDoc * pDoc = GetDocument();
		
	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;
			
	//图象数据的指针
	LPBYTE  lpDIBBits = pDib->m_lpImage;

	// 头文件信息
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;

	// 判断是否是8-bpp位图（处理8-bpp位图的DPCM编码）
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的DPCM编码！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
						
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 文件保存路径
	CString strFilePath;
	
	// 获取原始文件名
	strFilePath = pDoc->GetPathName();
	
	// 更改后缀为IMG
	if (strFilePath.Right(4).CompareNoCase(".BMP") == 0)
	{	
		strFilePath = strFilePath.Left(strFilePath.GetLength()-3) + "IMG";
	}
	else
	{
		strFilePath += ".IMG";
	}

	// 创建SaveAs对话框
	CFileDialog dlg(FALSE, "IMG", strFilePath, 
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		            "IMG图象文件 (*.IMG) | *.IMG|所有文件 (*.*) | *.*||", NULL);
	
	// 提示用户选择保存的路径
	if (dlg.DoModal() != IDOK)
	{
		// 恢复光标
		EndWaitCursor();
	
		return;
	}
	
	// 获取用户指定的文件路径
	strFilePath = dlg.GetPathName();
	
	// CFile和CFileException对象
	CFile file;
	CFileException fe;
	
	// 尝试创建指定的IMG文件
	if (!file.Open(strFilePath, CFile::modeCreate |
	  CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		MessageBox("打开指定IMG文件时失败！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
	
		return;
	}
	
	// 调用WRITE2IMG()函数将当前的DIB保存为IMG文件
	if (::WRITE2IMG(pDib, file))
	{
		MessageBox("成功保存为IMG文件！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
	}
	else
	{
		MessageBox("保存为IMG文件失败！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
	}	
		
	// 恢复光标
	EndWaitCursor();
	
}


void CImageProcessingView::OnCodingLoadimg() 
{
	// 读入IMG文件
	
	// 获取文档
	CImageProcessingDoc * pDoc = GetDocument();
		
	//  获得图象CDib类的指针
	CDib * pDib = pDoc->m_pDibInit;

	// 文件路径
	CString strFilePath;
	
	// 创建Open对话框
	CFileDialog dlg(TRUE, "PCX", NULL,
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					"IMG图象文件 (*.PCX) | *.IMG|所有文件 (*.*) | *.*||", NULL);
	
	// 提示用户选择保存的路径
	if (dlg.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	
	// 获取用户指定的文件路径
	strFilePath = dlg.GetPathName();
	
	// CFile和CFileException对象
	CFile file;
	CFileException fe;
	
	// 尝试打开指定的PCX文件
	if (!file.Open(strFilePath, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		// 提示用户
		MessageBox("打开指定PCX文件时失败！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();

	// 调用LOADIMG()函数读取指定的IMG文件
	BOOL Succ = LOADIMG(pDib, file);

	if (Succ == TRUE)
	{
		// 提示用户
		MessageBox("成功读取IMG文件！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
		
	}
	else
	{
		// 提示用户
		MessageBox("读取IMG文件失败！", "系统提示" , 
			MB_ICONINFORMATION | MB_OK);
	}

	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	// 恢复光标
	EndWaitCursor();
	
}


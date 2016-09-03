// CellDetectionView.cpp : CCellDetectionView 类的实现
//

#include "stdafx.h"
#include "CellDetection.h"

#include "CellDetectionDoc.h"
#include "CellDetectionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCellDetectionView

IMPLEMENT_DYNCREATE(CCellDetectionView, CView)

BEGIN_MESSAGE_MAP(CCellDetectionView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_OPEN_IMAGE, &CCellDetectionView::OnOpenImage)
	ON_COMMAND(ID_CELL_DETECTION, &CCellDetectionView::OnCellDetection)
END_MESSAGE_MAP()

// CCellDetectionView 构造/析构

CCellDetectionView::CCellDetectionView()
{
	// TODO: 在此处添加构造代码

}

CCellDetectionView::~CCellDetectionView()
{
}

BOOL CCellDetectionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CCellDetectionView 绘制

void CCellDetectionView::OnDraw(CDC* /*pDC*/)
{
	CCellDetectionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CCellDetectionView 打印

BOOL CCellDetectionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCellDetectionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCellDetectionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CCellDetectionView 诊断

#ifdef _DEBUG
void CCellDetectionView::AssertValid() const
{
	CView::AssertValid();
}

void CCellDetectionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCellDetectionDoc* CCellDetectionView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCellDetectionDoc)));
	return (CCellDetectionDoc*)m_pDocument;
}
#endif //_DEBUG


// CCellDetectionView 消息处理程序

void CCellDetectionView::OnOpenImage( )
{
	// 此函数实现打开并显示白细胞图像
	m_CellImage.LoadFromFile("CellImage.bmp");//打开白细胞图像

	//显示白细胞图像
	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_CellImage.GetWidth();    //获得灰度图像的宽度
	LONG lHeight=m_CellImage.GetHeight();  //获得灰度图像的高度
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_CellImage.Draw(pDC,point,size);  
}

void CCellDetectionView::OnCellDetection()
{
	// 此函数实现白细胞核的检测

    long i,j;			//循环变量
	unsigned char pixel;	//像素值

	CPoint point1,point2, point3,point4;	 //绘图的坐标起点
	CSize size;		 //绘图的尺寸

    LPBYTE	lpSrc;			// 指向指向灰度图像的指针
    
	LONG lWidth=m_CellImage.GetWidth();    //获得灰度图像的宽度
	LONG lHeight=m_CellImage.GetHeight();  //获得灰度图像的高度

	point1.x=0;
	point1.y=0;	
	point2.x=lWidth+20;	 //使显示的各个图像有所间隔，故+20
	point2.y=0;
	point3.x=0;
	point3.y=lHeight+20;
	point4.x=lWidth+20;
	point4.y=lHeight+20; 

	size.cx= lWidth	;
	size.cy= lHeight;

	CDC *pDC=GetDC();//获得设备DC

	m_CellImage.Draw(pDC,point1,size);//显示原图像
	
	m_CellImage.RgbToGrade();//转换为灰度图像
	
	LPBYTE lpDIBBits=m_CellImage.GetData();//找到灰度图像的起始位置	
	
	//灰度拉伸处理
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			
			// 指向原图像第j行，第i个象素的指针			
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			pixel = (unsigned char)*lpSrc;

			//灰度拉伸，最小值为75，最大值为245
			if(pixel<75)
			{
				*lpSrc=0;
			}
			else if (pixel<245)
			{
				*lpSrc=(unsigned char)(255*(pixel-75)/(245-75));
			}
			else
			{
				*lpSrc=255;
			}
		}
	}

	m_CellImage.Draw(pDC,point2,size);//显示拉伸后的灰度图像
	
	//阈值化为二值图像
	for(j = 0; j <lHeight; j++)
	{
		for(i = 0;i <lWidth; i++)
		{
			
			// 指向原图像第j行，第i个象素的指针			
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			pixel = (unsigned char)*lpSrc;

			//设定二值化阈值为50
			if(pixel<50)
			{
				*lpSrc=255;
			}
			
			else
			{
				*lpSrc=0;
			}
		}
	}

	m_CellImage.Draw(pDC,point3,size);   //显示二值图像
	
    CBinaryMorphDib BinMorphProcess(&m_CellImage);//定义一个形态学处理对象
	
	int Structure[3][3]={0,1,0,1,1,1,0,1,0};//定义结构元素

	//连续闭运算n1次，这里设置n1＝3
	for(i= 0; i<3; i++) 
	{
		BinMorphProcess.Closing(Structure);
	}
	//连续腐蚀n2次，这里设置n2＝3
	for(i= 0; i<3; i++) 
	{
		BinMorphProcess.Erosion(Structure);
	}


	//连续膨胀与腐蚀同样的次数
	for(i= 0; i<3; i++) 
	{
		BinMorphProcess.Dilation(Structure);

	}
	 
	m_CellImage.Draw(pDC,point4,size);   //显示检测结果	

	
}

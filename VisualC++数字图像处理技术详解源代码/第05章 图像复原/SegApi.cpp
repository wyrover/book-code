#include "GlobalApi.h"
#include "stdafx.h"
#include "cdib.h"
#include "math.h"
#include <direct.h>
#include <complex>
using namespace std;

/*************************************************************************
 *
 * \函数名称：
 *   RegionSegFixThreshold()
 *
 * \输入参数:
 *   CDib * pDib					- 指向CDib类的指针，含有原始图象信息
 *   int nThreshold					- 区域分割的阈值
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   1（逻辑）表示对应象素为前景区域，0表示背景
 *   阈值分割的关键问题在于阈值的选取。阈值的选取一般应该视实际的应用而
 *   灵活设定。
 *
 *************************************************************************
 */
void RegionSegFixThreshold(CDib * pDib, int nThreshold)
{
	//遍历图象的纵坐标
	int y;

	//遍历图象的横坐标
	int x;

	//图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	//图像在计算机在存储中的实际大小
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	//图像在内存中每一行象素占用的实际空间
	int nSaveWidth = sizeImageSave.cx;

	
	//图像数据的指针
	LPBYTE  pImageData = pDib->m_lpImage;

	for(y=0; y<nHeight ; y++ )
		for(x=0; x<nWidth ; x++ )
		{
			if( *(pImageData+y*nSaveWidth+x) < nThreshold)
				*(pImageData+y*nSaveWidth+x) = 0;
		}
}

/*************************************************************************
 *
 * \函数名称：
 *   RegionSegAdaptive()
 *
 * \输入参数:
 *   CDib * pDib					- 指向CDib类的指针，含有原始图象信息
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   1（逻辑）表示对应象素为前景区域，0表示背景
 *   阈值分割的关键问题在于阈值的选取。阈值的选取一般应该视实际的应用而
 *   灵活设定。本函数中，阈值不是固定的，而是根据图象象素的实际性质而设定的。
 *   这个函数把图像分成四个子图象，然后计算每个子图象的均值，根据均值设置阈值
 *   阈值只是应用在对应的子图象
 *
 *************************************************************************
 */
void RegionSegAdaptive(CDib * pDib)
{
	//遍历图象的纵坐标
	int y;

	//遍历图象的横坐标
	int x;

	//图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	//图像在计算机在存储中的实际大小
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	//图像在内存中每一行象素占用的实际空间
	int nSaveWidth = sizeImageSave.cx;

	//图像数据的指针
	LPBYTE  lpImage = pDib->m_lpImage;

	// 局部阈值
	int nThd[2][2] ;

	// 子图象的平均值
	int nLocAvg ;
	
	// 对左上图像逐点扫描：
	nLocAvg = 0 ;
	// y方向
	for(y=0; y<nHeight/2 ; y++ )
	{
		// x方向
		for(x=0; x<nWidth/2 ; x++ )
		{
			nLocAvg += lpImage[y*nSaveWidth + x];
		}
	}
	// 计算均值
	nLocAvg /= ( (nHeight/2) * (nWidth/2) ) ;

	// 设置阈值为子图象的平均值
	nThd[0][0] = nLocAvg ;

	// 对左上图像逐点扫描进行分割：
	// y方向
	for(y=0; y<nHeight/2 ; y++ )
	{
		// x方向
		for(x=0; x<nWidth/2 ; x++ )
		{
			if(lpImage[y*nSaveWidth + x]<nThd[0][0])
				lpImage[y*nSaveWidth + x] = 255 ;
			else
			{
				lpImage[y*nSaveWidth + x] = 0 ;
			}
			
		}
	}

	// =============================================
	// 对左下图像逐点扫描：
	nLocAvg = 0 ;
	// y方向
	for(y=nHeight/2; y<nHeight ; y++ )
	{
		// x方向
		for(x=0; x<nWidth/2 ; x++ )
		{
			nLocAvg += lpImage[y*nSaveWidth + x];
		}
	}
	// 计算均值
	nLocAvg /= ( (nHeight - nHeight/2) * (nWidth/2) ) ;

	// 设置阈值为子图象的平均值
	nThd[1][0] = nLocAvg ;

	// 对左下图像逐点扫描进行分割：
	// y方向
	for(y=nHeight/2; y<nHeight ; y++ )
	{
		// x方向
		for(x=0; x<nWidth/2 ; x++ )
		{
			if(lpImage[y*nSaveWidth + x]<nThd[1][0])
				lpImage[y*nSaveWidth + x] = 255 ;
			else
			{
				lpImage[y*nSaveWidth + x] = 0 ;
			}
			
		}
	}

	// =============================================
	// 对右上图像逐点扫描：
	nLocAvg = 0 ;
	// y方向
	for(y=0; y<nHeight/2 ; y++ )
	{
		// x方向
		for(x=nWidth/2; x<nWidth ; x++ )
		{
			nLocAvg += lpImage[y*nSaveWidth + x];
		}
	}
	// 计算均值
	nLocAvg /= ( (nHeight/2) * (nWidth - nWidth/2) ) ;
	
	// 设置阈值为子图象的平均值
	nThd[0][1] = nLocAvg ;

	// 对右上图像逐点扫描进行分割：
	// y方向
	for(y=0; y<nHeight/2 ; y++ )
	{
		// x方向
		for(x=nWidth/2; x<nWidth ; x++ )
		{
			if(lpImage[y*nSaveWidth + x]<nThd[0][1])
				lpImage[y*nSaveWidth + x] = 255 ;
			else
			{
				lpImage[y*nSaveWidth + x] = 0 ;
			}
			
		}
	}

	// =============================================
	// 对右下图像逐点扫描：
	nLocAvg = 0 ;
	// y方向
	for(y=nHeight/2; y<nHeight ; y++ )
	{
		// x方向
		for(x=nWidth/2; x<nWidth ; x++ )
		{
			nLocAvg += lpImage[y*nSaveWidth + x];
		}
	}
	// 计算均值
	nLocAvg /= ( (nHeight - nHeight/2) * (nWidth - nWidth/2) ) ;

	// 设置阈值为子图象的平均值
	nThd[1][1] = nLocAvg ;

	// 对右下图像逐点扫描进行分割：
	// y方向
	for(y=nHeight/2; y<nHeight ; y++ )
	{
		// x方向
		for(x=nWidth/2; x<nWidth ; x++ )
		{
			if(lpImage[y*nSaveWidth + x]<nThd[1][1])
				lpImage[y*nSaveWidth + x] = 255 ;
			else
			{
				lpImage[y*nSaveWidth + x] = 0 ;
			}
		}
	}
	
	// 为了显示方便显示，逻辑1用黑色显示，逻辑0用白色显示
	for(y=0; y<nHeight ; y++ )
	{
		// x方向
		for(x=0; x<nWidth ; x++ )
		{
			lpImage[y*nSaveWidth + x] = 255 - lpImage[y*nSaveWidth + x] ;
		}
	}
}

/*************************************************************************
 *
 * \函数名称：
 *   RobertsOperator()
 *
 * \输入参数:
 *   CDib * pDib		- 指向CDib类的指针，含有原始图象信息
 *   double * pdGrad	- 指向梯度数据的指针，含有图像的梯度信息
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   Roberts算子
 *
 *************************************************************************
 */
void RobertsOperator(CDib * pDib, double * pdGrad)
{
	// 遍历图象的纵坐标
	int y;

	// 遍历图象的横坐标
	int x;

	// 图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	// 图像在计算机在存储中的实际大小
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	// 图像在内存中每一行象素占用的实际空间
	int nSaveWidth = sizeImageSave.cx;

	// 图像数据的指针
	LPBYTE  pImageData = pDib->m_lpImage;

	// 初始化
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			*(pdGrad+y*nWidth+x)=0;
		}


	// 下面开始利用Roberts算子进行计算，为了保证计算所需要的
	// 的数据位于图像数据的内部，下面的两重循环的条件是
	// y<nHeight-1 而不是y<nHeight，相应的x方向也是x<nWidth-1
	// 而不是x<nWidth

	//这两个变量用来表示Roberts算子第一个模板的两个象素值
	int nUpLeft;
	int nDownRight;

	// 这两个变量用来表示Roberts算子第二个模板的两个象素值
	int nUpRight;
	int nDownLeft;

	// 这两个变量用来表示Roberts算子计算的结果
	int nValueOne;
	int nValueTwo;

	// 临时变量
	double dGrad;

	for(y=0; y<nHeight-1 ; y++ )
		for(x=0 ; x<nWidth-1 ; x++ )
		{
			// Roberts算子第一个模板需要的象素值
			nUpLeft		=*(pImageData+y*nSaveWidth+x) ; 
			nDownRight	=*(	pImageData+(y+1)*nSaveWidth+x+1	);
			nDownRight	*=-1;

			//Roberts算子的第一个模板计算结果
			nValueOne	=nUpLeft+nDownRight	;

			// Roberts算子第二个模板需要的象素值
			nUpRight	=*(	pImageData+y*nSaveWidth+x+1	)		;
			nDownLeft	=*(	pImageData+(y+1)*nSaveWidth+x	);
			nDownLeft	*=-1;

			// Roberts算子的第二个模板计算结果
			nValueTwo	=nUpRight+nDownLeft;

			// 计算两个偏导数的平方和
			dGrad=nValueOne*nValueOne + nValueTwo*nValueTwo;

			// 开方
			dGrad=pow(dGrad,0.5);

			// 范数采用欧式距离
			*(pdGrad+y*nWidth+x)=dGrad;
		}
}
/*************************************************************************
 *
 * \函数名称：
 *   LaplacianOperator()
 *
 * \输入参数:
 *   CDib * pDib		- 指向CDib类的指针，含有原始图象信息
 *   double * pdGrad	- 指向梯度数据的指针，含有图像的梯度信息
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   LaplacianOperator算子,是二阶算子，不想Roberts算子那样需要两个模板计算
 *   梯度，LaplacianOperator算子只要一个算子就可以计算梯度。但是因为利用了
 *   二阶信息，对噪声比较敏感
 *
 *************************************************************************
 */
void LaplacianOperator(CDib * pDib, double * pdGrad)
{
		// 遍历图象的纵坐标
	int y;

	// 遍历图象的横坐标
	int x;

	// 图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	// 图像在计算机在存储中的实际大小
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	// 图像在内存中每一行象素占用的实际空间
	int nSaveWidth = sizeImageSave.cx;

	// 图像数据的指针
	LPBYTE  lpImage = pDib->m_lpImage;

	// 初始化
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			*(pdGrad+y*nWidth+x)=0;
		}

	// 设置模板系数
	static int nWeight[3][3] ;
	nWeight[0][0] = -1 ;   
	nWeight[0][1] = -1 ;   
	nWeight[0][2] = -1 ;   
	nWeight[1][0] = -1 ;   
	nWeight[1][1] =  8 ;   
	nWeight[1][2] = -1 ;   
	nWeight[2][0] = -1 ;   
	nWeight[2][1] = -1 ;   
	nWeight[2][2] = -1 ;   



	//这个变量用来表示Laplacian算子象素值
	int nTmp[3][3];
	
	// 临时变量
	double dGrad;

	// 模板循环控制变量
	int yy ;
	int xx ;

	
	// 下面开始利用Laplacian算子进行计算，为了保证计算所需要的
	// 的数据位于图像数据的内部，下面的两重循环的条件是
	// y<nHeight-2 而不是y<nHeight，相应的x方向也是x<nWidth-2
	// 而不是x<nWidth
	for(y=1; y<nHeight-2 ; y++ )
		for(x=1 ; x<nWidth-2 ; x++ )
		{
			dGrad = 0 ; 
			// Laplacian算子需要的各点象素值

			// 模板第一行
			nTmp[0][0] = lpImage[(y-1)*nSaveWidth + x - 1 ] ; 
			nTmp[0][1] = lpImage[(y-1)*nSaveWidth + x     ] ; 
			nTmp[0][2] = lpImage[(y-1)*nSaveWidth + x + 1 ] ; 

			// 模板第二行
			nTmp[1][0] = lpImage[y*nSaveWidth + x - 1 ] ; 
			nTmp[1][1] = lpImage[y*nSaveWidth + x     ] ; 
			nTmp[1][2] = lpImage[y*nSaveWidth + x + 1 ] ; 

			// 模板第三行
			nTmp[2][0] = lpImage[(y+1)*nSaveWidth + x - 1 ] ; 
			nTmp[2][1] = lpImage[(y+1)*nSaveWidth + x     ] ; 
			nTmp[2][2] = lpImage[(y+1)*nSaveWidth + x + 1 ] ; 
			
			// 计算梯度
			for(yy=0; yy<3; yy++)
				for(xx=0; xx<3; xx++)
				{
					dGrad += nTmp[yy][xx] * nWeight[yy][xx] ;
				}
			
			// 梯度值写入内存
			*(pdGrad+y*nWidth+x)=dGrad;
		}

}

/*************************************************************************
 *
 * \函数名称：
 *   RegionGrow()
 *
 * \输入参数:
 *   CDib * pDib					- 指向CDib类的指针，含有原始图象信息
 *   unsigned char * pUnRegion		- 指向区域生长结果的指针
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   pUnRegion指针指向的数据区存储了区域生长的结果，其中1（逻辑）表示
 *	 对应象素为生长区域，0表示为非生长区域
 *   区域生长一般包含三个比较重要的问题：
 *		1. 种子点的选取
 *		2. 生长准则
 *		3. 终止条件
 *	 可以认为，这三个问题需要具体分析，而且每个问题解决的好坏直接关系到
 *	 区域生长的结果。
 *	 本函数的种子点选取为图像的中心，生长准则是相邻象素的象素值小于
 *	 nThreshold, 终止条件是一直进行到再没有满足生长准则需要的象素时为止
 *
 *************************************************************************
 */
void RegionGrow(CDib * pDib, unsigned char * pUnRegion, int nThreshold)
{
	static int nDx[]={-1,0,1,0};
	static int nDy[]={0,1,0,-1};

	// 遍历图象的纵坐标
//	int y;

	// 遍历图象的横坐标
//	int x;

	// 图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	// 图像在计算机在存储中的实际大小
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	// 图像在内存中每一行象素占用的实际空间
	int nSaveWidth = sizeImageSave.cx;

	// 初始化
	memset(pUnRegion,0,sizeof(unsigned char)*nWidth*nHeight);

	// 种子点
	int nSeedX, nSeedY;

	// 设置种子点为图像的中心
	nSeedX = nWidth /2 ;
	nSeedY = nHeight/2 ;

	// 定义堆栈，存储坐标
	int * pnGrowQueX ;
	int * pnGrowQueY ;
	
	// 分配空间
	pnGrowQueX = new int [nWidth*nHeight];
	pnGrowQueY = new int [nWidth*nHeight];

	// 图像数据的指针
	unsigned char *  pUnchInput =(unsigned char * )pDib->m_lpImage;
	
	// 定义堆栈的起点和终点
	// 当nStart=nEnd, 表示堆栈中只有一个点
	int nStart ;
	int nEnd   ;

	//初始化
	nStart = 0 ;
	nEnd   = 0 ;

	// 把种子点的坐标压入栈
	pnGrowQueX[nEnd] = nSeedX;
	pnGrowQueY[nEnd] = nSeedY;

	// 当前正在处理的象素
	int nCurrX ;
	int nCurrY ;

	// 循环控制变量
	int k ;

	// 图象的横纵坐标,用来对当前象素的4邻域进行遍历
	int xx;
	int yy;

	while (nStart<=nEnd)
	{
		// 当前种子点的坐标
		nCurrX = pnGrowQueX[nStart];
		nCurrY = pnGrowQueY[nStart];					

		// 对当前点的4邻域进行遍历
		for (k=0; k<4; k++)	
		{	
			// 4邻域象素的坐标
			xx = nCurrX+nDx[k];
			yy = nCurrY+nDy[k];
			
			// 判断象素(xx，yy) 是否在图像内部
			// 判断象素(xx，yy) 是否已经处理过
			// pUnRegion[yy*nWidth+xx]==0 表示还没有处理

			// 生长条件：判断象素(xx，yy)和当前象素(nCurrX,nCurrY) 象素值差的绝对值
			if (	(xx < nWidth) && (xx>=0) && (yy<nHeight) && (yy>=0) 
				    && (pUnRegion[yy*nWidth+xx]==0) 
					&& abs(pUnchInput[yy*nSaveWidth+xx] - pUnchInput[nCurrY*nSaveWidth+nCurrX])<nThreshold )
			{
				// 堆栈的尾部指针后移一位
				nEnd++;

				// 象素(xx，yy) 压入栈
				pnGrowQueX[nEnd] = xx;
				pnGrowQueY[nEnd] = yy;

				// 把象素(xx，yy)设置成逻辑1（255）
				// 同时也表明该象素处理过
				pUnRegion[yy*nWidth+xx] = 255 ;
			}
		}
		nStart++;
	}

	// 释放内存
	delete []pnGrowQueX;
	delete []pnGrowQueY;
    pnGrowQueX = NULL ;
	pnGrowQueY = NULL ;
}

void DFT_2D(CDib * pDib,double * pTrRstRpart, double * pTrRstIpart)
{
	double PI = 3.14159;
	//遍历图象的纵坐标
	int y;

	//遍历图象的横坐标
	int x;

	//频域的横坐标
	int m;

	//频域的纵坐标
	int n; 

	//图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	//图像在计算机在存储中的实际大小
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	int nSaveWidth = sizeImageSave.cx;

	//图像数据的指针
	LPBYTE  pImageData = pDib->m_lpImage;

	//初始化结果数据
	for(n=0; n<nHeight ; n++ )
		for(m=0 ; m<nWidth ; m++ )
		{
			*(	pTrRstRpart + n*nWidth + m	) =0;
			*(	pTrRstIpart + n*nWidth + m	) =0;
		}
	double fCosTable;
	double fSinTable;
	int	  nPxValue;

	fCosTable=0 ;
	nPxValue =0;

	double fTmpRstR;
	double fTmpRstI;
	for(n=0; n<nHeight ; n++ )
		for(m=0 ; m<nWidth ; m++ )
		{
			fTmpRstR=0;
			fTmpRstI=0;
			for(y=0; y<nHeight ; y++ )
				for(x=0 ; x<nWidth ; x++ )
				{
					fCosTable= cos(	2*PI*( ((double)m*x)/nWidth + ((double)n*y)/nHeight) ) ;
					fSinTable= sin(	-2*PI*( ((double)m*x)/nWidth + ((double)n*y)/nHeight) ) ;
					nPxValue = *(pImageData+ y*nSaveWidth + x )			;

					fTmpRstR+=fCosTable* nPxValue						;
					fTmpRstI+=fSinTable* nPxValue						;
				}
			*( pTrRstRpart + nWidth * n + m ) = fTmpRstR;
			*( pTrRstIpart + nWidth * n + m ) = fTmpRstI;
		}
}


void IDFT_2D(CDib * pDib,double * pTrRstRpart, double * pTrRstIpart)
{
	double PI = 3.14159;
	//遍历图象的纵坐标
	int y;

	//遍历图象的横坐标
	int x;

	//频域的横坐标
	int m;

	//频域的纵坐标
	int n; 

	//图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	//图像在计算机在存储中的实际大小
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	int nSaveWidth = sizeImageSave.cx;

	//图像数据的指针
	LPBYTE  pImageData = pDib->m_lpImage;

	double fCosTable;
	double fSinTable;
	fCosTable=0 ;
	fSinTable=0 ;

	double fTmpPxValue;
	double fRpartValue;
	double fIpartValue;
	fTmpPxValue=0;
	fRpartValue=0;
	fIpartValue=0;

	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			fTmpPxValue=0;
			for(n=0; n<nHeight ; n++ )
				for(m=0 ; m<nWidth ; m++ )
				{
					fCosTable= cos(	2*PI*( ((double)m*x)/nWidth + ((double)n*y)/nHeight) ) ;
					fSinTable= sin(	2*PI*( ((double)m*x)/nWidth + ((double)n*y)/nHeight) ) ;
					fRpartValue=*(pTrRstRpart+ n*nHeight + m ) ;
					fIpartValue=*(pTrRstIpart+ n*nHeight + m ) ;
					
					fTmpPxValue+=fCosTable* fRpartValue-fSinTable*fIpartValue;
				}
			fTmpPxValue=fTmpPxValue/(nHeight*nWidth);
			*( pImageData + nSaveWidth * y + x) = (unsigned char) fTmpPxValue ;
		}
}

/*************************************************************************
 *
 * \函数名称：
 *   SobelOperator()
 *
 * \输入参数:
 *   CDib * pDib		  - 指向CDib类的指针，含有原始图象信息
 *   double * pdGrad	- 指向梯度数据的指针，含有图像的梯度信息
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   Sobe算子
 *
 *   并行边界分割
 *
 *************************************************************************
 */
void SobelOperator(CDib * pDib, double * pdGrad)
{
		// 遍历图象的纵坐标
	int y;
	
	// 遍历图象的横坐标
	int x;
	
	// 图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;
	
	// 图像在计算机在存储中的实际大小
	CSize sizeImageSave	= pDib->GetDibSaveDim();
	
	// 图像在内存中每一行象素占用的实际空间
	int nSaveWidth = sizeImageSave.cx;
	
	// 图像数据的指针
	LPBYTE  lpImage = pDib->m_lpImage;
	
	// 初始化
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			*(pdGrad+y*nWidth+x)=0;
		}
		
		// 设置模板系数
		static int nWeight[2][3][3] ;
		nWeight[0][0][0] = -1 ;   
		nWeight[0][0][1] =  0 ;   
		nWeight[0][0][2] =  1 ;   
		nWeight[0][1][0] = -2 ;   
		nWeight[0][1][1] =  0 ;   
		nWeight[0][1][2] =  2 ;   
		nWeight[0][2][0] = -1 ;   
		nWeight[0][2][1] =  0 ;   
		nWeight[0][2][2] =  1 ;   
		
		nWeight[1][0][0] =  1 ;   
		nWeight[1][0][1] =  2 ;   
		nWeight[1][0][2] =  1 ;   
		nWeight[1][1][0] =  0 ;   
		nWeight[1][1][1] =  0 ;   
		nWeight[1][1][2] =  0 ;   
		nWeight[1][2][0] = -1 ;   
		nWeight[1][2][1] = -2 ;   
		nWeight[1][2][2] = -1 ;   
		
		
		
		//这个变量用来表示Laplacian算子象素值
		int nTmp[3][3];
		
		// 临时变量
		double dGrad   ;
		double dGradOne;
		double dGradTwo;
		
		// 模板循环控制变量
		int yy ;
		int xx ;
		
		
		// 下面开始利用Prewitt算子进行计算，为了保证计算所需要的
		// 的数据位于图像数据的内部，下面的两重循环的条件是
		// y<nHeight-1 而不是y<nHeight，相应的x方向也是x<nWidth-1
		// 而不是x<nWidth
		for(y=1; y<nHeight-1 ; y++ )
			for(x=1 ; x<nWidth-1 ; x++ )
			{
				dGrad    = 0 ; 
				dGradOne = 0 ;
				dGradTwo = 0 ;
				// Laplacian算子需要的各点象素值
				
				// 模板第一行
				nTmp[0][0] = lpImage[(y-1)*nSaveWidth + x - 1 ] ; 
				nTmp[0][1] = lpImage[(y-1)*nSaveWidth + x     ] ; 
				nTmp[0][2] = lpImage[(y-1)*nSaveWidth + x + 1 ] ; 
				
				// 模板第二行
				nTmp[1][0] = lpImage[y*nSaveWidth + x - 1 ] ; 
				nTmp[1][1] = lpImage[y*nSaveWidth + x     ] ; 
				nTmp[1][2] = lpImage[y*nSaveWidth + x + 1 ] ; 
				
				// 模板第三行
				nTmp[2][0] = lpImage[(y+1)*nSaveWidth + x - 1 ] ; 
				nTmp[2][1] = lpImage[(y+1)*nSaveWidth + x     ] ; 
				nTmp[2][2] = lpImage[(y+1)*nSaveWidth + x + 1 ] ; 
				
				// 计算梯度
				for(yy=0; yy<3; yy++)
					for(xx=0; xx<3; xx++)
					{
						dGradOne += nTmp[yy][xx] * nWeight[0][yy][xx] ;
						dGradTwo += nTmp[yy][xx] * nWeight[1][yy][xx] ;
					}
					dGrad = dGradOne*dGradOne + dGradTwo*dGradTwo  ;
					dGrad = sqrt(dGrad) ;
					// 梯度值写入内存
					*(pdGrad+y*nWidth+x)=dGrad;
			}
}

/*************************************************************************
 *
 * \函数名称：
 *   PrewittOperator()
 *
 * \输入参数:
 *   CDib * pDib		  - 指向CDib类的指针，含有原始图象信息
 *   double * pdGrad	- 指向梯度数据的指针，含有图像的梯度信息
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   Prewitt算子
 *
 *   并行边界分割
 *
 *************************************************************************
 */
void PrewittOperator(CDib * pDib, double * pdGrad)
{
	// 遍历图象的纵坐标
	int y;
	
	// 遍历图象的横坐标
	int x;
	
	// 图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;
	
	// 图像在计算机在存储中的实际大小
	CSize sizeImageSave	= pDib->GetDibSaveDim();
	
	// 图像在内存中每一行象素占用的实际空间
	int nSaveWidth = sizeImageSave.cx;
	
	// 图像数据的指针
	LPBYTE  lpImage = pDib->m_lpImage;
	
	// 初始化
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			*(pdGrad+y*nWidth+x)=0;
		}
		
		// 设置模板系数
		static int nWeight[2][3][3] ;
		nWeight[0][0][0] = -1 ;   
		nWeight[0][0][1] =  0 ;   
		nWeight[0][0][2] =  1 ;   
		nWeight[0][1][0] = -1 ;   
		nWeight[0][1][1] =  0 ;   
		nWeight[0][1][2] =  1 ;   
		nWeight[0][2][0] = -1 ;   
		nWeight[0][2][1] =  0 ;   
		nWeight[0][2][2] =  1 ;   
		
		nWeight[1][0][0] =  1 ;   
		nWeight[1][0][1] =  1 ;   
		nWeight[1][0][2] =  1 ;   
		nWeight[1][1][0] =  0 ;   
		nWeight[1][1][1] =  0 ;   
		nWeight[1][1][2] =  0 ;   
		nWeight[1][2][0] = -1 ;   
		nWeight[1][2][1] = -1 ;   
		nWeight[1][2][2] = -1 ;   
		
		
		
		//这个变量用来表示Laplacian算子象素值
		int nTmp[3][3];
		
		// 临时变量
		double dGrad   ;
		double dGradOne;
		double dGradTwo;
		
		// 模板循环控制变量
		int yy ;
		int xx ;
		
		
		// 下面开始利用Prewitt算子进行计算，为了保证计算所需要的
		// 的数据位于图像数据的内部，下面的两重循环的条件是
		// y<nHeight-1 而不是y<nHeight，相应的x方向也是x<nWidth-1
		// 而不是x<nWidth
		for(y=1; y<nHeight-1 ; y++ )
			for(x=1 ; x<nWidth-1 ; x++ )
			{
				dGrad    = 0 ; 
				dGradOne = 0 ;
				dGradTwo = 0 ;
				// Laplacian算子需要的各点象素值
				
				// 模板第一行
				nTmp[0][0] = lpImage[(y-1)*nSaveWidth + x - 1 ] ; 
				nTmp[0][1] = lpImage[(y-1)*nSaveWidth + x     ] ; 
				nTmp[0][2] = lpImage[(y-1)*nSaveWidth + x + 1 ] ; 
				
				// 模板第二行
				nTmp[1][0] = lpImage[y*nSaveWidth + x - 1 ] ; 
				nTmp[1][1] = lpImage[y*nSaveWidth + x     ] ; 
				nTmp[1][2] = lpImage[y*nSaveWidth + x + 1 ] ; 
				
				// 模板第三行
				nTmp[2][0] = lpImage[(y+1)*nSaveWidth + x - 1 ] ; 
				nTmp[2][1] = lpImage[(y+1)*nSaveWidth + x     ] ; 
				nTmp[2][2] = lpImage[(y+1)*nSaveWidth + x + 1 ] ; 
				
				// 计算梯度
				for(yy=0; yy<3; yy++)
					for(xx=0; xx<3; xx++)
					{
						dGradOne += nTmp[yy][xx] * nWeight[0][yy][xx] ;
						dGradTwo += nTmp[yy][xx] * nWeight[1][yy][xx] ;
					}
					dGrad = dGradOne*dGradOne + dGradTwo*dGradTwo  ;
					dGrad = sqrt(dGrad) ;
					// 梯度值写入内存
					*(pdGrad+y*nWidth+x)=dGrad;
			}
}

/*************************************************************************
 *
 * \函数名称：
 *   EdgeTrack()
 *
 * \输入参数:
 *   CDib * pDib					- 指向CDib类的指针，含有原始图象信息
 *   unsigned char * pUnEdgeTrack	- 指向边界跟踪结果的指针
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   pUnEdgeTrack指针指向的数据区存储了边界跟踪的结果，其中1（逻辑）表示
 *	 对应象素为边界点，0表示为非边界点
 *
 *   串行边界分割
 *
 *************************************************************************
 */
void EdgeTrack(CDib * pDib, unsigned char * pUnEdgeTrack)
{
	static int nDx[8]={-1,-1,-1, 0, 0, 1, 1, 1};
	static int nDy[8]={-1, 0, 1,-1, 1,-1, 0, 1};

	// 遍历图象的纵坐标
	int y;

	// 遍历图象的横坐标
	int x;

	// 图象的长宽大小
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;
	
	// 指向梯度数据的指针
	double * pdGrad;

	// 按照图像的大小开辟内存空间，存储梯度计算的结果
	pdGrad=new double[nHeight*nWidth];

    // 调用Roberts算子求梯度
	RobertsOperator(pDib, pdGrad);

	// 定义当前象素梯度值
	double dCurrGrad = 0;
	
	// 定义最大梯度值
	double dMaxGrad;

	// 设置初值
	dMaxGrad = 0;

	// 最大梯度值对应的象素点坐标
	int nPx;
	int nPy;

	nPx = 0;
	nPy = 0;

	// 求梯度最大值所在的象素点坐标
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dCurrGrad = pdGrad[y*nWidth + x] ;
			if( dMaxGrad< dCurrGrad )
			{
				dMaxGrad = dCurrGrad;
				nPx = x ;
				nPy = y ; 
			}
		}
	}

	// 初始化
	memset(pUnEdgeTrack,0,sizeof(unsigned char)*nWidth*nHeight);
	dCurrGrad = pdGrad[nPy*nWidth + nPx] ;

	// 从（nPx，nPy）点开始进行边界跟踪
	pUnEdgeTrack[nPy*nWidth + nPx] = 255 ;

	// 循环变量，遍历当前象素的8邻域
	int i ;

	int yy;
	int xx;
	
	int nDetX;
	int nDetY;

	while(dCurrGrad>10)
	{
		// 设置当前点为边界点
		pUnEdgeTrack[nPy*nWidth + nPx] = 255 ;

		dMaxGrad = 0 ;
		for(i=0; i<8; i++)
		{
			nDetX=nDx[i];
			nDetY=nDy[i];
			y = nPy + nDetY;
			x = nPx + nDetX;

			// 判断是否在图像内部
			if(x>=0 && x<nWidth && y>=0 && y<nHeight)
			{
				if( ( pdGrad[y*nWidth + x] > dMaxGrad)  && ( pUnEdgeTrack[y*nWidth + x] == 0) )
				{
					dMaxGrad = pdGrad[y*nWidth + x] ;
					yy = y;
					xx = x;
				}
			}
		}
		// 下一个边界点的梯度，横纵坐标
		dCurrGrad = dMaxGrad ;
		nPy = yy;
		nPx = xx;
	}

	//释放内存
	delete pdGrad;
	pdGrad = NULL;

}

/*************************************************************************
 *
 * \函数名称：
 *   MakeGauss()
 *
 * \输入参数:
 *   double sigma									        - 高斯函数的标准差
 *   double **pdKernel										- 指向高斯数据数组的指针
 *   int *pnWindowSize										- 数据的长度
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   这个函数可以生成一个一维的高斯函数的数字数据，理论上高斯数据的长度应
 *   该是无限长的，但是为了计算的简单和速度，实际的高斯数据只能是有限长的
 *   pnWindowSize就是数据长度
 *   
 *************************************************************************
 */
void MakeGauss(double sigma, double **pdKernel, int *pnWindowSize)
{
	// 循环控制变量
	int i   ;
	
	// 数组的中心点
	int nCenter;

	// 数组的某一点到中心点的距离
	double  dDis  ; 

	double PI = 3.14159;
	// 中间变量
	double  dValue; 
	double  dSum  ;
	dSum = 0 ; 
	
	// 数组长度，根据概率论的知识，选取[-3*sigma, 3*sigma]以内的数据。
	// 这些数据会覆盖绝大部分的滤波系数
	*pnWindowSize = 1 + 2 * ceil(3 * sigma);
	
	// 中心
	nCenter = (*pnWindowSize) / 2;
	
	// 分配内存
	*pdKernel = new double[*pnWindowSize] ;
	
	for(i=0; i< (*pnWindowSize); i++)
	{
		dDis = (double)(i - nCenter);
		dValue = exp(-(1/2)*dDis*dDis/(sigma*sigma)) / (sqrt(2 * PI) * sigma );
		(*pdKernel)[i] = dValue ;
		dSum += dValue;
	}
	
	// 归一化
	for(i=0; i<(*pnWindowSize) ; i++)
	{
		(*pdKernel)[i] /= dSum;
	}
}

/*************************************************************************
 *
 * \函数名称：
 *   GaussianSmooth()
 *
 * \输入参数:
 *   unsigned char * pUnchImg				- 指向图象数据的指针
 *   int nWidth											- 图象数据宽度
 *   int nHeight										- 图象数据高度
 *   double dSigma									- 高斯函数的标准差
 *   unsigned char * pUnchSmthdImg	- 指向经过平滑之后的图象数据
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   为了抑止噪声，采用高斯滤波对图象进行滤波，滤波先对x方向进行，然后对
 *   y方向进行。
 *   
 *************************************************************************
 */
void GaussianSmooth(unsigned char *pUnchImg, int nWidth, int nHeight, 
										double sigma, unsigned char * pUnchSmthdImg)
{
	// 循环控制变量
  int y;
	int x;
	
	int i;
	
	// 高斯滤波器的数组长度
	
	int nWindowSize;
	
	//  窗口长度的1/2
	int	nHalfLen;   
	
	// 一维高斯数据滤波器
	double *pdKernel ;
	
	// 高斯系数与图象数据的点乘
	double  dDotMul     ;
	
	// 高斯滤波系数的总和
	double  dWeightSum     ;          
  
	// 中间变量
	double * pdTmp ;
	
	// 分配内存
	pdTmp = new double[nWidth*nHeight];
	
	// 产生一维高斯数据滤波器
	// MakeGauss(sigma, &dKernel, &nWindowSize);
	MakeGauss(sigma, &pdKernel, &nWindowSize) ;
	
	// MakeGauss返回窗口的长度，利用此变量计算窗口的半长
	nHalfLen = nWindowSize / 2;
	
  // x方向进行滤波
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dDotMul		= 0;
			dWeightSum = 0;
			for(i=(-nHalfLen); i<=nHalfLen; i++)
			{
				// 判断是否在图象内部
				if( (i+x) >= 0  && (i+x) < nWidth )
				{
					dDotMul += (double)pUnchImg[y*nWidth + (i+x)] * pdKernel[nHalfLen+i];
					dWeightSum += pdKernel[nHalfLen+i];
				}
			}
			pdTmp[y*nWidth + x] = dDotMul/dWeightSum ;
		}
	}
	
	// y方向进行滤波
	for(x=0; x<nWidth; x++)
	{
		for(y=0; y<nHeight; y++)
		{
			dDotMul		= 0;
			dWeightSum = 0;
			for(i=(-nHalfLen); i<=nHalfLen; i++)
			{
				// 判断是否在图象内部
				if( (i+y) >= 0  && (i+y) < nHeight )
				{
					dDotMul += (double)pdTmp[(y+i)*nWidth + x] * pdKernel[nHalfLen+i];
					dWeightSum += pdKernel[nHalfLen+i];
				}
			}
			pUnchSmthdImg[y*nWidth + x] = (unsigned char)(int)dDotMul/dWeightSum ;
		}
	}

	// 释放内存
	delete []pdKernel;
	pdKernel = NULL ;
	
	delete []pdTmp;
	pdTmp = NULL;
}


/*************************************************************************
 *
 * \函数名称：
 *   DirGrad()
 *
 * \输入参数:
 *   unsigned char *pUnchSmthdImg					- 经过高斯滤波后的图象
 *   int nWidht														- 图象宽度
 *   int nHeight      										- 图象高度
 *   int *pnGradX                         - x方向的方向导数
 *   int *pnGradY                         - y方向的方向导数
 * \返回值:
 *   无
 *
 * \说明:
 *   这个函数计算方向倒数，采用的微分算子是(-1 0 1) 和 (-1 0 1)'(转置)
 *   计算的时候对边界象素采用了特殊处理
 *   
 *   
 *************************************************************************
 */
void DirGrad(unsigned char *pUnchSmthdImg, int nWidth, int nHeight,
										 int *pnGradX , int *pnGradY)
{
	// 循环控制变量
	int y ;
	int x ;
	
	// 计算x方向的方向导数，在边界出进行了处理，防止要访问的象素出界
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pnGradX[y*nWidth+x] = (int) ( pUnchSmthdImg[y*nWidth+min(nWidth-1,x+1)]  
													          - pUnchSmthdImg[y*nWidth+max(0,x-1)]     );
		}
	}

	// 计算y方向的方向导数，在边界出进行了处理，防止要访问的象素出界
	for(x=0; x<nWidth; x++)
	{
		for(y=0; y<nHeight; y++)
		{
			pnGradY[y*nWidth+x] = (int) ( pUnchSmthdImg[min(nHeight-1,y+1)*nWidth + x]  
																		- pUnchSmthdImg[max(0,y-1)*nWidth+ x ]     );
		}
	}
}
/*************************************************************************
 *
 * \函数名称：
 *   GradMagnitude()
 *
 * \输入参数:
 *   int *pnGradX                         - x方向的方向导数
 *   int *pnGradY                         - y方向的方向导数
 *   int nWidht														- 图象宽度
 *   int nHeight      										- 图象高度
 *   int *pnMag                           - 梯度幅度   
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   这个函数利用方向倒数计算梯度幅度，方向倒数是DirGrad函数计算的结果
 *   
 *************************************************************************
 */
void GradMagnitude(int *pnGradX, int *pnGradY, int nWidth, int nHeight, int *pnMag)
{
	
	// 循环控制变量
	int y ;
	int x ;

	// 中间变量
	double dSqtOne;
	double dSqtTwo;
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dSqtOne = pnGradX[y*nWidth + x] * pnGradX[y*nWidth + x];
			dSqtTwo = pnGradY[y*nWidth + x] * pnGradY[y*nWidth + x];
			pnMag[y*nWidth + x] = (int)(sqrt(dSqtOne + dSqtTwo) + 0.5);
		}
	}
}
/*************************************************************************
 *
 * \函数名称：
 *   NonmaxSuppress()
 *
 * \输入参数:
 *   int *pnMag                - 梯度图
 *   int *pnGradX							 - x方向的方向导数	
 *   int *pnGradY              - y方向的方向导数
 *   int nWidth                - 图象数据宽度
 *   int nHeight               - 图象数据高度
 *   unsigned char *pUnchRst   - 经过NonmaxSuppress处理后的结果
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   抑止梯度图中非局部极值点的象素。
 *   
 *************************************************************************
 */
void NonmaxSuppress(int *pnMag, int *pnGradX, int *pnGradY, int nWidth, 
										int nHeight,	unsigned char *pUnchRst)
{
	// 循环控制变量
	int y ;
	int x ;
	int nPos;

	// x方向梯度分量
	int gx  ;
	int gy  ;

	// 临时变量
	int g1, g2, g3, g4 ;
	double weight  ;
	double dTmp1   ;
	double dTmp2   ;
	double dTmp    ;
	
	// 设置图象边缘部分为不可能的边界点
	for(x=0; x<nWidth; x++)		
	{
		pUnchRst[x] = 0 ;
		pUnchRst[nHeight-1+x] = 0;
	}
	for(y=0; y<nHeight; y++)		
	{
		pUnchRst[y*nWidth] = 0 ;
		pUnchRst[y*nWidth + nWidth-1] = 0;
	}

	for(y=1; y<nHeight-1; y++)
	{
		for(x=1; x<nWidth-1; x++)
		{
			nPos = y*nWidth + x ;
			
			// 如果当前象素的梯度幅度为0，则不是边界点
			if(pnMag[nPos] == 0 )
			{
				pUnchRst[nPos] = 0 ;
			}
			else
			{
				// 当前象素的梯度幅度
				dTmp = pnMag[nPos] ;
				
				// x，y方向导数
				gx = pnGradX[nPos]  ;
				gy = pnGradY[nPos]  ;

				// 如果方向导数y分量比x分量大，说明导数的方向更加“趋向”于y分量。
				if (abs(gy) > abs(gx)) 
				{
					// 计算插值的比例
					weight = fabs(gx)/fabs(gy); 

					g2 = pnMag[nPos-nWidth] ; 
					g4 = pnMag[nPos+nWidth] ;
					
					// 如果x，y两个方向的方向导数的符号相同
					// C是当前象素，与g1-g4的位置关系为：
					//	g1 g2 
					//		 C         
					//		 g4 g3 
					if (gx*gy > 0) 
					{ 					
						g1 = pnMag[nPos-nWidth-1] ;
						g3 = pnMag[nPos+nWidth+1] ;
					} 

					// 如果x，y两个方向的方向导数的符号相反
					// C是当前象素，与g1-g4的位置关系为：
					//	   g2 g1
					//		 C         
					//	g3 g4  
					else 
					{ 
						g1 = pnMag[nPos-nWidth+1] ;
						g3 = pnMag[nPos+nWidth-1] ;
					} 
				}
				
				// 如果方向导数x分量比y分量大，说明导数的方向更加“趋向”于x分量
				// 这个判断语句包含了x分量和y分量相等的情况
				else
				{
					// 计算插值的比例
					weight = fabs(gy)/fabs(gx); 
					
					g2 = pnMag[nPos+1] ; 
					g4 = pnMag[nPos-1] ;
					
					// 如果x，y两个方向的方向导数的符号相同
					// C是当前象素，与g1-g4的位置关系为：
					//	g3   
					//	g4 C g2       
					//       g1
					if (gx*gy > 0) 
					{				
						g1 = pnMag[nPos+nWidth+1] ;
						g3 = pnMag[nPos-nWidth-1] ;
					} 
					// 如果x，y两个方向的方向导数的符号相反
					// C是当前象素，与g1-g4的位置关系为：
					//	     g1
					//	g4 C g2       
					//  g3     
					else 
					{ 
						g1 = pnMag[nPos-nWidth+1] ;
						g3 = pnMag[nPos+nWidth-1] ;
					}
				}

				// 下面利用g1-g4对梯度进行插值
				{
					dTmp1 = weight*g1 + (1-weight)*g2 ;
					dTmp2 = weight*g3 + (1-weight)*g4 ;
					
					// 当前象素的梯度是局部的最大值
					// 该点可能是个边界点
					if(dTmp>=dTmp1 && dTmp>=dTmp2)
					{
						pUnchRst[nPos] = 128 ;
					}
					else
					{
						// 不可能是边界点
						pUnchRst[nPos] = 0 ;
					}
				}
			} //else
		} // for

	}
} 

/*************************************************************************
 *
 * \函数名称：
 *   TraceEdge()
 *
 * \输入参数:
 *   int    x									- 跟踪起点的x坐标 
 *   int    y									- 跟踪起点的y坐标
 *   int nLowThd							- 判断一个点是否为边界点的低阈值
 *   unsigned char *pUnchEdge - 记录边界点的缓冲区
 *   int *pnMag               - 梯度幅度图
 *   int nWidth               - 图象数据宽度
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   递归调用  
 *   从(x,y)坐标出发，进行边界点的跟踪，跟踪只考虑pUnchEdge中没有处理并且
 *   可能是边界点的那些象素(=128),象素值为0表明该点不可能是边界点，象素值
 *   为255表明该点已经被设置为边界点，不必再考虑
 *   
 *   
 *************************************************************************
 */
void TraceEdge (int y, int x, int nLowThd, unsigned char *pUnchEdge, int *pnMag, int nWidth) 
{ 
	// 对8邻域象素进行查询
	int xNb[8] = {1, 1, 0,-1,-1,-1, 0, 1} ;
	int yNb[8] = {0, 1, 1, 1,0 ,-1,-1,-1} ;

	int yy ;
	int xx ;
	
	int k  ;
	
	for(k=0; k<8; k++)
	{
		yy = y + yNb[k] ;
		xx = x + xNb[k] ;
		// 如果该象素为可能的边界点，又没有处理过
		// 并且梯度大于阈值
		if(pUnchEdge[yy*nWidth+xx] == 128  && pnMag[yy*nWidth+xx]>=nLowThd)
		{
			// 把该点设置成为边界点
			pUnchEdge[yy*nWidth+xx] = 255 ;

			// 以该点为中心进行跟踪
			TraceEdge(yy, xx, nLowThd, pUnchEdge, pnMag, nWidth);
		}
	}
} 

/*************************************************************************
 *
 * \函数名称：
 *   EstimateThreshold()
 *
 * \输入参数:
 *   int *pnMag               - 梯度幅度图
 *	 int nWidth               - 图象数据宽度
 *	 int nHeight              - 图象数据高度
 *   int *pnThdHigh           - 高阈值
 *   int *pnThdLow            - 低阈值
 *	 double dRatioLow         - 低阈值和高阈值之间的比例
 *	 double dRatioHigh        - 高阈值占图象象素总数的比例
 *   unsigned char *pUnchEdge - 经过non-maximum处理后的数据
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   经过non-maximum处理后的数据pUnchEdge，统计pnMag的直方图，确定阈值。
 *   本函数中只是统计pUnchEdge中可能为边界点的那些象素。然后利用直方图，
 *   根据dRatioHigh设置高阈值，存储到pnThdHigh。利用dRationLow和高阈值，
 *   设置低阈值，存储到*pnThdLow。dRatioHigh是一种比例：表明梯度小于
 *   *pnThdHigh的象素数目占象素总数目的比例。dRationLow表明*pnThdHigh
 *   和*pnThdLow的比例，这个比例在canny算法的原文里，作者给出了一个区间。
 *
 *************************************************************************
 */
void EstimateThreshold(int *pnMag, int nWidth, int nHeight, int *pnThdHigh,int *pnThdLow, 
											 unsigned char * pUnchEdge, double dRatioHigh, double dRationLow) 
{ 
	// 循环控制变量
	int y;
	int x;
	int k;
	
	// 该数组的大小和梯度值的范围有关，如果采用本程序的算法，那么梯度的范围不会超过pow(2,10)
	int nHist[1024] ;

	// 可能的边界数目
	int nEdgeNb     ;

	// 最大梯度值
	int nMaxMag     ;

	int nHighCount  ;

	nMaxMag = 0     ; 
	
	// 初始化
	for(k=0; k<1024; k++) 
	{
		nHist[k] = 0; 
	}

	// 统计直方图，然后利用直方图计算阈值
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			// 只是统计那些可能是边界点，并且还没有处理过的象素
			if(pUnchEdge[y*nWidth+x]==128)
			{
				nHist[ pnMag[y*nWidth+x] ]++;
			}
		}
	}

	nEdgeNb = nHist[0]  ;
	nMaxMag = 0         ;
	// 统计经过“非最大值抑止(non-maximum suppression)”后有多少象素
	for(k=1; k<1024; k++)
	{
		if(nHist[k] != 0)
		{
			// 最大梯度值
			nMaxMag = k;
		}
		
		// 梯度为0的点是不可能为边界点的
		// 经过non-maximum suppression后有多少象素
		nEdgeNb += nHist[k];
	}

	// 梯度比高阈值*pnThdHigh小的象素点总数目
	nHighCount = (int)(dRatioHigh * nEdgeNb +0.5);
	
	k = 1;
	nEdgeNb = nHist[1];
	
	// 计算高阈值
	while( (k<(nMaxMag-1)) && (nEdgeNb < nHighCount) )
	{
		k++;
		nEdgeNb += nHist[k];
	}

	// 设置高阈值
	*pnThdHigh = k ;

	// 设置低阈值
	*pnThdLow  = (int)((*pnThdHigh) * dRationLow+ 0.5);
}

/*************************************************************************
 *
 * \函数名称：
 *   Hysteresis()
 *
 * \输入参数:
 *   int *pnMag               - 梯度幅度图
 *	 int nWidth               - 图象数据宽度
 *	 int nHeight              - 图象数据高度
 *	 double dRatioLow         - 低阈值和高阈值之间的比例
 *	 double dRatioHigh        - 高阈值占图象象素总数的比例
 *   unsigned char *pUnchEdge - 记录边界点的缓冲区
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   本函数实现类似“磁滞现象”的一个功能，也就是，先调用EstimateThreshold
 *   函数对经过non-maximum处理后的数据pUnchSpr估计一个高阈值，然后判断
 *   pUnchSpr中可能的边界象素(=128)的梯度是不是大于高阈值nThdHigh，如果比
 *   该阈值大，该点将作为一个边界的起点，调用TraceEdge函数，把对应该边界
 *   的所有象素找出来。最后，当整个搜索完毕时，如果还有象素没有被标志成
 *   边界点，那么就一定不是边界点。
 *   
 *************************************************************************
 */
void Hysteresis(int *pnMag, int nWidth, int nHeight, double dRatioLow, 
								double dRatioHigh, unsigned char *pUnchEdge)
{
	// 循环控制变量
	int y;
	int x;

	int nThdHigh ;
	int nThdLow  ;

	int nPos;

	// 估计TraceEdge需要的低阈值，以及Hysteresis函数使用的高阈值
	EstimateThreshold(pnMag, nWidth, nHeight, &nThdHigh, 
		               &nThdLow, pUnchEdge,dRatioHigh, dRatioLow);

  // 这个循环用来寻找大于nThdHigh的点，这些点被用来当作边界点，然后用
	// TraceEdge函数来跟踪该点对应的边界
   for(y=0; y<nHeight; y++)
	 {
      for(x=0; x<nWidth; x++)
			{
				nPos = y*nWidth + x ; 

				// 如果该象素是可能的边界点，并且梯度大于高阈值，该象素作为
				// 一个边界的起点
				if((pUnchEdge[nPos] == 128) && (pnMag[nPos] >= nThdHigh))
				{
					// 设置该点为边界点
					pUnchEdge[nPos] = 255;
					TraceEdge(y, x, nThdLow, pUnchEdge, pnMag, nWidth);
				}
      }
   }

	 // 那些还没有被设置为边界点的象素已经不可能成为边界点
   for(y=0; y<nHeight; y++)
	 {
		 for(x=0; x<nWidth; x++)
		 {
			 nPos = y*nWidth + x ; 
			 if(pUnchEdge[nPos] != 255)
			 {
				 // 设置为非边界点
				 pUnchEdge[nPos] = 0 ;
			 }
		 }
	 }
}


/*************************************************************************
 *
 * \函数名称：
 *   Canny()
 *
 * \输入参数:
 *   unsigned char *pUnchImage- 图象数据
 *	 int nWidth               - 图象数据宽度
 *	 int nHeight              - 图象数据高度
 *   double sigma             - 高斯滤波的标准方差
 *	 double dRatioLow         - 低阈值和高阈值之间的比例
 *	 double dRatioHigh        - 高阈值占图象象素总数的比例
 *   unsigned char *pUnchEdge - canny算子计算后的分割图
 *
 * \返回值:
 *   无
 *
 * \说明:
 *   canny分割算子，计算的结果保存在pUnchEdge中，逻辑1(255)表示该点为
 *   边界点，逻辑0(0)表示该点为非边界点。该函数的参数sigma，dRatioLow
 *   dRatioHigh，是需要指定的。这些参数会影响分割后边界点数目的多少
 *************************************************************************
 */
void Canny(unsigned char *pUnchImage, int nWidth, int nHeight, double sigma,
					 double dRatioLow, double dRatioHigh, unsigned char *pUnchEdge)
{
	// 经过高斯滤波后的图象数据
	unsigned char * pUnchSmooth ;
  
	// 指向x方向导数的指针
	int * pnGradX ; 

	// 指向y方向导数的指针
	int * pnGradY ;

	// 梯度的幅度
	int * pnGradMag ;

	pUnchSmooth  = new unsigned char[nWidth*nHeight] ;
	pnGradX      = new int [nWidth*nHeight]          ;
	pnGradY      = new int [nWidth*nHeight]          ;
	pnGradMag    = new int [nWidth*nHeight]          ;

	// 对原图象进行滤波
	GaussianSmooth(pUnchImage, nWidth, nHeight, sigma, pUnchSmooth) ;

	// 计算方向导数
	DirGrad(pUnchSmooth, nWidth, nHeight, pnGradX, pnGradY) ;

	// 计算梯度的幅度
	GradMagnitude(pnGradX, pnGradY, nWidth, nHeight, pnGradMag) ;

	// 应用non-maximum 抑制
	NonmaxSuppress(pnGradMag, pnGradX, pnGradY, nWidth, nHeight, pUnchEdge) ;

	// 应用Hysteresis，找到所有的边界
	Hysteresis(pnGradMag, nWidth, nHeight, dRatioLow, dRatioHigh, pUnchEdge);


	// 释放内存
	delete pnGradX      ;
	pnGradX      = NULL ;
	delete pnGradY      ;
	pnGradY      = NULL ;
	delete pnGradMag    ;
	pnGradMag    = NULL ;
	delete pUnchSmooth ;
	pUnchSmooth  = NULL ;
}
					
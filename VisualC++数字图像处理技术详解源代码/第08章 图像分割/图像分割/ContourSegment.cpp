#include "StdAfx.h"
#include "ContourSegment.h"

CContourSegment::CContourSegment(CDib *pDib)
{
    m_pDib = pDib;
}

CContourSegment::~CContourSegment(void)
{
}

//=======================================================
// 函数功能： 轮廓提取
// 输入参数： void
// 返回值：   void 
//=======================================================
void CContourSegment::ContourExtract(void)
{
    // 循环变量
	int i, j;

	// 原图像数据区指针
	LPBYTE p_data, lpSrc;
    p_data = m_pDib->GetData();

    // 图像每行像素所占的字节数
    int nLineByte = m_pDib->GetLineByte();

	// 图像的宽度
	int nWidth = m_pDib->GetWidth();

	// 图像的高度
	int nHeight = m_pDib->GetHeight();

    // 新图像缓冲区的指针
	LPBYTE lpTemp, lpDst; 
    lpTemp = new BYTE[nLineByte * nHeight];

    // 初始化新分配的内存，设定初始值为255
	memset(lpTemp, 255, nLineByte * nHeight);

    // 当前像素点的8邻域的像素值
	int n1,n2,n3,n4,n5,n6,n7,n8;

	// 像素值
	int nPixel;

	// 对原图进行二值化预处理
    for (j = 0; j < nHeight; j ++)
		for(i = 0; i< nWidth; i ++)
		{
			lpSrc = p_data + nLineByte * j + i;
			if(*lpSrc > 127)
				*lpSrc = 255;
			else
				*lpSrc = 0;
		}

    // 轮廓提取过程
	for(j = 1; j < nHeight-1; j ++)
		for(i = 1; i < nWidth-1; i ++)
		{
			// 指向原图像倒数第j行，第i列像素的指针			
			lpSrc = p_data + nLineByte * j + i;           

			// 指向新图像倒数第j行，第i列像素的指针			
			lpDst = lpTemp + nLineByte * j + i;

			// 获取原图当前指针所指像素的灰度值
			nPixel = *lpSrc;

			// 如果当前像素点为黑色，获取它的8邻域像素点
            if(nPixel == 0)
			{
				// 在新图像中置当前像素点为黑色
                *lpDst =0;
				n1 = *(lpSrc + nLineByte -1);
				n2 = *(lpSrc + nLineByte);
				n3 = *(lpSrc + nLineByte +1);
				n4 = *(lpSrc -1);
				n5 = *(lpSrc +1);
				n6 = *(lpSrc - nLineByte -1);
				n7 = *(lpSrc - nLineByte);
				n8 = *(lpSrc - nLineByte +1);

				// 如果8邻域的像素点都是黑色，在新图像中置当前像素点为白色
				if(n1+n2+n3+n4+n5+n6+n7+n8==0)
					*lpDst = 255;
			}
		}

	// 将处理后的新图像复制到原图像中
	memcpy(p_data,lpTemp, nLineByte * nHeight);

	// 释放内存
	delete [] lpTemp;	 
}

//=======================================================
// 函数功能： 边界跟踪
// 输入参数： void
// 返回值：   void 
//======================================================= 
void CContourSegment::ContourTrack(void)
{ 
    // 循环变量
	int i, j;

	// 原图像数据区指针
	LPBYTE p_data, lpSrc;
    p_data=m_pDib->GetData();

	// 图像每行像素所占的字节数
    int nLineByte = m_pDib->GetLineByte();

	// 图像的宽度
    int nWidth = m_pDib->GetWidth();
    
    // 图像的高度
	int nHeight = m_pDib->GetHeight();

    // 新图像缓冲区的指针
	LPBYTE lpTemp, lpDst; 
    lpTemp = new BYTE[nLineByte * nHeight];

    // 初始化新分配的内存，设定初始值为255
	memset(lpTemp, 255, nLineByte * nHeight);
    
	// 像素值
    int nPixel;

	// 是否找到起始边界点以及是否回到起始起始点
	BOOL bFindStartPoint;

	// 是否扫描到一个边界点
	BOOL bFindPoint;

	// 起始边界点和当前边界点
	CPoint StartPoint,CurrentPoint;

	// 顺时针定义中心像素点的8邻域坐标
	int Direction[8][2]={{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0}};

    // 起始方向
	int BeginDirect;

    // 对原图像做二值化预处理
	for (j = 0; j < nHeight; j ++)
		for(i = 0; i < nWidth; i ++)
		{
			lpSrc = p_data + nLineByte * j + i;
			if(*lpSrc > 127)
				*lpSrc = 255;
			else
				*lpSrc = 0;
		}
	
	// 寻找最左下方的黑色边界点
	bFindStartPoint = FALSE;
	for (j = 0; j < nHeight && !bFindStartPoint; j ++)
		for(i = 0; i < nWidth && !bFindStartPoint; i ++)
		{
			// 指向原图像倒数第j行，第i列像素的指针			
			lpSrc = p_data + nLineByte * j + i;

			// 获取原图当前指针所指像素的灰度值
			nPixel =  *lpSrc;
           
            // 如果当前像素点为黑色，则找到了最左下方的边界点
            if(nPixel ==0)
			{
                // 找到起始边界点
                bFindStartPoint= TRUE ;      
				StartPoint.y = j;
				StartPoint.x = i;

				// 指向新图像倒数第j行，第i列像素的指针			
				lpDst = lpTemp + nLineByte * j + i;

                // 在新图像中置当前像素点为黑色
				*lpDst = 0;                
			}           
		}

	// 由于起始点是在左下方，故起始扫描沿左上方向
    BeginDirect = 0;

	// 尚未回到起始边界点
	bFindStartPoint = FALSE;

	// 从起始边界点开始扫描
 	CurrentPoint.y = StartPoint.y;
	CurrentPoint.x = StartPoint.x;
	while(!bFindStartPoint)
	{
		// 尚未找到边界点
        bFindPoint = FALSE;
		while(!bFindPoint)
		{
			// 沿扫描方向查看一个像素               
			lpSrc = p_data +nLineByte * ( CurrentPoint.y + Direction[BeginDirect][1])
				  + (CurrentPoint.x + Direction[BeginDirect][0]);
			nPixel =  *lpSrc;

            // 跟踪原则
			if(nPixel== 0)
			{
				// 找到边界点，并将其设置为当前边界点
                bFindPoint = TRUE;
				CurrentPoint.y = CurrentPoint.y + Direction[BeginDirect][1];
				CurrentPoint.x = CurrentPoint.x + Direction[BeginDirect][0];

                // 如果当前边界点就是起始边界点，说明边界跟踪回到了起点，跟踪结束
				if(CurrentPoint.y == StartPoint.y && CurrentPoint.x == StartPoint.x)
					bFindStartPoint = TRUE;

                // 指向新图像对应当前边界点的指针，并设置其所指像素为黑色
				lpDst =  lpTemp + nLineByte * CurrentPoint.y + CurrentPoint.x;
				*lpDst = 0;

				// 沿扫描方向逆时针旋转90°
				BeginDirect -= 2;
				if(BeginDirect < 0)
					BeginDirect += 8;
			}
			else
			{
				// 沿扫描方向顺时针旋转45°
				BeginDirect++;
				if(BeginDirect == 8)
					BeginDirect = 0;
			}
		}
	}

	// 复制图像
	memcpy(p_data, lpTemp, nLineByte * nHeight);

	// 释放内存
    delete [] lpTemp; 
}

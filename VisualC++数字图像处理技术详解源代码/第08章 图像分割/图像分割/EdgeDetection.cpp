#include "StdAfx.h"
#include "EdgeDetection.h"
#include "math.h"

CEdgeDetection::CEdgeDetection(CDib *pDib)
{
    m_pDib = pDib;
}

CEdgeDetection::~CEdgeDetection(void)
{
}

//=======================================================
// 函数功能： Roberts边缘检测算子
// 输入参数： void
// 返回值：   void 
//=======================================================
void CEdgeDetection::Roberts(void)
{
    // 循环变量
	int i, j;

	// 原图像数据区指针
	LPBYTE lpData;
    lpData = m_pDib->GetData();

    // 图像每行像素所占的字节数
    int nLineByte = m_pDib->GetLineByte();

	// 图像的宽度
	int nWidth = m_pDib->GetWidth();

	// 图像的高度
	int nHeight = m_pDib->GetHeight();
    
    // 新图像缓冲区的指针
	LPBYTE lpTemp; 
    lpTemp = new BYTE[nLineByte * nHeight];

    // 初始化新分配的内存，设定初始值为255
	memset(lpTemp, 255, nLineByte * nHeight);

    // Roberts算子
    int pixel[4]; 

    // 由于使用2*2的模板，为防止越界，不处理最上边和最右边的边界像素
    for(j = 0; j < nHeight - 1; j ++)
	    for(i = 0; i < nWidth - 1; i ++)
		{
	        //生成Roberts算子
			pixel[0] = lpData[j * nLineByte + i];
            pixel[1] = lpData[j * nLineByte + i + 1];
			pixel[2] = lpData[(j + 1) * nLineByte + i];
			pixel[3] = lpData[(j + 1) * nLineByte + i + 1];

			// 处理当前像素
			lpTemp[j * nLineByte + i] = sqrt(double ( (pixel[0] - pixel[3]) * (pixel[0] - pixel[3])
                                    + (pixel[1] - pixel[2]) * (pixel[1] - pixel[2]) ) );
		}

	// 将处理后的新图像复制到原图像中
	memcpy(lpData,lpTemp, nLineByte * nHeight);

	// 释放内存
	delete [] lpTemp;
}

//=======================================================
// 函数功能： 灰度模板
// 输入参数： LPBYTE lpData: 指向原图像数据区指针 
//            int nLineByte: 原图像每行像素所占的字节数
//            int nWidth: 原图像宽度                                    
//            int nHeight: 原图像高度                                  
//            int nTemH: 模板高度                                     
//            int nTemW: 模板宽度                                     
//            int nTemCX: 模板中心元素的X坐标(<nTemW-1)               
//            int nTemCY: 模板的中心元素的Y坐标(<nTemH-1)               
//            float *fpArray: 指向模板数组的指针                      
//            float fCoef：模板系数         
// 返回值：   void 
//=======================================================
void CEdgeDetection::Template(LPBYTE lpData, int nLineByte, int nWidth, int nHeight, int nTemH, int nTemW, int nTemCX, int nTemCY, float *fpArray, float fCoef)
{   
	// 循环变量
    int i, j, k, l; 

    // 新图像缓冲区的指针
	LPBYTE lpTemp; 
    lpTemp = new BYTE[nLineByte * nHeight];

    // 初始化新分配的内存，设定初始值为255
	memset(lpTemp, 255, nLineByte * nHeight);

    // 像素值计算结果
    float fResult; 

	for(j = nTemCY; j < nHeight - nTemH + nTemCY + 1; j ++)
		for(i = nTemCX; i < nWidth - nTemW + nTemCX + 1; i ++)
		{
			// 计算当前像素值
			fResult = 0;

			for(k = 0; k < nTemH; k ++)
				for(l = 0; l < nTemW; l ++)
                    // 卷积运算
					fResult += lpData[(j - nTemCY + k) * nLineByte + (i - nTemCX + l)]
                             * fpArray[k * nTemW + l];
			
            //用系数乘以当前像素值计算结果
			fResult *= fCoef;

			//取绝对值
			fResult = (float) fabs(fResult);

			// 判断是否超过255
			if(fResult > 255)
				
                // 若超过255，在新图像当前像素位置直接赋值为255
                lpTemp[j * nLineByte + i] = 255;
			else
				
                // 未超过255，在新图像当前像素位置赋值为计算结果
				lpTemp[j * nLineByte + i] = (int) (fResult + 0.5);
		}

    // 将处理后的新图像复制到原图像中
	memcpy(lpData,lpTemp, nLineByte * nHeight);

	// 释放内存
	delete [] lpTemp;
}

//=======================================================
// 函数功能： Sobel边缘检测算子
// 输入参数： void
// 返回值：   void 
//=======================================================
void CEdgeDetection::Sobel()
{
	// 原图像数据区指针
	LPBYTE lpData;
    lpData = m_pDib->GetData();

    // 图像每行像素所占的字节数
    int nLineByte = m_pDib->GetLineByte();

	// 图像的宽度
	int nWidth = m_pDib->GetWidth();

	// 图像的高度
	int nHeight = m_pDib->GetHeight();
       
    // 模板宽度和高度
    int nTemW, nTemH; 

    // 模板系数
	float fCoef; 

    //模板中心元素的X坐标和Y坐标
	int nTemCX, nTemCY;  

	//模板数组
    float arTemplate[9]; 

    // 两幅新图像缓冲区的指针
	LPBYTE lpTemp1, lpTemp2; 
    lpTemp1 = new BYTE[nLineByte * nHeight];
    lpTemp2 = new BYTE[nLineByte * nHeight];

    // 初始化新分配的内存，复制原图像
	memcpy(lpTemp1, lpData, nLineByte * nHeight);
    memcpy(lpTemp2, lpData, nLineByte * nHeight);
  
	// 设置Sobel模板一的参数
	nTemW = 3;
	nTemH = 3;
	fCoef = 1.0;
	nTemCX = 1;
	nTemCY = 1;
	arTemplate[0] = -1.0;
    arTemplate[1] = -2.0;
	arTemplate[2] = -1.0;
	arTemplate[3] = 0.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = 0.0;
	arTemplate[6] = 1.0;
	arTemplate[7] = 2.0;
	arTemplate[8] = 1.0;

	// 调用Template()函数
	Template(lpTemp1, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef); 

    // 设置Sobel模板二的参数
    arTemplate[0] = 1.0;
    arTemplate[1] = 0.0;
	arTemplate[2] = -1.0;
	arTemplate[3] = 2.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = -2.0;
	arTemplate[6] = 1.0;
	arTemplate[7] = 0.0;
	arTemplate[8] = -1.0;

	// 调用Template()函数
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// 取两幅缓存图像对应像素的最大值，并存放在第一个缓存图像中
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

    // 将第一个缓存图像复制到原图像中
	memcpy(lpData, lpTemp1, nLineByte * nHeight);

	// 删除缓冲区
	delete  [] lpTemp1;
	delete  [] lpTemp2;
}

//=======================================================
// 函数功能： Prewitt边缘检测算子
// 输入参数： void
// 返回值：   void 
//=======================================================
void CEdgeDetection::Prewitt(void)
{
    // 原图像数据区指针
	LPBYTE lpData;
    lpData = m_pDib->GetData();

    // 图像每行像素所占的字节数
    int nLineByte = m_pDib->GetLineByte();

	// 图像的宽度
	int nWidth = m_pDib->GetWidth();

	// 图像的高度
	int nHeight = m_pDib->GetHeight();
       
    // 模板宽度和高度
    int nTemW, nTemH; 

    // 模板系数
	float fCoef; 

    //模板中心元素的X坐标和Y坐标
	int nTemCX, nTemCY;  

	//模板数组
    float arTemplate[9]; 

    // 两幅新图像缓冲区的指针
	LPBYTE lpTemp1, lpTemp2; 
    lpTemp1 = new BYTE[nLineByte * nHeight];
    lpTemp2 = new BYTE[nLineByte * nHeight];

    // 初始化新分配的内存，复制原图像
	memcpy(lpTemp1, lpData, nLineByte * nHeight);
    memcpy(lpTemp2, lpData, nLineByte * nHeight);
  
	// 设置Prewitt模板一的参数
	nTemW = 3;
	nTemH = 3;
	fCoef = 1.0;
	nTemCX = 1;
	nTemCY = 1;
	arTemplate[0] = -1.0;
    arTemplate[1] = -1.0;
	arTemplate[2] = -1.0;
	arTemplate[3] = 0.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = 0.0;
	arTemplate[6] = 1.0;
	arTemplate[7] = 1.0;
	arTemplate[8] = 1.0;

	// 调用Template()函数
	Template(lpTemp1, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef); 

    // 设置Prewitt模板二的参数
    arTemplate[0] = 1.0;
    arTemplate[1] = 0.0;
	arTemplate[2] = -1.0;
	arTemplate[3] = 1.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = -1.0;
	arTemplate[6] = 1.0;
	arTemplate[7] = 0.0;
	arTemplate[8] = -1.0;

	// 调用Template()函数
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// 取两幅缓存图像对应像素的最大值，并存放在第一个缓存图像中
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

    // 将第一个缓存图像复制到原图像中
	memcpy(lpData, lpTemp1, nLineByte * nHeight);

	// 删除缓冲区
	delete  [] lpTemp1;
	delete  [] lpTemp2;
}

//=======================================================
// 函数功能： Laplacian边缘检测算子
// 输入参数： void
// 返回值：   void 
//=======================================================
void CEdgeDetection::Laplacian(void)
{
    // 原图像数据区指针
	LPBYTE lpData;
    lpData = m_pDib->GetData();

    // 图像每行像素所占的字节数
    int nLineByte = m_pDib->GetLineByte();

	// 图像的宽度
	int nWidth = m_pDib->GetWidth();

	// 图像的高度
	int nHeight = m_pDib->GetHeight();
       
    // 模板宽度和高度
    int nTemW, nTemH; 

    // 模板系数
	float fCoef; 

    //模板中心元素的X坐标和Y坐标
	int nTemCX, nTemCY;  

	//模板数组
    float arTemplate[9]; 

    // 新图像缓冲区的指针
	LPBYTE lpTemp; 
    lpTemp = new BYTE[nLineByte * nHeight];

    // 初始化新分配的内存，复制原图像
	memcpy(lpTemp, lpData, nLineByte * nHeight);
  
	// 设置Laplacian模板的参数
	nTemW = 3;
	nTemH = 3;
	fCoef = 1.0;
	nTemCX = 1;
	nTemCY = 1;
	arTemplate[0] = -1.0;
    arTemplate[1] = -1.0;
	arTemplate[2] = -1.0;
	arTemplate[3] = -1.0;
	arTemplate[4] = 8.0;
	arTemplate[5] = -1.0;
	arTemplate[6] = -1.0;
	arTemplate[7] = -1.0;
	arTemplate[8] = -1.0;

	// 调用Template()函数
	Template(lpTemp, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef); 

    // 将缓存图像复制到原图像中
	memcpy(lpData, lpTemp, nLineByte * nHeight);

	// 删除缓冲区
	delete  [] lpTemp;
}

//=======================================================
// 函数功能： GuassLaplacian边缘检测算子
// 输入参数： void
// 返回值：   void 
//=======================================================
void CEdgeDetection::GuassLaplacian(void)
{
    // 原图像数据区指针
	LPBYTE lpData;
    lpData = m_pDib->GetData();

    // 图像每行像素所占的字节数
    int nLineByte = m_pDib->GetLineByte();

	// 图像的宽度
	int nWidth = m_pDib->GetWidth();

	// 图像的高度
	int nHeight = m_pDib->GetHeight();
       
    // 模板宽度和高度
    int nTemW, nTemH; 

    // 模板系数
	float fCoef; 

    //模板中心元素的X坐标和Y坐标
	int nTemCX, nTemCY;  

	//模板数组
    float arTemplate[25]; 

    // 新图像缓冲区的指针
	LPBYTE lpTemp; 
    lpTemp = new BYTE[nLineByte * nHeight];

    // 初始化新分配的内存，复制原图像
	memcpy(lpTemp, lpData, nLineByte * nHeight);
  
	// 设置GuassLaplacian模板的参数
	nTemW = 5;
	nTemH = 5;
	fCoef = 0.25;
	nTemCX = 4;
	nTemCY = 4;
	arTemplate[0] = -2.0;
    arTemplate[1] = -4.0;
	arTemplate[2] = -4.0;
	arTemplate[3] = -4.0;
	arTemplate[4] = -2.0;
	arTemplate[5] = -4.0;
	arTemplate[6] = 0.0;
	arTemplate[7] = 8.0;
	arTemplate[8] = 0.0;
    arTemplate[9] = -4.0;
    arTemplate[10] = -4.0;
	arTemplate[11] = 8.0;
	arTemplate[12] = 24.0;
	arTemplate[13] = 8.0;
	arTemplate[14] = -4.0;
	arTemplate[15] = -4.0;
	arTemplate[16] = 0.0;
	arTemplate[17] = 8.0;
    arTemplate[18] = 0.0;
    arTemplate[19] = -4.0;
	arTemplate[20] = -2.0;
	arTemplate[21] = -4.0;
	arTemplate[22] = -4.0;
	arTemplate[23] = -4.0;
	arTemplate[24] = -2.0;

	// 调用Template()函数
	Template(lpTemp, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef); 

    // 将缓存图像复制到原图像中
	memcpy(lpData, lpTemp, nLineByte * nHeight);

	// 删除缓冲区
	delete  [] lpTemp;
}

//=======================================================
// 函数功能： Krisch边缘检测算子
// 输入参数： void
// 返回值：   void 
//=======================================================
void CEdgeDetection::Krisch(void)
{
    // 原图像数据区指针
	LPBYTE lpData;
    lpData = m_pDib->GetData();

    // 图像每行像素所占的字节数
    int nLineByte = m_pDib->GetLineByte();

	// 图像的宽度
	int nWidth = m_pDib->GetWidth();

	// 图像的高度
	int nHeight = m_pDib->GetHeight(); 
       
    // 模板宽度和高度
    int nTemW, nTemH; 

    // 模板系数
	float fCoef; 

    //模板中心元素的X坐标和Y坐标
	int nTemCX, nTemCY;  

	//模板数组
    float arTemplate[9]; 

    // 两幅新图像缓冲区的指针
	LPBYTE lpTemp1, lpTemp2; 
    lpTemp1 = new BYTE[nLineByte * nHeight];
    lpTemp2 = new BYTE[nLineByte * nHeight];

    // 初始化新分配的内存，复制原图像
	memcpy(lpTemp1, lpData, nLineByte * nHeight);
    memcpy(lpTemp2, lpData, nLineByte * nHeight);
  
	// 设置Krisch模板一的参数
	nTemW = 3;
	nTemH = 3;
	fCoef = 0.5;
	nTemCX = 1;
	nTemCY = 1;
	arTemplate[0] = 5.0;
    arTemplate[1] = 5.0;
	arTemplate[2] = 5.0;
	arTemplate[3] = -3.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = -3.0;
	arTemplate[6] = -3.0;
	arTemplate[7] = -3.0;
	arTemplate[8] = -3.0;

	// 调用Template()函数
	Template(lpTemp1, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef); 

    // 设置Krisch模板二的参数
    arTemplate[0] = -3.0;
    arTemplate[1] = 5.0;
	arTemplate[2] = 5.0;
	arTemplate[3] = -3.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = 5.0;
	arTemplate[6] = -3.0;
	arTemplate[7] = -3.0;
	arTemplate[8] = -3.0;

	// 调用Template()函数
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// 取两幅缓存图像对应像素的最大值，并存放在第一个缓存图像中
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

    // 将原图像复制到第二个缓存图像中
	memcpy(lpTemp2, lpData, nLineByte * nHeight);

    // 设置Krisch模板三的参数
    arTemplate[0] = -3.0;
    arTemplate[1] = -3.0;
	arTemplate[2] = 5.0;
	arTemplate[3] = -3.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = 5.0;
	arTemplate[6] = -3.0;
	arTemplate[7] = -3.0;
	arTemplate[8] = 5.0;

	// 调用Template()函数
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// 取两幅缓存图像对应像素的最大值，并存放在第一个缓存图像中
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

    // 将原图像复制到第二个缓存图像中
	memcpy(lpTemp2, lpData, nLineByte * nHeight);

    // 设置Krisch模板四的参数
    arTemplate[0] = -3.0;
    arTemplate[1] = -3.0;
	arTemplate[2] = -3.0;
	arTemplate[3] = -3.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = 5.0;
	arTemplate[6] = -3.0;
	arTemplate[7] = 5.0;
	arTemplate[8] = 5.0;

	// 调用Template()函数
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// 取两幅缓存图像对应像素的最大值，并存放在第一个缓存图像中
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

    // 将原图像复制到第二个缓存图像中
	memcpy(lpTemp2, lpData, nLineByte * nHeight);

    // 设置Krisch模板五的参数
    arTemplate[0] = -3.0;
    arTemplate[1] = -3.0;
	arTemplate[2] = -3.0;
	arTemplate[3] = -3.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = -3.0;
	arTemplate[6] = 5.0;
	arTemplate[7] = 5.0;
	arTemplate[8] = 5.0;

	// 调用Template()函数
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// 取两幅缓存图像对应像素的最大值，并存放在第一个缓存图像中
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
		    if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

    // 将原图像复制到第二个缓存图像中
	memcpy(lpTemp2, lpData, nLineByte * nHeight);

    // 设置Krisch模板六的参数
    arTemplate[0] = -3.0;
    arTemplate[1] = -3.0;
	arTemplate[2] = -3.0;
	arTemplate[3] = 5.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = -3.0;
	arTemplate[6] = 5.0;
	arTemplate[7] = 5.0;
	arTemplate[8] = -3.0;

	// 调用Template()函数
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// 取两幅缓存图像对应像素的最大值，并存放在第一个缓存图像中
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

     // 将原图像复制到第二个缓存图像中
	memcpy(lpTemp2, lpData, nLineByte * nHeight);

    // 设置Krisch模板七的参数
    arTemplate[0] = 5.0;
    arTemplate[1] = -3.0;
	arTemplate[2] = -3.0;
	arTemplate[3] = 5.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = -3.0;
	arTemplate[6] = 5.0;
	arTemplate[7] = -3.0;
	arTemplate[8] = -3.0;

	// 调用Template()函数
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// 取两幅缓存图像对应像素的最大值，并存放在第一个缓存图像中
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

     // 将原图像复制到第二个缓存图像中
	memcpy(lpTemp2, lpData, nLineByte * nHeight);

    // 设置Krisch模板八的参数
    arTemplate[0] = 5.0;
    arTemplate[1] = 5.0;
	arTemplate[2] = -3.0;
	arTemplate[3] = 5.0;
	arTemplate[4] = 0.0;
	arTemplate[5] = -3.0;
	arTemplate[6] = -3.0;
	arTemplate[7] = -3.0;
	arTemplate[8] = -3.0;

	// 调用Template()函数
	Template(lpTemp2, nLineByte, nWidth, nHeight, nTemH, nTemW, nTemCX, nTemCY, arTemplate, fCoef);

	// 取两幅缓存图像对应像素的最大值，并存放在第一个缓存图像中
	for(int j = 0; j < nHeight; j ++)
		for(int i = 0; i < nWidth; i ++)
			if( lpTemp2[j * nLineByte + i] > lpTemp1[j * nLineByte + i] )
				 lpTemp1[j * nLineByte + i] = lpTemp2[j * nLineByte + i];

    // 将第一个缓存图像复制到原图像中
	memcpy(lpData, lpTemp1, nLineByte * nHeight);

	// 删除缓冲区
	delete  [] lpTemp1;
	delete  [] lpTemp2;
}
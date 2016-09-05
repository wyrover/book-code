#include "StdAfx.h"
#include "Threshold.h"
#include "math.h"

CThreshold::CThreshold()
{
}

CThreshold::CThreshold(CDib *pDib)
{
    m_pDib = pDib;
}

CThreshold::~CThreshold(void)
{
}

//=======================================================
// 函数功能： 最大方差阈值分割
// 输入参数： 无
// 返回值：   无
//=======================================================
void CThreshold::OtusThreshold(void)
{
    // 循环变量
	int i, j;

    // 原图数据区指针
    LPBYTE p_data;
    p_data = m_pDib->GetData();

    // 图像每行像素所占的字节数
    int nLineByte = m_pDib->GetLineByte();

        // 图像的宽度
    int nWidth = m_pDib->GetWidth();

    // 图像的高度
    int nHeight = m_pDib->GetHeight();

	// 灰度直方图数组，并初始化
	int nGrayHistogram[256];
	memset(nGrayHistogram, 0, sizeof(nGrayHistogram));
     
	// 统计各个灰度级对应的像素个数，并存放到灰度直方图数组中
    int nPixel;
	for (j = 0; j < nHeight; j ++)
	    for (i = 0; i < nWidth; i ++)
	    {
		    // 获取当前像素点的灰度值
            nPixel = p_data[nLineByte * j + i];	

		    // 对灰度值统计计数
		    nGrayHistogram[nPixel] ++;		
	    }

    // c0组和c1组的均值
    float u0, u1;

    // c0组和c1组的概率
    float w0, w1;

    // c0组的像素总数
    int nCount0;

    // 阈值和最佳阈值（对应方差最大时的阈值）
    int nT, nBestT;

    // 方差和最大方差
    float fVaria, fMaxVaria = 0;

    // 统计直方图中像素点的总数，并存放到nSum中
    int nSum=0;
    for(i = 0; i < 256; i ++)
        nSum += nGrayHistogram[i];


    // 令阈值nT从0遍历到255
    for(nT = 0; nT < 256; nT ++)
    {
        // 当阈值为nT时，计算c0组的均值和概率
        u0 = 0;
        nCount0 = 0;
        for(i = 0; i <= nT; i++)
        {
            u0 += i * nGrayHistogram[i];
            nCount0 += nGrayHistogram[i];
        }
        u0 /= nCount0;
        w0 = (float) nCount0 / nSum;

        // 当阈值为nT时，计算c1组的均值和概率
        u1 = 0;
        for(i = nT+1; i < 256; i ++)
             u1 += i * nGrayHistogram[i];
        u1 /= (nSum - nCount0);
        w1 = 1 - w0;

        // 计算两组间的方差
        fVaria = w0 * w1 * (u0 - u1) * (u0 - u1);

        // 记录最大方差和最佳阈值
        if(fVaria > fMaxVaria)
        {
            fMaxVaria = fVaria;
            nBestT = nT;
        }
    }

    // 利用最佳阈值对原图像作分割处理
    for(j = 0; j < nHeight; j ++)
        for(i = 0; i < nWidth; i ++)
        {
            if(p_data[j * nLineByte + i] < nBestT)
                p_data[j * nLineByte + i] = 0;
            else
                p_data[j * nLineByte + i] = 255;
        }
}


//=======================================================
// 函数功能： 自适应阈值分割
// 输入参数： 无
// 返回值：   无
//=======================================================
void CThreshold::AdaptiveThreshold(void)
{
    // 循环变量
    int i,j;
    
    // 原图像数据区指针
    LPBYTE p_data;
    p_data = m_pDib->GetData();

    // 图像每行像素所占的字节数
    int nLineByte = m_pDib->GetLineByte();

    // 图像的宽度
    int nWidth = m_pDib->GetWidth();

    // 图像的高度
    int nHeight = m_pDib->GetHeight();

    // 局部阈值
    int nThreshold[2][2];

    // 子图像的灰度平均值
    int nAvgValue;

    // 对左上图像逐点扫描,计算该子图像的灰度平均值
    nAvgValue = 0;
    for(j = nHeight / 2; j < nHeight; j ++)
        for(i = 0; i < nWidth / 2; i ++)
            nAvgValue += p_data[j * nLineByte + i];
    nAvgValue /= ((nHeight / 2) * (nLineByte / 2));

    // 设置阈值为子图像的平均值
    nThreshold[0][0] = nAvgValue;

    // 对左上图像逐点扫描并进行阈值分割
    for(j = nHeight / 2; j < nHeight; j ++)
        for(i = 0; i < nWidth / 2; i ++)
        {
            if(p_data[j * nLineByte + i] < nThreshold[0][0])
                p_data[j * nLineByte + i] = 0;
            else
                p_data[j * nLineByte + i] = 255;
        }

    // 对右上图像逐点扫描,计算该子图像的灰度平均值
    nAvgValue = 0;
    for(j = nHeight / 2; j < nHeight; j ++)
        for(i = nWidth / 2; i < nWidth; i ++)
            nAvgValue += p_data[j * nLineByte + i];
    nAvgValue /= ((nHeight / 2) * (nLineByte / 2));

    // 设置阈值为子图像的平均值
    nThreshold[0][1] = nAvgValue;

    // 对右上图像逐点扫描并进行阈值分割
    for(j = nHeight / 2; j < nHeight; j ++)
        for(i = nWidth / 2; i < nWidth; i ++)
        {
            if(p_data[j * nLineByte + i] < nThreshold[0][0])
                p_data[j * nLineByte + i] = 0;
            else
                p_data[j * nLineByte + i] = 255;
        }

    // 对左下图像逐点扫描,计算该子图像的灰度平均值
    nAvgValue = 0;
    for(j = 0; j < nHeight / 2; j ++)
        for(i = 0; i < nWidth / 2; i ++)
            nAvgValue += p_data[j * nLineByte + i];
    nAvgValue /= ((nHeight / 2) * (nLineByte / 2));

    // 设置阈值为子图像的平均值
    nThreshold[1][0] = nAvgValue;

    // 对左下图像逐点扫描并进行阈值分割
    for(j = 0; j < nHeight / 2; j ++)
        for(i = 0; i < nWidth / 2; i ++)
        {
            if(p_data[j * nLineByte + i] < nThreshold[0][0])
                p_data[j * nLineByte + i] = 0;
            else
                p_data[j * nLineByte + i] = 255;
        }

    // 对右下图像逐点扫描,计算该子图像的灰度平均值
    nAvgValue = 0;
    for(j = 0; j < nHeight / 2; j ++)
        for(i = nWidth / 2; i < nWidth; i ++)
            nAvgValue += p_data[j * nLineByte + i];
    nAvgValue /= ((nHeight / 2) * (nLineByte / 2));

    // 设置阈值为子图像的平均值
    nThreshold[1][1] = nAvgValue;

    // 对右下下图像逐点扫描并进行阈值分割
    for(j = 0; j < nHeight / 2; j ++)
        for(i = nWidth / 2; i < nWidth;i ++)
        {
            if(p_data[j * nLineByte + i] < nThreshold[0][0])
                p_data[j * nLineByte + i] = 0;
            else
                p_data[j * nLineByte + i] = 255;
        }
}
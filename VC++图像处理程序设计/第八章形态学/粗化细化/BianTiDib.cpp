#include "stdafx.h"
#include "windowsx.h"
#include "math.h"
#include "BianTiDib.h"
#include "MainFrm.h"
#include "DynSplitView2.h"

 
 
BianTiDib::BianTiDib()
{
	 
}
BianTiDib::~BianTiDib()
{
}

 

///***************************************************************/           
/*函数名称：Xihua()                                       
/*函数类型：void                                      
/*功能：对图像进行细化处理。            
/***************************************************************/
void BianTiDib::Xihua()
{
  	// 循环变量
	LONG i;
	LONG j;
	LONG m;
	LONG n;
	// 5×5相邻区域像素值
	unsigned char S[5][5];
	// 计数器
	unsigned char Num;
	// 循环跳出标志
	BOOL fp=TRUE;
	// 指向DIB象素指针
	LPBYTE  p_data;
	// 指向源图像的指针
	LPBYTE	lpSrc;
	// 指向缓存图像的指针
	LPBYTE	lpDst;
	// 指向缓存DIB图像的指针
	LPBYTE	temp;
	// 找到DIB图像象素起始位置
	p_data = this->GetData ();  
    if(m_pBitmapInfoHeader->biBitCount<9)//灰度图像
	{ 
		// DIB的宽度
		LONG wide = this->GetWidth ();
		// DIB的高度
		LONG height = this->GetHeight ();
		// 暂时分配内存，以保存新图像
		temp = new BYTE [wide *height];
		// 初始化新分配的内存，设定初始值为255
		lpDst = temp;
		memset(lpDst, (BYTE)255, wide  * height);
		while (fp)
		{
			fp = FALSE;
			// 初始化新分配的内存，设定初始值为255
			lpDst = temp;
			memset(lpDst, (BYTE)255, wide  * height);
			// 由于使用5×5的结构元素，为防越界，不处理外围的2行、2列像素
			for (j = 2; j < height - 2; j++)
			{
				for (i = 2 ; i < wide  - 2 ; i ++)
				{
					// 指向源图像倒数第j行，第i个象素的指针			
					lpSrc = (LPBYTE)(p_data + wide  *j + i);
					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (LPBYTE)(temp + wide  * j + i);
					// 如果源图像中当前点为白色，则跳过
					if (*lpSrc > 127)
						continue;
					// 获得当前点相邻的5×5区域内像素值，白色用0代表，黑色用1代表
					for (m = 0; m < 5; m++)
					{
						for (n = 0; n < 5; n++)
						{
							if (*(lpSrc + (2- m) * wide  + (n - 2)*2 ) > 127)
								S[m][n] = 0;
							else
								S[m][n] = 1;
						}
					}
					// 判断条件一是否成立：
					Num =  S[1][1] + S[1][2] + S[1][3] + S[2][1] 
						+ S[2][3] + S[3][1]	+ S[3][2] + S[3][3];
					if (Num < 2 || Num >6)
					{
						*lpDst = 0;
						continue;
					}
					// 判断条件二是否成立：
					Num = 0;
					if (S[1][2] == 0 && S[1][1] == 1) 
						Num++;
					if (S[1][1] == 0 && S[2][1] == 1)
						Num++;
					if (S[2][1] == 0 && S[3][1] == 1)
						Num++;
					if (S[3][1] == 0 && S[3][2] == 1)
						Num++;
					if (S[3][2] == 0 && S[3][3] == 1)
						Num++;
					if (S[3][3] == 0 && S[2][3] == 1)
						Num++;
					if (S[2][3] == 0 && S[1][3] == 1)
						Num++;
					if (S[1][3] == 0 && S[1][2] == 1)
						Num++;
					if (Num != 1)
					{
						*lpDst = 0;
						continue;
					}
					// 判断条件三是否成立；
					if (S[1][2] * S[2][1] * S[2][3] != 0)
					{
						Num = 0;
						if (S[0][2] == 0 && S[0][1] == 1)
							Num++;
						if (S[0][1] == 0 && S[1][1] == 1)
							Num++;
						if (S[1][1] == 0 && S[2][1] == 1)
							Num++;
						if (S[2][1] == 0 && S[2][2] == 1)
							Num++;
						if (S[2][2] == 0 && S[2][3] == 1)
							Num++;
						if (S[2][3] == 0 && S[1][3] == 1)
							Num++;
						if (S[1][3] == 0 && S[0][3] == 1)
							Num++;
						if (S[0][3] == 0 && S[0][2] == 1)
							Num++;
						if (Num == 1)
						{
							*lpDst = 0;
							continue;
						}
					}
					// 判断条件四是否成立：
					if (S[1][2] * S[2][1] * S[3][2] != 0)
					{
						Num = 0;
						if (S[1][1] == 0 && S[1][0] == 1)
							Num++;
						if (S[1][0] == 0 && S[2][0] == 1)
							Num++;
						if (S[2][0] == 0 && S[3][0] == 1)
							Num++;
						if (S[3][0] == 0 && S[3][1] == 1)
							Num++;
						if (S[3][1] == 0 && S[3][2] == 1)
							Num++;
						if (S[3][2] == 0 && S[2][2] == 1)
							Num++;
						if (S[2][2] == 0 && S[1][2] == 1)
							Num++;
						if (S[1][2] == 0 && S[1][1] == 1)
							Num++;
						if (Num == 1)
						{
							*lpDst = 0;
							continue;
						}
					}
					// 如果条件均满足则删除该点
					*lpDst = 255;
					fp = TRUE;
				}
			}
			// 复制细化后的图像
			memcpy(p_data, temp, wide  * height);
		}
		 // 复制细化后的图像
		memcpy(p_data, temp, wide  * height);
		// 释放内存
		delete temp ;
	}
	else//24位真彩色
	{
			// DIB的宽度
			LONG wide = this->GetDibWidthBytes();
			// DIB的高度
			LONG height = this->GetHeight ();
			// 暂时分配内存，以保存新图像
			temp = new BYTE [wide *height];
			// 初始化新分配的内存，设定初始值为255
			lpDst = temp;
			memset(lpDst, (BYTE)255, wide  * height);
			while (fp)
			{
				fp = FALSE;
				// 初始化新分配的内存，设定初始值为255
				lpDst = temp;
				memset(lpDst, (BYTE)255, wide  * height);
				// 由于使用5×5的结构元素，为防越界，不处理外围的2行、2列像素
				for (j = 2; j < height - 2; j++)
				{
					for (i = 2 *3; i < wide  - 2 *3; i=i +3)
					{
						// 指向源图像倒数第j行，第i个象素的指针			
						lpSrc = (LPBYTE)(p_data + wide  *j + i);
						// 指向目标图像倒数第j行，第i个象素的指针			
						lpDst = (LPBYTE)(temp + wide  * j + i);
						// 如果源图像中当前点为白色，则跳过
						if (*lpSrc > 245)
							continue;
						// 获得当前点相邻的5×5区域内像素值，白色用0代表，黑色用1代表
						for (m = 0; m < 5; m++)
						{
							for (n = 0; n < 5; n++)
							{
								if (*(lpSrc + (2- m) * wide  + (n - 2)*3 ) > 245)
									S[m][n] = 0;
								else
									S[m][n] = 1;
							}
						}
						// 判断条件一是否成立：
						Num =  S[1][1] + S[1][2] + S[1][3] + S[2][1] 
							+ S[2][3] + S[3][1]	+ S[3][2] + S[3][3];
						if (Num < 2 || Num >6)
						{
							*lpDst = 0;
							*(lpDst+1) = 0;
							*(lpDst+2) = 0;
							continue;
						}
						// 判断条件二是否成立：
						Num = 0;
						if (S[1][2] == 0 && S[1][1] == 1) 
							Num++;
						if (S[1][1] == 0 && S[2][1] == 1)
							Num++;
						if (S[2][1] == 0 && S[3][1] == 1)
							Num++;
						if (S[3][1] == 0 && S[3][2] == 1)
							Num++;
						if (S[3][2] == 0 && S[3][3] == 1)
							Num++;
						if (S[3][3] == 0 && S[2][3] == 1)
							Num++;
						if (S[2][3] == 0 && S[1][3] == 1)
							Num++;
						if (S[1][3] == 0 && S[1][2] == 1)
							Num++;
						if (Num != 1)
						{
							*lpDst = 0;
							*(lpDst+1) = 0;
							*(lpDst+2) = 0;
							continue;
						}
						// 判断条件三是否成立；
						if (S[1][2] * S[2][1] * S[2][3] != 0)
						{
							Num = 0;
							if (S[0][2] == 0 && S[0][1] == 1)
								Num++;
							if (S[0][1] == 0 && S[1][1] == 1)
								Num++;
							if (S[1][1] == 0 && S[2][1] == 1)
								Num++;
							if (S[2][1] == 0 && S[2][2] == 1)
								Num++;
							if (S[2][2] == 0 && S[2][3] == 1)
								Num++;
							if (S[2][3] == 0 && S[1][3] == 1)
								Num++;
							if (S[1][3] == 0 && S[0][3] == 1)
								Num++;
							if (S[0][3] == 0 && S[0][2] == 1)
								Num++;
							if (Num == 1)
							{
								*lpDst = 0;
								*(lpDst+1) = 0;
								*(lpDst+2) = 0;
								continue;
							}
						}
						// 判断条件四是否成立：
						if (S[1][2] * S[2][1] * S[3][2] != 0)
						{
							Num = 0;
							if (S[1][1] == 0 && S[1][0] == 1)
								Num++;
							if (S[1][0] == 0 && S[2][0] == 1)
								Num++;
							if (S[2][0] == 0 && S[3][0] == 1)
								Num++;
							if (S[3][0] == 0 && S[3][1] == 1)
								Num++;
							if (S[3][1] == 0 && S[3][2] == 1)
								Num++;
							if (S[3][2] == 0 && S[2][2] == 1)
								Num++;
							if (S[2][2] == 0 && S[1][2] == 1)
								Num++;
							if (S[1][2] == 0 && S[1][1] == 1)
								Num++;
							if (Num == 1)
							{
								*lpDst = 0;
								*(lpDst+1) = 0;
								*(lpDst+2) = 0;
								continue;
							}
						}
						// 如果条件均满足则删除该点
						*lpDst = 255;
						*(lpDst+1) = 255;
						*(lpDst+2) = 255;		 
						fp = TRUE;
					}
				}
				// 复制细化后的图像
				memcpy(p_data, temp, wide  * height);
			}
			// 复制细化后的图像
			memcpy(p_data, temp, wide  * height);
			// 释放内存
			delete temp ;
	}
}



///***************************************************************/           
/*函数名称：Cuhua()                                       
/*函数类型：void                                      
/*功能：对图像进行粗化处理。            
/***************************************************************/
void BianTiDib::Cuhua()
{
	// 循环变量
	LONG i;
	LONG j;
	// 指向DIB象素指针
	LPBYTE p_data;
	// 找到DIB图像象素起始位置
	p_data = this->GetData();
	if(m_pBitmapInfoHeader->biBitCount<9)//灰度图像
	{
		// DIB的宽度
		LONG wide = GetWidth();
		// DIB的高度
		LONG height = GetHeight();
		// 对各像素进行灰度转换
		for (j = 0; j < height; j ++)
		{
			for (i = 0; i < wide; i ++)
			{
				// 对像素各颜色分量进行二值化求补处理
				unsigned char temp = *((unsigned char *)p_data + wide * j +i);
				if (temp > 127)
					*((unsigned char *)p_data + wide * j + i) = 0;
				else
					*((unsigned char *)p_data + wide * j + i) = 255;
			}
		}
		// 在求补后再对图象进行细化
		Xihua(); 
	}
	else//24位真彩色
	{
		// DIB的宽度
		LONG wide = GetDibWidthBytes();
		// DIB的高度
		LONG height = GetHeight();
		// 对各像素进行灰度转换
		for (j = 0; j < height; j ++)
		{
			for (i = 0; i < wide; i ++)
			{
				// 对像素各颜色分量进行二值化求补处理
				unsigned char temp = *((unsigned char *)p_data + wide * j +i);
				if (temp > 127)
					*((unsigned char *)p_data + wide * j + i) = 0;
				else
					*((unsigned char *)p_data + wide * j + i) = 255;			 		 
			}
		}
		// 在求补后再对图象进行细化
		Xihua(); 
	}
}



///***************************************************************/           
/*函数名称：Zhongzhoubianhuan()                                       
/*函数类型：void                                      
/*功能：对图像进行中轴变换。            
/***************************************************************/
void BianTiDib::Zhongzhoubianhuan()
{
	// 循环变量 
	LONG i;
	LONG j;
	LONG m;
	LONG n;
	// 5×5相邻区域像素值
	unsigned char S[5][5];
	// 计数器
	unsigned char Num;
	// 循环跳出标志 
	BOOL fp=TRUE;
	// 指向DIB象素指针
	LPBYTE p_data;
	// 指向源图像的指针
	LPBYTE	lpSrc;
	// 指向缓存图像的指针
	LPBYTE	lpDst;
	// 指向缓存DIB图像的指针
	LPBYTE	temp;
	// 找到DIB图像象素起始位置
	p_data = GetData();
	if(m_pBitmapInfoHeader->biBitCount<9)//灰度图像
	{	
		// DIB的宽度
		LONG wide = GetWidth();
		// DIB的高度 
		LONG height = GetHeight();
		// 暂时分配内存，以保存新图像
		temp =  new BYTE [wide*height];
		// 初始化新分配的内存，设定初始值为255
		lpDst = (LPBYTE)temp;
		memset(lpDst, (BYTE)255, wide * height);
		while (fp)
		{
			fp = FALSE;
			// 初始化新分配的内存，设定初始值为255
			lpDst = (LPBYTE)temp;
			memset(lpDst, (BYTE)255, wide * height);
			// 由于使用5×5的结构元素，为防越界，不处理外围的2行、2列像素
			for (j= 2; j < height - 2; j++)
			{
				for (i = 2  ; i < wide - 2  ; i ++)
				{
					// 指向源图像倒数第j行，第i个象素的指针			
					lpSrc = (LPBYTE)(p_data + wide * j + i);
					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (LPBYTE)(temp + wide * j + i);
					// 如果源图像中当前点为白色，则跳过
					if (*lpSrc > 127)
						continue;
					// 获得当前点相邻的5×5区域内像素值，白色用0代表，黑色用1代表
					for (m = 0; m < 5; m++)
					{
						for (n = 0; n < 5; n++)
						{
							if (*(lpSrc + (2 - m) * wide + (n - 2)  ) > 127)
								S[m][n] = 0;
							else
								S[m][n] = 1;
						}
					}
					// 判断条件1-1是否成立：
					Num =  S[1][1] + S[1][2] + S[1][3] + S[2][1] 
						+ S[2][3] + S[3][1]	+ S[3][2] + S[3][3];
					if (Num < 2 || Num >6)
					{
						*lpDst = 0;			 
						continue;
					}
					// 判断条件1-2是否成立：
					Num = 0;
					if (S[1][2] == 0 && S[1][1] == 1) 
						Num++;
					if (S[1][1] == 0 && S[2][1] == 1)
						Num++;
					if (S[2][1] == 0 && S[3][1] == 1)
						Num++;
					if (S[3][1] == 0 && S[3][2] == 1)
						Num++;
					if (S[3][2] == 0 && S[3][3] == 1)
						Num++;
					if (S[3][3] == 0 && S[2][3] == 1)
						Num++;
					if (S[2][3] == 0 && S[1][3] == 1)
						Num++;
					if (S[1][3] == 0 && S[1][2] == 1)
						Num++;
					if (Num != 1)
					{
						*lpDst = 0;			 
						continue;
					}
					// 判断条件1-3是否成立；
					if (S[1][2] * S[2][1] * S[3][2] != 0)
					{
						*lpDst = 0;			 
						continue;
					}
					// 判断条件1-4是否成立：
					if (S[2][1] * S[3][2] * S[2][3] != 0)
					{
						*lpDst = 0;			 
						continue;
					}
					// 如果条件均满足则删除该点
					*lpDst = 255;			 
				}
			}
			// 由于使用5×5的结构元素，为防越界，不处理外围的2行、2列像素
			for (j = 2; j < height - 2; j++)
			{
				for (i = 2  ; i < wide - 2  ; i ++)
				{
					// 指向源图像倒数第j行，第i个象素的指针			
					lpSrc = (LPBYTE)(p_data + wide * j + i);
					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (LPBYTE)(temp + wide *j + i);
					// 如果源图像中当前点为白色，则跳过
					if (*lpSrc > 127)
						continue;
					// 获得当前点相邻的5×5区域内像素值，白色用0代表，黑色用1代表
					for (m = 0; m < 5; m++)
					{
						for (n = 0; n < 5; n++)
						{
							if (*(lpSrc + (2 - m) * wide + (n - 2)  ) > 200)
								S[m][n] = 0;
							else
								S[m][n] = 1;
						}
					}
					// 判断条件2-1是否成立：
					Num =  S[1][1] + S[1][2] + S[1][3] + S[2][1] 
						+ S[2][3] + S[3][1]	+ S[3][2] + S[3][3];
					if (Num < 2 || Num >6)
					{
						*lpDst = 0;			 
						continue;
					}
					// 判断条件2-2是否成立：
					Num = 0;
					if (S[1][2] == 0 && S[1][1] == 1) 
						Num++;
					if (S[1][1] == 0 && S[2][1] == 1)
						Num++;
					if (S[2][1] == 0 && S[3][1] == 1)
						Num++;
					if (S[3][1] == 0 && S[3][2] == 1)
						Num++;
					if (S[3][2] == 0 && S[3][3] == 1)
						Num++;
					if (S[3][3] == 0 && S[2][3] == 1)
						Num++;
					if (S[2][3] == 0 && S[1][3] == 1)
						Num++;
					if (S[1][3] == 0 && S[1][2] == 1)
						Num++;
					if (Num != 1)
					{
						*lpDst = 0;			 
						continue;
					}
					// 判断条件2-3是否成立；
					if (S[1][2] * S[2][1] * S[2][3] != 0)
					{
						*lpDst = 0;			 
						continue;
					}
					// 判断条件2-4是否成立：
					if (S[1][2] * S[3][2] * S[2][3] != 0)
					{
						*lpDst = 0;			 
						continue;
					}
					// 如果条件均满足则删除该点
					*lpDst = 255;			 
					fp = TRUE;
				}
			}
			// 复制变换后的图像
			memcpy(p_data, temp, wide * height);
		}
		// 复制变换后的图像
		memcpy(p_data, temp, wide * height);
		//  释放内存
		delete temp; 
	}
	else//24位真彩色
	{
		// DIB的宽度
		LONG wide = GetDibWidthBytes();
		// DIB的高度 
		LONG height = GetHeight();
		// 暂时分配内存，以保存新图像
		temp =  new BYTE [wide*height];
		// 初始化新分配的内存，设定初始值为255
		lpDst = (LPBYTE)temp;
		memset(lpDst, (BYTE)255, wide * height);
		while (fp)
		{
			fp = FALSE;
			// 初始化新分配的内存，设定初始值为255
			lpDst = (LPBYTE)temp;
			memset(lpDst, (BYTE)255, wide * height);
			// 由于使用5×5的结构元素，为防越界，不处理外围的2行、2列像素
			for (j= 2; j < height - 2; j++)
			{
				for (i = 2*3  ; i < wide - 2 *3 ; i =i+3)
				{
					// 指向源图像倒数第j行，第i个象素的指针			
					lpSrc = (LPBYTE)(p_data + wide * j + i);
					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (LPBYTE)(temp + wide * j + i);
					// 如果源图像中当前点为白色，则跳过
					if (*lpSrc > 127)
						continue;
					// 获得当前点相邻的5×5区域内像素值，白色用0代表，黑色用1代表
					for (m = 0; m < 5; m++)
					{
						for (n = 0; n < 5; n++)
						{
							if (*(lpSrc + (2 - m) * wide + (n - 2) *3 ) > 200)
								S[m][n] = 0;
							else
								S[m][n] = 1;
						}
					}
					// 判断条件1-1是否成立：
					Num =  S[1][1] + S[1][2] + S[1][3] + S[2][1] 
						+ S[2][3] + S[3][1]	+ S[3][2] + S[3][3];
					if (Num < 2 || Num >6)
					{
						*lpDst = 0;
						*(lpDst+1) = 0;
						*(lpDst+2) = 0;
						continue;
					}
					// 判断条件1-2是否成立：
					Num = 0;
					if (S[1][2] == 0 && S[1][1] == 1) 
						Num++;
					if (S[1][1] == 0 && S[2][1] == 1)
						Num++;
					if (S[2][1] == 0 && S[3][1] == 1)
						Num++;
					if (S[3][1] == 0 && S[3][2] == 1)
						Num++;
					if (S[3][2] == 0 && S[3][3] == 1)
						Num++;
					if (S[3][3] == 0 && S[2][3] == 1)
						Num++;
					if (S[2][3] == 0 && S[1][3] == 1)
						Num++;
					if (S[1][3] == 0 && S[1][2] == 1)
						Num++;
					if (Num != 1)
					{
						*lpDst = 0;
						*(lpDst+1) = 0;
						*(lpDst+2) = 0;
						continue;
					}
					// 判断条件1-3是否成立；
					if (S[1][2] * S[2][1] * S[3][2] != 0)
					{
						*lpDst = 0;
						*(lpDst+1) = 0;
						*(lpDst+2) = 0;
						continue;
					}
					// 判断条件1-4是否成立：
					if (S[2][1] * S[3][2] * S[2][3] != 0)
					{
						*lpDst = 0;
						*(lpDst+1) = 0;
						*(lpDst+2) = 0;
						continue;
					}
					// 如果条件均满足则删除该点
					*lpDst = 255;
					*(lpDst+1) = 255;
					*(lpDst+2) = 255;
				}
			}
			// 由于使用5×5的结构元素，为防越界，不处理外围的2行、2列像素
			for (j = 2; j < height - 2; j++)
			{
				for (i = 2*3  ; i < wide - 2*3  ; i=i +3)
				{
					// 指向源图像倒数第j行，第i个象素的指针			
					lpSrc = (LPBYTE)(p_data + wide * j + i);
					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (LPBYTE)(temp + wide *j + i);
					// 如果源图像中当前点为白色，则跳过
					if (*lpSrc > 127)
						continue;
					// 获得当前点相邻的5×5区域内像素值，白色用0代表，黑色用1代表
					for (m = 0; m < 5; m++)
					{
						for (n = 0; n < 5; n++)
						{
							if (*(lpSrc + (2 - m) * wide + (n - 2)*3  ) > 200)
								S[m][n] = 0;
							else
								S[m][n] = 1;
						}
					}
					// 判断条件2-1是否成立：
					Num =  S[1][1] + S[1][2] + S[1][3] + S[2][1] 
						+ S[2][3] + S[3][1]	+ S[3][2] + S[3][3];
					if (Num < 2 || Num >6)
					{
						*lpDst = 0;
						*(lpDst+1) = 0;
						*(lpDst+2) = 0;
						continue;
					}
					// 判断条件2-2是否成立：
					Num = 0;
					if (S[1][2] == 0 && S[1][1] == 1) 
						Num++;
					if (S[1][1] == 0 && S[2][1] == 1)
						Num++;
					if (S[2][1] == 0 && S[3][1] == 1)
						Num++;
					if (S[3][1] == 0 && S[3][2] == 1)
						Num++;
					if (S[3][2] == 0 && S[3][3] == 1)
						Num++;
					if (S[3][3] == 0 && S[2][3] == 1)
						Num++;
					if (S[2][3] == 0 && S[1][3] == 1)
						Num++;
					if (S[1][3] == 0 && S[1][2] == 1)
						Num++;
					if (Num != 1)
					{
						*lpDst = 0;
						*(lpDst+1) = 0;
						*(lpDst+2) = 0;
						continue;
					}
					// 判断条件2-3是否成立；
					if (S[1][2] * S[2][1] * S[2][3] != 0)
					{
						*lpDst = 0;
						*(lpDst+1) = 0;
						*(lpDst+2) = 0;
						continue;
					}
					// 判断条件2-4是否成立：
					if (S[1][2] * S[3][2] * S[2][3] != 0)
					{
						*lpDst = 0;
						*(lpDst+1) = 0;
						*(lpDst+2) = 0;
						continue;
					}
					// 如果条件均满足则删除该点
					*lpDst = 255;
					*(lpDst+1) = 255;
					*(lpDst+2) = 255;
					fp = TRUE;
				}
			}
			// 复制变换后的图像
			memcpy(p_data, temp, wide * height);
		}
		// 复制变换后的图像
		memcpy(p_data, temp, wide * height);	
		//  释放内存
		delete temp; 
	}
}
 
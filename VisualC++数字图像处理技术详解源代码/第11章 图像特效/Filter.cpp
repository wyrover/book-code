#include "StdAfx.h"
#include "Filter.h"
#include "improve.h"

//-----------------------------------------
//	作用:			图像反色函数
//	参数:		
//		pixel		原始图像的像素数组
//		tempPixel	输出图像的像素数组
//		width		原始图像宽度
//		height		原始图像高度
//-----------------------------------------
void Negative(BYTE* pixel, BYTE* tempPixel, int width, int height)
{
	//计算像素数组的长度
	int sum = width * height * 4;
	memcpy(pixel, tempPixel, sum);

	for(int i = 0; i < sum; i += 4) 
	{
		// 对像素值取反
		tempPixel[i]	= 255 - tempPixel[i];	//blue
		tempPixel[i+1]	= 255 - tempPixel[i+1];	//green
		tempPixel[i+2]	= 255 - tempPixel[i+2];	//red
	}
}

//-----------------------------------------
//	作用:			实现图像的浮雕效果
//	参数:		
//		pixel		原始图像的像素数组
//		tempPixel	输出图像的像素数组
//		width		原始图像宽度
//		height		原始图像高度
//-----------------------------------------
void Emboss(BYTE* pixel, BYTE* tempPixel, int width, int height)
{
	//计算像素数组的长度
	int sum = width * height *4;
	memcpy(tempPixel, pixel, sum);		

	int r = 0, g = 0, b = 0;
	for(int i = 0; i < height-1; i++) 
	{
		for (int j = 0; j < (width-1)*4; j+=4) 
		{
			//处理像素值
			b = abs(tempPixel[i*width*4+j] 
					-tempPixel[(i+1)*width*4+j+4]+128);
			g = abs(tempPixel[i*width*4+j+1]
					-tempPixel[(i+1)*width*4+j+5]+128);
			r = abs(tempPixel[i*width*4+j+2]
					-tempPixel[(i+1)*width*4+j+6]+128);

			//对于越界的像素值进行处理
			if (r>255)
				r=255;

			if (g>255)
				g=255;

			if (b>255)
				b=255;

			tempPixel[i*width*4 + j]		= b;//blue
			tempPixel[i*width*4 + j + 1]	= g;//green
			tempPixel[i*width*4 + j + 2]	= r;//red
		}
	}

	for (int k = width * 4 * (height-1); k < width*4*height; k += 4) 
	{
		tempPixel[k]=128;
		tempPixel[k+1]=128;
		tempPixel[k+2]=128;
	}

	for (int l = (width-1) * 4; l < width*4*height; l += width*4) 
	{
		tempPixel[l]=128;
		tempPixel[l+1]=128;
		tempPixel[l+2]=128;
	}
}

//-----------------------------------------
//	作用:			实现图像的黑白效果
//	参数:		
//		pixel		原始图像的像素数组
//		tempPixel	输出图像的像素数组
//		width		原始图像宽度
//		height		原始图像高度
//-----------------------------------------
void ColorToBW(BYTE* pixel, BYTE* tempPixel, int width, int height)
{
	//计算像素数组的长度
	int sum = width * height * 4;

	memcpy(tempPixel, pixel, sum);

	for(int i = 0; i < sum; i += 4)
	{
		//平均值法
		tempPixel[i]	 = (tempPixel[i] + tempPixel[i+1] + tempPixel[i+2]) / 3; //blue
		tempPixel[i + 1] = tempPixel[i];										 //green
		tempPixel[i + 2] = tempPixel[i];										 //red

		//最大值法
		/*
		tempPixel[i] = tempPixel[i] > tempPixel[i+1]? 
							  tempPixel[i] : tempPixel[i+1];
		tempPixel[i] = tempPixel[i] >tempPixel[i+2]?
							  tempPixel[i] : tempPixel[i+2];

		tempPixel[i+1] = tempPixel[i] ;
		tempPixel[i+2] = tempPixel[i];
		*/

		//加权平均值法
		/*
		tempPixel[i] =( ((int) (tempPixel[i]*0.5)) + 
							   ((int) (tempPixel[i+1]*0.3)) + 
							   ((int) (tempPixel[i+2]*0.2)) ;

		tempPixel[i + 1] = tempPixel[i];										 //green
		tempPixel[i + 2] = tempPixel[i];		
		*/
	}
}

//-----------------------------------------
//	作用:			实现水平方式的雾化效果
//	参数:		
//		pixel		原始图像的像素数组
//		tempPixel	输出图像的像素数组
//		width		原始图像宽度
//		height		原始图像高度
//		f			雾化参数
//-----------------------------------------
void HorFog(BYTE* pixel, BYTE* tempPixel, int width, int height, int f)
{
	int k;
	int n;
	for(int i = 0; i < height; i++) 
	{
		for (int j = 0; j < width*4; j += 4) 
		{
			k = abs(rand() % f);
			n = j + k*4;

			if (n>(width-1) * 4)
				n = (width-1)*4;

			tempPixel[i*width*4 + j]     = pixel[i*width*4 + n];
			tempPixel[i*width*4 + j + 1] = pixel[i*width*4 + n + 1];
			tempPixel[i*width*4 + j + 2] = pixel[i*width*4 + n + 2];
			tempPixel[i*width*4 + j + 3] = pixel[i*width*4 + n + 3];
		}
	}
}

//-----------------------------------------
//	作用:			实现垂直方式的雾化效果
//	参数:		
//		pixel		原始图像的像素数组
//		tempPixel	输出图像的像素数组
//		width		原始图像宽度
//		height		原始图像高度
//		f			雾化参数
//-----------------------------------------
void VerFog(BYTE* pixel, BYTE* tempPixel, int width, int height, int f)
{
	int k;
	int m;
	for(int i = 0; i < height; i++) 
	{
		for (int j = 0; j < width*4; j += 4) 
		{
			k = abs(rand() % f);
			m = i + k;
			if (m>height-1)
				m = height-1;

			tempPixel[i*width*4 + j]     = pixel[m*width*4 + j];
			tempPixel[i*width*4 + j + 1] = pixel[m*width*4 + j + 1];
			tempPixel[i*width*4 + j + 2] = pixel[m*width*4 + j + 2];
			tempPixel[i*width*4 + j + 3] = pixel[m*width*4 + j + 3];
		}
	}
}

//-----------------------------------------
//	作用:			实现复合方式的雾化效果
//	参数:		
//		pixel		原始图像的像素数组
//		tempPixel	输出图像的像素数组
//		width		原始图像宽度
//		height		原始图像高度
//		f			雾化参数
//-----------------------------------------
void ComFog(BYTE* pixel, BYTE* tempPixel, int width, int height, int f)
{
	int k;
	int m, n;
	for(int i = 0; i < height; i++) 
	{
		for (int j = 0; j < width*4; j += 4) 
		{
			k = abs(rand() % f);
			m = i + k;
			n = j + k * 4;

			// 对超出图像区域的点进行相应处理
			if (m > height-1) 
				m = height-1;

			if (n > (width-1) * 4) 
				n = (width-1) * 4;

			// 更新像素数组
			tempPixel[i*width*4 + j]     = pixel[m*width*4 + n];
			tempPixel[i*width*4 + j + 1] = pixel[m*width*4 + n + 1];
			tempPixel[i*width*4 + j + 2] = pixel[m*width*4 + n + 2];
			tempPixel[i*width*4 + j + 3] = pixel[m*width*4 + n + 3];
		}
	}
}

//-----------------------------------------
//	作用:		把线形存储的像素转化为二维数组形式
//	参数:		
//		image	线形存储的象素
//		width	图像的宽度
//		height	图象的高度
//-----------------------------------------
static BYTE** CreatImage(BYTE* image, unsigned int width, unsigned int height)
{
	BYTE** imageBuf = (BYTE**)malloc(sizeof(BYTE*) * (height));
	for(unsigned int y = 0; y < height; y++)
	{
		//使imageBuf中每个指针分别指向其下标表示的行的行首地址
		imageBuf[y] = image + y * width * 4;
	}

	return imageBuf;
}

//-----------------------------------------
//	作用:			实现图像的马赛克效果
//	参数:		
//		pixel		原始图像的像素数组
//		tempPixel	输出图像的像素数组
//		w			原始图像宽度
//		h			原始图像高度
//		size		马赛克方块的大小
//-----------------------------------------
void Mosaic(BYTE* pixel, BYTE* tempPixel, unsigned int w, 
			unsigned int h, unsigned int size)
{
	BYTE** imageBuf0 = CreatImage(pixel, w, h);
	BYTE** imageBuf1 = CreatImage(tempPixel, w, h);
	int x = 0,y = 0;        //马赛克方格的左上角坐标
	int tx = 0,ty = 0;      //马赛克方格的右下角坐标
	int i, j;
	while(true)
	{
		if(tx == w)  //如果已经处理完一行
		{
			if(ty == h) break;

			//确定马赛克方格的上方边界
			y = ty;  

			//确定马赛克方格的下方边界
			if(y + size <= h)
				ty = y + size;
			else
				ty = h;

			//把方格位置移动到下一行首部
			x = 0;
		}

		//确定马赛克方格的左边边界
		if(x + size <= w)
			tx = x + size;
		else
			tx = w;

		for(i = y;i < ty; i++)
			for(j = x; j<tx; j++)
			{
				//把马赛克方格填充为中点的象素
				imageBuf1[i][j*4]	  = imageBuf0[(y+ty) / 2][(x+tx)/2*4];
				imageBuf1[i][j*4 + 1] = imageBuf0[(y+ty) / 2][(x+tx)/2*4 + 1];
				imageBuf1[i][j*4 + 2] = imageBuf0[(y+ty) / 2][(x+tx)/2*4 + 2];
				imageBuf1[i][j*4 + 3] = imageBuf0[(y+ty) / 2][(x+tx)/2*4 + 3];
			}
			//水平移动到下一个马赛克方格
			x = tx;
	}

	free(imageBuf0);
	free(imageBuf1);
}


//-----------------------------------------
//模板的卷积运算
//-----------------------------------------
int TempltExcute(BYTE** imageBuf0, int w, int h, int* templt, int tw, int x, int y)
{
	int i,j;
	int m=0,px,py;
	//分别对模板上每个位置进行计算
	for(i=0; i<tw; i++)
		for(j=0; j<tw; j++)
		{
			//计算像素在原图像上的位置
			py=y-tw/2+i;
			px=x-tw/2+j;
			//将每个像素的灰度乘以权值再相加
			m+=GetAsh(imageBuf0,px,py) * templt[i*tw+j];
		}
		return m;
}


//-----------------------------------------
// 用a表示的灰度填充指定像素
//-----------------------------------------
void SetPixel2(BYTE** imageBuf1, int x, int y, int a)
{
	imageBuf1[y][x*4] = a;
	imageBuf1[y][x*4+1] = a;
	imageBuf1[y][x*4+2] = a;
	imageBuf1[y][x*4+3]= 255;
}

//-----------------------------------------
//	作用:			实现图像的素描效果
//	参数:		
//		pixel		原始图像的像素数组
//		tempPixel	输出图像的像素数组
//		w			原始图像宽度
//		h			原始图像高度
//		size		马赛克方块的大小
//-----------------------------------------
void LaplacianB(BYTE* pixel, BYTE* tempPixel, int w, int h)
{
	//定义临时图象存储空间
	BYTE* tempImage;

	tempImage = (BYTE*)malloc(sizeof(BYTE)*w*h*4);

	//将图像转化为矩阵形式
	BYTE** imageBuf0 = CreatImage(pixel, w, h);
	BYTE** imageBuf1 = CreatImage(tempPixel, w, h);
	BYTE** tempImageBuf = CreatImage(tempImage, w, h);
	double scale = 2;
	//拉普拉斯正相模板
	int templt[9]={ 1, 1, 1, 1,-8, 1, 1, 1, 1 };
	//噪声检测模板
	int templtTest1[9]={ 1, 1,-1, 1, 0,-1, 1,-1,-1 };
	int templtTest2[9]={ 1, 1, 1,-1, 0, 1,-1,-1,-1 };
	//模糊处理模板
	int templtAve[9]={ 1, 1, 1, 1, 4, 1, 1, 1, 1};
	int x,y;
	int a,b,b1,b2;

	//依次对原图像的每个像素进行处理
	for(y = 1; y < h - 1; y++)
		for(x = 1; x < w - 1; x++)
		{
			//拉普拉斯卷积运算
			a=TempltExcute(imageBuf0, w, h, templt, 3, x, y);
			//噪声检测
			b1=abs(TempltExcute(imageBuf0, w, h, templtTest1, 3, x, y));
			b2=abs(TempltExcute(imageBuf0, w, h, templtTest2, 3, x, y));
			b=b1>b2?b1:b2;
			if(b<25) a=0;
			else
			{
				a = (int)(a * scale);
				//过限处理
				if(a > 255) a = 255;
				else if(a < 32) a=0;
			}
			//反色处理
			a=255-a;
			SetPixel2(tempImageBuf, x, y, a);
		}

		//模糊处理
		for(y = 1; y < h - 1; y++)
			for(x = 1; x < w - 1; x++)
			{
				a=TempltExcute(tempImageBuf, w, h, templtAve, 3, x, y) / 12;
				SetPixel2(imageBuf1, x, y, a);
			}

	free(tempImage);
	free(imageBuf0);
	free(imageBuf1);
	free(tempImageBuf);
}
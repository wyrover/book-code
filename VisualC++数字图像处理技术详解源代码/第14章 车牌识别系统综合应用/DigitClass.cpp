// DigitClass.cpp: implementation of the CDigitClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "DigitClass.h"
#include "math.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDigitClass::CDigitClass()
{

}

CDigitClass::~CDigitClass()
{

}

/*void CDigitClass::CopyArToBitmap(void)
{
	LONG x, y;
	digitWidth = digitdib.GetWidth();
	digitHeight = digitdib.GetHeight();
	for(x=0; x<digitWidth; x++)
	{
		for(y=0; y<digitHeight; y++)
		{
			digitdib.SetPixel8(x, y, digitarray[x][y]);
			//digitdib.SetPixel8(x, y, 0);
		}
	}
}

void CDigitClass::CopyBitmapToAr(void)
{
	LONG x, y;
	digitWidth = digitdib.GetWidth();
	digitHeight = digitdib.GetHeight();

	for(x=0; x<digitWidth; x++)
	{
		for(y=0; y<digitHeight; y++)
		{
			digitarray[x][y] = digitdib.GetPixel8(x, y);
			TRACE("%d ",digitdib.GetPixel8(x, y));
		}
	}
}*/

void CDigitClass::BinaryDigit(BYTE thre)
{
//	digitWidth = digitdib.GetWidth();
//	digitHeight = digitdib.GetHeight();

	LONG x, y;

	for(x=0; x<digitWidth; x++)
	{
		for(y=0; y<digitHeight; y++)
		{
			if(digitarray[x][y] >= thre)
				digitarray[x][y]=(BYTE)1;
			else
				digitarray[x][y]=(BYTE)0;
		}
	}
}

void CDigitClass::BinaryDigit()
{
    int x,y;	
	int k=0;

	long double total=0;
	float aver=0;

	for(x=0;x<digitWidth;x++)
	{
		for(y=0;y<digitHeight;y++)
		{	
			total+=digitarray[x][y];	
		}
	}
	aver=(BYTE)(total/(float)(digitWidth*digitHeight));
	
	float delt; 
	float sub=0; 
	for(x=0;x<digitWidth;x++)
	{
		for(y=0;y<digitHeight;y++)
		{			
			sub+=(digitarray[x][y]-aver)*(digitarray[x][y]-aver);			 
		}
	}
	delt=(float)(sqrt(sub/(float)(digitWidth*digitHeight)));
	
	BYTE judge;
	judge=(BYTE)(delt+aver);
	for(x=0;x<digitWidth;x++)
	{
		for(y=0;y<digitHeight;y++)
		{
			if(digitarray[x][y]>=judge)
			{
				digitarray[x][y]=1;
			}
			else
			{
				digitarray[x][y]=0;
			}
		}
	}
}


void CDigitClass::ThinDigit_1()
{
	LONG x,y,k;
	k=0;

//	digitWidth = digitdib.GetWidth();
//	digitHeight = digitdib.GetHeight();

	for(x=0; x<digitWidth; x++)
	{
		digitarray[x][0] = (BYTE)0;
		digitarray[x][digitHeight-1] = (BYTE)0;
	}

	for(y=0; y<digitHeight; y++)
	{
		digitarray[0][y] = (BYTE)0;
		digitarray[digitWidth-1][y] = (BYTE)0;
	}

	for(x=0; x<digitWidth; x++)
	{
		for(y=0; y<digitHeight; y++)
		{
			digitarray1[k] = digitarray[x][y];
			if(digitarray1[k] != 0)
				digitarray1[k] = (BYTE)1;
			k++;
		}
	}
    //四种细化函数，不进行细化

	//ThinnerHilditch((void *)digitarray1, digitWidth, digitHeight);

	k=0;
	for(x=0; x<digitWidth; x++)
	{
		for(y=0; y<digitHeight; y++)
		{
			digitarray[x][y] = digitarray1[k];
			if(digitarray[x][y]!=0)
				digitarray[x][y] = (BYTE)1;
			k++;
		}
	}
}

void CDigitClass::ThinDigit_2()
{
	LONG x,y,k;
	k=0;

//	digitWidth = digitdib.GetWidth();
//	digitHeight = digitdib.GetHeight();

	for(x=0; x<digitWidth; x++)
	{
		digitarray[x][0] = (BYTE)0;
		digitarray[x][digitHeight-1] = (BYTE)0;
	}

	for(y=0; y<digitHeight; y++)
	{
		digitarray[0][y] = (BYTE)0;
		digitarray[digitWidth-1][y] = (BYTE)0;
	}

	for(x=0; x<digitWidth; x++)
	{
		for(y=0; y<digitHeight; y++)
		{
			digitarray1[k] = digitarray[x][y];
			if(digitarray1[k] != 0)
				digitarray1[k] = (BYTE)1;
			k++;
		}
	}

//	ThinnerPavlidis((void *)digitarray1, digitWidth, digitHeight);

	k=0;
	for(x=0; x<digitWidth; x++)
	{
		for(y=0; y<digitHeight; y++)
		{
			digitarray[x][y] = digitarray1[k];
			if(digitarray[x][y]!=0)
				digitarray[x][y] = (BYTE)1;
			k++;
		}
	}
}

void CDigitClass::ThinDigit_3()
{
	LONG x,y,k;
	k=0;

//	digitWidth = digitdib.GetWidth();
//	digitHeight = digitdib.GetHeight();

	for(x=0; x<digitWidth; x++)
	{
		digitarray[x][0] = (BYTE)0;
		digitarray[x][digitHeight-1] = (BYTE)0;
	}

	for(y=0; y<digitHeight; y++)
	{
		digitarray[0][y] = (BYTE)0;
		digitarray[digitWidth-1][y] = (BYTE)0;
	}

	for(x=0; x<digitWidth; x++)
	{
		for(y=0; y<digitHeight; y++)
		{
			digitarray1[k] = digitarray[x][y];
			if(digitarray1[k] != 0)
				digitarray1[k] = (BYTE)1;
			k++;
		}
	}

//	ThinnerHilditch((void *)digitarray1, digitWidth, digitHeight);

	k=0;
	for(x=0; x<digitWidth; x++)
	{
		for(y=0; y<digitHeight; y++)
		{
			digitarray[x][y] = digitarray1[k];
			if(digitarray[x][y]!=0)
				digitarray[x][y] = (BYTE)1;
			k++;
		}
	}
}

void CDigitClass::ThinDigit_4()
{
	LONG x,y,k;
	k=0;

//	digitWidth = digitdib.GetWidth();
//	digitHeight = digitdib.GetHeight();
	
	for(x=0; x<digitWidth; x++)
	{
		digitarray[x][0] = (BYTE)0;
		digitarray[x][digitHeight-1] = (BYTE)0;
	}

	for(y=0; y<digitHeight; y++)
	{
		digitarray[0][y] = (BYTE)0;
		digitarray[digitWidth-1][y] = (BYTE)0;
	}

	for(x=0; x<digitWidth; x++)
	{
		for(y=0; y<digitHeight; y++)
		{
			digitarray1[k] = digitarray[x][y];
			if(digitarray1[k] != 0)
				digitarray1[k] = (BYTE)1;
			k++;
		}
	}

//	ThiningDIBSkeleton((LPSTR)digitarray1, digitHeight, digitWidth);

	k=0;
	for(x=0; x<digitWidth; x++)
	{
		for(y=0; y<digitHeight; y++)
		{
			digitarray[x][y] = digitarray1[k];
			if(digitarray[x][y]!=0)
				digitarray[x][y] = (BYTE)1;
			k++;
		}
	}
}

void CDigitClass::GetFeature()
{
	int i,j;
	for(i=0; i<13; i++)
		feature[i] = 0;

	//图象是20×36大小的，分成9块

	//第一块
	for(i=0; i<7; i++)
	{
		for(j=0; j<12; j++)
		{
			if(digitarray[i][j]==1)
				feature[0]+=1.0;
		}
	}

	//第二块
	for(i=0; i<7; i++)
	{
		for(j=12; j<24; j++)
		{
			if(digitarray[i][j]==1)
				feature[1]+=1.0;
		}
	}

	//第三块
	for(i=0; i<7; i++)
	{
		for(j=24; j<36; j++)
		{
			if(digitarray[i][j]==1)
				feature[2]+=1.0;
		}
	}

	//第四块
	for(i=7; i<13; i++)
	{
		for(j=0; j<12; j++)
		{
			if(digitarray[i][j]==1)
				feature[3]+=1.0;
		}
	}

	//第五块
	for(i=7; i<13; i++)
	{
		for(j=12; j<24; j++)
		{
			if(digitarray[i][j]==1)
				feature[4]+=1.0;
		}
	}

	//第六块
	for(i=7; i<13; i++)
	{
		for(j=24; j<36; j++)
		{
			if(digitarray[i][j]==1)
				feature[5]+=1.0;
		}
	}

	//第七块
	for(i=13; i<20; i++)
	{
		for(j=0; j<12; j++)
		{
			if(digitarray[i][j]==1)
				feature[6]+=1.0;
		}
	}

	//第八块
	for(i=13; i<20; i++)
	{
		for(j=12; j<24; j++)
		{
			if(digitarray[i][j]==1)
				feature[7]+=1.0;
		}
	}

	//第九块
	for(i=13; i<20; i++)
	{
		for(j=24; j<36; j++)
		{
			if(digitarray[i][j]==1)
				feature[8]+=1.0;
		}
	}

	//下面统计方向交点特征

	for(j=0; j<36; j++)
	{
		if(digitarray[7][j]==1)
			feature[9]+=1.0;
	}

	for(j=0; j<36; j++)
	{
		if(digitarray[13][j]==1)
			feature[10]+=1.0;
	}

	for(i=0; i<20; i++)
	{
		if(digitarray[i][12]==1)
			feature[11]+=1.0;
	}

	for(i=0; i<20; i++)
	{
		if(digitarray[i][24]==1)
			feature[12]+=1.0;
	}
}

void CDigitClass::FixSize()
{
	if(digitWidth==20&&digitHeight==36)
		return;
	
	LONG	lWidth=digitWidth;
	// 源图像的宽度和高度
	LONG	lHeight=digitHeight;
	
	BYTE *temp_img=new BYTE[lWidth*lHeight];
	
	// 循环变量
	LONG	i;
	LONG	j;

	//memcpy(temp_img,digitarray,lWidth*lHeight);
	for(i = 0; i < digitHeight; i++)
	{
		for(j = 0; j < digitWidth; j++)
		{
			
			temp_img[i*digitWidth+j]=digitarray[j][i];
		}
	}

// 缩放后图像的宽度和高度
	LONG	lNewWidth;
	LONG	lNewHeight;

	float XZRatio=(float) 20/digitWidth;
	float YZRatio=(float) 36/digitHeight;

//	float XZRatio=(float) digitWidth/20;
//	float YZRatio=(float) digitHeight/36;


//	lNewWidth = (LONG) (lWidth * XZRatio + 0.5);
//	lNewHeight = (LONG) (lHeight * YZRatio + 0.5);

	
	
	// 象素在源坐标
	LONG	i0;
	LONG	j0;
	
	//归一后的大小
	lNewWidth = 20;
	lNewHeight = 36;
	
	digitHeight=lNewHeight;
	digitWidth=lNewWidth;


	for(i = 0; i < lNewHeight; i++)
	{
		
		for(j = 0; j < lNewWidth; j++)
		{
			i0 = (LONG) (i / YZRatio + 0.5);
			j0 = (LONG) (j / XZRatio + 0.5);
			
			digitarray[j][i]=Interpolation(temp_img,lWidth,lHeight,j0,i0);
/*			

  
			// 判断是否在源图范围内
			if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				// 复制象素
				digitarray[j][i] = temp_img[lHeight * j0 + i0];
			
			}
			else
			{
				// 对于源图中没有的象素，直接赋值为255
				//digitarray[j][i] = 255;
				digitarray[j][i] = 0;
			}*/
			
		}
		
	}
	delete [] temp_img;
}

void CDigitClass::FixSize1()
{
	if(digitWidth==20&&digitHeight==36)
		return;
	
	// 源图像的宽度和高度
	LONG	lWidth=digitWidth;
	LONG	lHeight=digitHeight;
	
	BYTE *temp_img=new BYTE[lWidth*lHeight];
	
	// 循环变量
	LONG	i;
	LONG	j;

	//memcpy(temp_img,digitarray,lWidth*lHeight);
	for(i = 0; i < digitHeight; i++)
	{
		for(j = 0; j < digitWidth; j++)
		{
			
			temp_img[j*digitHeight+i]=digitarray[j][i];
		}
	}
		// 缩放后图像的宽度和高度
	LONG	lNewWidth;
	LONG	lNewHeight;

	float XZRatio=(float) 20/digitWidth;
	float YZRatio=(float) 36/digitHeight;

	

	// 象素在源坐标
	LONG	i0;
	LONG	j0;
	
	// 计算缩放后的图像实际宽度
	// 此处直接加0.5是由于强制类型转换时不四舍五入，而是直接截去小数部分
	lNewWidth = (LONG) (lWidth * XZRatio + 0.5);
	
	// 计算缩放后的图像高度
	lNewHeight = (LONG) (lHeight * YZRatio + 0.5);


	for(i = 0; i < lNewHeight; i++)
	{
		
		for(j = 0; j < lNewWidth; j++)
		{
			i0 = (LONG) (i / YZRatio + 0.5);
			j0 = (LONG) (j / XZRatio + 0.5);
			
			// 判断是否在源图范围内
			if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 <lHeight ))
			{
				// 复制象素
				digitarray[j][i] = temp_img[lHeight * j0 + i0];
			}
			else
			{
				// 对于源图中没有的象素，直接赋值为255
				digitarray[j][i] = 255;
			}
			
		}
		
	}
	digitHeight=lNewHeight;
	digitWidth=lNewWidth;

	delete [] temp_img;
}

float CDigitClass::Marroperator(float dr, int x, int y)
{
	double a=(float) (-2.0/3.1415926/dr/dr/dr/dr);
	a=a*(1-(x*x+y*y)/2.0/dr/dr);
	double b=-(double)((x*x+y*y)/2.0/dr/dr);
	b=(double)(exp(b));
	a=a*b;
	return (float)(12.0*a);
}



void CDigitClass::MarrBinary()
{
	float *temp=NULL;
	float *Temp=NULL;
	int i,j;
	temp=new float[digitWidth*digitHeight];
	memset(temp,255,digitWidth*digitHeight*sizeof(float));
	Temp=temp;
	float dr=(float) 1.6;
	float Marr[9][9];
	for(int x=-4;x<=4;x++)
	{
		for(int y=-4;y<=4;y++)
		{
			float result=(float)(-Marroperator(dr,x,y));
			Marr[x+4][y+4]=result;
		}
	}

	for(i=4;i<digitHeight-4;i++)
	{
		for(j=4;j<digitWidth-4;j++)
		{
			Temp[i*digitWidth+j]=0;
			for(int di=-4;di<4;di++)
			{
				for(int dj=-4;dj<4;dj++)
				{
					Temp[i*digitWidth+j]+=
						Marr[4+di][4+dj]*(float)digitarray[j+dj][i+di];
				}
			}
		}
	}

	for(i=0;i<digitHeight;i++)
	{
		for(j=0;j<digitWidth;j++)
		{
			digitarray[j][i]=Temp[i*digitWidth+j]>0?1:0;
		}
	}
	delete [] temp;



}

BYTE CDigitClass::Interpolation(BYTE *image, LONG lWidth, LONG lHeight, FLOAT x, FLOAT y)
{
	// 四个最临近象素的坐标(i1, j1), (i2, j1), (i1, j2), (i2, j2)
	LONG	i1, i2;
	LONG	j1, j2;
	
	// 四个最临近象素值
	BYTE	f1, f2, f3, f4;
	
	// 二个插值中间值
	BYTE	f12, f34;
	
	// 定义一个值，当象素坐标相差小于改值时认为坐标相同
	FLOAT			EXP;
	
	// 赋值
	EXP = (FLOAT) 0.0001;
	
	// 计算四个最临近象素的坐标
	i1 = (LONG) x;
	i2 = i1 + 1;
	j1 = (LONG) y;
	j2 = j1 + 1;
	
	// 根据不同情况分别处理
	if( (x < 0) || (x > lWidth - 1) || (y < 0) || (y > lHeight - 1))
	{
		// 要计算的点不在源图范围内，直接返回255。
		return 255;
	}
	else
	{
		if (fabs(x - lWidth + 1) <= EXP)
		{
			// 要计算的点在图像右边缘上
			if (fabs(y - lHeight + 1) <= EXP)
			{
				// 要计算的点正好是图像最右下角那一个象素，直接返回该点象素值
				f1 = (BYTE) image[lWidth * j1 + i1];
				return f1;
			}
			else
			{
				// 在图像右边缘上且不是最后一点，直接一次插值即可
				f1 = (BYTE)image[lWidth * j1 + i1];
				f3 = (BYTE)image[lWidth * j1 + i2];
				
				// 返回插值结果
				return ((BYTE) (f1 + (y -j1) * (f3 - f1)));
			}
		}
		else if (fabs(y - lHeight + 1) <= EXP)
		{
			// 要计算的点在图像下边缘上且不是最后一点，直接一次插值即可
			f1 = (BYTE)image[lWidth * j1 + i1];
			f2 = (BYTE)image[lWidth * j2 + i1];
			
			// 返回插值结果
			return ((BYTE) (f1 + (x -i1) * (f2 - f1)));
		}
		else
		{
			// 计算四个最临近象素值
			f1 = (BYTE) image[lWidth * j1 + i1];
			f2 = (BYTE) image[lWidth * j2 + i1];
			f3 = (BYTE) image[lWidth * j1 + i2];
			f4 = (BYTE) image[lWidth * j2 + i2];
			
			// 插值1
			f12 = (BYTE) (f1 + (x - i1) * (f2 - f1));
			
			// 插值2
			f34 = (BYTE) (f3 + (x - i1) * (f4 - f3));
			
			// 插值3
			return ((BYTE) (f12 + (y -j1) * (f34 - f12)));
		}
	}

}
 
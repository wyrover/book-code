// WaterShedDoc.cpp : implementation of the CWaterShedDoc class
//

#include "stdafx.h"
#include "WaterShed.h"

#include "WaterShedDoc.h"
#include <queue>
using namespace std;
#include "MainFrm.h"
#include "math.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAXV 256
// Coefficient matrix for xyz and rgb spaces
static const int    XYZ[3][3] = { { 4125, 3576, 1804 },
{ 2125, 7154,  721 },
{  193, 1192, 9502 } };
static const double  RGB[3][3] = { 
	{ (float)3.2405, (float)-1.5371, (float)-0.4985 },
	{(float)-0.9693,  (float)1.8760,  (float)0.0416 },
	{ (float)0.0556, (float)-0.2040,  (float)1.0573 } };
	
	// Constants for LUV transformation 
	static const float     Xn = (float)0.9505;
	static const float     Yn = (float)1.0;
	static const float     Zn = (float)1.0888;
	static const float     Un_prime = (float)0.1978;
	static const float     Vn_prime = (float)0.4683;
	static const float     Lt = (float)0.008856;


/////////////////////////////////////////////////////////////////////////////
// CWaterShedDoc

IMPLEMENT_DYNCREATE(CWaterShedDoc, CDocument)

BEGIN_MESSAGE_MAP(CWaterShedDoc, CDocument)
	//{{AFX_MSG_MAP(CWaterShedDoc)
	ON_COMMAND(ID_WATERSHED, OnWatershed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaterShedDoc construction/destruction

CWaterShedDoc::CWaterShedDoc()
{
	// TODO: add one-time construction code here
	imageData = NULL;
	isImageLoaded=FALSE;
	imageName = "";
	myImageObject = NULL;
	luvData = NULL;
}

CWaterShedDoc::~CWaterShedDoc()
{
	if (imageData!=NULL)
	{
		delete [] imageData;
		imageData = NULL;
	}
	if (luvData!=NULL)
	{
		delete [] luvData;
		luvData = NULL;
	}
	
	if (myImageObject!=NULL)
	{
		delete myImageObject;
		myImageObject = NULL;
	}
}

BOOL CWaterShedDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWaterShedDoc serialization

void CWaterShedDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWaterShedDoc diagnostics

#ifdef _DEBUG
void CWaterShedDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWaterShedDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWaterShedDoc commands

BOOL CWaterShedDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
// 	if (!CDocument::OnOpenDocument(lpszPathName))
// 		return FALSE;
	
	// TODO: Add your specialized creation code here
	CString strPathName = lpszPathName;
	imageName = strPathName;
	while ( imageName.Find("\\", 0)>=0 && imageName!="") 
	{
		imageName.Delete(0, 1);
	} 
	
	myImageObject = new CImageObject(strPathName);
	if(myImageObject==NULL)
	{
		AfxMessageBox("Could not create image class!");
		return FALSE;
	}
	isImageLoaded = TRUE;
	
	//以下将RGB数据存入数组以备处理；
	LONG width = myImageObject->GetWidth();
	LONG height = myImageObject->GetHeight();
	dataLen = width*height*3;
	if ( imageData != NULL )
	{
		delete [] imageData;
		imageData = NULL;
	}
	imageData = new BYTE[width*height*3];
	imageWidth = width;
	imageHeight = height;
	myImageObject->LoadDIBToBuf(imageData);
	
	   //以下保存LUV数据；
	if ( luvData != NULL )
	{
		delete [] luvData;
		luvData = NULL;
	}
	luvData = new MyLUV[width*height];
	RgbtoLuvPcm(imageData, width, height, luvData);

	return TRUE;
}

void CWaterShedDoc::OnWatershed() 
{
	// TODO: Add your command handler code here

	BeginWaitCursor();

	LONG imagelen = imageWidth*imageHeight;
	FLOAT* deltar = new FLOAT[imagelen];//梯度模数组；
	FLOAT* deltasita = new FLOAT[imagelen];//梯度角度数组；
	INT*   flag = new INT[imagelen];//各点标识数组；
	INT*  gradientfre = new INT[256];//图像中各点梯度值频率；
	INT*  gradientadd = new INT[257];//各梯度起终位置；
	memset( gradientfre, 0, 256*sizeof(INT));
	memset( gradientadd, 0, 257*sizeof(INT));

	//首先得到各点梯度；
    GetGradient(imageData, imageWidth, imageHeight
		, deltar, deltasita);

	LONG temptime1 = GetTickCount();//初始时刻；
	//以下统计各梯度频率；
	MyImageGraPt*  graposarr = new MyImageGraPt[imagelen];
	LONG xstart, imagepos, deltapos;
	xstart = imagepos = deltapos = 0;
	for (INT y=0; y<imageHeight; y++)
	{
		xstart = y*imageWidth;
		for (INT x=0; x<imageWidth; x++)
		{
			deltapos = xstart + x;
			if (deltar[deltapos]>255)
			{
				deltar[deltapos] = 255;
			}
			INT tempi = (INT)(deltar[deltapos]);
			gradientfre[tempi] ++;//灰度值频率；
		}
	}

	//统计各梯度的累加概率；
	INT added = 0;
	gradientadd[0] = 0;//第一个起始位置为0；
	for (INT ii=1; ii<256; ii++)
	{
		added += gradientfre[ii-1];
		gradientadd[ii] = added;
	}
	gradientadd[256] = imagelen;//最后位置；

	memset( gradientfre, 0, 256*sizeof(INT));//清零，下面用作某梯度内的指针；
	//自左上至右下排序
    int y;
	for (y=0; y<imageHeight; y++)
	{
		xstart = y*imageWidth;
		for (INT x=0; x<imageWidth; x++)
		{
			deltapos = xstart + x;
			INT tempi = (INT)(deltar[deltapos]);//当前点的梯度值，由于前面的步骤，最大只能为255；
			//根据梯度值决定在排序数组中的位置；
			INT tempos = gradientadd[tempi] + gradientfre[tempi];
			gradientfre[tempi] ++;//梯度内指针后移；
			graposarr[tempos].gradient = tempi;	//根据当前点的梯度将该点信息放后排序后数组中的合适位置中去；		
			graposarr[tempos].x = x;
			graposarr[tempos].y = y;
		}
	}

	LONG temptime2 = GetTickCount();//vincent泛洪前的时刻；
	INT rgnumber = 0;//分割后的区域数；
	FloodVincent(graposarr, gradientadd, 0, 255, flag, rgnumber);//Flooding；
	LONG temptime3 = GetTickCount();//vincent泛洪后的时刻；
	LONG kk0 = temptime2 - temptime1;//排序用时；
	LONG kk1 = temptime3 - temptime2;//flood用时;
	LONG allkk = temptime3 - temptime1;//总用时；
	allkk = temptime3 - temptime1;//总用时；

 //区域增长步骤
	//以下准备计算各个区域的LUV均值；
	MyRgnInfo*  rginfoarr = new MyRgnInfo[rgnumber+1];//分割后各个区的一些统计信息,第一个元素不用，图像中各点所属区域的信息存放在flag数组中；
	//清空该数组；
	for (INT i=0; i<=rgnumber; i++)
	{
		rginfoarr[i].isflag = FALSE;
		rginfoarr[i].ptcount = 0;
		rginfoarr[i].l = 0;
		rginfoarr[i].u = 0;
		rginfoarr[i].v = 0;
	}

	for (y=0; y<imageHeight; y++)
	{
		xstart = y*imageWidth;
		for (INT x=0; x<imageWidth; x++)
		{
			INT pos = xstart + x;
			INT rgid = flag[pos];//当前位置点所属区域在区统计信息数组中的位置；
			//以下将该点的信息加到其所属区信息中去；
			rginfoarr[rgid].ptcount ++;
			rginfoarr[rgid].l += luvData[pos].l;//////////////////////////////////
			rginfoarr[rgid].u += luvData[pos].u;
			rginfoarr[rgid].v += luvData[pos].v;
		}
	}
	//求出各个区的LUV均值;
    int i;
	for (i=0; i<=rgnumber; i++)
	{
		rginfoarr[i].l = (FLOAT) ( rginfoarr[i].l / rginfoarr[i].ptcount );
		rginfoarr[i].u = (FLOAT) ( rginfoarr[i].u / rginfoarr[i].ptcount );
		rginfoarr[i].v = (FLOAT) ( rginfoarr[i].v / rginfoarr[i].ptcount );
	}

	//根据各区LUV均值（rginfoarr）和各区之间邻接关系（用flag计算）进行区域融合
	INT* mergearr = new INT[rgnumber+1];
	memset( mergearr, -1, sizeof(INT)*(rgnumber+1) );
	INT mergednum = 0;
	LONG temptime4 = GetTickCount();
	MergeRgs(rginfoarr, rgnumber, flag, imageWidth, imageHeight, mergearr, mergednum);
	LONG temptime5 = GetTickCount();
	LONG kk2 = temptime5 - temptime4;//合并用时;
	//确定合并后各像素点所属区域；
	for (y=0; y<(imageHeight); y++)
	{
		xstart = y*imageWidth;
		for (INT x=0; x<(imageWidth); x++)
		{
			INT pos = xstart + x;
			INT rgid = flag[pos];//该点所属区域；
			flag[pos] = FindMergedRgn(rgid, mergearr);
		}
	}
	delete [] mergearr; mergearr = NULL;

	//用各区均值代替原像素点值；
	FLOAT* luvbuff = NULL;
	luvbuff = new FLOAT[imageWidth*imageHeight*3];

	for (y=1; y<(imageHeight-1); y++)
	{
		xstart = y*imageWidth;
		for (INT x=1; x<(imageWidth-1); x++)
		{
			INT pos = xstart + x;
			INT rgid = flag[pos];//该点所属区域；
			luvData[pos].l = rginfoarr[rgid].l;//luvData用于全局保存LUV值;
			luvbuff[pos*3] = rginfoarr[rgid].l;//luvbuff用于传递参数给LuvToRgb();
			luvData[pos].u = rginfoarr[rgid].u;
			luvbuff[pos*3+1] = rginfoarr[rgid].u;
			luvData[pos].v = rginfoarr[rgid].v;
			luvbuff[pos*3+2] = rginfoarr[rgid].v;
		}
	}

	LuvToRgb(luvbuff, imageWidth, imageHeight, imageData);

	delete [] luvbuff; luvbuff = NULL;
	delete [] rginfoarr; rginfoarr = NULL;//大小等于区域总数


	//以下根据标识数组查找边界点（不管四边点以减小复杂度）；
	for (y=1; y<(imageHeight-1); y++)
	{
		xstart = y*imageWidth;
		for (INT x=1; x<(imageWidth-1); x++)
		{
			INT pos = xstart + x;
			INT imagepos = pos * 3;	
			INT left = pos - 1;
			INT up = pos - imageWidth;
			INT right = pos +1;
			INT down = pos + imageWidth;
			if ( ( flag[right]!=flag[pos] )
				|| ( flag[down]!=flag[pos] ) )
			//if ( flag[pos]==0 )
			{
				imageData[imagepos] = 0;
				imageData[imagepos+1] = 0;
				imageData[imagepos+2] = 250;
			}

		}
	}

	delete [] gradientadd; gradientadd = NULL;//大小256
	delete [] gradientfre; gradientfre = NULL;//大小256
	delete [] deltasita; deltasita = NULL;//大小imagelen
	delete [] deltar;    deltar    = NULL;//大小imagelen
	delete [] graposarr; graposarr = NULL;//大小imagelen
	delete [] flag;      flag      = NULL;//大小imagelen

	RefreshImageObject();

	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->GetMainWnd();
	EndWaitCursor();
	pFrame->pImageView->Invalidate(FALSE);	
	
}

void CWaterShedDoc::GetGradient(BYTE *image, INT width, INT height, FLOAT *deltar, FLOAT *deltasita)
{
	//下面计算各像素在水平和垂直方向上的梯度,边缘点梯度计为0；
	INT* deltaxarr;
	INT* deltayarr;
	INT grawidth = width;
	INT graheight = height;
	INT deltacount = grawidth * graheight;
	deltaxarr = new INT[deltacount];
	deltayarr = new INT[deltacount];
	
    //暂不计算边缘点；
	for (INT y=1; y<graheight-1; y++)
	{
		for (INT x=1; x<grawidth-1; x++)
		{
			INT inarrpos = ((y)*width + (x))*3 + 1;//在输入块中的位置；
			INT deltaarrpos = y*grawidth + x;//在梯度数组中的位置；
			//卷积计算；
			deltaxarr[deltaarrpos] = (INT) ( (
				image[((y-1)*width + (x+1))*3 + 1] //右上
				+ image[((y)*width + (x+1))*3 + 1] //右
				+ image[((y+1)*width + (x+1))*3 + 1] //右下
				- image[((y-1)*width + (x-1))*3 + 1] //左上
				- image[((y)*width + (x-1))*3 + 1] //左
				- image[((y+1)*width + (x-1))*3 + 1] ) / 3 );//左下
			deltayarr[deltaarrpos] = (INT) ( ( 
				image[((y-1)*width + (x+1))*3 + 1] //右上
				+ image[((y-1)*width + (x))*3 + 1] //上
				+ image[((y-1)*width + (x-1))*3 + 1] //左上
				- image[((y+1)*width + (x-1))*3 + 1] //左下
				- image[((y+1)*width + (x))*3 + 1] //下
				- image[((y+1)*width + (x+1))*3 + 1]) / 3 );//右下
		}
	}
	
	//边缘赋为其内侧点的值；
    int y;
	for (y=0; y<graheight; y++)
	{
		INT x1 = 0;
		INT pos1 = y*grawidth + x1;
		deltaxarr[pos1] = deltaxarr[pos1+1];
		deltayarr[pos1] = deltayarr[pos1+1];
		INT x2 = grawidth-1;
		INT pos2 = y*grawidth + x2;
		deltaxarr[pos2] = deltaxarr[pos2-1];
		deltayarr[pos2] = deltayarr[pos2-1];
	}
	for (INT x=0; x<grawidth; x++)
	{
		INT y1 = 0;
		INT pos1 = x;
		INT inner = x + grawidth;//下一行；
		deltaxarr[pos1] = deltaxarr[inner];
		deltayarr[pos1] = deltayarr[inner];
		INT y2 = graheight-1;
		INT pos2 = y2*grawidth + x;
		inner = pos2 - grawidth;//上一行；
		deltaxarr[pos2] = deltaxarr[inner];
		deltayarr[pos2] = deltayarr[inner];
	}
	
	int x;
	for (y=0; y<graheight; y++)
	{
		for (x=0; x<grawidth; x++)
		{
			INT temppos = y*grawidth + x;
			if ( (deltaxarr[temppos])==0 )
			{
				if (deltayarr[temppos]!=0)
				{
					deltasita[temppos] = 0;//水平方向;
					deltar[temppos] = (FLOAT) abs(deltayarr[temppos]);
				}else
				{
					deltasita[temppos] = -1;//无确定方向;
					deltar[temppos] = (FLOAT) abs(deltayarr[temppos]);
				}
				continue;
			}
			deltasita[temppos] = (FLOAT) ( atan( 
				(FLOAT)deltayarr[temppos]
				/ (FLOAT)deltaxarr[temppos] ) + PI/2. );
			deltar[temppos] = (FLOAT) sqrt((DOUBLE) 
				( deltayarr[temppos]*deltayarr[temppos]
				+ deltaxarr[temppos]*deltaxarr[temppos] ) );
		}
	}
	
	delete [] deltaxarr;
	deltaxarr = NULL; //删除水平和垂直梯度数组；
	delete [] deltayarr;
	deltayarr = NULL;
}

void CWaterShedDoc::RefreshImageObject()
{
	if(NULL!=myImageObject)
	{
		delete myImageObject;		
		myImageObject = NULL;
	}
	myImageObject = new CImageObject;
	myImageObject->CreateDIBFromBits(imageWidth, imageHeight, imageData);
}

void CWaterShedDoc::FloodVincent(MyImageGraPt *imiarr, INT *graddarr, INT minh, INT maxh, INT *flagarr, INT &outrgnumber)
{
	const INT INIT = -2;
	const INT MASK = -1;
	const INT WATERSHED = 0;
	INT h = 0;
	INT imagelen = imageWidth * imageHeight;
	for (INT i=0; i<imagelen; i++)
	{
		flagarr[i] = INIT;
	}
	//memset(flagarr, INIT, sizeof(INT)*imagelen);
	INT* imd = new INT[imagelen];//距离数组，直接存取；
    int i;
	for (i=0; i<imagelen; i++)
	{
		imd[i] = 0;
	}
	//memset(imd, 0, sizeof(INT)*imagelen);
	std::queue <int> myqueue;
	INT curlabel = 0;//各盆地标记；

	for (h=minh; h<=maxh; h++)
	{
		INT stpos = graddarr[h];
		INT edpos = graddarr[h+1];
		for (INT ini=stpos; ini<edpos; ini++)
		{
			INT x = imiarr[ini].x;
			INT y = imiarr[ini].y;
			INT ipos = y*imageWidth + x;
			flagarr[ipos] = MASK;
			//以下检查该点邻域是否已标记属于某区或分水岭，若是，则将该点加入fifo;
			INT left = ipos - 1;
			if (x-1>=0) 
			{
				if (flagarr[left]>=0)
				{
					imd[ipos] = 1;
					myqueue.push(ipos);//点位置压入fifo;
					continue;
				}				
			}
			INT right = ipos + 1;
			if (x+1<imageWidth) 
			{
				if (flagarr[right]>=0) 
				{
					imd[ipos] = 1;
					myqueue.push(ipos);//点位置压入fifo;
					continue;
				}
			}
			INT up = ipos - imageWidth;
			if (y-1>=0) 
			{
				if (flagarr[up]>=0)
				{
					imd[ipos] = 1;
					myqueue.push(ipos);//点位置压入fifo;
					continue;
				}				
			}
			INT down = ipos + imageWidth;
			if (y+1<imageHeight)
			{
				if (flagarr[down]>=0) 
				{
					imd[ipos] = 1;
					myqueue.push(ipos);//点位置压入fifo;
					continue;
				}			
			}
		}

    	//以下根据先进先出队列扩展现有盆地；
		INT curdist = 1; myqueue.push(-99);//特殊标记；
		while (TRUE)
		{
			INT p = myqueue.front();
			myqueue.pop();
			if (p == -99)
			{
				if ( myqueue.empty() )
				{
					break;
				}else
				{
					myqueue.push(-99);
					curdist = curdist + 1;
					p = myqueue.front();
					myqueue.pop();
				}
			}

			//以下找p的邻域；
			INT y = (INT) (p/imageWidth);
			INT x = p - y*imageWidth;
			INT left = p - 1;
			if  (x-1>=0)
			{
				if ( ( (imd[left]<curdist) && flagarr[left]>0)
					|| (flagarr[left]==0) ) 
				{
					if ( flagarr[left]>0 )
					{
						//ppei属于某区域（不是分水岭）；
						if ( (flagarr[p]==MASK) 
							|| (flagarr[p]==WATERSHED) )
						{
							//将其设为邻点所属区域；
							flagarr[p] = flagarr[left];
						}else if (flagarr[p]!=flagarr[left])
						{
							//原来赋的区与现在赋的区不同，设为分水岭；
							//flagarr[p] = WATERSHED;
						}
					}else if (flagarr[p]==MASK)//ppei为分岭；
					{
						flagarr[p] = WATERSHED;
					}
				}else if ( (flagarr[left]==MASK) && (imd[left]==0) )
				//ppei中已MASK的点，但尚未标记（即不属某区也不是分水岭）;
				{
					imd[left] = curdist + 1; myqueue.push(left);
				}
			}
			
			INT right = p + 1;
			if (x+1<imageWidth) 
			{
				if ( ( (imd[right]<curdist) &&  flagarr[right]>0)
					|| (flagarr[right]==0) )
				{
					if ( flagarr[right]>0 )
					{
						//ppei属于某区域（不是分水岭）；
						if ( (flagarr[p]==MASK) 
							|| (flagarr[p]==WATERSHED) )
						{
							//将其设为邻点所属区域；
							flagarr[p] = flagarr[right];
						}else if (flagarr[p]!=flagarr[right])
						{
							//原来赋的区与现在赋的区不同，设为分水岭；
							//flagarr[p] = WATERSHED;
						}
					}else if (flagarr[p]==MASK)//ppei为分岭；
					{
						flagarr[p] = WATERSHED;
					}
				}else if ( (flagarr[right]==MASK) && (imd[right]==0) )
					//ppei中已MASK的点，但尚未标记（即不属某区也不是分水岭）;
				{
					imd[right] = curdist + 1; myqueue.push(right);
				}
			}
			
			INT up = p - imageWidth;
			if (y-1>=0) 
			{
				if ( ( (imd[up]<curdist) &&  flagarr[up]>0)
					|| (flagarr[up]==0) )
				{
					if ( flagarr[up]>0 )
					{
						//ppei属于某区域（不是分水岭）；
						if ( (flagarr[p]==MASK) 
							|| (flagarr[p]==WATERSHED) )
						{
							//将其设为邻点所属区域；
							flagarr[p] = flagarr[up];
						}else if (flagarr[p]!=flagarr[up])
						{
							//原来赋的区与现在赋的区不同，设为分水岭；
							//flagarr[p] = WATERSHED;
						}
					}else if (flagarr[p]==MASK)//ppei为分岭；
					{
						flagarr[p] = WATERSHED;
					}
				}else if ( (flagarr[up]==MASK) && (imd[up]==0) )
					//ppei中已MASK的点，但尚未标记（即不属某区也不是分水岭）;
				{
					imd[up] = curdist + 1; myqueue.push(up);
				}
			}
			
			INT down = p + imageWidth;
			if (y+1<imageHeight) 
			{
				if ( ( (imd[down]<curdist) &&  flagarr[down]>0)
					|| (flagarr[down]==0) )
				{
					if ( flagarr[down]>0 )
					{
						//ppei属于某区域（不是分水岭）；
						if ( (flagarr[p]==MASK) 
							|| (flagarr[p]==WATERSHED) )
						{
							//将其设为邻点所属区域；
							flagarr[p] = flagarr[down];
						}else if (flagarr[p]!=flagarr[down])
						{
							//原来赋的区与现在赋的区不同，设为分水岭；
							//flagarr[p] = WATERSHED;
						}
					}else if (flagarr[p]==MASK)//ppei为分岭；
					{
						flagarr[p] = WATERSHED;
					}
				}else if ( (flagarr[down]==MASK) && (imd[down]==0) )
					//ppei中已MASK的点，但尚未标记（既不属某区也不是分水岭）;
				{
					imd[down] = curdist + 1; myqueue.push(down);
				}	
			}

		}//以上现有盆地的扩展；

		//以下处理新发现的盆地；
        int ini;
		for (ini=stpos; ini<edpos; ini++)
		{
			INT x = imiarr[ini].x;
			INT y = imiarr[ini].y;
			INT ipos = y*imageWidth + x;
			imd[ipos] = 0;//重置所有距离
			if (flagarr[ipos]==MASK)
			{
				//经过前述扩展后该点仍为MASK，则该点必为新盆地的一个起始点;
				curlabel = curlabel + 1;
				myqueue.push(ipos); 
				flagarr[ipos] = curlabel;
				
				while ( myqueue.empty()==FALSE )
				{
					INT ppei = myqueue.front();
					myqueue.pop();
					INT ppeiy = (INT) (ppei/imageWidth);
			        INT ppeix = ppei - ppeiy*imageWidth;
					
					INT ppeileft = ppei - 1;
					if ( (ppeix-1>=0) && (flagarr[ppeileft]==MASK) )
					{
						myqueue.push(ppeileft);//点位置压入fifo;
						flagarr[ppeileft] = curlabel;
					}
					INT ppeiright = ppei + 1;
					if ( (ppeix+1<imageWidth) && (flagarr[ppeiright]==MASK) )
					{
						myqueue.push(ppeiright);//点位置压入fifo;
						flagarr[ppeiright] = curlabel;
					}
					INT ppeiup = ppei - imageWidth;
					if ( (ppeiy-1>=0) && (flagarr[ppeiup]==MASK) )
					{
						myqueue.push(ppeiup);//点位置压入fifo;
						flagarr[ppeiup] = curlabel;
					}
					INT ppeidown = ppei + imageWidth;
					if ( (ppeiy+1<imageHeight) && (flagarr[ppeidown]==MASK) )
					{
						myqueue.push(ppeidown);//点位置压入fifo;
						flagarr[ppeidown] = curlabel;
					}					
				}				
			}
		}//以上处理新发现的盆地；

	}

	outrgnumber = curlabel;	
	delete [] imd; imd = NULL;
}

BOOL CWaterShedDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	//return CDocument::OnSaveDocument(lpszPathName);
	if( myImageObject != NULL )
	{
	//	CString strPathName = lpszPathName;
		myImageObject->SaveToFile( lpszPathName );
		SetModifiedFlag(FALSE);
	}	
	return TRUE;
}

	//////////////////////////////////////////////////////////////////////////
	//1、建立各区的邻域数组；
	//2、依次扫描各区域，寻找极小区域；
	//3、对每个极小区（A），在相邻区中找到最相似者；
	//4、与相似区（B）合并（各种信息刷新），在极小区（A）的邻域中
	//   删除相似区（B），在邻域数组中删除相似区（B）对应的项，将
	//   相似区（B）的相邻区s加到极小区（A）的邻域中去；
	//5、记录合并信息，设一数组专门存放该信息，该数组的第A个元素值设为B；
	//6、判断是否仍为极小区，若是则返回3；
	//7、是否所有区域都已处理完毕，若非则返回2；
	//
	//   由于各区的相邻区不会太多，因此采用邻接数组作为存储结构；
	//////////////////////////////////////////////////////////////////////////
void CWaterShedDoc::MergeRgs(MyRgnInfo *rginfoarr, INT rgnumber, INT *flag, INT width, INT height, INT *outmerge, INT &rgnum)
{

	CString* neiarr = new CString[rgnumber+1];//第一个不用；
	INT* mergearr = outmerge;//记录合并情况数组；
	
	//建立邻域数组；
	for (INT y=0; y<height; y++)
	{
		INT lstart = y * width;
		for (INT x=0; x<width; x++)
		{
			INT pos = lstart + x;
			INT left=-1, right=-1, up=-1, down=-1;
			myMath.GetNeiInt(x, y, pos, width, height
				, left, right, up, down);//找pos的四个邻域；
			//确定并刷新邻域区信息；
			INT curid = flag[pos];
			AddNeiOfCur(curid, left, right
				, up, down, flag, neiarr);
		}
	}//建立邻域数组；
	
	//区域信息数组中的有效信息从1开始，第i个位置存放第i个区域的信息；
	for (INT rgi=1; rgi<=rgnumber; rgi++)
	{
		//扫描所有区域，找极小区；
		LONG allpoints = imageWidth * imageHeight;
		LONG nmin = (LONG) (allpoints / 400);
		INT curid = rgi;
		
		//rginfoarr[rgi].isflag初始为FALSE，在被合并到其它区后改为TRUE；
		while ( ( (rginfoarr[rgi].ptcount)<nmin ) 
			&& !rginfoarr[rgi].isflag )
		{
			//该区为极小区，遍历所有相邻区，找最接近者；
			CString neistr = neiarr[curid];
			INT nearid = FindNearestNei(curid, neistr, rginfoarr, mergearr);
			//合并curid与nearid；
			MergeTwoRgn(curid, nearid, neiarr
				, rginfoarr, mergearr);			
		} 
	}
	
	//以下再合并相似区域，（无论大小）,如果不需要，直接将整个循环注释掉就行了；
	INT countjjj = 0;
	//区域信息数组中的有效信息从1开始，第i个位置存放第i个区域的信息；
	for (INT ii=1; ii<=rgnumber; ii++)
	{
		if (!rginfoarr[ii].isflag)
		{
			INT curid = ii;
			MergeNearest(curid, rginfoarr, neiarr, mergearr);
		}
	}
	
	
	
	INT counttemp = 0;
	for (INT i=0; i<rgnumber; i++)
	{
		if (!rginfoarr[i].isflag)
		{
			counttemp ++;
		}
	}
	
	rgnum = counttemp;
	
	delete [] neiarr; neiarr = NULL;
}


BOOL CWaterShedDoc::LuvToRgb(FLOAT *luvData, INT width, INT height, BYTE *rgbData)
{
	if (NULL == rgbData)
	{
		CString tempstr;
		tempstr.Format("传入内存未分配，在MyColorSpace::RGBtoHSV");
		AfxMessageBox(tempstr,NULL,MB_OK);
		return FALSE;		
	}
	
	int i,j;
	INT r,g,b;
	FLOAT l, u, v;
	unsigned long pos;
	
	for(i=0;i<height;i++)
	{
		for (j=0;j<width;j++)
		{
			pos = (i*width+j) * 3;
			l = luvData[pos];
			u = luvData[pos+1];
			v = luvData[pos+2];
			
			Luv2Rgb(l, u, v, r, g, b);
			rgbData[pos] = b;
			rgbData[pos+1] = g;
			rgbData[pos+2] = r;			
		}
	}
	return TRUE;
}

BOOL CWaterShedDoc::Luv2Rgb(FLOAT l, FLOAT u, FLOAT v, INT &r, INT &g, INT &b)
{
	if(l<0.1)
	{
		r = g = b =0;
	}
	else
	{
		float my_x, my_y, my_z;
		if(l < 8.0)
			my_y = (FLOAT) ( Yn * l / 903.3 );
		else
			my_y = (FLOAT) ( Yn * pow((l + 16.0) / 116.0, 3) );
		
		float u_prime = u / (13 * l) + Un_prime;
		float v_prime = v / (13 * l) + Vn_prime;
		
		my_x = 9 * u_prime * my_y / (4 * v_prime);
		my_z = (12 - 3 * u_prime - 20 * v_prime) * my_y / (4 * v_prime);
		
		r =int((RGB[0][0]*my_x + RGB[0][1]*my_y + RGB[0][2]*my_z)*255.0);
		g =int((RGB[1][0]*my_x + RGB[1][1]*my_y + RGB[1][2]*my_z)*255.0);
		b =int((RGB[2][0]*my_x + RGB[2][1]*my_y + RGB[2][2]*my_z)*255.0);
		
		if(r>255) r=255;
		else if(r<0) r=0;
		
		if(g>255) g=255;
		else if(g<0) g=0;
		
		if(b>255) b=255;
		else if(b<0) b=0;
	}
	
	return TRUE;
}

//刷新当前点的所有相邻区；
void CWaterShedDoc::AddNeiOfCur(INT curid, INT left, INT right, INT up, INT down, INT *flag, CString *neiarr)
{
	INT leftid, rightid, upid, downid;
	leftid = rightid = upid = downid = curid;
	if (left>=0)
	{
		leftid = flag[left];
		if (leftid!=curid)
		{
			//邻点属于另一区, 加邻域点信息；
			AddNeiRgn(curid, leftid, neiarr);
		}
	}
	if (right>0)
	{
		rightid = flag[right];
		if (rightid!=curid)
		{
			//邻点属于另一区, 加邻域点信息；
			AddNeiRgn(curid, rightid, neiarr);
		}
	}
	if (up>=0)
	{
		upid = flag[up];
		if (upid!=curid)
		{
			//邻点属于另一区, 加邻域点信息；
			AddNeiRgn(curid, upid, neiarr);
		}
	}
	if (down>0)
	{
		downid = flag[down];
		if (downid!=curid)
		{
			//邻点属于另一区, 加邻域点信息；
			AddNeiRgn(curid, downid, neiarr);
		}
	}
}

void CWaterShedDoc::AddNeiRgn(INT curid, INT neiid, CString *neiarr)
//增加neiid为curid的相邻区
{
	CString tempneis = neiarr[curid];//当前的相邻区；
	CString toaddstr;
	toaddstr.Format("%d ", neiid);
	
	INT temppos = tempneis.Find(toaddstr, 0);
	while (temppos>0 && neiarr[curid].GetAt(temppos-1)!=' ')
	{
		temppos = neiarr[curid].Find(toaddstr, temppos+1);
	}
	
	if ( temppos<0 )
	{
		//当前相邻区中没有tempneis,则加入
		neiarr[curid] += toaddstr;
	}
}

//找到idint最终所合并到的区号；
int CWaterShedDoc::FindMergedRgn(INT idint, INT *mergearr)
{
	INT outid = idint;
	while ( mergearr[outid] > 0 )
	{
		outid = mergearr[outid];
	}
	return outid;
}

int CWaterShedDoc::FindNearestNei(INT curid, CString neistr, MyRgnInfo *rginfoarr, INT *mergearr)
//寻找neistr中与curid最接近的区，返回该区id号；
{
	INT outid = -1;
	DOUBLE mindis = 999999;
	FLOAT cl, cu, cv;
	cl = rginfoarr[curid].l;//当前区的LUV值；
	cu = rginfoarr[curid].u;
	cv = rginfoarr[curid].v;
	
	CString tempstr = neistr;//用于本函数内部处理；
	while (tempstr.GetLength()>0)
	{
		INT pos = tempstr.Find(" ");
		ASSERT(pos>=0);
		CString idstr = tempstr.Left(pos);
		tempstr.Delete(0, pos+1);
		
		INT idint = (INT) strtol(idstr, NULL, 10);
		//判断该区是否已被合并，若是，则一直找到该区当前的区号；
		idint = FindMergedRgn(idint, mergearr);
		if (idint==curid)
		{
			continue;//这个邻区已被合并到当前区，跳过；
		}
		FLOAT tl, tu, tv;
		tl = rginfoarr[idint].l;//当前处理的邻区的LUV值;
		tu = rginfoarr[idint].u;
		tv = rginfoarr[idint].v;
		DOUBLE tempdis = pow(tl-cl, 2) 
			+ pow(tu-cu, 2) + pow(tv-cv, 2);
		if (tempdis<mindis)
		{
			mindis = tempdis;//最大距离和对应的相邻区ID；
			outid = idint;
		}		
	}
	
	return outid;
}

//将nearid合并到curid中去，更新合并后的区信息，并记录该合并；
void CWaterShedDoc::MergeTwoRgn(INT curid, INT nearid, CString *neiarr, MyRgnInfo *rginfoarr, INT *mergearr)
{
	//将区信息中nearid对应项的标记设为已被合并；
	rginfoarr[nearid].isflag = TRUE;
	//更新合并后的LUV信息；
	LONG ptincur = rginfoarr[curid].ptcount;
	LONG ptinnear = rginfoarr[nearid].ptcount;
	DOUBLE curpercent = (FLOAT)ptincur / (FLOAT)(ptincur+ptinnear);
	rginfoarr[curid].ptcount = ptincur + ptinnear;
	rginfoarr[curid].l = (FLOAT) ( curpercent * rginfoarr[curid].l
		+ (1-curpercent) * rginfoarr[nearid].l );
	rginfoarr[curid].u = (FLOAT) ( curpercent * rginfoarr[curid].u
		+ (1-curpercent) * rginfoarr[nearid].u );
	rginfoarr[curid].v = (FLOAT) ( curpercent * rginfoarr[curid].v
		+ (1-curpercent) * rginfoarr[nearid].v );
	//将nearid的邻域加到curid的邻域中去；
	AddBNeiToANei(curid, nearid, neiarr, mergearr);
	//记录该合并；
	mergearr[nearid] = curid;
}

//将nearid的邻域加到curid的邻域中去；
void CWaterShedDoc::AddBNeiToANei(INT curid, INT nearid, CString *neiarr, INT *mergearr)
{
	//先从curid的邻区中把nearid删去；
	/*
	CString tempstr;
	tempstr.Format("%d ", nearid);
	INT temppos = neiarr[curid].Find(tempstr, 0);
	while (temppos>0 && neiarr[curid].GetAt(temppos-1)!=' ')
	{
	temppos = neiarr[curid].Find(tempstr, temppos+1);
	}
	if (temppos>=0)
	{
	//否则邻近区为合并过来的区，忽略；
	neiarr[curid].Delete(temppos, tempstr.GetLength());
	}
	*/
    //将nearid的邻区依次加到curid的邻区中去；
	CString neistr = neiarr[nearid];
	CString curstr = neiarr[curid];
	//一般说来，极小区的邻域应该较少，因此，为着提高合并速度，将
	//curstr加到neistr中去，然后将结果赋给neiarr[curid];
	while ( curstr.GetLength()>0 )
	{
		INT pos = curstr.Find(" ");		
		ASSERT(pos>=0);
		CString idstr = curstr.Left(pos);
        curstr.Delete(0, pos+1);
		INT idint = (INT) strtol(idstr, NULL, 10);
		idint = FindMergedRgn(idint, mergearr);
		idstr += " ";
		if ( (idint == curid) || (idint == nearid) )
		{
			continue;//本区不与本区相邻；
		}else
		{
			if ( neistr.Find(idstr, 0) >= 0 )
			{
				continue;
			}else
			{
				neistr += idstr;//加到邻区中去;
			}
		}		
	}
	neiarr[curid] = neistr;
	/*
	CString toaddneis = neiarr[nearid];
	while (toaddneis.GetLength()>0)
	{
	INT pos = toaddneis.Find(" ");		
	ASSERT(pos>=0);
	CString idstr = toaddneis.Left(pos);
	toaddneis.Delete(0, pos+1);
	INT idint = (INT) strtol(idstr, NULL, 10);
	idint = FindMergedRgn(idint, mergearr);
	idstr += " ";
	if ( (idint == curid) || (idint == nearid) )
	{
	continue;//本区不与本区相邻；
	}else
	{
	if ( neiarr[curid].Find(idstr, 0) >= 0 )
	{
				continue;
				}else
				{
				neiarr[curid] += idstr;//加到邻区中去;
				}
				}		
				}
	*/
}

#define NearMeasureBias 200.0//判定区域颜色相似的阈值；
void CWaterShedDoc::MergeNearest(INT curid, MyRgnInfo *rginfoarr, CString *neiarr, INT *mergearr)
//合并相似区域；
{
	//依次处理各个邻域，若相似，则合并；
	//CString neistr = neiarr[curid];
	FLOAT cl, cu, cv;
	cl = rginfoarr[curid].l;//当前区的LUV值；
	cu = rginfoarr[curid].u;
	cv = rginfoarr[curid].v;
	BOOL loopmerged = TRUE;//一次循环中是否有合并操作发生，若无，则退出循环；
	
	while (loopmerged)
	{
		loopmerged = FALSE;
		CString tempstr = neiarr[curid];//用于本函数内部处理；
		while (tempstr.GetLength()>0)
		{
			INT pos = tempstr.Find(" ");
			ASSERT(pos>=0);
			CString idstr = tempstr.Left(pos);
			tempstr.Delete(0, pos+1);
			
			INT idint = (INT) strtol(idstr, NULL, 10);
			//判断该区是否已被合并，若是，则一直找到该区当前的区号；
			idint = FindMergedRgn(idint, mergearr);
			if (idint==curid)
			{
				continue;//这个邻区已被合并到当前区，跳过；
			}
			FLOAT tl, tu, tv;
			tl = rginfoarr[idint].l;//当前处理的邻区的LUV值;
			tu = rginfoarr[idint].u;
			tv = rginfoarr[idint].v;
			DOUBLE tempdis = pow(tl-cl, 2) 
				+ pow(tu-cu, 2) + pow(tv-cv, 2);
			if (tempdis<NearMeasureBias)
			{
				MergeTwoRgn(curid, idint, neiarr, rginfoarr, mergearr);
				cl = rginfoarr[curid].l;//当前区的LUV值刷新；
				cu = rginfoarr[curid].u;
				cv = rginfoarr[curid].v;
				loopmerged = TRUE;
			}		
		}
	}
}

//基于表转换，将位图图像数据转换为LUV数据, 修改自D. Comaniciu, P. Meer，
//Robust Analysis of Feature Spaces: Color Image Segmentation 的相应代码

BOOL CWaterShedDoc::RgbtoLuvPcm(BYTE *inDatas, int width, int height, MyLUV *luvbuff)	
{
	int x, y, z, my_temp;
	
	float l_star, u_star, v_star;
	float u_prime, v_prime;
	//register int temp_col, temp_index, temp_ind;
	register int j;//,k;
	
	int a00=XYZ[0][0], a01=XYZ[0][1], a02=XYZ[0][2];
	int a10=XYZ[1][0], a11=XYZ[1][1], a12=XYZ[1][2];
	int a20=XYZ[2][0], a21=XYZ[2][1], a22=XYZ[2][2];
	
	int *A00 = new int[MAXV]; int *A01 = new int[MAXV]; int *A02 = new int[MAXV];
	int *A10 = new int[MAXV]; int *A11 = new int[MAXV]; int *A12 = new int[MAXV];
	int *A20 = new int[MAXV]; int *A21 = new int[MAXV]; int *A22 = new int[MAXV];
	
	for(j=0; j<MAXV; j++)
	{
		A00[j]=a00*j; A01[j]=a01*j; A02[j]=a02*j;
		A10[j]=a10*j; A11[j]=a11*j; A12[j]=a12*j;
		A20[j]=a20*j; A21[j]=a21*j; A22[j]=a22*j;
	}
	
	float *my_pow = new float[MAXV];
	for(j=0; j<MAXV; j++)
		my_pow[j]= (FLOAT) ( 116.0 * pow(j/255.0, 0.3333333) - 16 );
	
	for ( j = 0; j < width*height; j++)
	{
		INT R = inDatas[j*3+2];
		INT G = inDatas[j*3+1];
		INT B = inDatas[j*3];
		
		x = A00[R] + A01[G] + A02[B];
		y = A10[R] + A11[G] + A12[B];
		z = A20[R] + A21[G] + A22[B];
		
		float  tval = (FLOAT) ( y / 2550000.0 ); //Yn==1
		if ( tval >  Lt)  l_star = my_pow[(int)(tval*255+0.5)];
		else  l_star = (FLOAT) ( 903.3 * tval );
		
		my_temp = x + 15 * y + 3 * z;
		if(my_temp)
		{
			u_prime = (float)(x << 2) / (float)(my_temp);
			v_prime = (float)(9 * y) / (float)(my_temp);
		}
		else
		{
			u_prime = 4.0;
			v_prime = (FLOAT) ( 9.0/15.0 );
		}
		
		tval = 13*l_star;
		u_star = tval * (u_prime - Un_prime); // Un_prime = 0.1978
		v_star = tval * (v_prime - Vn_prime); // Vn_prime = 0.4683
		
		luvbuff[j].l = l_star;
		luvbuff[j].u = u_star;
		luvbuff[j].v = v_star;
	}
	
	delete [] my_pow;
	delete [] A22; delete [] A21; delete [] A20;
	delete [] A12; delete [] A11; delete [] A10;
	delete [] A02; delete [] A01; delete [] A00;
	
	return TRUE;
}


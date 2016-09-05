// PlateIdentifyDoc.cpp : CPlateIdentifyDoc 类的实现
//

#include "stdafx.h"
#include "PlateIdentify.h"

#include "PlateIdentifyDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int Units[NUM_LAYERS] = {N, 10, M};                            //用一维数组记录各层神经元个数 
FILE* f;                                                       //声明文件指针 

double Input[NUM_DATA][N];                                     //用来记录学习样本输入模式 
double Inputtest[NUM_DATA][N];                                 //用来记录测试样本输入模式 
char Pattern[NUM_DATA][Y][X];                                  //用来记录训练集的数组
char testPattern[NUM_DATA][Y][X];                              //用来记录测试集的数组
double Target[NUM_DATA][M] ;                                   //用来记录目标向量的数组
// CPlateIdentifyDoc

IMPLEMENT_DYNCREATE(CPlateIdentifyDoc, CDocument)

BEGIN_MESSAGE_MAP(CPlateIdentifyDoc, CDocument)
    ON_COMMAND(IDM_GRAY, &CPlateIdentifyDoc::OnGray)
    ON_COMMAND(IDM_GRAYSTRENTH, &CPlateIdentifyDoc::OnGraystrenth)
    ON_COMMAND(IDM_EDGEADD, &CPlateIdentifyDoc::OnEdgeadd)
    ON_COMMAND(IDM_BINARY, &CPlateIdentifyDoc::OnBinary)
    ON_COMMAND(IDM_POSITION, &CPlateIdentifyDoc::OnPosition)
    ON_COMMAND(IDM_STATE, &CPlateIdentifyDoc::OnState)
    ON_COMMAND(IDM_REVISE, &CPlateIdentifyDoc::OnRevise)
    ON_COMMAND(ID_SPLIT, &CPlateIdentifyDoc::OnSplit)
    ON_COMMAND(IDM_NETTRAIN, &CPlateIdentifyDoc::OnNettrain)
    ON_COMMAND(ID_GUIYI, &CPlateIdentifyDoc::OnGuiyi)
    //ON_COMMAND(ID_TEST, &CPlateIdentifyDoc::OnTest)
    ON_COMMAND(IDM_CHART_IDENTIFY, &CPlateIdentifyDoc::OnChartIdentify)
END_MESSAGE_MAP()


// CPlateIdentifyDoc 构造/析构

CPlateIdentifyDoc::CPlateIdentifyDoc()
{
	// TODO: 在此添加一次性构造代码
    //构造函数中变量初始化
     m_image=NULL;                         
     gray=NULL;
     slipimage=NULL;

}

CPlateIdentifyDoc::~CPlateIdentifyDoc()
{
     //释放资源
     cvReleaseImage(&m_image);
     cvReleaseImage(&gray);
     cvReleaseImage(&slipimage);
}

BOOL CPlateIdentifyDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CPlateIdentifyDoc 序列化

void CPlateIdentifyDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CPlateIdentifyDoc 诊断

#ifdef _DEBUG
void CPlateIdentifyDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPlateIdentifyDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CPlateIdentifyDoc 命令

BOOL CPlateIdentifyDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;

    // TODO:  在此添加您专用的创建代码
    //使用opencv打开文件
    if(0==(m_image=cvLoadImage(lpszPathName,CV_LOAD_IMAGE_ANYCOLOR)))
        return FALSE;
    else
    {
        m_Cimage.mSetImg(m_image);

        return TRUE;
    }
}

BOOL CPlateIdentifyDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    // TODO: 在此添加专用代码和/或调用基类
        
    //使用opencv保存文件   
    m_Cimage.Save(lpszPathName);
    return TRUE;
}
/********************************************************************************
   灰度化处理
   采用opencv

*********************************************************************************/
void CPlateIdentifyDoc::OnGray()
{
    // TODO: 在此添加命令处理程序代码
    gray = cvCreateImage( cvGetSize(m_image), 8, 1 );
    cvCvtColor(m_image,gray,CV_BGR2GRAY);
    m_Cimage.mSetImg(gray);
    //文档数据改变通知视图改变窗口
    UpdateAllViews(NULL);
}

/********************************************************************************
   灰度拉伸处理
   采用opencv

*********************************************************************************/
void CPlateIdentifyDoc::OnGraystrenth()
{
    // TODO: 在此添加命令处理程序代码
    ImageStretchByHistogram(gray,gray); 
    m_Cimage.mSetImg(gray);
    UpdateAllViews(NULL);

}
/********************************************************************************
   灰度增强处理
   由灰度图像的域值拉伸到0-255根据直方图进行图像增强
*********************************************************************************/
int ImageStretchByHistogram(IplImage *src,IplImage *dst)
{
    assert(src->width==dst->width);
    float p[256],p1[256],num[256];
    //清空三个数组
    memset(p,0,sizeof(p));
    memset(p1,0,sizeof(p1));
    memset(num,0,sizeof(num));

    int height=src->height;
    int width=src->width;
    long wMulh = height * width;

    //求存放图象各个灰度级出现的次数
    for(int x=0;x<width;x++)
    {
        for(int y=0;y<height;y++)
        {
            uchar v=((uchar*)(src->imageData + src->widthStep*y))[x];
            num[v]++;
        }
    }

    //求存放图像各个灰度级的出现概率
    for(int i=0;i<256;i++)
    {
        p[i]=num[i]/wMulh;
    }

    //求存放各个灰度级之前的概率和
    for(int i=0;i<256;i++)
    {
        for(int k=0;k<=i;k++)
            p1[i]+=p[k];
    }

    //直方图变换
    for(int x=0;x<width;x++)
    {
        for(int y=0;y<height;y++)
        {
            uchar v=((uchar*)(src->imageData + src->widthStep*y))[x];
            ((uchar*)(dst->imageData + dst->widthStep*y))[x]= p1[v]*255+0.5;           
        }
    }

    return 0;

} 
/********************************************************************************
   图像边缘化处理
   采用opencv的Canny算子
*********************************************************************************/
void CPlateIdentifyDoc::OnEdgeadd()
{
    // TODO: 在此添加命令处理程序代码
    cvCanny(gray,gray,50,150,3);
    m_Cimage.mSetImg(gray);
	UpdateAllViews(NULL);

}
/********************************************************************************
   图像二值化处理
   采用opencv
*********************************************************************************/
void CPlateIdentifyDoc::OnBinary()
{
    // TODO: 在此添加命令处理程序代码
    cvThreshold( gray, gray, 0,255,CV_THRESH_OTSU );
    m_Cimage.mSetImg(gray);
	UpdateAllViews(NULL);
}
/********************************************************************************
   车牌定位
  
*********************************************************************************/
void CPlateIdentifyDoc::OnPosition()
{
    // TODO: 在此添加命令处理程序代码
    //车牌定位
  CvScalar s;                                       //用于读取图像的二值值
  int imgBottom,imgTop,imgLeft,imgRight;            //车牌的顶部、底部、左侧、右侧的坐标
  int *imgResult;
  int imgHeight= gray->height;
  int imgWidth= gray->width;
  imgResult=new int[imgHeight];
  imgBottom=imgHeight;
  imgTop=0;
  imgLeft=0;
  imgRight=imgWidth;
  bool tag=FALSE;
  CString str;
  //从图像的1/2处到4/5处搜索做水平投影当有超过12个跳变的时候认为是车牌位置确定水平位置
   for(int ht=imgHeight/2;ht<imgHeight*0.8;ht+=3)
   {
       imgResult[ht]=0;
       for(int wt=0;wt<gray->width;wt++)
       {
            s=cvGet2D(gray,ht,wt);
            if(255==s.val[0])
            {
                imgResult[ht]++;
            }
       }
       if((!tag)&&imgResult[ht]>12)
       {
           str.Format("%d",imgResult[ht]);
            AfxMessageBox(str);
           imgTop=ht;
           tag=TRUE;
       }
       if(tag&&(ht>imgTop+8)&&imgResult[ht]<12)  
       {
           imgBottom=ht;
           tag=FALSE;
           break;
       }
   }

  
   //从图像左侧到右侧搜索做垂直投影当有超过10个跳变的时候认为是车牌位置确定垂直位置
   CvRect selection;                                
   for(int wt_new=gray->width/3;wt_new<gray->width;wt_new++)
   {
       imgResult[wt_new]=0;
       for(int ht_new=imgTop;ht_new<imgBottom;ht_new++)
       {
            s=cvGet2D(gray,ht_new,wt_new);
            if(255==s.val[0])
            {
                imgResult[wt_new]++;
            }
       }
       if((!tag)&&((imgResult[wt_new]>10)||(imgResult[wt_new]>imgResult[wt_new-1]+5)))
       {
           imgLeft=wt_new;
           break;
       }
       
   }     
   
   //确定车牌的位置坐标
   selection.x=imgLeft+30;       
   selection.y=imgTop;
   selection.width=120;
   selection.height=imgBottom-imgTop-10;
   cvSetImageROI(m_image, selection);
   cvCopy(m_image,m_image);
   m_Cimage.mSetImg(m_image);
   UpdateAllViews(NULL);

}
/********************************************************************************
   形态学处理腐蚀膨胀
   采用opencv
*********************************************************************************/
void CPlateIdentifyDoc::OnState()
{
    // TODO: 在此添加命令处理程序代
    //形态学处理

    IplConvKernel *element=cvCreateStructuringElementEx(3,1,0.5,0.5,CV_SHAPE_RECT,0);
    cvMorphologyEx( gray, gray, NULL,element, CV_MOP_CLOSE, 8);

    m_Cimage.mSetImg(gray);
	UpdateAllViews(NULL);
}
/********************************************************************************
   车牌的倾斜校正
   通过计算图像的前半部分的斜率和后半部分的斜率进行调整然后将图像映射到新的位置上
*********************************************************************************/
void CPlateIdentifyDoc::OnRevise()
{
    // TODO: 在此添加命令处理程序代码
    CString str;
    CvScalar s,s_new;
    double num=0;
    double leftaverage=0;
    double rightaverage=0;
    int iHeight=gray->height;
    int iWidth=gray->width;
    double slope=0;
    int pix_new;
    //计算前半部分斜率
    for(int ht=0;ht<iHeight;ht++)
    {
        for(int wt=0;wt<iWidth/2;wt++)
        {
            s=cvGet2D(gray,ht,wt);
            if(0==s.val[0])
            {
                num+=iWidth/2-wt;
                leftaverage+=ht*(iWidth/2-wt);
            }
        }
    }
    leftaverage/=num;
    num=0;
    //计算后半部分斜率
    for(int ht=0;ht<iHeight;ht++)
    {
        for(int wt=iWidth/2;wt<iWidth;wt++)
        {
            s=cvGet2D(gray,ht,wt);
            if(0==s.val[0])
            {
                num+=iWidth-wt;
                rightaverage+=ht*(iWidth-wt);
            }
        }
    }
    rightaverage/=num;
    slope=(rightaverage-leftaverage)/ (iWidth/2);
    str.Format("%f",slope);
    AfxMessageBox(str);
    //图像映射
    for(int ht=0;ht<iHeight;ht++)
    {
        for(int wt=0;wt<iWidth;wt++)
        {
            pix_new=(ht-(wt-iWidth/2)*slope);
            s=cvGet2D(gray,ht,wt);
            
            if(pix_new<=0||iHeight<=pix_new)
            {
                
                s.val[0]=255;
                cvSet2D(gray,ht,wt,s);
            }
            else
            {
                s_new=cvGet2D(gray,pix_new,wt);
                s_new.val[0]=s.val[0];
                cvSet2D(gray,pix_new,wt,s_new);

            }
            
        }

    }
    m_Cimage.mSetImg(gray);
	UpdateAllViews(NULL);

}
/********************************************************************************
   车牌的字符分割
   通过计算图像的前半部分的斜率和后半部分的斜率进行调整然后将图像映射到新的位置上
*********************************************************************************/

void CPlateIdentifyDoc::OnSplit()
{
    // TODO: 在此添加命令处理程序代码
    FILE *fp;
    int iHeight=gray->height;
    int iWidth=gray->width;
    IplImage * graytemp=NULL;
    CvRect result_rect;
    graytemp=cvCreateImage ( cvGetSize(gray), 8, 1 );
    cvCopy(gray,graytemp);
    result=cvCreateImage( cvSize(g_width*7,g_height), 8, 1 );
    //graytemp=cvCreateImage( cvGetSize(gray), 8, 1 );
    CvRect selection;
    CvScalar s;
    CRect rect;
    rect.bottom=iHeight;
    rect.top=0;
    rect.left=0;
    rect.right=iWidth;
    CvSize dst_cvsize; 
    dst_cvsize.width=g_width;
    dst_cvsize.height=g_height;
    //打开文件，将分割的字符存入文件中
    if((fp=fopen("test_set.txt","w"))==NULL)
   {
       AfxMessageBox("创建文件失败");
       exit(0);

   }

    //遇到第一个黑点确定顶部坐标位置
    for(int ht=0;ht<iHeight;ht++) 
    {
        for(int wt=0;wt<iWidth;wt++)
        {
            s=cvGet2D(gray,ht,wt);
            if(255==s.val[0])
            {
                //黑点
                rect.top=ht;
                ht=iHeight;
                break;
            }
        }
    }
    //遇到第一个黑点确定底部坐标位置
    for(int ht=iHeight-1;ht>=0;ht--)
    {
        for(int wt=0;wt<iWidth;wt++)
        {
            s=cvGet2D(gray,ht,wt);
            if(255==s.val[0])
            {
                rect.bottom=ht;
                ht=-1;
                break;
            }
        }
    }

    bool lab=FALSE;                                  //是否进入一个字符分割状态
    bool black=FALSE;                                //是否发现黑点
    bool change=FALSE;
     int num=0;
    for(int wt=0;wt<iWidth;wt++) 
    {
       
        int cout=0;
        for(int ht=0;ht<iHeight;ht++)
        {
            s=cvGet2D(gray,ht,wt);
            if((255==s.val[0])&&(!change))
            {
                cout++;
                change=TRUE;
            }
            else if((0==s.val[0])&&(change)) 
            {
                cout++;
                change=FALSE;
            }
        }
        
        if(!lab&&(cout>5))
            {
                
                rect.left=wt-3;                      //设置左侧边界
                lab=TRUE;                            //字符分割开始
             }
        if(wt==iWidth-1)
            break;
        if(lab&&(cout<5)&&(wt>(rect.left+8))&&num<7)
        {
           
            rect.right=wt+2;
            lab=FALSE; 
            CvPoint  pt1,pt2;                                            //设置每个字符的区域
            pt1.x=rect.left;
            pt1.y=rect.top;
            pt2.x=rect.right;
            pt2.y=rect.bottom; 
            selection.x=pt1.x+1;       
            selection.y=pt1.y;
            selection.width=rect.right-rect.left+1;
            selection.height=rect.bottom-rect.top;
            cvSetImageROI(graytemp, selection);
            //slipimage=slipimage+num;
            slipimage=cvCreateImage( cvGetSize(graytemp), 8, 1 );
            cvCopy(graytemp,slipimage);                                  //复制对象区域  
            cvResetImageROI(graytemp);
            IplImage *dst = cvCreateImage( dst_cvsize, 8, 1);           //存放分割后的字符
            cvResize(slipimage, dst, CV_INTER_LINEAR);                  //缩放源图像到目标图像归一化到6*7
            CvScalar s_tem;
            char ch;
            //将图像归一化后的信息写入文件中作为字符识别的测试集
            for(int ht=0;ht<g_height;ht++) 
           {
               
            for(int wt=0;wt<g_width;wt++)
               {
                    s_tem=cvGet2D(dst,ht,wt);
                    if(255==s_tem.val[0])
                    {
                        ch='0';
                    }
                    else 
                        ch=' ';
                    fputc(ch, fp);

               }
               fputc('\r', fp);
               fputc('\n', fp);
            }
            result_rect=cvRect(num*g_width,0,g_width,g_height);         
            cvSetImageROI(result,result_rect);
            cvCopy(dst,result);
            cvResetImageROI(result);
            cvReleaseImage(&dst);
            num++;
            cvRectangle(m_image,pt1,pt2,CV_RGB(255,0,0),1,8,0) ;        //将每个字符用框表示出来
            m_Cimage.mSetImg(m_image);
	        UpdateAllViews(NULL);
        
        }
    }
    cvReleaseImage(&graytemp);
    fclose(fp);
   
}


//以下函数为实现神经网络
 /****************************************************************************** 
        产生随机数 
******************************************************************************/ 

//设置伪随机数种子 
void InitializeRandoms()  
{ 
  srand(4711); 
} 
//产生一个LOW - TOP之间的伪随机整数 
int RandomEqualINT(int Low, int High)  
{ 
  return rand() % (High-Low+1) + Low; 
}      
//产生一个LOW - TOP之间的伪随机浮点数 
double RandomEqualREAL(double Low, double High) 
{ 
  return ((double) rand() / RAND_MAX) * (High-Low) + Low; 
} 
/****************************************************************************** 
  //关闭文件          
******************************************************************************/ 
void FinalizeApplication(NET* Net) 
{ 
  fclose(f); 
}      
/****************************************************************************** 
//随机生成联接权            
******************************************************************************/ 
void RandomWeights(NET* Net)  
{ 
  int l,i,j; 
  
  for (l=1; l<NUM_LAYERS; l++) {  //每层 
    for (i=1; i<=Net->Layer[l]->Units; i++) { 
      for (j=0; j<=Net->Layer[l-1]->Units; j++) { 
        Net->Layer[l]->Weight[i][j] = RandomEqualREAL(-0.5, 0.5);//随机值 
      } 
    } 
  } 
} 
/****************************************************************************** 
//保存连接权，防止丢失宝贵的联接权                                    
******************************************************************************/ 
void SaveWeights(NET* Net) 
{ 
  int l,i,j; 

  for (l=1; l<NUM_LAYERS; l++) { 
    for (i=1; i<=Net->Layer[l]->Units; i++) { 
      for (j=0; j<=Net->Layer[l-1]->Units; j++) { 
        Net->Layer[l]->WeightSave[i][j] = Net->Layer[l]->Weight[i][j]; 
      } 
    } 
  } 
} 
/****************************************************************************** 
//恢复连接权，以便需要的时候可以重新调用，重组网络              
******************************************************************************/ 
void RestoreWeights(NET* Net)                
{ 
  int l,i,j; 

  for (l=1; l<NUM_LAYERS; l++) { 
    for (i=1; i<=Net->Layer[l]->Units; i++) { 
      for (j=0; j<=Net->Layer[l-1]->Units; j++) { 
        Net->Layer[l]->Weight[i][j] = Net->Layer[l]->WeightSave[i][j];    
      } 
    } 
  } 
} 
/****************************************************************************** 
//创建网络，为网络分配空间                        
******************************************************************************/ 
void GenerateNetwork(NET* Net) 
{ 
  int l,i; 

  Net->Layer = (LAYER**) calloc(NUM_LAYERS, sizeof(LAYER*)); 
  
  for (l=0; l<NUM_LAYERS; l++) { 
    Net->Layer[l] = (LAYER*) malloc(sizeof(LAYER)); 
      
    Net->Layer[l]->Units      = Units[l]; 
    Net->Layer[l]->Output    = (double*)  calloc(Units[l]+1, sizeof(double)); 
    Net->Layer[l]->Activation = (double*)  calloc(Units[l]+1, sizeof(double)); 
    Net->Layer[l]->Error      = (double*)  calloc(Units[l]+1, sizeof(double)); 
    Net->Layer[l]->Weight    = (double**) calloc(Units[l]+1, sizeof(double*)); 
    Net->Layer[l]->WeightSave = (double**) calloc(Units[l]+1, sizeof(double*)); 
    Net->Layer[l]->dWeight    = (double**) calloc(Units[l]+1, sizeof(double*)); 
    Net->Layer[l]->Output[0]  = BIAS; 
      
    if (l != 0) { 
      for (i=1; i<=Units[l]; i++) { 
        Net->Layer[l]->Weight[i]    = (double*) calloc(Units[l-1]+1, sizeof(double)); 
        Net->Layer[l]->WeightSave[i] = (double*) calloc(Units[l-1]+1, sizeof(double)); 
        Net->Layer[l]->dWeight[i]    = (double*) calloc(Units[l-1]+1, sizeof(double)); 
      } 
    } 
  } 
  Net->InputLayer  = Net->Layer[0];//为输入层分配指针 
  Net->OutputLayer = Net->Layer[NUM_LAYERS-1];//为输出层分配指针 
  Net->Alpha      = 0.8;//冲量参数 
  Net->Eta        = 0.5;//学习率 
  Net->Epsilon    =0.005;//控制精度 
} 
/****************************************************************************** 
将输入样本转换成为输入模式，并创建一个文件以保存显示结果 
******************************************************************************/ 
void InitializeApplication(NET* Net)  
{ 
  int  n, i,j;      
  
  for (n=0; n<NUM_DATA; n++) { 
      for (i=0; i<Y; i++) { 
          for (j=0; j<X; j++) { 
              if ( Pattern[n][i][j] == '0' ) 
                    Input[n][i*X+j]  = HI ; 
                else Input[n][i*X+j]  =LO ;  
              //NUM_DATA输入模式，输入层X*Y个神经元 
          } 
      } 
  } 
  f = fopen("result.txt", "w"); 
} 
/****************************************************************************** 
                      训练网络 
//将每个样本投入网络运作，Input是转换后的输入模式，Target为导师信号，通过布尔型 
//的Training和Ptotocoling值控制是否训练和打印输入/输出模式 
******************************************************************************/ 
void SimulateNet(NET* Net, double* Input, double* Target, bool Training,bool Protocoling) 
{ 
  double Output[M]; //用来记录输出层输出 
  SetInput(Net, Input,Protocoling);//设置输入层，获得输入层的输出 
  PropagateNet(Net);//计算网络各层的输出          
  GetOutput(Net, Output,Protocoling);//获得输出层的输出 
  
  ComputeOutputError(Net, Target);//计算输出层误差 
  if (Training) { 
    BackpropagateNet(Net);//误差反向传播 
    AdjustWeights(Net);//调整联接权 
  } 
} 
/****************************************************************************** 
    获得输入层的输出          
******************************************************************************/ 
void SetInput(NET* Net, double* Input,bool Protocoling) 
{ 
  int i; 
  
  for (i=1; i<=Net->InputLayer->Units; i++) {        
    Net->InputLayer->Output[i] = Input[i-1];      //输入层输入 
  } 
  if (Protocoling) { 
    WriteInput(Net, Input);//根据Protocoling值输出输入模式 
  } 
} 
/****************************************************************************** 
//计算当前层的网络输出，upper 为当前层，LOWER为前一层                    
******************************************************************************/ 
void PropagateLayer(NET* Net, LAYER* Lower, LAYER* Upper)  
{ 
  int  i,j; 
  double Sum; 

  for (i=1; i<=Upper->Units; i++) { 
    Sum = 0; 
    for (j=0; j<=Lower->Units; j++) { 
      Sum += Upper->Weight[i][j] * Lower->Output[j];  //计算本层的净输入 
    }  
  Upper->Activation[i] = Sum;//保留激活值 
  //计算本层的输出,激活函数必须是S形函数，这样才可导，这是BP网络的理论前提 
  Upper->Output[i]=1/(1+exp(-Sum)); 
  } 
} 
/****************************************************************************** 
              //计算整个网络各层的输出 
******************************************************************************/ 
void PropagateNet(NET* Net) 
{ 
  int l; 
  
  for (l=0; l<NUM_LAYERS-1; l++) { 
    PropagateLayer(Net, Net->Layer[l], Net->Layer[l+1]); 
  } 
} 
/****************************************************************************** 
//获得输出层的输出              
******************************************************************************/ 
void GetOutput(NET* Net, double* Output,bool Protocoling) 
{ 
  int i; 
  
  for (i=1; i<=Net->OutputLayer->Units; i++) {  
    Output[i-1] = Net->OutputLayer->Output[i];//输出层输出 
  } 
  if (Protocoling) { 
    WriteOutput(Net, Output);//根据Protocoling值输出输出模式 
  } 
} 
/****************************************************************************** 
                  //计算输出层误差，* Target是导师信号 
******************************************************************************/ 
void ComputeOutputError(NET* Net, double* Target)  
{ 
  int  i; 
  double  Err,Out; 
  
  Net->Error = 0; 
  for (i=1; i<=Net->OutputLayer->Units; i++) { 
    Out = Net->OutputLayer->Output[i];//输出层的输出 
    Err = Target[i-1]-Out;//误差计算 
	
    Net->OutputLayer->Error[i] = Out * (1-Out) * Err; 
        //用delta规则计算误差，因为用了可导的s形函数 
    Net->Error += 0.5 * sqr(Err);//平方差公式 
  } 
} 
/****************************************************************************** 
  //误差反向传播 Upper 为前层，Lower为后层 ，层数值大的为前层            
******************************************************************************/ 
void BackpropagateLayer(NET* Net, LAYER* Upper, LAYER* Lower)  
{                                                              
  int  i,j;//循环变量 
  double Out, Err; 
  
  for (i=1; i<=Lower->Units; i++) { 
    Out = Lower->Output[i];//后层的输出 
    Err = 0;//用来记录隐含层输出的误差的估计值 
    for (j=1; j<=Upper->Units; j++) { 
      Err += Upper->Weight[j][i] * Upper->Error[j]; 
    //误差的反馈，通过已经处理的前层的delta值和联接权去估计，有理论基础 
    } 
    Lower->Error[i] =Out * (1-Out) * Err;  //delta规则 
  } 
} 
/****************************************************************************** 
              //整个网络误差的后传 
******************************************************************************/ 
void BackpropagateNet(NET* Net) 
{ 
  int l;//循环变量 
  
  for (l=NUM_LAYERS-1; l>1; l--) { 
    BackpropagateLayer(Net, Net->Layer[l], Net->Layer[l-1]);//对每层处理 
  } 
} 
/****************************************************************************** 
              //调整网络每一层的联接权 
******************************************************************************/ 
void AdjustWeights(NET* Net)    
{ 
  int  l,i,j;//循环变量 
  double Out, Err, dWeight; 
  //记录后层的输出、当前层的输出误差、当前神经元联接权上次的调整量 
  
  for (l=1; l<NUM_LAYERS; l++) { 
    for (i=1; i<=Net->Layer[l]->Units; i++) { 
      for (j=0; j<=Net->Layer[l-1]->Units; j++) { 
        Out = Net->Layer[l-1]->Output[j];//后层的输出 
        Err = Net->Layer[l]->Error[i];//当前层的输出误差 
        dWeight = Net->Layer[l]->dWeight[i][j]; 
        //将本神经元联接权上次的调整量取出，初始值为0，初始化网络时赋值的 
        Net->Layer[l]->Weight[i][j] += Net->Eta * Err * Out + Net->Alpha * dWeight; 
        //Alpha为冲量参数，加快网络的收敛速度 
        Net->Layer[l]->dWeight[i][j] = Net->Eta * Err * Out; 
        //记录本次神经元联接权的调整量 
      } 
    } 
  } 
} 
/****************************************************************************** 
//显示输入模式              
******************************************************************************/ 
void WriteInput(NET* Net, double* Input)  
{ 
    /*
  int i; 
  
  for (i=0; i<N; i++) { 
    if (i%X == 0) { 
      fprintf(f, "\\n"); 
    } 
    fprintf(f, "%c", (Input[i] == HI) ?'0' : ' '); 
  } 
  fprintf(f, " -> "); 
  */
} 
/****************************************************************************** 
//显示输出模式              
******************************************************************************/ 
void WriteOutput(NET* Net, double* Output) 
{ 
  int  i;//循环变量 
  int  Index;//用来记录最大输出值的下标，也就是最后识别的结果 
  double  MaxOutput;//用来记录最大的输出值 
    
  MaxOutput=0;//初始化 
  for (i=0; i<M; i++) 
  {    
      if(MaxOutput<Output[i]){ 
          MaxOutput=MAX(MaxOutput,Output[i]);//保存最大值 
          Index=i; 
      } 
  } 
  if(Index==0)    
     fputs("辽",f);//写进文件 
  if(Index==1)    
     fputs("A",f);//写进文件  
  if(Index==2)    
     fputs("U",f);//写进文件   
  if(Index==3)    
     fputs("E",f);//写进文件
   if(Index==4)    
     fputs("8",f);//写进文件 
   if(Index==5)    
     fputs("8",f);//写进文件 
   if(Index==6)    
     fputs("6",f);//写进文件 
} 
/****************************************************************************** 
              初始化测试样本 
******************************************************************************/ 
void Initializetest() 
{ 
    int n,i,j;//循环变量 
    
    for (n=0; n<NUM_DATA; n++) { 
        for (i=0; i<Y; i++) { 
            for (j=0; j<X; j++) {                               
                if (testPattern[n][i][j]=='0')
                    Inputtest[n][i*X+j] = HI; 
                else Inputtest[n][i*X+j] =LO;  //NUM_DATA输入模式，输入层X*Y个神经元                                    
            } 
        }
     }
}
/****************************************************************************** 
             从文件读出目标向量 
******************************************************************************/ 
void TegetData(char taget[NUM_DATA][M],double tagetdouble[NUM_DATA][M])
{
    FILE *fp;
    char ch ;
     if((fp=fopen("teget_num.txt","r"))==NULL)
   {
       AfxMessageBox("打开目标文件失败");
        exit(0);

   }
     for(int i=0;i<NUM_DATA;i++)
     {
         for(int j=0;j<M;j++)
         {
             ch=fgetc(fp);
             while(ch=='\n' || ch=='\r'||ch==' ')
             {
                  ch=fgetc(fp);
              }
             taget[i][j]=ch;
         }
     }
     for(int i=0;i<NUM_DATA;i++)
     {
         for(int j=0;j<M;j++)
         {
             if(taget[i][j]=='H')
             {
                 tagetdouble[i][j]=HI;
             }
             else
                 tagetdouble[i][j]=LO;
         }
     }
    fclose(fp);

}
/****************************************************************************** 
              从文件中读出训练样本集 
******************************************************************************/ 
void GetSwatch(char swatch[NUM_DATA][Y][X])
{
    FILE *fp;
    char ch ;
     if((fp=fopen("train_num.txt","r"))==NULL)
   {
        AfxMessageBox("打开训练集文件失败");
         exit(0);

   }
     for(int i=0;i<NUM_DATA;i++)
     {
         for(int j=0;j<Y;j++)
         {                
             for(int k=0;k<X;k++)
             {
                ch=fgetc(fp);
                while(ch=='\n' || ch=='\r')
                    {
                        ch=fgetc(fp);
                    }
                 swatch[i][j][k]=ch;
              }
           }
     }
     fclose(fp);

}
/****************************************************************************** 
              从文件中读出测试样本集 
******************************************************************************/ 
void GetTest(char swatch11[NUM_DATA][Y][X])
{
    FILE *fp;
    char ch ;
     if((fp=fopen("test_set.txt","r"))==NULL)
   {
        AfxMessageBox("打开测试集文件失败");
       exit(0);

   }
     for(int i=0;i<NUM_DATA;i++)
     {
         for(int j=0;j<Y;j++)
         {                
             for(int k=0;k<X;k++)
             {
                ch=fgetc(fp);
                while(ch=='\n' || ch=='\r')
                    {
                        ch=fgetc(fp);
                    }
                 swatch11[i][j][k]=ch;
              }
           }
     }
      fclose(fp);
}
/****************************************************************************** 
              训练BP神经网络 
******************************************************************************/ 
void CPlateIdentifyDoc::OnNettrain()
{
    NET Net;
    char   Target1[NUM_DATA][M];
    int  m,n,count;                                 //循环变量 
    bool Stop;                                      //学习是否结束的控制变量 
  double Error;                                     //记录当前所有样本的最大误差
  GetSwatch(Pattern);                               //从文件中读取训练样本
  GetTest(testPattern);                             //从文本中读测试集
  TegetData(Target1,Target);                        //从文件中读取目标
  InitializeRandoms();                              //生成随机数 
  GenerateNetwork(&Net);                            //创建网络并初始化网络，分配空间 
  RandomWeights(&Net);                              //初始化网络联接权 
  InitializeApplication(&Net);                      //初始化输入层，将学习样本转换成输入模式 
  count=0;                                          //显示学习进度的控制变量 
  do { 
    Error = 0;                                      //误差 
    Stop = true;                                    //初始化 
    for (n=0; n<NUM_DATA; n++) {                    //对每个模式计算模拟神经网络误差 
      SimulateNet(&Net, Input[n],Target[n], false, false);  //计算模拟神经网络误差
      Error = MAX(Error, Net.Error);                //获取结构的值,获取误差最大值 
      Stop = Stop && (Net.Error < Net.Epsilon); 
      count++; 
    } 
    Error = MAX(Error, Net.Epsilon);                //作用:防止溢出,保证到100%的时候停止训练，获取误差最大值 
    if (count%1000==0){ 
        break;
    }
    if (! Stop) { 
        for (m=0; m<10*NUM_DATA; m++) {             //对各模式进行训练 
        n = RandomEqualINT(0,NUM_DATA-1);           //随机选择训练模式    
        SimulateNet(&Net, Input[n],Target[n], true,false ); 
      } 
    }    
  } while (! Stop);  
  AfxMessageBox("BP神经网络训练结束\n"); 
  SaveWeights(&Net);                                //学习结束后保存宝贵的联接权  
  Initializetest();                                 //初始化测试样本，将其转化成输入模式
  for (int n=0; n<NUM_DATA; n++) { 
    SimulateNet(&Net, Inputtest[n],Target[n], false, true); //将结果写入文件   
  } 
  FinalizeApplication(&Net);                        //关闭文件   
}
/****************************************************************************** 
             归一化响应函数处理过程在分割中只做显示作用 
******************************************************************************/ 

void CPlateIdentifyDoc::OnGuiyi()
{
    // TODO: 在此添加命令处理程序代
       m_Cimage.mSetImg(result);
	   UpdateAllViews(NULL);
}
/****************************************************************************** 
             字符分割已在训练测试网络时写入保存结果的文件，从文件中读取结果 
******************************************************************************/ 

void CPlateIdentifyDoc::OnChartIdentify()
{
    // TODO: 在此添加命令处理程序代码
  
  AfxMessageBox("结果已存入当前目录的result.txt中从文件读出结果");
  FILE *fp;
    char ch ;
     if((fp=fopen("result.txt","r"))==NULL)
   {
        AfxMessageBox("打开训练集文件失败");
         exit(0);

   }
    char buff[9];
    fgets(buff,sizeof(buff),fp); 
    AfxMessageBox(buff);
}

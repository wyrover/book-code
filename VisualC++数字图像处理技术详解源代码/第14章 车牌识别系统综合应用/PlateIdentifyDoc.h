// PlateIdentifyDoc.h : CPlateIdentifyDoc 类的接口
//


#pragma once
#include "cv.h"
#include "highgui.h"
#include "myimg.h"


#define MIN(x,y)      ((x)<(y) ? (x) : (y)) 
#define MAX(x,y)      ((x)>(y) ? (x) : (y))
#define sqr(x)        ((x)*(x)) 

const double LO=0.1; 
const double HI=0.9; 
const double BIAS=0.5; 

const int NUM_LAYERS=3;                                               //网络层数 
const int NUM_DATA=7;                                                 //样本数 
const int X=6;                                                        //每个样本的列数 
const int Y=7;                                                        //每个样本的行数 
const int N=X * Y;                                                    //输入层神经元个数 
const int M=7;                                                        //输出层神经元个数 
typedef struct {                    
        int            Units;                                         //层神经元数量 
        double*        Output;                                        //输出数 （即输出个矢量元素个数） 
        double*        Activation;                                    //激活值 
        double*        Error;                                         //本层误差            
        double**       Weight;                                        //连接权 
        double**       WeightSave;                                    //保存训练调整后的连接权 
        double**       dWeight;                                       //调整量 
} LAYER;                                                              //神经网络层结构 

typedef struct {                    
        LAYER**       Layer;                                          //神经网络各层指针 
        LAYER*        InputLayer;                                     //输入层 
        LAYER*        OutputLayer;                                    //输出层 
        double        Alpha;                                          //冲量参数 
        double        Eta;                                            //学习率        
        double        Error;                                          //总误差 
        double        Epsilon;                                        //控制精度 
} NET;                                                                //神经网络



/****************************************************************************** 
                  各函数声明 
******************************************************************************/ 

void InitializeRandoms(); //设置伪随机数种子 
int  RandomEqualINT(int Low, int High);//产生一个LOW - TOP之间的伪随机整数 
double RandomEqualREAL(double Low, double High);//产生一个LOW - TOP之间的伪随机浮点数 
void FinalizeApplication(NET* Net);//关闭文件 
void RandomWeights(NET* Net) ;//随机生成网络各层联接权 
void SaveWeights(NET* Net);//保存网络各层联接权，以防丢失宝贵的联接权 
void RestoreWeights(NET* Net);//恢复网络各层联接权，以便重建网络 
void GenerateNetwork(NET* Net);//创建网络，为网络分配空间 
void InitializeApplication(NET* Net);//将学习样本转换成为输入模式，并创建一个文件以保存显示结果 
void SimulateNet(NET* Net, double* Input, double* Target, bool Training,bool Protocoling);//将每个数字样本投入网络运作
void SetInput(NET* Net, double* Input,bool Protocoling);// 获得输入层的输出      
void PropagateLayer(NET* Net, LAYER* Lower, LAYER* Upper);//计算当前层的网络输出，upper 为当前层，LOWER为前一层 
void PropagateNet(NET* Net);//计算整个网络各层的输出 
void GetOutput(NET* Net, double* Output,bool Protocoling);//获得输出层的输出 
void ComputeOutputError(NET* Net, double* Target);//计算网络输出层的输出误差 
void BackpropagateLayer(NET* Net, LAYER* Upper, LAYER* Lower);//当前层误差反向传播 
void BackpropagateNet(NET* Net);////整个网络误差的后传 
void AdjustWeights(NET* Net); //调整网络各层联接权，提取样本特征 
void WriteInput(NET* Net, double* Input);//显示输入模式    
void WriteOutput(NET* Net, double* Output);//显示输出模式  
void Initializetest();//将测试样本转换成为输入模式 
void TegetData(char taget[NUM_DATA][M],double tagetdouble[NUM_DATA][M]);//从文件中读出目标向量
void GetSwatch(char swatch[NUM_DATA][Y][X]);   //从文件中读出训练集
void GetTest(char swatch11[NUM_DATA][Y][X]);   //从文件读出测试集
int ImageStretchByHistogram(IplImage *src,IplImage *dst); 

const int g_width=6;                      //设置归一化的宽度 
const int g_height=7;                     //设置归一化的高度 

class CPlateIdentifyDoc : public CDocument
{
protected: // 仅从序列化创建
	CPlateIdentifyDoc();
	DECLARE_DYNCREATE(CPlateIdentifyDoc)


// 属性
public:
    //用于Open cv的变量的定义
    IplImage * m_image,*gray;             //保存原图像、灰度图像
    IplImage *slipimage;                  //保存分割的笑图像   
    IplImage * result;                    //保存分割结果的图像
    myimg m_Cimage;                       //主要用于显示图像的类继承
    NET Net;                              //神经网络
// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CPlateIdentifyDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
public:
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
public:
    afx_msg void OnGray();
public:
    afx_msg void OnGraystrenth();
public:
    afx_msg void OnEdgeadd();
public:
    afx_msg void OnBinary();
public:
    afx_msg void OnPosition();
public:
    afx_msg void OnState();
public:
    afx_msg void OnRevise();
public:
    afx_msg void OnSplit();
public:
    afx_msg void OnNettrain();

    int CPlateIdentifyDoc::map(char *character);
public:
    afx_msg void OnGuiyi();
public:
    
public:
    
public:
    afx_msg void OnTest();
public:
    afx_msg void OnChartIdentify();
};



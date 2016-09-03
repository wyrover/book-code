#pragma once

#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 
#include <conio.h> 


#define MAX(x,y)      ((x)>(y) ? (x) : (y)) 
#define sqr(x)        ((x)*(x)) 

const double LO=0.1; 
const double HI=0.9; 
const double BIAS=0.5; 

const int NUM_LAYERS=3;      //网络层数 
const int NUM_DATA=10;      //样本数 
const int X=6;      //每个样本的列数 
const int Y=7;      //每个样本的行数 

const int N=X * Y; //输入层神经元个数 
const int M=10;      //输出层神经元个数 

const int Units[NUM_LAYERS] ={N, 10, M}; //用一维数组记录各层神经元个数







typedef struct {                    
            int          Units;        //层神经元数量 
            double*        Output;        //输出数 （即输出个矢量元素个数） 
            double*        Activation;    //激活值 
            double*        Error;        //本层误差            
            double**        Weight;        //连接权 
            double**        WeightSave;    //保存训练调整后的连接权 
            double**        dWeight;      //调整量 
    } LAYER;    //神经网络层结构 

    typedef struct {                    
            LAYER**      Layer;        //神经网络各层指针 
            LAYER*        InputLayer;    //输入层 
            LAYER*        OutputLayer;  //输出层 
            double          Alpha;        //冲量参数 
            double          Eta;          //学习率        
            double          Error;        //总误差 
            double          Epsilon;      //控制精度 
    } NET; //  神经网络


class BpNet
{
    public:
        
     
    static double Input_bp[NUM_DATA][N];
    FILE* f;//声明文件指针 

    
    
    BpNet(void);

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
void SimulateNet(NET* Net, double* Input, double* Target, int Training,int Protocoling);//将每个样本投入网络运作 
void SetInput(NET* Net, double* Input,int Protocoling);// 获得输入层的输出      
void PropagateLayer(NET* Net, LAYER* Lower, LAYER* Upper);//计算当前层的网络输出，upper 为当前层，LOWER为前一层 
void PropagateNet(NET* Net);//计算整个网络各层的输出 
void GetOutput(NET* Net, double* Output,int Protocoling);//获得输出层的输出 
void ComputeOutputError(NET* Net, double* Target);//计算网络输出层的输出误差 
void BackpropagateLayer(NET* Net, LAYER* Upper, LAYER* Lower);//当前层误差反向传播 
void BackpropagateNet(NET* Net);////整个网络误差的后传 
void AdjustWeights(NET* Net); //调整网络各层联接权，提取样本特征 
void WriteInput(NET* Net, double* Input);//显示输入模式    
void WriteOutput(NET* Net, double* Output);//显示输出模式  
void Initializetest();//将测试样本转换成为输入模式

public:
    ~BpNet(void);
};

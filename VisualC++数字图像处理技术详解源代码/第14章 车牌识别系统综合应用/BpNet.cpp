#include "StdAfx.h"
#include "BpNet.h"
 

/****************************************************************************** 
        产生随机数 
******************************************************************************/ 

//设置伪随机数种子 
void BpNet:: InitializeRandoms()  
{ 
  srand(4711); 
} 
//产生一个LOW - TOP之间的伪随机整数 
int BpNet::RandomEqualINT(int Low, int High)  
{ 
  return rand() % (High-Low+1) + Low; 
}      
//产生一个LOW - TOP之间的伪随机浮点数 
double BpNet::RandomEqualREAL(double Low, double High) 
{ 
  return ((double) rand() / RAND_MAX) * (High-Low) + Low; 
} 
/****************************************************************************** 
  //关闭文件          
******************************************************************************/ 
void BpNet::FinalizeApplication(NET* Net) 
{ 
  fclose(f); 
}      
/****************************************************************************** 
//随机生成联接权            
******************************************************************************/ 
void BpNet::RandomWeights(NET* Net)  
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
void BpNet::SaveWeights(NET* Net) 
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
void BpNet::RestoreWeights(NET* Net)                
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
void BpNet::GenerateNetwork(NET* Net) 
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
void BpNet::InitializeApplication(NET* Net)  
{ 
  int  n, i,j;      
  
  for (n=0; n<NUM_DATA; n++) { 
      for (i=0; i<Y; i++) { 
          for (j=0; j<X; j++) { 
              char c=Pattern[n][i][j];
              if ( Pattern[n][i][j] == 'O') 
                    Input_bp[n][i*X+j]  = HI ; 
                else Input_bp[n][i*X+j]  =LO ;  
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
void BpNet::SimulateNet(NET* Net, double* Input_bp, double* Target, BOOL Training,BOOL Protocoling) 
{ 
  double Output[M]; //用来记录输出层输出 
  SetInput(Net, Input_bp,Protocoling);//设置输入层，获得输入层的输出 
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
void BpNet::SetInput(NET* Net, double* Input_bp,BOOL Protocoling) 
{ 
  int i; 
  
  for (i=1; i<=Net->InputLayer->Units; i++) {        
    Net->InputLayer->Output[i] = Input_bp[i-1];      //输入层输入 

  } 
  if (Protocoling) { 
    WriteInput(Net, Input_bp);//根据Protocoling值输出输入模式 
  } 
} 
/****************************************************************************** 
//计算当前层的网络输出，upper 为当前层，LOWER为前一层                    
******************************************************************************/ 
void BpNet::PropagateLayer(NET* Net, LAYER* Lower, LAYER* Upper)  
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
void BpNet::PropagateNet(NET* Net) 
{ 
  int l; 
  
  for (l=0; l<NUM_LAYERS-1; l++) { 
    PropagateLayer(Net, Net->Layer[l], Net->Layer[l+1]); 
  } 
} 
/****************************************************************************** 
//获得输出层的输出              
******************************************************************************/ 
void BpNet::GetOutput(NET* Net, double* Output,BOOL Protocoling) 
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
void BpNet::ComputeOutputError(NET* Net, double* Target)  
{ 
  int  i; 
  double  Err,Out;
  double temp;
  
  Net->Error = 0; 
  for (i=1; i<=Net->OutputLayer->Units; i++) { 
    Out = Net->OutputLayer->Output[i];//输出层的输出 
    temp=Target[i-1];
    Err = temp-Out;//误差计算 
    Net->OutputLayer->Error[i] = Out * (1-Out) * Err; 
        //用delta规则计算误差，因为用了可导的s形函数 
    Net->Error += 0.5 * sqr(Err);//平方差公式 
  } 
} 
/****************************************************************************** 
  //误差反向传播 Upper 为前层，Lower为后层 ，层数值大的为前层            
******************************************************************************/ 
void BpNet::BackpropagateLayer(NET* Net, LAYER* Upper, LAYER* Lower)  
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
void BpNet::BackpropagateNet(NET* Net) 
{ 
  int l;//循环变量 
  
  for (l=NUM_LAYERS-1; l>1; l--) { 
    BackpropagateLayer(Net, Net->Layer[l], Net->Layer[l-1]);//对每层处理 
  } 
} 
/****************************************************************************** 
              //调整网络每一层的联接权 
******************************************************************************/ 
void BpNet::AdjustWeights(NET* Net)    
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
void BpNet::WriteInput(NET* Net, double* Input_bp)  
{ 
  int i; 
  
  for (i=0; i<N; i++) { 
    if (i%X == 0) { 
      fprintf(f, "\\n"); 
    } 
    fprintf(f, "%c", (Input_bp[i] == HI) ?'0' : ' '); 
  } 
  fprintf(f, " -> ");  
} 
/****************************************************************************** 
//显示输出模式              
******************************************************************************/ 
void BpNet::WriteOutput(NET* Net, double* Output) 
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
      
  fprintf(f, "%i\\n", Index);//写进文件 
  
} 
/****************************************************************************** 
              初始化测试样本 
******************************************************************************/ 
void BpNet::Initializetest() 
{ 
    int n,i,j;//循环变量 
    
    for (n=0; n<NUM_DATA; n++) { 
        for (i=0; i<Y; i++) { 
            for (j=0; j<X; j++)                                
                if (testPattern[n][i][j]=='O') 
                    Inputtest[n][i*X+j] = HI; 
                else Inputtest[n][i*X+j] =LO;  //NUM_DATA输入模式，输入层X*Y个神经元                                    
        } 
    } 
}

BpNet::~BpNet(void)
{
}

#include <iostream.h>
#include "matlib.h"
#include "mymagic.h"
#include "myeig.h"

void main()
{
// 循环变量
	int i;
	
	// 矩阵维数
	int m,n;
	
	// 输入幻方矩阵的维数
	int nDim;
	cout<<"请输入欲生成幻方矩阵的维数："<<endl;
	cin>>nDim;
	// 生成幻方矩阵
    Mm X = mymagic(nDim);	
	// 获取矩阵维数
	m = X.size(1);
	n = X.size(2);
	// 为数据分配空间
	double *pXData = (double*)malloc(sizeof(double)*m*n);
	memset(pXData,0,sizeof(double)*m*n);
	// 复制矩阵数据
	memcpy(pXData,X.addr(),sizeof(double)*m*n);	
	// 输出幻方矩阵
	cout<<"幻方矩阵为："<<endl;
	for(i = 0; i < m*n; i++)
	{
		cout.width(4);
		cout<<*(pXData+i)<<" ";

		if(i!=0 && (i+1)%nDim==0)
		{
			cout<<endl;
		}
	}

	// 求矩阵特征值
	Mm E = myeig(X);
	// 获取特征值的个数	
	nDim = E.size(1);
	// 输出矩阵的特征值
	cout<<"矩阵的特征值如下："<<endl;
	for(i = 1; i <= nDim; i++)
	{
		cout<<E.r(i,1)<<endl;
	}
	
	// 暂停程序，观察结果
	cout<<"输入任意字符，按回车退出程序。"<<endl;
	getchar();
}

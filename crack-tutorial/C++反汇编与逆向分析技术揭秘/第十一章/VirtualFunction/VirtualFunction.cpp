// VirtualFunction.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

class CVirtual
{
public:
	~CVirtual()
	{
		printf("~CVirtual");
	}
	CVirtual()
	{
	}
	CVirtual(int nNumber)
	{
		m_nNumber = nNumber;
	}
	virtual int GetNumber()
	{
		return m_nNumber;
	}
	virtual void SetNumber(int nNumber)
	{
		m_nNumber = nNumber;
	}
private:
	int m_nNumber;
};


int main(int argc, char* argv[])
{
	// 获取含有虚函数表的类大小
//	int nSize = sizeof(CVirtual);
	CVirtual MyVirtual;
	CVirtual MyVirtual1(1);
// 	MyVirtual.SetNumber(argc);
// 	printf("%d\r\n", MyVirtual.GetNumber());
	return 0;
}


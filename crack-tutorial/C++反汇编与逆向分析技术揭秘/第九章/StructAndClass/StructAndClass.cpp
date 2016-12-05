// StructAndClass.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>
#include <iostream.h>

class CNumber
{
public:
	CNumber()
	{
		m_nOne = 1;
		m_nTwo = 2;
	}
	int GetNumberOne()
	{
		return m_nOne;
	}
	int GetNumberTwo()
	{
		return m_nTwo;
	}
private:
	int m_nOne;
	int m_nTwo;

};

class  tagTEST
{
public:
	short sShort;		// 占2字节
	int nInt;			// 占4字节
};

// this指针识别
// class CTest
// {
// public:
// 	 void __stdcall SetNumber(int nNumber)
// 	{
// 		m_nInt = nNumber;
// 	}
// public:
// 	int m_nInt;
//};
// 
// class CTest
// {
// public:
// 	void Show(){
// 		// 由于传递this指针，因此这里实际为this->GetNumber()
// 		printf("%d\r\n", GetNumber()); 
// 	}
// 	int GetNumber(){
// 		return m_nInt;		// 等同于retrun this->m_nInt;
// 	}
// private:
// 	int m_nInt;
//};



// 含有静态数据成员的类
class CStatic 
{
public:
	void ShowNumber()
	{
		printf("m_nInt = %d , m_snInt = %d", m_nInt, m_snInt);
	}

	static int m_snInt;
	int m_nInt;
};
int CStatic::m_snInt = 5;

// 类作为参数
class CFunTest
{
public:
	void ShowNumber()
	{
		printf("%d %d %s\r\n", m_nOne, m_nTwo, m_szName);
	}
	int m_nOne;
	int m_nTwo;
	char m_szName[32];
};

void ShowFunTest(CFunTest FunTest)
{
	//FunTest.ShowNumber();
	printf("%d %d %s\r\n", 
		FunTest .m_nOne, FunTest.m_nTwo, FunTest .m_szName);
}

// 带有资源申请与释放的类
class CMyString
{
public:
	CMyString(){
		m_pString = new char[10];
		if (m_pString == NULL){
			return;
		}
		strcpy(m_pString, "Hello");
	}
	~CMyString(){
		if (m_pString != NULL){
			delete m_pString;
			m_pString = NULL;
		}
	}
	char * GetString(){
		return m_pString;
	}
	void SetString(char * pString)
	{
		int nLen = strlen(pString);
		if (m_pString != NULL)
		{
			delete [] m_pString;
			m_pString = NULL;
		}
		m_pString = new char[nLen + sizeof(char)];
		strcpy(m_pString, pString);
	}
private:
	char * m_pString;
};

void ShowMyString(CMyString MyStringCpy)
{
	printf(MyStringCpy.GetString());
}


class CReturn
{
public:
	int m_nNumber;
	int m_nArry[10];
};

// 对象作为返回值

CReturn GetCReturn()
{
	CReturn Return ;
	Return.m_nNumber = 0;
	for (int i = 0; i < 10; i++)
	{
		Return.m_nArry[i] = i+1;
	}
	return Return;
}

// 类、结构体识别总结
void ShowTest(tagTEST * pTest)
{
	printf("%d  %d\r\n", pTest->nInt, pTest->sShort);
}



void main(int argc, char* argv[])
{
	// 内存对其演示
// 	int nOne = 0x12345678;
// 	char cChar = 0;
// 	int nTwo = 0x87654321;

	// 了解this指针
//  CTest Test;
// 	Test.SetNumber(5);
//	printf("CTest : %d\r\n", Test.m_nInt);

	// 静态成员变量讲解
	// 普通数据成员与静态数据成员区别

// 	CStatic staticOne;
// 	printf("%08x\r\n", &staticOne.m_nInt);
// 	printf("%08x\r\n", &staticOne.m_snInt);
// 	CStatic staticTwo;
// 	printf("%08x\r\n", &staticTwo.m_nInt);
//	printf("%08x\r\n", &staticTwo.m_snInt);

//	CStatic Static;
// 	int nSize = sizeof(Static);
// 	printf("CStatic : %d\r\n", nSize);
// 
// 	printf("0x%08x\r\n", &Static.m_snInt);
// 	printf("0x%08x\r\n", &Static.m_nInt);

// 	Static.m_nInt = 1;
// 	Static.m_snInt = 2;
//	Static.ShowNumber();

	// 对象作为函数参数
// 	CFunTest FunTest;
// 	FunTest.m_nOne = 1;
// 	FunTest.m_nTwo = 2;
// 	strcpy(FunTest.m_szName, "Name");
//	ShowFunTest(FunTest);


	// 对象中存在资源释放时，做为函数参数
// 	CMyString MyString;
// 	ShowMyString(MyString);
//	ShowMyString(MyString);


	// 返回值为对象
// 	CReturn Return;
// 	Return = GetCReturn();
//	printf("%d %d %d", Return.m_nNumber, Return.m_nArry[0], Return.m_nArry[9]);

	char a = '\xaa';
	
	// 类、结构识别总结

 	tagTEST test;
	test.sShort = 3;
	test.nInt = 2;
	int nSize = sizeof(test);

 	ShowTest(&test);
}



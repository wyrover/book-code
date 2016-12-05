// object.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>


class CNumber{

public:
	CNumber()
	{
		m_nNumber = 1;
	}
	~CNumber()
	{
		printf("~CNumber\r\n");
	}

	int m_nNumber;
};

class CCpyClass{
public:
	int m_nInt;
	short m_sShort;
	char m_cChar;
};

class CMyString{
public:
	CMyString(){
		m_pString = NULL;
	}
	CMyString(CMyString& obj){
		// 注：如在拷贝构造中这样拷贝指针，仍然属于浅拷贝，并没有拷贝指针所指向的
		// 堆空间中的数据制作副本，仍然会造成错误。
		// this->m_pString = obj.m_pString;
		int nLen = strlen(obj.m_pString);
		this->m_pString = new char[nLen + sizeof(char)];
		strcpy(this->m_pString, obj.m_pString);
	}
	~CMyString(){
		if (m_pString != NULL)
		{
			delete [] m_pString;
			m_pString = NULL;
		}
	}
	void SetString(char * pString){
		int nLen = strlen(pString);
		if (m_pString != NULL)
		{
			delete [] m_pString;
			m_pString = NULL;
		}
		m_pString = new char[nLen + sizeof(char)];
		strcpy(m_pString, pString);
		
	}
	char * m_pString;
};

void Show(CMyString MyString){
	printf(MyString.m_pString);
}

// 返回对象
CMyString GetMyString()
{
	CMyString MyString;
	MyString.SetString("World");
	return MyString;
}

// 参数为指针类型的函数
CMyString * GetMyString(CMyString * pMyString)
{
	CMyString MyString;
	MyString.SetString("World");
	pMyString = &MyString;
	return &MyString;
}

// 全局对象定义
CMyString g_MyStringOne;
CMyString g_MyStringTwo;


// 无默认构造函数的类
class CInit
{
public:
	void SetNumber(int nNumber)
	{
		m_nNumber = nNumber;
	}
	int GetNumber()
	{
		return m_nNumber;
	}
private:
	int m_nNumber;
};


int main(int argc, char* argv[])
{
	// 局部对象定义调用构造函数
//	CNumber Number;
	// 申请堆对象
// 	CNumber * pNumber = NULL;
// 	pNumber = new CNumber[4];
// 	pNumber->m_nNumber = 2;
// 	printf("%d \r\n", pNumber->m_nNumber);
// 
// 	if (pNumber != NULL)
// 	{
// 		delete pNumber;
// 		pNumber = NULL;
// 	}


	// 释放多个堆对象
	CNumber * pArray = new CNumber[2];
	if (pArray != NULL)
	{
		delete [] pArray;
		pArray = NULL;
	}


	

	// 浅拷贝说明
// 	CNumber one;
// 	CNumber two(one);

	// 拷贝构造函数的使用
// 	CMyString MyString;
// 	MyString.SetString("Hello");
// 	Show(MyString);

	// 返回值为对象类型的拷贝构造使用
//	CMyString MyString = GetMyString();
	CMyString * pMyString = NULL;
	pMyString = GetMyString(pMyString);


	// 无默认构造函数的类
// 	CInit Init;
// 	Init.SetNumber(5);
// 	printf("%d", Init.GetNumber());
	
}


// ShowNumber.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string.h>

class CMyString
{
public:
	CMyString();
	CMyString(CMyString& obj);
	CMyString(char *pstrBuff);
	~CMyString();

	void SetString(char *pstrBuff);
	char* GetString();
	
	int GetStringLen();

private:
	char *m_pstrBuff;
};

CMyString::CMyString()
{
	m_pstrBuff = NULL;
}
CMyString::CMyString(CMyString& obj)
{
	m_pstrBuff = NULL;
	this->SetString(obj.GetString());
}
CMyString::CMyString(char *pstrBuff)
{
	m_pstrBuff = NULL;
	SetString(pstrBuff);
}
CMyString::~CMyString()
{
	if (m_pstrBuff != NULL)
	{
		delete [] m_pstrBuff;
		m_pstrBuff = NULL;
	}
}

void CMyString::SetString(char *pstrBuff)
{
	if (m_pstrBuff != NULL)
	{
		delete [] m_pstrBuff;
		m_pstrBuff = NULL;
	}

	int nLen = strlen(pstrBuff);
	m_pstrBuff = new char[nLen + sizeof(char)];

	do 
	{
		m_pstrBuff[nLen] = pstrBuff[nLen];
		nLen--;
	} while (nLen >= 0);

}
char* CMyString::GetString()
{
	return m_pstrBuff;
}
int CMyString::GetStringLen()
{
	int nLen = 0;
	do 
	{
		nLen++;
	} while (m_pstrBuff[nLen]);
	return nLen;
}


CMyString ShowString(CMyString MyString)
{
	CMyString MyStringRet;

	char szBuff[50] = {0};
	scanf("%50s", szBuff);

	printf("%s\r\n", MyString.GetString());

	MyStringRet.SetString(szBuff);

	return MyStringRet;
}

int main(int argc, char* argv[])
{
	char szBuff[50] = {0};
	scanf("%50s", szBuff);
	CMyString * pMyString = NULL;

	CMyString MyString(szBuff);
	
	CMyString MyStringRet = ShowString(MyString);

	printf("%s\r\n", MyStringRet.GetString());


	// 堆对象的使用
	pMyString = new CMyString[2];

	if (pMyString == NULL)
	{
		return -1;
	}

	pMyString[0].SetString(MyStringRet.GetString());
	pMyString[1].SetString(MyString.GetString());
	printf("%s %s\r\n", (*pMyString).GetString(), (*(pMyString+1)).GetString());

	if (pMyString != NULL)
	{
		delete [] pMyString;		// BUG所在处
		pMyString = NULL;
	}
	
	return 0;
}


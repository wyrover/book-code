// FirstNotRepeatingChar.cpp : Defines the entry point for the console application.
//

// 《剑指Offer——名企面试官精讲典型编程题》代码
// 著作权所有者：何海涛

#include "stdafx.h"
#include <string>

char FirstNotRepeatingChar(char* pString)
{
    if (pString == NULL)
        return '\0';

    const int tableSize = 256;
    unsigned int hashTable[tableSize];

    for (unsigned int i = 0; i < tableSize; ++ i)
        hashTable[i] = 0;

    char* pHashKey = pString;

    while (*(pHashKey) != '\0')
        hashTable[*(pHashKey++)] ++;

    pHashKey = pString;

    while (*pHashKey != '\0') {
        if (hashTable[*pHashKey] == 1)
            return *pHashKey;

        pHashKey++;
    }

    return '\0';
}

// ====================测试代码====================
void Test(char* pString, char expected)
{
    if (FirstNotRepeatingChar(pString) == expected)
        printf("Test passed.\n");
    else
        printf("Test failed.\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
    // 常规输入测试，存在只出现一次的字符
    Test("google", 'l');
    // 常规输入测试，不存在只出现一次的字符
    Test("aabccdbd", '\0');
    // 常规输入测试，所有字符都只出现一次
    Test("abcdefg", 'a');
    // 鲁棒性测试，输入NULL
    Test(NULL, '\0');
    return 0;
}


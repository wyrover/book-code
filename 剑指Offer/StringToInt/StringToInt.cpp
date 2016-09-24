// StringToInt.cpp : Defines the entry point for the console application.
//

// 《剑指Offer——名企面试官精讲典型编程题》代码
// 著作权所有者：何海涛

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>

long long StrToIntCore(const char* str, bool minus);

enum Status {kValid = 0, kInvalid};
int g_nStatus = kValid;

int StrToInt(const char* str)
{
    g_nStatus = kInvalid;
    long long num = 0;

    if (str != NULL && *str != '\0') {
        bool minus = false;

        if (*str == '+')
            str ++;
        else if (*str == '-') {
            str ++;
            minus = true;
        }

        if (*str != '\0') {
            num = StrToIntCore(str, minus);
        }
    }

    return (int)num;
}

long long StrToIntCore(const char* digit, bool minus)
{
    long long num = 0;

    while (*digit != '\0') {
        if (*digit >= '0' && *digit <= '9') {
            int flag = minus ? -1 : 1;
            num = num * 10 + flag * (*digit - '0');

            if ((!minus && num > 0x7FFFFFFF)
                || (minus && num < (signed int)0x80000000)) {
                num = 0;
                break;
            }

            digit++;
        } else {
            num = 0;
            break;
        }
    }

    if (*digit == '\0') {
        g_nStatus = kValid;
    }

    return num;
}

// ====================测试代码====================
void Test(char* string)
{
    int result = StrToInt(string);

    if (result == 0 && g_nStatus == kInvalid)
        printf("the input %s is invalid.\n", string);
    else
        printf("number for %s is: %d.\n", string, result);
}

int _tmain(int argc, _TCHAR* argv[])
{
    Test(NULL);
    Test("");
    Test("123");
    Test("+123");
    Test("-123");
    Test("1a33");
    Test("+0");
    Test("-0");
    //有效的最大正整数, 0x7FFFFFFF
    Test("+2147483647");
    Test("-2147483647");
    Test("+2147483648");
    //有效的最小负整数, 0x80000000
    Test("-2147483648");
    Test("+2147483649");
    Test("-2147483649");
    Test("+");
    Test("-");
    return 0;
}

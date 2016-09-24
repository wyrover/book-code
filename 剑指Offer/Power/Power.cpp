// Power.cpp : Defines the entry point for the console application.
//

// 《剑指Offer——名企面试官精讲典型编程题》代码
// 著作权所有者：何海涛

#include "stdafx.h"
#include <math.h>

bool g_InvalidInput = false;
bool equal(double num1, double num2);
double PowerWithUnsignedExponent(double base, unsigned int exponent);

double Power(double base, int exponent)
{
    g_InvalidInput = false;

    if (equal(base, 0.0) && exponent < 0) {
        g_InvalidInput = true;
        return 0.0;
    }

    unsigned int absExponent = (unsigned int)(exponent);

    if (exponent < 0)
        absExponent = (unsigned int)(-exponent);

    double result = PowerWithUnsignedExponent(base, absExponent);

    if (exponent < 0)
        result = 1.0 / result;

    return result;
}

/*
double PowerWithUnsignedExponent(double base, unsigned int exponent)
{
    double result = 1.0;
    /
    for(int i = 1; i <= exponent; ++i)
        result *= base;

    return result;
}
*/
double PowerWithUnsignedExponent(double base, unsigned int exponent)
{
    if (exponent == 0)
        return 1;

    if (exponent == 1)
        return base;

    double result = PowerWithUnsignedExponent(base, exponent >> 1);
    result *= result;

    if ((exponent & 0x1) == 1)
        result *= base;

    return result;
}

bool equal(double num1, double num2)
{
    if ((num1 - num2 > -0.0000001)
        && (num1 - num2 < 0.0000001))
        return true;
    else
        return false;
}

// ====================测试代码====================
void Test(double base, int exponent, double expectedResult, bool expectedFlag)
{
    double result = Power(base, exponent);

    if (abs(result - expectedResult) < 0.00000001
        && g_InvalidInput == expectedFlag)
        printf("Test passed.\n");
    else
        printf("Test failed.\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
    // 底数、指数都为正数
    printf("Test1 begins.\n");
    Test(2, 3, 8, false);
    // 底数为负数、指数为正数
    printf("Test2 begins.\n");
    Test(-2, 3, -8, false);
    // 指数为负数
    printf("Test3 begins.\n");
    Test(2, -3, 0.125, false);
    // 指数为0
    printf("Test4 begins.\n");
    Test(2, 0, 1, false);
    // 底数、指数都为0
    printf("Test5 begins.\n");
    Test(0, 0, 1, false);
    // 底数为0、指数为正数
    printf("Test6 begins.\n");
    Test(0, 4, 0, false);
    // 底数为0、指数为负数
    printf("Test7 begins.\n");
    Test(0, -4, 0, true);
    return 0;
}


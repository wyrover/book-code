// AddTwoNumbers.cpp : Defines the entry point for the console application.
//

// 《剑指Offer——名企面试官精讲典型编程题》代码
// 著作权所有者：何海涛

#include "stdafx.h"

int Add(int num1, int num2)
{
    int sum, carry;

    do {
        sum = num1 ^ num2;
        carry = (num1 & num2) << 1;
        num1 = sum;
        num2 = carry;
    } while (num2 != 0);

    return num1;
}

// ====================测试代码====================
void Test(int num1, int num2, int expected)
{
    int result = Add(num1, num2);

    if (result == expected)
        printf("%d + %d is %d. Passed\n", num1, num2, result);
    else
        printf("%d + %d is %d. Failed\n", num1, num2, result);
}

int _tmain(int argc, _TCHAR* argv[])
{
    Test(1, 2, 3);
    Test(111, 899, 1010);
    Test(-1, 2, 1);
    Test(1, -2, -1);
    Test(3, 0, 3);
    Test(0, -4, -4);
    Test(-2, -8, -10);
}


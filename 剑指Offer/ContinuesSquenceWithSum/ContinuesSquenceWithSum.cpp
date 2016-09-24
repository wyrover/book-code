// ContinuesSquenceWithSum.cpp : Defines the entry point for the console application.
//

// 《剑指Offer——名企面试官精讲典型编程题》代码
// 著作权所有者：何海涛

#include "stdafx.h"

void PrintContinuousSequence(int small, int big);

void FindContinuousSequence(int sum)
{
    if (sum < 3)
        return;

    int small = 1;
    int big = 2;
    int middle = (1 + sum) / 2;
    int curSum = small + big;

    while (small < middle) {
        if (curSum == sum)
            PrintContinuousSequence(small, big);

        while (curSum > sum && small < middle) {
            curSum -= small;
            small ++;

            if (curSum == sum)
                PrintContinuousSequence(small, big);
        }

        big ++;
        curSum += big;
    }
}

void PrintContinuousSequence(int small, int big)
{
    for (int i = small; i <= big; ++ i)
        printf("%d ", i);

    printf("\n");
}

// ====================测试代码====================
void Test(char* testName, int sum)
{
    if (testName != NULL)
        printf("%s for %d begins: \n", testName, sum);

    FindContinuousSequence(sum);
}

int _tmain(int argc, _TCHAR* argv[])
{
    Test("test1", 1);
    Test("test2", 3);
    Test("test3", 4);
    Test("test4", 9);
    Test("test5", 15);
    Test("test6", 100);
    return 0;
}


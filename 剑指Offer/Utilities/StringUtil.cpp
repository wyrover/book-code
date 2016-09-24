// 《剑指Offer——名企面试官精讲典型编程题》代码
// 著作权所有者：何海涛

#include "stdafx.h"
#include "StringUtil.h"

void Reverse(char *pBegin, char *pEnd)
{
    if (pBegin == NULL || pEnd == NULL)
        return;

    while (pBegin < pEnd) {
        char temp = *pBegin;
        *pBegin = *pEnd;
        *pEnd = temp;
        pBegin ++, pEnd --;
    }
}

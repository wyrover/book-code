// 《剑指Offer——名企面试官精讲典型编程题》代码
// 著作权所有者：何海涛

#pragma once

struct ComplexListNode {
    int                 m_nValue;
    ComplexListNode*    m_pNext;
    ComplexListNode*    m_pSibling;
};

ComplexListNode* CreateNode(int nValue);
void BuildNodes(ComplexListNode* pNode, ComplexListNode* pNext, ComplexListNode* pSibling);
void PrintList(ComplexListNode* pHead);
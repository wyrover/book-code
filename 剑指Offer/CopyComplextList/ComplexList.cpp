#include "stdafx.h"
#include "ComplexList.h"

// 《剑指Offer——名企面试官精讲典型编程题》代码
// 著作权所有者：何海涛

ComplexListNode* CreateNode(int nValue)
{
    ComplexListNode* pNode = new ComplexListNode();
    pNode->m_nValue = nValue;
    pNode->m_pNext = NULL;
    pNode->m_pSibling = NULL;
    return pNode;
}

void BuildNodes(ComplexListNode* pNode, ComplexListNode* pNext, ComplexListNode* pSibling)
{
    if (pNode != NULL) {
        pNode->m_pNext = pNext;
        pNode->m_pSibling = pSibling;
    }
}

void PrintList(ComplexListNode* pHead)
{
    ComplexListNode* pNode = pHead;

    while (pNode != NULL) {
        printf("The value of this node is: %d.\n", pNode->m_nValue);

        if (pNode->m_pSibling != NULL)
            printf("The value of its sibling is: %d.\n", pNode->m_pSibling->m_nValue);
        else
            printf("This node does not have a sibling.\n");

        printf("\n");
        pNode = pNode->m_pNext;
    }
}

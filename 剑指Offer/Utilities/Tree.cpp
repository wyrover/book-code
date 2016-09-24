// 《剑指Offer——名企面试官精讲典型编程题》代码
// 著作权所有者：何海涛

#include "StdAfx.h"
#include "Tree.h"

TreeNode* CreateTreeNode(int value)
{
    TreeNode* pNode = new TreeNode();
    pNode->m_nValue = value;
    return pNode;
}

void ConnectTreeNodes(TreeNode* pParent, TreeNode* pChild)
{
    if (pParent != NULL) {
        pParent->m_vChildren.push_back(pChild);
    }
}

void PrintTreeNode(TreeNode* pNode)
{
    if (pNode != NULL) {
        printf("value of this node is: %d.\n", pNode->m_nValue);
        printf("its children is as the following:\n");
        std::vector<TreeNode*>::iterator i = pNode->m_vChildren.begin();

        while (i < pNode->m_vChildren.end()) {
            if (*i != NULL)
                printf("%d\t", (*i)->m_nValue);
        }

        printf("\n");
    } else {
        printf("this node is null.\n");
    }

    printf("\n");
}

void PrintTree(TreeNode* pRoot)
{
    PrintTreeNode(pRoot);

    if (pRoot != NULL) {
        std::vector<TreeNode*>::iterator i = pRoot->m_vChildren.begin();

        while (i < pRoot->m_vChildren.end()) {
            PrintTree(*i);
            ++i;
        }
    }
}

void DestroyTree(TreeNode* pRoot)
{
    if (pRoot != NULL) {
        std::vector<TreeNode*>::iterator i = pRoot->m_vChildren.begin();

        while (i < pRoot->m_vChildren.end()) {
            DestroyTree(*i);
            ++i;
        }

        delete pRoot;
    }
}

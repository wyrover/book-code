/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
#ifndef __BSTREE_H
#define __BSTREE_H

#include <windows.h>
#include <strsafe.h>

class CBinaryTree
{
	public:
		CBinaryTree();

		BOOL Add(int num);
		VOID Traverse();


	private:
		class _TreeNode
		{
			public:
				_TreeNode(int num) {pLeftChild=NULL; pRightChild=NULL; data=num;}
				
				int data;
				_TreeNode* pLeftChild;
				_TreeNode* pRightChild;
		};

		BOOL AddNode(_TreeNode* item, _TreeNode* current, DWORD dwRecursiveCount);
		VOID TraverseTree(_TreeNode* current);

		_TreeNode* 	m_pHead;
		DWORD		m_dwDepth;
		DWORD		m_dwNumNodes;
};
#endif

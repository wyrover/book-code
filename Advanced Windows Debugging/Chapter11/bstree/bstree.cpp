/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/

#include "bstree.h"
#include <stdio.h>

CBinaryTree::CBinaryTree () : m_pHead(NULL), m_dwDepth(0), m_dwNumNodes(0)
{
}

BOOL CBinaryTree::Add ( int num )
{
	BOOL bRet = FALSE ;
	_TreeNode* item = new _TreeNode ( num ) ;
	if ( item != NULL )
	{
		bRet = AddNode ( item, m_pHead, 1 ) ;
		if ( bRet == TRUE )
		{
			m_dwNumNodes++ ;
		}
	}
	
	return bRet ;
}

VOID CBinaryTree::Traverse ( )
{
	TraverseTree ( m_pHead ) ;
}


VOID CBinaryTree::TraverseTree  ( _TreeNode* current )
{
	if ( current == NULL )
	{
		return ;
	}


	//
	// Simple left->right subtree traversal
	//
	TraverseTree ( current->pLeftChild ) ;
	printf ( "Item: %d\n", current->data ) ;

	return ;
}

BOOL CBinaryTree::AddNode ( _TreeNode* item, _TreeNode* current, DWORD dwRecursiveCount )
{
	BOOL bRet = FALSE ;
	
	//
	// Empty tree, initialize the head pointer
	//
	if ( current == NULL )
	{
		m_pHead = item ;
		m_dwDepth = dwRecursiveCount ;
		return TRUE ;
	}
	else
	{
		if ( item->data > current->data )
		{
			if ( current->pRightChild == NULL )
			{
				current->pRightChild = item ;
				m_dwDepth = ( m_dwDepth < dwRecursiveCount+1 ) ? dwRecursiveCount + 1 : m_dwDepth ;
				bRet = TRUE ;
			}
			else
			{
				return AddNode ( item, current->pRightChild, dwRecursiveCount + 1 ) ;
			}
		}
		else if ( item->data < current->data )
		{
			if ( current->pLeftChild == NULL )
			{
				current->pLeftChild = item ;
				m_dwDepth = ( m_dwDepth < dwRecursiveCount+1 ) ? dwRecursiveCount + 1 : m_dwDepth ;				
				bRet = TRUE ;
			}
			else
			{
				return AddNode ( item, current->pLeftChild, dwRecursiveCount + 1 ) ;
			}
		}

		return bRet ;
	}
}

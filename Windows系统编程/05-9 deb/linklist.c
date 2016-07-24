// ************************************************************************
//
//                      Microsoft Developer Support
//             Copyright (c) 1992-1997 Microsoft Corporation
//
// ************************************************************************
// MODULE    : LinkList.C - Ordered Double Linked List Package
// PURPOSE   : Provide a general, sorted, double linked list package
// FUNCTIONS :
//   CreateList        - allocate memory for a new list, registers the
//                       ordering function
//   CreateNode        - allocate memory fpr a new node that can be put
//                       into the linked list
//   InsertNode        - insert a new node into the list
//   SetCurrentNode    - finds the 1st occurence of a node that matches
//                       a key(s) according to the match function
//                       and sets it as the current node
//   SetCurrentNodeEx  - finds the Nth occurence of a node that matches
//                       a key(s) according to the match function
//                       and sets it as the current node
//   GetCurrentNode    - get the current node
//   GetFirstNode      - get the first (left-most) node in the list
//   GetLastNode       - get the last (right-most) node in the list
//   GetNextNode       - get the next (right) node from the current
//   GetPrevNode       - get the previous (left) node from the current
//   DeleteCurrentNode - deletes the current node from the list and
//                       frees the memory associated with it
//   DestroyNode       - deallocates the memory associated with a node
//   DestroyList       - deallocates the memory associated with a list,
//                       does not delete any of the nodes
//   GetListError      - gets the current list error
//
// COMMENTS  : The application must serialize access to the linked list
//
//   Format of the Ordering and Matching functions is as follows:
//   -----------------------------------------------------------
//
//   int OrderFunc( PNODE pNodeOne, PNODE pNodeTwo )
//   {
//     if( (pNodeOne->pNodeData).SortKey <  (pNodeTwo->pNodeData).SortKey )
//       return( LIST_LEFT_OF  );
//     if( (pNodeOne->pNodeData).SortKey == (pNodeTwo->pNodeData).SortKey )
//       return( LIST_MATCH    );
//     if( (pNodeOne->pNodeData).SortKey  > (pNodeTwo->pNodeData).SortKey )
//       return( LIST_RIGHT_OF );
//   }
//
// ************************************************************************
#include <StdLib.H>     // malloc(), free()
#include "LinkList.H"

//-- NULL pointer dereferencing checking macro
#define CHECK_POINTER( ppNode )                                       \
          if( ppNode == NULL ) {                                      \
            return( pList->ListError = LIST_ERROR_DEREFERENCE_NULL ); \
          }


// ************************************************************************
// FUNCTION : CreateList( PLIST* ppList, int (*)(PNODE, PNODE) ) )
// PURPOSE  : allocate a new list, registers the ordering function
// COMMENTS : sets all pointers to NULL
// ************************************************************************
BOOL
CreateList( PLIST* ppList, int (*OrderFunction)( PNODE pNodeOne, PNODE pNodeTwo ) )
{
  *ppList = (PLIST) malloc( sizeof(LIST) );
  if( ppList == NULL ) {
    (*ppList)->ListError = LIST_NO_ERROR;
    return( FALSE );
  }

  //-- register the link list data ordering function
  (*ppList)->OrderFunction = OrderFunction;

  //-- initialize the general linked list node pointers to NULL
  (*ppList)->pFirstNode   = NULL;
  (*ppList)->pCurrentNode = NULL;
  (*ppList)->pLastNode    = NULL;

  //-- indicate no error
  (*ppList)->ListError  = LIST_NO_ERROR;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : CreateNode( PNODE* )
// PURPOSE  : create/allocate a new node that can be put into list
// COMMENTS :
// ************************************************************************
BOOL
CreateNode( PNODE* ppNewNode )
{
  *ppNewNode = (PNODE) malloc( sizeof(NODE) );
  if( *ppNewNode == NULL ) {
    return( FALSE );
  }

  //-- initialize the node pointers to NULL
  (*ppNewNode)->pRightLink = NULL;
  (*ppNewNode)->pLeftLink  = NULL;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : InsertNode( PLIST pList, PNODE )
// PURPOSE  : insert a new node into the list
// COMMENTS :
//   All nodes must have a unique key entry (from order function), if a
//   match is found during an insert then the old node gets replaced with
//   the new node.
//
//  NOTE: changes pList->pCurrentNode
// ************************************************************************
BOOL
InsertNode( PLIST pList, PNODE pNewNode )
{
  int Position;
  int LastPosition;

  //-- if this is the first node in a new list
  if( pList->pFirstNode == NULL ) {
    pList->pFirstNode   = pNewNode;
    pList->pLastNode    = pNewNode;
    pList->pCurrentNode = pNewNode;
    pList->ListError    = LIST_NO_ERROR;
    return( TRUE );
  }

  Position = (*(pList->OrderFunction))( pNewNode, pList->pCurrentNode );

  //-- search for insertion point
  while( pList->pCurrentNode != NULL ) {

    LastPosition = Position;
    Position = (*(pList->OrderFunction))(pNewNode, pList->pCurrentNode);

    if( pList->pFirstNode == pList->pLastNode )
      break;

    if( Position != LastPosition )
      break;

    if( pList->pCurrentNode == pList->pFirstNode ) {
      if( Position == LIST_RIGHT_OF ) {
        pList->pCurrentNode = pList->pCurrentNode->pRightLink;
        continue;
      }
      break;
    }

    if( pList->pCurrentNode == pList->pLastNode ) {
      if( Position == LIST_LEFT_OF ) {
        pList->pCurrentNode = pList->pCurrentNode->pLeftLink;
        continue;
      }
      break;
    }

    if( Position == LastPosition ) {
      if( Position == LIST_LEFT_OF) {
        pList->pCurrentNode = pList->pCurrentNode->pLeftLink;
        continue;
      }
      if( Position == LIST_RIGHT_OF ) {
        pList->pCurrentNode = pList->pCurrentNode->pRightLink;
        continue;
      }
      break;
    }

  }

  //-- now, insert the pNewNode
  switch( Position ) {

    case LIST_LEFT_OF:
      pNewNode->pRightLink = pList->pCurrentNode;
      pNewNode->pLeftLink  = pList->pCurrentNode->pLeftLink;
      if( pList->pCurrentNode == pList->pFirstNode )
        pList->pFirstNode = pNewNode;
      else
        pList->pCurrentNode->pLeftLink->pRightLink = pNewNode;
      pList->pCurrentNode->pLeftLink = pNewNode;
      break;

  #if 1  // replace duplicates with new data

    case LIST_MATCH:
      pNewNode->pLeftLink  = pList->pCurrentNode->pLeftLink;
      pNewNode->pRightLink = pList->pCurrentNode->pRightLink;
      if( pList->pCurrentNode == pList->pLastNode )
        pList->pLastNode = pNewNode;
      else
        pList->pCurrentNode->pRightLink->pLeftLink = pNewNode;
      if( pList->pCurrentNode == pList->pFirstNode )
        pList->pFirstNode = pNewNode;
      else
        pList->pCurrentNode->pLeftLink->pRightLink = pNewNode;
      // note: doesn't destroy extra data associated with the node
      DestroyNode( pList->pCurrentNode );
      break;

  #else // or allow duplicates

    case LIST_MATCH:

  #endif

    case LIST_RIGHT_OF:
      pNewNode->pLeftLink  = pList->pCurrentNode;
      pNewNode->pRightLink = pList->pCurrentNode->pRightLink;
      if( pList->pCurrentNode == pList->pLastNode )
        pList->pLastNode = pNewNode;
      else
        pList->pCurrentNode->pRightLink->pLeftLink = pNewNode;
      pList->pCurrentNode->pRightLink = pNewNode;
      break;

  }
  pList->pCurrentNode = pNewNode;
  pList->ListError    = LIST_NO_ERROR;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : SetCurrentNode( PLIST pList, PNODE, int (*)(PNODE, PNODE) )
// PURPOSE  : finds the first occurence of a node from the list that matches
//            a key(s) according to the match function.
// COMMENTS :
//   May use the ordering function or a new matching function if not
//   searching for a match based on the primary ordering key.
//   NOTE: the matching and ordering functions have the same definition.
//   NOTE: changes pList->pCurrentNode.
// ************************************************************************
BOOL
SetCurrentNode( PLIST pList, PNODE pKeyNode,
         int (*MatchFunction)( PNODE pNodeOne, PNODE pNodeTwo ) )
{
  return( SetCurrentNodeEx( pList, pKeyNode, MatchFunction, 1 ) );
}


// ************************************************************************
// FUNCTION : SetCurrentNodeEx( PLIST pList, PNODE, int (*)(PNODE, PNODE), int )
// PURPOSE  : finds the Nth occurence of a node from the list that matches
//            a key(s) according to the match function.
// COMMENTS :
//   May use the ordering function or a new matching function if not
//   searching for a match based on the primary ordering key.
//   NOTE: the matching and ordering functions have the same definition.
//   NOTE: changes pList->pCurrentNode.
// ************************************************************************
BOOL
SetCurrentNodeEx( PLIST pList, PNODE pKeyNode,
  int (*MatchFunction)( PNODE pNodeOne, PNODE pNodeTwo ),
  int Occurence )
{
  int Position;

  //-- if list is empty, exit
  if( pList->pCurrentNode == NULL ) {
    pList->ListError = LIST_ERROR_NO_NODE;
    return( FALSE );
  }

  //-- if match and order are same function
  if( MatchFunction == (pList->OrderFunction) ) {
    int  LastPosition;

    LastPosition = Position = (*MatchFunction)(pKeyNode, pList->pCurrentNode);

    while( Occurence ) {
      if( ( Position == LIST_LEFT_OF ) && ( LastPosition == LIST_LEFT_OF )
          && ( pList->pCurrentNode != pList->pFirstNode ) )
        pList->pCurrentNode = pList->pCurrentNode->pLeftLink;
      else if( ( Position == LIST_RIGHT_OF ) && ( LastPosition == LIST_RIGHT_OF )
            && ( pList->pCurrentNode != pList->pLastNode ) )
        pList->pCurrentNode = pList->pCurrentNode->pRightLink;
      else {
        Occurence--;
        continue;
      }
      LastPosition = Position;
      Position = (*MatchFunction)(pKeyNode, pList->pCurrentNode);
    }

  }
  //-- match and order are not the same function, thus start
  //   the search at the front of the list
  else {
    pList->pCurrentNode = pList->pFirstNode;
    while( (Occurence > 0) && ( (pList->pCurrentNode) != NULL ) ) {
      Position = (*MatchFunction)(pKeyNode, pList->pCurrentNode);
      if( Position == LIST_MATCH )
        Occurence--;
      if( Occurence > 0 )
        pList->pCurrentNode = pList->pCurrentNode->pRightLink;
    }
  }

  if( ( Position == LIST_MATCH ) && ( Occurence == 0 ) ) {
    pList->ListError = LIST_NO_ERROR;
    return( TRUE );
  }
  pList->ListError = LIST_ERROR_NO_MATCH;

  return( FALSE );
}


// ************************************************************************
// FUNCTION : GetCurrentNode( PLIST pList, PNODE* )
// PURPOSE  : gets the current node from the list
// COMMENTS :
//   Does not change pList->pCurrentNode.
//   Changes ppNode, thus do not pass in a ppNode which has additional
//   memory associated with it.
// ************************************************************************
BOOL
GetCurrentNode( PLIST pList, PNODE* ppNode )
{
  CHECK_POINTER( ppNode );
  if( pList->pCurrentNode == NULL ) {
    pList->ListError = LIST_ERROR_NO_NODE;
    return( FALSE );
  }
  *ppNode = pList->pCurrentNode;
  pList->ListError = LIST_NO_ERROR;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : GetFirstNode( PLIST pList, PNODE* )
// PURPOSE  : gets the first (left-most) node from the list
// COMMENTS :
//   Does not change pList->pCurrentNode.
//   Changes ppNode, thus do not pass in a ppNode which has additional
//   memory associated with it.
// ************************************************************************
BOOL
GetFirstNode( PLIST pList, PNODE* ppNode )
{
  CHECK_POINTER( ppNode );
  if( pList->pFirstNode == NULL ) {
    pList->ListError = LIST_ERROR_NO_NODE;
    return( FALSE );
  }
  *ppNode = pList->pFirstNode;
  pList->ListError = LIST_NO_ERROR;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : GetLastNode( PLIST pList, PNODE* )
// PURPOSE  : get the last (right-most) node from the List
// COMMENTS :
//   Does not change pList->pCurrentNode.
//   Changes ppNode, thus do not pass in a ppNode which has additional
//   memory associated with it.
// ************************************************************************
BOOL
GetLastNode( PLIST pList, PNODE* ppNode )
{
  CHECK_POINTER( ppNode );
  if( pList->pLastNode == NULL ) {
    pList->ListError = LIST_ERROR_NO_NODE;
    return( FALSE );
  }
  *ppNode = pList->pLastNode;
  pList->ListError = LIST_NO_ERROR;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : GetNextNode( PLIST pList, PNODE* )
// PURPOSE  : get the next (right) node from the pList->pCurrentNode
// COMMENTS :
//   Does not change pList->pCurrentNode.
//   Changes ppNode, thus do not pass in a ppNode which has additional
//   memory associated with it.
// ************************************************************************
BOOL
GetNextNode( PLIST pList, PNODE* ppNode )
{
  CHECK_POINTER( ppNode );
  if( (*ppNode)->pRightLink == NULL ) {
    pList->ListError = LIST_ERROR_NO_NODE;
    return( FALSE );
  }
  *ppNode = (*ppNode)->pRightLink;
  pList->ListError = LIST_NO_ERROR;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : GetPrevNode( PLIST pList, PNODE* )
// PURPOSE  : get the previous (left) node from the pList->pCurrentNode
// COMMENTS :
//   Does not change pList->pCurrentNode.
//   Changes ppNode, thus do not pass in a ppNode which has additional
//   memory associated with it.
// ************************************************************************
BOOL
GetPrevNode( PLIST pList, PNODE* ppNode )
{
  CHECK_POINTER( ppNode );
  if( (*ppNode)->pLeftLink == NULL ) {
    pList->ListError = LIST_ERROR_NO_NODE;
    return( FALSE );
  }
  *ppNode = (*ppNode)->pLeftLink;
  pList->ListError = LIST_NO_ERROR;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : DeleteCurrentNode( PLIST pList )
// PURPOSE  : deletes the current node from the list and frees the memory
//            associated with it
// COMMENTS :
//   Changes pList->pCurrentNode.
//
//   Typically, SetCurrentNode (or SetCurrentNodeEx) is called first to set
//   pList->pCurrentNode.  Also, any addtional memory associated with this
//   node should be freed first before calling this function.
// ************************************************************************
BOOL
DeleteCurrentNode( PLIST pList )
{
  PNODE pOldCurrentNode;

  if( pList->pCurrentNode != NULL ) {
    pOldCurrentNode = pList->pCurrentNode;

    if( pOldCurrentNode == pList->pFirstNode ) {
      pList->pFirstNode   = pOldCurrentNode->pRightLink;
      pList->pCurrentNode = pOldCurrentNode->pRightLink;
    }
    else {
      pOldCurrentNode->pLeftLink->pRightLink = pOldCurrentNode->pRightLink;
      pList->pCurrentNode = pOldCurrentNode->pLeftLink;
    }

    if( pOldCurrentNode == pList->pLastNode )
      pList->pLastNode = pOldCurrentNode->pLeftLink;
    else
      pOldCurrentNode->pRightLink->pLeftLink = pOldCurrentNode->pLeftLink;

    DestroyNode( pOldCurrentNode );
    pList->ListError = LIST_NO_ERROR;
    return( TRUE );
  }
  pList->ListError = LIST_NO_ERROR;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : DestroyNode( PNODE pNode )
// PURPOSE  : deallocates a node
// COMMENTS :
// ************************************************************************
BOOL
DestroyNode( PNODE pNode )
{
  free( pNode );

  return( TRUE );
}


// ************************************************************************
// FUNCTION : DestroyList( PLIST pList )
// PURPOSE  : deallocates a list, does not free any nodes, if present
// COMMENTS :
// ************************************************************************
BOOL
DestroyList( PLIST pList )
{
  free( pList );
  pList->ListError = LIST_NO_ERROR;

  return( TRUE );
}


// ************************************************************************
// FUNCTION : GetListError( PLIST pList )
// PURPOSE  : get the last linked list error
// COMMENTS :
// ************************************************************************
int
GetListError( PLIST pList )
{
  return( pList->ListError );
}

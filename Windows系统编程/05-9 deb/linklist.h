// ************************************************************************
//
//                      Microsoft Developer Support
//             Copyright (c) 1992-1997 Microsoft Corporation
//
// ************************************************************************
// HEADER    : LinkList.H - Ordered Double Linked List Package
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
//     if( (pNodeOne->pNodeData).SortKey < (pNodeTwo->pNodeData).SortKey )
//       return( LIST_LEFT_OF );
//     if( (pNodeOne->pNodeData).SortKey > (pNodeTwo->pNodeData).SortKey )
//       return( LIST_RIGHT_OF );
//     return( LIST_MATCH );
//   }
//
// ************************************************************************
#ifndef LINKLIST_H

 #define LINKLIST_H

 #ifdef __cplusplus
 extern "C" { /* Assume C declarations for C++ */
 #endif /* __cplusplus */

 // public typedefs and defines
 // -----------------------------------------------------------------------
 #define LIST_NO_ERROR                 1
 #define LIST_ERROR_NO_NODE           -1
 #define LIST_ERROR_NO_MATCH          -2
 #define LIST_ERROR_NO_FREE_MEM       -3
 #define LIST_ERROR_DEREFERENCE_NULL -99

 #define LIST_LEFT_OF   -1
 #define LIST_MATCH      0
 #define LIST_RIGHT_OF   1

 #ifndef TRUE
   #define TRUE  1
   #define FALSE 0
   typedef int   BOOL;
 #endif

 typedef void* PVOID;

 //-- definition of a node
 typedef struct NODE_STRUCT* PNODE;
 typedef struct NODE_STRUCT {
           PVOID pNodeData;
           PNODE pLeftLink;
           PNODE pRightLink;
         } NODE;

 //-- definition of a list
 typedef struct LIST_STRUCT* PLIST;
 typedef struct LIST_STRUCT {
           PVOID pListData;
           PNODE pFirstNode;
           PNODE pCurrentNode;
           PNODE pLastNode;
           int   (*OrderFunction)(PNODE, PNODE);
           int   ListError;
         } LIST;

 // public function prototypes
 // -----------------------------------------------------------------------
 BOOL CreateList( PLIST* ppList, int (*OrderFunction)( PNODE pNodeOne, PNODE pNodeTwo ) );
 BOOL CreateNode( PNODE* ppNewNode );
 BOOL InsertNode( PLIST pList, PNODE pNewNode );
 BOOL SetCurrentNode( PLIST pList, PNODE pKeyNode, int (*MatchFunction)( PNODE pNodeOne, PNODE pNodeTwo ) );
 BOOL SetCurrentNodeEx( PLIST pList, PNODE pKeyNode, int (*MatchFunction)( PNODE pNodeOne, PNODE pNodeTwo ), int Occurence );
 BOOL GetCurrentNode( PLIST pList, PNODE* ppNode );
 BOOL GetFirstNode( PLIST pList, PNODE* ppNode );
 BOOL GetLastNode( PLIST pList, PNODE* ppNode );
 BOOL GetNextNode( PLIST pList, PNODE* ppNode );
 BOOL GetPrevNode( PLIST pList, PNODE* ppNode );
 BOOL DeleteCurrentNode( PLIST pList );
 BOOL DestroyNode( PNODE pNode );
 BOOL DestroyList( PLIST pList );
 int  GetListError( PLIST pList );

 #ifdef __cplusplus
 }
 #endif /* __cplusplus */


#endif // LINKLIST_H

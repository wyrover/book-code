/*----------------------------------------------------
    BPNODE.C
	 Linked list functions for STEPPER.  These functions
	 manage a linked list of nodes containing info
	 about each breakpoint that has been created.

	 (c) 1994 by Brian G. Myers
  ----------------------------------------------------*/
#define STRICT
#include <windows.h>
#pragma hdrstop
#include "stepper.h"

/*----------------------------------------------------
    STATIC VARIABLES
  ----------------------------------------------------*/
extern HWND ghMainWnd;
PBPNODE pHead = NULL;
HANDLE hHeap = NULL;

/*----------------------------------------------------
    NEW NODE
  ----------------------------------------------------*/
PBPNODE NewNode ( void )
{
  PBPNODE pNew;
  
  __try {
    /* create the BPNODE heap, if necessary */
    if (!hHeap) 
      hHeap = HeapCreate(HEAP_NO_SERIALIZE, 0L, 10*sizeof(BPNODE));
    if (!hHeap) ERROR_MSG(ghMainWnd);

    /* allocate space for a new node */
    pNew = HeapAlloc(hHeap, HEAP_ZERO_MEMORY,
      sizeof(BPNODE));

    if (!pNew) {
      ERROR_MSG(ghMainWnd);
      return(NULL);
    }

    /* Place the new node at the head of the list. */
    /* Note that the "Run to cursor" command expects to */
    /* find the most recently added node before any */
    /* other breakpoints set on the same line. */
    pNew->pNext = pHead;
    pHead = pNew;

  } __except(EXCEPTION_EXECUTE_HANDLER) {
    return(NULL);
  }

  return(pHead);
}

/*----------------------------------------------------
    DELETE NODE
  ----------------------------------------------------*/
BOOL DeleteNode ( PBPNODE pDelNode )
{
  PBPNODE pNode, pPrev;

  /* if list doesn't exist, return */
  if (!pHead) return(FALSE);

  /* delete head node if requested */
  if (pHead == pDelNode) {
    pNode = pHead->pNext;
    HeapFree(hHeap, 0L, pHead);
    pHead = pNode;
    return(TRUE);
  }

  /* scan list for node to be deleted */
  pNode = pHead;
  while (pNode->pNext != pDelNode) {
    pNode = pNode->pNext;
    if (!pNode->pNext)
      return(FALSE);
  }

  /* delete the node */
  pPrev = pNode;
  pNode = pNode->pNext;
  pPrev->pNext = pNode->pNext;
  HeapFree(hHeap, 0L, pNode); 
  return(TRUE);
}

/*----------------------------------------------------
    FIRST NODE, NEXT NODE, LAST NODE
  ----------------------------------------------------*/
PBPNODE FirstNode( void )
{ return(pHead); }

PBPNODE NextNode( PBPNODE pNode )
{ return(pNode->pNext); }

PBPNODE LastNode( void )
{
  PBPNODE pNode;

  if (!pHead) return(NULL);
  
  pNode = pHead;
  while (pNode->pNext)
    pNode = pNode->pNext;
  return(pNode);
}

/*----------------------------------------------------
    FIND BP BY ADDRESS 
  ----------------------------------------------------*/
PBPNODE FindBPbyAddress ( PBYTE pAddress )
{
  PBPNODE pNode = pHead;
  if (!pNode) return(NULL);
  
  while (pNode->pAddress != pAddress) {
    pNode = pNode->pNext;
    if (!pNode) break;
  }
  return(pNode);
}

/*----------------------------------------------------
    FIND NEXT BP BY LINE
  ----------------------------------------------------*/
PBPNODE FindNextBPbyLine( PBPNODE pStart, UINT uLine )
{
  PBPNODE pNode = (pStart ? pStart : pHead);
  if (!pNode) return(NULL);

  while (pNode->uLine != uLine) {
    pNode = pNode->pNext;
    if (!pNode) break;
  }
  return(pNode);
}

/*----------------------------------------------------
    DESTROY LIST
  ----------------------------------------------------*/
void DestroyList ( void )
{
  PBPNODE pNode;
  
  while (pHead)  {
    pNode = pHead;
    pHead = pHead->pNext;
    HeapFree(hHeap, 0L, pNode);
  }
  pHead = NULL;

  if (hHeap) {
    HeapDestroy(hHeap);
    hHeap = NULL;
  }
}


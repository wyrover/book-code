

/****************************** Module Header *******************************
* Module Name: LIST.C
*
*
* Functions:
*
* Alloc()
* Free()
* List_Init()
* List_Dump()
* List_Show()
* List_Create()
* List_Destroy()
* List_AddFirst()
* List_NewFirst()
* List_DeleteFirst()
* List_AddLast()
* List_NewLast()
* LIst_DeleteLast()
* List_AddAfter()
* List_NewAfter()
* List_AddBefore()
* List_NewBefore()
* List_Delete()
* List_DeleteForwards()
* List_DeleteBackwards()
* List_ItemLength()
* List_First()
* List_Last()
* List_Next()
* List_Prev()
* List_Clear()
* List_IsEmpty()
* SwitchLists()
* List_Join()
* List_InsertListAfter()
* List_InsertListBefore()
* List_SplitAfter()
* List_SplitBefore()
* List_Card()
* List_IsOK()
* LIst_MakeOK()
* List_Check()
* List_Recover()
*
* Comments:
*
*
****************************************************************************/

#include <memory.h>
#include <windows.h>

#include "gutils.h"
#include "list.h"
#include "gutilsrc.h"

#define memcpy  memcpy

char msg[80];  /* a temp for building up wsprintf messages in */

#define BLOCKSIZE 25000
 typedef struct
 { HANDLE hMem;     /* memory handle for this block */
   int iInUse;    /* number of allocations taken out of it.  0 => free it */
   int iNext;     /* next byte to use */
   char chData[BLOCKSIZE];
 } BLOCK, FAR *PBLOCK;

static CRITICAL_SECTION CritSec;  /* to protect pCurrent */
#define List_Enter_Crit(x)      EnterCriticalSection(x)
#define List_Leave_Crit(x)      LeaveCriticalSection(x)

static PBLOCK pCurrent = NULL;  /* block currently in use */
                          /* must always be either NULL or valid */

 /* Allocate storage for List elements.  n.b. after a call to this
    you MUST record the value of pCurrent as you need to hand that in
    to Free.  You don't hand in the value of the actual storage.
    See screed above.
    This function Enters the critical section.  The caller must Leave it.
 */
static LPVOID Alloc(int size)
 { HANDLE hMem;
   LPVOID pRet;
   List_Enter_Crit(&CritSec);
   if ((pCurrent==NULL)||(pCurrent->iNext+size>BLOCKSIZE+1))
   { hMem = GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE,(DWORD)(sizeof(BLOCK)));
     if (hMem==NULL)
       { pCurrent = NULL;
         OutputDebugString("GlobalAlloc failed!!\n");
         return NULL;
       }
     pCurrent = (PBLOCK)GlobalLock(hMem);
     if (pCurrent==NULL)
       { OutputDebugString("GlobalLock failed!!\n");
         return NULL;
       }
     pCurrent->hMem = hMem;
     pCurrent->iInUse = 0;
     pCurrent->iNext = 0;
   }
   pRet = &(pCurrent->chData[pCurrent->iNext]);
   ++(pCurrent->iInUse);
   pCurrent->iNext += size;

   /* for MIPS we must also ensure that the data is aligned 4 byte*/
   pCurrent->iNext += 3;
   pCurrent->iNext -= pCurrent->iNext % 4;

   return pRet;
 } /* Alloc */

static void Free(PBLOCK pBlock, LPVOID p)
 { HANDLE hMem;
   List_Enter_Crit(&CritSec);
    --pBlock->iInUse;
   if (pBlock->iInUse<=0)
   { if (pBlock->iInUse<0)
     { 
        extern HANDLE hLibInst;
        TCHAR szBuf[512];
        LoadString(hLibInst, IDS_LIST_ALLOC_NEGATIVE, szBuf, sizeof(szBuf));
 
        wsprintf(msg, szBuf, pBlock->iInUse);
        MessageBox(NULL, msg, NULL, MB_OK | MB_ICONSTOP);
      }
 
     hMem = pBlock->hMem;
     GlobalUnlock(hMem);
     GlobalFree(hMem);
     if (pCurrent==pBlock) pCurrent = NULL; /* defend the invariant */
   }
   List_Leave_Crit(&CritSec);
 } /* Free */

  /* The following definition tells the truth about what an ITEM is.  The
  |  header file says only that there's a structure with the tag item_tag and
  |  that a LIST is a pointer to one.  Here we spell out what that structure
  |  is (and a LIST is still a pointer to one).  A PLIST is defined as a
  |  pointer to one of those, but is only really used because the C
  |  parameter mechanism demands an extra level of indirection for a
  |  parameter that can be updated.  (Modula-2 VAR parameter).
  */
  typedef struct item_tag
  { struct item_tag FAR *pitNext;    /* to next in circular list */
    struct item_tag FAR *pitPrev;    /* to prev in circular list */
    PBLOCK pBlock;               /* to memory block */
    BOOL bAnchor;                /* TRUE iff an anchor block */
    BOOL bOK;                    /* true unless a list op has failed */
    int iLen;                    /* length of data only */
    char Data[1];                /* the caller's data.  The '1' is a lie */
  } ITEM;

  /* For an anchor block, only the fields pitNext thru bAnchor are allocated.
  |  For a normal list element, Data may well be longer than 1 byte.
  |  The bOK flag is to support a style of programming where several
  |  successive operations can be done without having to check the return
  |  code at each stage.  At the end, the list can be examined to see if
  |  the data in it is valid or if it has been made invalid by the failure
  |  of any of the previous operations.  Certain operations may result in
  |  having no list at all if they fail (e.g. create) and for these, you'd
  |  better check the result at once!
  |  ??? Some of this screed belongs in the header!!!
  */

  static int iAnchorSize;      /* Size of anchor block (no data, no dummy) */
  static int iHeaderSize;      /* Size of data block not counting Data
                                  and offset from cursor back to item.
                               */
  static BOOL bInited = FALSE; /* TRUE <=> iAnchorSize and iHeaderSize are OK*/

#define MOVEBACK(Curs)                                               \
   { Curs = ((char FAR *)Curs-iHeaderSize); } /*move from Data to pitNext*/

  /*==================================================================
  || Lists are circular, doubly linked with an anchor block which holds
  || pointers to both ends.  Every block has a flag which shows whether
  || it's an anchor or not.
  ||
  || Empty list:
  ||
  ||      -------------
  ||     |             |
  ||     |   Anchor    |
  ||     v   -------   |
  ||  Ul--->| Next--+--|
  ||        |-------|  |
  ||        | Prev--+--
  ||         -------
  ||
  || One entry list:
  ||
  ||      ------------------------------------
  ||     |                                    |
  ||     |   Anchor                           |
  ||     v   -------                ------    |
  ||  Ul--->| Next--+------------->| Next-+---|
  ||        |-------|    |         |------|   |
  ||        | Prev--+----          | Prev-+---
  ||         -------               |------|
  ||                               | Len  |
  ||                               |------|
  ||                               | Data |
  ||                                ------
  || Two entry list:
  ||
  ||      -------------------------------------------------
  ||     | ---------------    ---------------              |
  ||     ||               |  |               |             |
  ||     ||  Anchor       |  |               |             |
  ||     vv  --------     |  v    ------     |    ------   |
  ||  Ul--->| Next--+-----+----->| Next-+----+-->| Next-+--
  ||        |-------|     |      |------|  | |   |------|
  ||        | Prev--+--    ------+-Prev |  |  ---+-Prev |
  ||         -------   |         |------|  |     |------|
  ||                   |         | Len  |  |     | Len  |
  ||                   |         |------|  |     |------|<----Cursor
  ||                   |         | Data |  |     | Data |
  ||                   |          ------   |      ------
  ||                   |                   |
  ||                    -------------------
  ||
  || etc.
  ||
  || Note that an external cursor (i.e one which is seen by the caller)
  || points to the Data field, not to the start of the structure.
  || This allows easy access to the data by the user at the cost of a
  || slightly slower traverse.
  || Within this module, we may sometimes traverse a list with  a cursor
  || that points to the start of an item.  This is called an item cursor.
  È===================================================================*/

  /*------------------------------------------------------------------
  | Set iAnchorSize and iHeaderSize.  Implementation independent!
   -------------------------------------------------------------------*/
void APIENTRY List_Init(void)
  {  LIST P;
     P = (LIST)&P;                  /* really any old address will do */
     iAnchorSize = (char FAR *)&(P->iLen) - (char FAR *)&(P->pitNext);
     iHeaderSize = (char FAR *)&(P->Data) - (char FAR *)&(P->pitNext);
     InitializeCriticalSection(&CritSec);
     /* assumes layout in storage is linear */
  }

  /* Dump the internals to the debugger. */
void APIENTRY List_Dump(LPSTR Header, LIST lst)
  {  LIST pit;
     char msg[250];

     OutputDebugString(Header);  OutputDebugString("\n");
     pit = lst;
     do
     { wsprintf(msg,"%8x %8x %8x %ld %s "
               , pit, pit->pitNext, pit->pitPrev, pit->iLen
               , (pit->bAnchor ? "Anchor" : "Data")
               );
       OutputDebugString(msg);
       if (pit->pitNext->pitPrev != pit)
         OutputDebugString(" Next Prev error!!");
       if (pit->pitPrev->pitNext != pit)
         OutputDebugString(" Prev Next error!!");
       OutputDebugString("\n");
       pit = pit->pitNext;
     } while (pit!=lst);
     OutputDebugString("End of list dump\n");
  } /* List_Dump */

  /* Dump hex representation of handle to debugger */
void APIENTRY List_Show(LIST lst)
  { char msg[50];               
    wsprintf(msg, "%8x", lst);
    OutputDebugString(msg);
  } /* List_Show */

  /*------------------------------------------------------------------
  | Create a list.  It will be initially empty
   -------------------------------------------------------------------*/
LIST APIENTRY List_Create(void)
  {  LIST lst;
     if (!bInited) {List_Init(); }          /* prevent some silly errors */
     lst = Alloc(iAnchorSize);
     if (lst==NULL) { return NULL; }
     lst->pBlock = pCurrent;
     List_Leave_Crit(&CritSec);
     lst->bOK = TRUE;
     lst->pitNext = lst;
     lst->pitPrev = lst;
     lst->bAnchor = TRUE;
     /* no length field set in an anchor block */
     return lst;
  } /* List_Create */

  /*------------------------------------------------------------------
  | Destroy *plst.  It does not need to be empty first
   -------------------------------------------------------------------*/
  void APIENTRY List_Destroy(PLIST plst)
  {  LIST pitP;    /* item cursor on * plst */
     LIST pitQ;    /* item cursor runs one step ahead of pitQ */

     if (plst==NULL)
       return;
     /* There is at least an anchor block to destroy */
     pitP = *plst;
     do
     {  pitQ = pitP->pitNext;
        Free(pitP->pBlock, pitP);
        pitP = pitQ;
     }while(pitP != *plst);
     *plst = NULL;
  } /* List_Destroy */

  /*------------------------------------------------------------------
  | Add an item holding Object to the beginning of * plst
   -------------------------------------------------------------------*/
  void APIENTRY List_AddFirst(LIST lst, LPVOID pObject, UINT uLen)
  {  LIST pit;      /* newly allocated item */

     if (lst==NULL)
       return;
     pit = Alloc(iHeaderSize+uLen);
     if (pit==NULL) { lst->bOK = FALSE; return; }
     pit->pBlock = pCurrent;
     List_Leave_Crit(&CritSec);
     pit->iLen = uLen;
     pit->pitPrev = lst;
     pit->pitNext = lst->pitNext;
     lst->pitNext->pitPrev = pit; /* for empty list that set lst->pitPrev */
     lst->pitNext = pit;
     pit->bAnchor = FALSE;
     memcpy( &(pit->Data), pObject, uLen );
  } /* List_AddFirst */

  /*------------------------------------------------------------------
  | Return the address of the place for Len bytes of data in a new
  | item at the start of lst
   -------------------------------------------------------------------*/
  LPVOID APIENTRY List_NewFirst(LIST lst, UINT uLen)
  {  LIST pit;

     if (lst==NULL)
       return NULL;
     pit = Alloc(iHeaderSize+uLen);
     if (pit==NULL) { lst->bOK = FALSE; return NULL; }
     pit->pBlock = pCurrent;
     List_Leave_Crit(&CritSec);
     pit->iLen = uLen;
     pit->pitPrev = lst;
     pit->pitNext = lst->pitNext;
     lst->pitNext->pitPrev = pit; /* for empty list that set lst->pitPrev */
     lst->pitNext = pit;
     pit->bAnchor = FALSE;
     return (char FAR *)&(pit->Data);
  } /* List_NewFirst */

  /*------------------------------------------------------------------
  | Delete the first item in lst.  Error if lst is empty
   -------------------------------------------------------------------*/
  void APIENTRY List_DeleteFirst(LIST lst)
  {  LIST pit;

     if (lst==NULL)
       return;
                               /* attempting to delete the anchor block! */
     if (lst->pitNext==lst) {lst->bOK = FALSE; }
     else
        {  pit = lst->pitNext;
           pit->pitNext->pitPrev = pit->pitPrev;
           pit->pitPrev->pitNext = pit->pitNext;
           Free(pit->pBlock, pit);
        }
  } /* List_DeleteFirst */

  /*------------------------------------------------------------------
  | Add an item holding Object to the end of lst
   -------------------------------------------------------------------*/
  void APIENTRY List_AddLast(LIST lst, LPVOID pObject, UINT uLen)
  {  LIST pit;

     if (lst==NULL)
       return;
     pit = Alloc(iHeaderSize+uLen);
     if (pit==NULL) { lst->bOK = FALSE; return; }
     pit->pBlock = pCurrent;
     List_Leave_Crit(&CritSec);
     pit->iLen = uLen;
     pit->pitNext = lst;
     pit->pitPrev = lst->pitPrev;
     lst->pitPrev->pitNext = pit; /* for empty list that set lst->pitNext */
     lst->pitPrev = pit;
     pit->bAnchor = FALSE;
     memcpy( &(pit->Data), pObject, uLen );
  } /* ListAddLast */

  /*------------------------------------------------------------------
  | Return the address of the place for uLen bytes of data in a new
  |  item at the end of lst
   -------------------------------------------------------------------*/
  LPVOID APIENTRY List_NewLast(LIST lst, UINT uLen)
  {  LIST pit;

     if (lst==NULL)
       return NULL;
     pit = Alloc(iHeaderSize+uLen);
     if (pit==NULL) { lst->bOK = FALSE; return NULL; }
     pit->pBlock = pCurrent;
     List_Leave_Crit(&CritSec);
     pit->iLen = uLen;
     pit->pitNext = lst;
     pit->pitPrev = lst->pitPrev;
     lst->pitPrev->pitNext = pit; /* for empty list that set lst->pitNext */
     lst->pitPrev = pit;
     pit->bAnchor = FALSE;
     return (char FAR *)&(pit->Data);
  } /* ListNewLast */

  /*------------------------------------------------------------------
  | Delete the last item in lst.  Error if lst is empty
   -------------------------------------------------------------------*/
  void APIENTRY List_DeleteLast(LIST lst)
  {  LIST pit;

     if (lst==NULL)
       return;
                               /* attempting to delete the anchor block! */
     if (lst->pitNext==lst) {lst->bOK = FALSE; }
     else
        {  pit = lst->pitPrev;
           pit->pitNext->pitPrev = pit->pitPrev;
           pit->pitPrev->pitNext = pit->pitNext;
           Free(pit->pBlock, pit);
        }
  } /* List_DeleteLast */

  /*--------------------------------------------------------------------
  | Add an item holding * pObject to lst immediately after Curs.
  | List_AddAfter(lst,NULL,pObject,Len) adds it to the start of the lst
   ---------------------------------------------------------------------*/
  void APIENTRY List_AddAfter( LIST lst
                    , LPVOID Curs
                    , LPVOID pObject
                    , UINT uLen
                    )
  {  LIST pitNew;
     LIST pitAfter;

     if (lst==NULL)
       return;
     if (Curs==NULL){ List_AddFirst(lst, pObject, uLen);}
     else
        {  MOVEBACK(Curs);
           pitAfter = (LIST)Curs;
           pitNew = Alloc(iHeaderSize+uLen);
           if (pitNew==NULL) { lst->bOK = FALSE; return; }
           pitNew->pBlock = pCurrent;
           List_Leave_Crit(&CritSec);
           pitNew->iLen = uLen;
           pitNew->pitPrev = pitAfter;
           pitNew->pitNext = pitAfter->pitNext;
           pitAfter->pitNext->pitPrev = pitNew;
           pitAfter->pitNext = pitNew;
           pitNew->bAnchor = FALSE;
           memcpy( &(pitNew->Data), pObject, uLen );
        }
  } /* List_AddAfter */

  /*--------------------------------------------------------------------
  | Return the address of the place for uLen bytes of data in a new
  | item immediately after Curs.
  | List_NewAfter(Lst,NULL,uLen) returns a pointer
  | to space for uLen bytes in a new first element.
   ---------------------------------------------------------------------*/
  LPVOID APIENTRY List_NewAfter(LIST lst, LPVOID Curs, UINT uLen)
  {  LIST pitNew;
     LIST pitAfter;

     if (lst==NULL)
       return NULL;
     if (Curs==NULL){ return List_NewFirst(lst, uLen);}
     else
        {  MOVEBACK(Curs);
           pitAfter = (LIST)Curs;
           pitNew = Alloc(iHeaderSize+uLen);
           if (pitNew==NULL) { lst->bOK = FALSE; return NULL; }
           pitNew->pBlock = pCurrent;
           List_Leave_Crit(&CritSec);
           pitNew->iLen = uLen;
           pitNew->pitPrev = pitAfter;
           pitNew->pitNext = pitAfter->pitNext;
           pitAfter->pitNext->pitPrev = pitNew;
           pitAfter->pitNext = pitNew;
           pitNew->bAnchor = FALSE;
           return (char FAR *)&(pitNew->Data);
        }
  } /* List_NewAfter */

  /*--------------------------------------------------------------------
  | Add an item holding Object to lst immediately before Curs.
  | List_AddBefore(Lst,NULL,Object,uLen) adds it to the end of the list
   ---------------------------------------------------------------------*/
  void APIENTRY List_AddBefore( LIST lst
                     , LPVOID Curs
                     , LPVOID pObject
                     , UINT uLen
                     )
  {  LIST pitNew;
     LIST pitBefore;

     if (lst==NULL)
       return;
     if (Curs==NULL){ List_AddLast(lst, pObject, uLen);}
     else
        {  MOVEBACK(Curs);
           pitBefore = (LIST)Curs;
           pitNew = Alloc(iHeaderSize+uLen);
           if (pitNew==NULL) { lst->bOK = FALSE; return; }
           pitNew->pBlock = pCurrent;
           List_Leave_Crit(&CritSec);
           pitNew->iLen = uLen;
           pitNew->pitNext = pitBefore;
           pitNew->pitPrev = pitBefore->pitPrev;
           pitBefore->pitPrev->pitNext = pitNew;
           pitBefore->pitPrev = pitNew;
           pitNew->bAnchor = FALSE;
           memcpy( &(pitNew->Data), pObject, uLen );
        }
  } /* List_AddBefore */

  /*--------------------------------------------------------------------
  | Return the address of the place for uLen bytes of data in a new
  | item immediately before Curs.
  | List_NewBefore(Lst,NULL,uLen) returns a pointer
  | to space for uLen bytes in a new last element.
   ---------------------------------------------------------------------*/
  LPVOID APIENTRY List_NewBefore(LIST lst, LPVOID Curs, UINT uLen )
  {  LIST pitNew;
     LIST pitBefore;

     if (lst==NULL)
       return NULL;
     if (Curs==NULL){ return List_NewLast(lst, uLen);}
     else
        {  MOVEBACK(Curs);
           pitBefore = (LIST)Curs;
           pitNew = Alloc(iHeaderSize+uLen);
           if (pitNew==NULL) { lst->bOK = FALSE; return NULL; }
           pitNew->pBlock = pCurrent;
           List_Leave_Crit(&CritSec);
           pitNew->iLen = uLen;
           pitNew->pitNext = pitBefore;
           pitNew->pitPrev = pitBefore->pitPrev;
           pitBefore->pitPrev->pitNext = pitNew;
           pitBefore->pitPrev = pitNew;
           pitNew->bAnchor = FALSE;
           return (char FAR *) &(pitNew->Data);
        }
  } /* List_NewBefore */

  /*------------------------------------------------------------------
  | Delete the item that Curs identifies.
  | This will be only a few (maybe as little as 3) machine instructions
  | quicker than DeleteForwards or DeleteBackwards but leaves Curs dangling.
  | It is therefore NOT usually to be preferred.
  | It may be useful when you have a function which returns an LPVOID
  | since the argument does not need to be a variable.
  |     Trivial example: List_Delete(List_First(L));
   -------------------------------------------------------------------*/
  void APIENTRY List_Delete(LPVOID Curs)
  {  LIST pit;

     if(Curs==NULL)
       return;
     MOVEBACK(Curs)
     pit = (LIST)Curs;
     pit->pitNext->pitPrev = pit->pitPrev;
     pit->pitPrev->pitNext = pit->pitNext;
     Free(pit->pBlock, pit);
  } /* List_Delete */

  /*-----------------------------------------------------------------------
  | Delete the item that Curs identifies and return a cursor that
  | identifies the next item (NULL if already on last)
   ------------------------------------------------------------------------*/
  LPVOID APIENTRY List_DeleteForwards(LPVOID Curs)
  {  LIST pitDel;  /* the item to delete */
     LIST pitN;    /* the item after (could be anchor) */

     if(Curs==NULL)
       return NULL;
     MOVEBACK(Curs)
     pitDel = (LIST)Curs;
     pitN = pitDel->pitNext;

     pitN->pitPrev = pitDel->pitPrev;
     pitDel->pitPrev->pitNext = pitN;
     Free(pitDel->pBlock, pitDel);
     if (pitN->bAnchor) return NULL;
     else return (char FAR *)&(pitN->Data);
  } /* List_DeleteForwards */

  /*-----------------------------------------------------------------------
  | Delete the item that Curs identifies and return a cursor that
  | identifies the previous item (NULL if already on first)
   ------------------------------------------------------------------------*/
  LPVOID APIENTRY List_DeleteBackwards(LPVOID Curs)
  {  LIST pitDel;  /* the one to delete */
     LIST pitB;    /* the one before */

     if(Curs==NULL)
       return NULL;
     MOVEBACK(Curs)
     pitDel = (LIST)Curs;
     pitB = pitDel->pitPrev;
     pitDel->pitNext->pitPrev = pitB;
     pitB->pitNext = pitDel->pitNext;
     Free(pitDel->pBlock, pitDel);
     if (pitB->bAnchor) return NULL;
     else return (char FAR *)&(pitB->Data);
  } /* List_DeleteBackwards */

  /*-------------------------------------------------------------------
  | Return the length of the object identified by the cursor Curs
   -------------------------------------------------------------------*/
  int APIENTRY List_ItemLength(LPVOID Curs)
  {  LIST pit;

     if(Curs==NULL)
       return 0;
     MOVEBACK(Curs)
     pit = (LIST)Curs;
     return pit->iLen;
  } /* List_ItemLength */

  /*------------------------------------------------------------------
  | Return the address of the first object in lst
  |  If lst is empty then Return NULL.
   -------------------------------------------------------------------*/
  LPVOID APIENTRY List_First(LIST lst)
  {  
     if (lst==NULL)
       return NULL;
     if (lst->pitNext==lst) { return NULL; }
     return &(lst->pitNext->Data);
  } /* List_First */

  /*------------------------------------------------------------------
  | Return the address of the last object in lst
  | If lst is empty then return NULL.
   -------------------------------------------------------------------*/
  LPVOID APIENTRY List_Last(LIST lst)
  {  
     if (lst==NULL)
       return NULL;
     if (lst->pitNext==lst) { return NULL; }
     return &(lst->pitPrev->Data);
  } /* List_Last */

  /*------------------------------------------------------------------
  | Return the address of the object after Curs^.
  | List_Next(List_Last(lst)) == NULL;  List_Next(NULL) is an error.
   -------------------------------------------------------------------*/
  LPVOID APIENTRY List_Next(LPVOID Curs)
  {  LIST pit;

     if(Curs==NULL)
       return NULL;
     MOVEBACK(Curs)
     pit = (LIST)Curs;
     pit = pit->pitNext;
     if (pit->bAnchor) {return NULL;} else {return &(pit->Data);}
  } /* List_Next */

  /*------------------------------------------------------------------
  | Return the address of the object after Curs^.
  | List_Prev(List_First(L)) == NULL;  List_Prev(NULL) is an error.
   -------------------------------------------------------------------*/
  LPVOID APIENTRY List_Prev(LPVOID Curs)
  {  LIST pit;

     if(Curs==NULL)
       return NULL;
     MOVEBACK(Curs)
     pit = (LIST)Curs;
     pit = pit->pitPrev;
     if (pit->bAnchor) {return NULL;} else {return &(pit->Data);}
  } /* List_Prev */

  /*-------------------------------------------------------------------
  | Arrange that lst is empty after this call
   --------------------------------------------------------------------*/
  void APIENTRY List_Clear(LIST lst)
  {  LIST pitP;   /* item cursor on List, points to element starts */
     LIST pitQ;   /* runs one step ahead of pitP                   */

     if (lst==NULL)
       return;
     pitP = lst->pitNext;   /* first element of list proper */
     while (pitP!=lst)      /* while not wrapped onto anchor */
        {  pitQ = pitP->pitNext;
           Free(pitP->pBlock, pitP);
           pitP = pitQ;
        }
     lst->bOK = TRUE;
     lst->pitNext = lst;
     lst->pitPrev = lst;
  } /* List Clear */

  /*---------------------------------------------------------------------
  | Return TRUE if and only if lst is empty
   ----------------------------------------------------------------------*/
  BOOL APIENTRY List_IsEmpty(LIST lst)
  {  if (lst==NULL)
       return TRUE;   /* well it's sort of true isn't it? */
     return lst->pitNext ==lst;
  } /* List_IsEmpty */

  /*------------------------------------------------------------------
  | l1 had better be empty.  l1 then acquires all the elements from l2
   -------------------------------------------------------------------*/
  void APIENTRY SwitchLists(LIST l1, LIST l2)
  {  /* connect l1 to l2's elements, l1 had better be initially empty */
     l1->pitPrev = l2->pitPrev;
     l1->pitNext = l2->pitNext;
     /* connect the elements to l1 anchor block. */
     l1->pitPrev->pitNext = l1;
     l1->pitNext->pitPrev = l1;
     /* make l2 empty */
     l2->pitPrev = l2;
     l2->pitNext = l2;
  } /* SwitchLists */

  /*-----------------------------------------------------------------------
  | l1 := l1||l2; l2 := empty
  | The elements themselves are not moved, so pointers to them remain valid.
  |
  | l1 gets all the elements of l1 in their original order followed by
  | all the elements of l2 in the order they were in in l2.
  | l2 becomes empty.
   ------------------------------------------------------------------------*/
  void APIENTRY List_Join(LIST l1, LIST l2)
  {  if((l1==NULL)||(l2==NULL))
       return;
     l1->bOK = l1->bOK &&l2->bOK;  /* result OK if both inputs OK */
     l2->bOK = TRUE;               /* as l2 always becomes empty */
     if (l2->pitNext==l2) { /* no elements need moving */ }
     else if (l2->pitNext==l2) { SwitchLists(l1,l2); return; }
     else
        {  l2->pitNext->pitPrev = l1->pitPrev;
           l1->pitPrev->pitNext = l2->pitNext;
           l1->pitPrev = l2->pitPrev;
           l1->pitPrev->pitNext = l1;
           l2->pitNext = l2;
           l2->pitPrev = l2;
        }
  } /* List_Join */

  /*-----------------------------------------------------------------------
  | Let L1 be *pl1 and L2 be *pl2
  | L1 := L1[...Curs] || L2 || L1[Curs+1...]; L2 := empty
  | Curs=NULL means insert L2 at the start of L1
  | The elements themselves are not moved, so pointers to them remain valid.
  |
  | L1 gets the elements of L1 from the start up to and including the element
  | that Curs points at, in their original order,
  | followed by all the elements that were in L2, in their original order,
  | followed by the rest of L1
   ------------------------------------------------------------------------*/
  void APIENTRY List_InsertListAfter(LIST l1, LIST l2, LPVOID Curs)
  {  LIST pitA;     /* The element after Curs, could be anchor */
     LIST pit;      /* The start of the element that Curs points at
                    |  or the anchor block if Curs==NULL
                    */

     if ( (l1==NULL) || (l2==NULL))
       return;
     l1->bOK = l1->bOK && l2->bOK;
     l2->bOK = TRUE;
     if (l2->pitNext==l2) { /* no elements need moving */ }
     else if ( l1->pitNext==l1)
     {  /* the easy way to code this would be simply to switch the two
        |  pointers l1 and l2, but they are value parameters and we don't
        |  want to change that.
        */
        SwitchLists(l1,l2);
        return;
     }
     else
     {  if(Curs==NULL){ pit = l1;}
        else
        {  MOVEBACK(Curs)
           pit = (LIST)Curs;
        }
        /* pit points to a block to insert after, could be anchor */
        pitA = pit->pitNext;      /* Cannot be same as P, already checked */
        l2->pitNext->pitPrev = pit;    /*  P<-- elems-of-l2    A */
        l2->pitPrev->pitNext = pitA;   /*  P<-- elems-of-l2 -->A */
        pit->pitNext = l2->pitNext;    /*  P<-->elems-of-l2 -->A */
        pitA->pitPrev = l2->pitPrev;   /*  P<-->elems-of-l2<-->A */

        l2->pitNext = l2;
        l2->pitPrev = l2;
     }
  }  /* List_InsertListAfter */


  /*-----------------------------------------------------------------------
  | l1 := l1[...Curs-1] || l2 || l1[Curs...]; l2 := empty
  | Curs=NULL means insert l2 at the end of l1
  | The elements themselves are not moved, so pointers to them remain valid.
  |
  | l1 gets the elements of l1 from the start up to but not including the
  | element that Curs points at, in their original order,
  | followed by all the elements that were in l2, in their original order,
  | followed by the rest of l1.
   ------------------------------------------------------------------------*/
  void APIENTRY List_InsertListBefore(LIST l1, LIST l2, LPVOID Curs)
  {  LIST pitB;     /* The element before Curs, could be anchor */
     LIST pit;      /* The start of the element that Curs points at
                    |  or the anchor block if Curs==NULL
                    */

     if ((l1==NULL) || (l2==NULL))
       return;
     l1->bOK = l1->bOK && l2->bOK;
     l2 ->bOK = TRUE;
     if (l2->pitNext==l2) { /* no action needed */ }
     else if (l1->pitNext==l1)
     {  /* the easy way to code this would be simply to switch the two
        |  pointers l1 and l2, but they are value parameters and we don't
        |  want to change that.
        */
        SwitchLists(l1,l2);
        return;
     }
     else
     {  if(Curs==NULL) { pit = l1; }
        else
        {  MOVEBACK(Curs)
           pit = (LIST)Curs;
        }

        /* P points to a block to insert before, could be anchor */
        pitB = pit->pitPrev;       /* Cannot be same as P, already checked */
        l2->pitNext->pitPrev = pitB; /*  B<-- elems-of-L2    P */
        l2->pitPrev->pitNext = pit;  /*  B<-- elems-of-L2 -->P */
        pitB->pitNext = l2->pitNext; /*  B<-->elems-of-L2 -->P */
        pit->pitPrev = l2->pitPrev;  /*  B<-->elems-of-L2<-->P */
        l2->pitNext = l2;
        l2->pitPrev = l2;
     }
  } /* List_InsertListBefore */


  /*-----------------------------------------------------------------------
  | Let l1 be l1 and l2 be l2
  | Split l2 off from the front of l1:    final l2,l1 = original l1
  |
  | Split l1 into l2: objects of l1 up to and including Curs object
  |               l1: objects of l1 after Curs
  | Any original contents of l2 are freed.
  | List_Spilt(l1, l2, NULL) splits l1 before the first object so l1 gets all.
  | The elements themselves are not moved.
   ------------------------------------------------------------------------*/
  void APIENTRY List_SplitAfter(LIST l1, LIST l2, LPVOID Curs)
  {  LIST pit;

     if ((l1==NULL) || (l2==NULL))
       return;
     if (l2->pitNext!=l2){ List_Clear(l2); };
     if (Curs!=NULL)
     {  MOVEBACK(Curs)
        pit = (LIST)Curs;
        /* Curs had better be an item in l1! l2 had better be created! */
        if (pit==l1) { l1->bOK = FALSE; l2->bOK = FALSE; return; }
        if (pit->pitNext==l1)
        {  /* transfer whole of l2 to l1 */
           SwitchLists(l2,l1);
           return;
        }
        l2->pitPrev = pit;
        l2->pitNext = l1->pitNext;
        l1->pitNext = pit->pitNext;
        pit->pitNext = l2;
        l2->pitNext->pitPrev = l2;
        l1->pitNext->pitPrev = l1;
     }
  } /* List_SplitAfter */

  /*----------------------------------------------------------------------
  | Split l2 off from the back of l1:  final l1,l2 = original l1
  |
  | Split l1 into l1: objects of l1 up to but not including Curs object
  |               l2: objects of l1 from Curs onwards
  | Any original contants of l2 are freed.
  | List_Spilt(l1, l2, NULL) splits l1 after the last object so l1 gets all.
  | The elements themselves are not moved.
   -----------------------------------------------------------------------*/
  void APIENTRY List_SplitBefore(LIST l1, LIST l2, LPVOID Curs)
  {  LIST pit;

     if ((l1==NULL) || (l2==NULL))
       return;
     if (l2->pitNext!=l2){ List_Clear(l2); }
     if (Curs!=NULL)
     {  MOVEBACK(Curs)
        pit = (LIST)Curs;
        /* Curs had better be an item in L1! L2 had better be created! */
        if (pit==l1){ l1->bOK = FALSE; l2->bOK = FALSE; return; }
        if (pit->pitPrev==l1) { SwitchLists(l2,l1); return; }
        l2->pitNext = pit;
        l2->pitPrev = l1->pitPrev;
        l1->pitPrev = pit->pitPrev;
        pit->pitPrev = l2;
        l2->pitPrev->pitNext = l2;
        l1->pitPrev->pitNext = l1;
     }
  } /* List_SplitBefore */

  /*------------------------------------------------------------------
  | Return the number of items in L
   -------------------------------------------------------------------*/
  int APIENTRY List_Card(LIST lst)
  {  LIST pit;     /* item cursor on lst */
     int cit;

     if (lst==NULL)
       return 0;    /* well it is sort of 0 */
     pit = lst->pitNext;
     cit = 0;
     while(pit!=lst)
        {  cit++;
           pit = pit->pitNext;
        }
     return cit;
  } /* List_Card */

  /*------------------------------------------------------------------
  | Check return code
   -------------------------------------------------------------------*/
  BOOL APIENTRY List_IsOK(LIST lst)
  {  if(lst==NULL)
       return FALSE;       /* well it is sick ain't it! */
     return lst->bOK;
  } /* List_IsOK */

  /*------------------------------------------------------------------
  | Set return code to good
   -------------------------------------------------------------------*/
  void APIENTRY List_MakeOK(LIST lst)
  {  if(lst==NULL)
       return;
     lst->bOK = TRUE;
  } /* List_MakeOK */

  BOOL APIENTRY List_Check(LIST lst)
  { LIST pel;
    BOOL bOK;
    /*-----------------------------------------------------------------
    | Check the anchor block has the Anchor flag set.
    | Run through the LIST using the Anchor flag (which should be FALSE)
    | to mark where we have been (to test for loops in the chain)
    | and carry on until we see the Anchor flag again.  Check that this
    | is the anchor block that we started from.  Now do another pass
    | turning the Anchor flags off again and checking the Prev pointers.
     -------------------------------------------------------------------*/
    if(lst==NULL) return FALSE;  /* Should we trap?  Arguable */
    bOK = lst->bAnchor;
    pel = lst->pitNext;
    while(! pel->bAnchor)
    { pel->bAnchor = TRUE;
      pel = pel->pitNext;
    }
    bOK = bOK && (pel==lst);
    if(bOK)
    { /* Turn all the bAnchor flags off */
      pel = lst;
      do
      { pel->bAnchor = FALSE;
        bOK = bOK & (pel->pitNext->pitPrev==pel);
        pel = pel->pitNext;
      } while (pel!=lst);
      lst->bAnchor = TRUE;  /* except the real one */
    }
    else
    { /* just turn off those that we set on */
      pel = lst->pitNext;
      while (pel->bAnchor)
      { pel->bAnchor = FALSE;
        pel = pel->pitNext;
      }
      lst->bAnchor = TRUE;
    }
    return bOK;
  } /* List_Check */


  void APIENTRY List_Recover(PLIST plst)
  {  LIST Last, P,Q;
     BOOL OK;
    /* For no particular reason we presume that the forward chain
       is good and reconstruct the back chain from it.  A better
       algorithm would do the kind of things that List_Check does
       to figure out where the problems lie.  This just steps along
       until it sees either an address that it has already seen or
       else the anchor block.  (It's an n-squared algorithm).
       It links the last good block found back to the anchor and
       fixes all the Anchor flags.
    */
    if (plst==NULL) return;
    if (*plst==NULL)
    {  *plst = List_Create();
       return;
    }
    (*plst)->bAnchor = TRUE;
    P = (*plst)->pitNext;
    Last = *plst;
    for (; ; )
    {  if (P==*plst) break;
       Last = P;
       if (P->pitNext!=*plst)
       {   OK = TRUE;
           Q = *plst;
           for (; ; )
           {   OK &= (P->pitNext!=Q);
               if (Q==P) break;
               Q = Q->pitNext;
           }
           if (!OK) break;
       }
       P = P->pitNext;
    }
    P = *plst;
    while (P!=Last)
    {  P->pitNext->pitPrev = P;
       P->bAnchor = FALSE;
       P = P->pitNext;
    }
    Last->pitNext = *plst;
    (*plst)->pitPrev = Last;
    (*plst)->bAnchor = TRUE;
    (*plst)->bOK = TRUE;   /* Here's hoping! */
  } /* List_Recover */

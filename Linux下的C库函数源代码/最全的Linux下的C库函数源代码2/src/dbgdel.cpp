/***
*dbgnew.cpp - defines C++ scalar delete routine, debug version
*
*       Copyright (c) Microsoft Corporation.  All rights reserved.
*
*Purpose:
*       Defines C++ scalar delete() routine.
*
*******************************************************************************/

#ifdef _DEBUG

#include <cruntime.h>
#include <malloc.h>
#include <mtdll.h>
#include <dbgint.h>
#include <rtcsup.h>

/***
*void operator delete() - delete a block in the debug heap
*
*Purpose:
*       Deletes any type of block.
*
*Entry:
*       void *pUserData - pointer to a (user portion) of memory block in the
*                         debug heap
*
*Return:
*       <void>
*
*******************************************************************************/

void operator delete(
        void *pUserData
        )
{
        _CrtMemBlockHeader * pHead;

        RTCCALLBACK(_RTC_Free_hook, (pUserData, 0));

        if (pUserData == NULL)
            return;

        _mlock(_HEAP_LOCK);  /* block other threads */
        __TRY

            /* get a pointer to memory block header */
            pHead = pHdr(pUserData);

             /* verify block type */
            _ASSERTE(_BLOCK_TYPE_IS_VALID(pHead->nBlockUse));

            _free_dbg( pUserData, pHead->nBlockUse );

        __FINALLY
            _munlock(_HEAP_LOCK);  /* release other threads */
        __END_TRY_FINALLY

        return;
}

#endif  /* _DEBUG */

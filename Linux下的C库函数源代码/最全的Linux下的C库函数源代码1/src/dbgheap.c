/***
*dbgheap.c - Debug CRT Heap Functions
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines debug versions of heap functions.
*
*******************************************************************************/

#ifdef _DEBUG

#include <windows.h>
#include <winheap.h>
#include <ctype.h>
#include <dbgint.h>
#include <crtdbg.h>
#include <rtcsup.h>
#include <internal.h>
#include <limits.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <mtdll.h>
#include <setlocal.h>

#pragma warning(disable:4390)
extern "C" void * __cdecl _heap_alloc_base (size_t size);

/*---------------------------------------------------------------------------
 *
 * Heap management
 *
 --------------------------------------------------------------------------*/

#define IGNORE_REQ  0L              /* Request number for ignore block */
#define IGNORE_LINE 0xFEDCBABC      /* Line number for ignore block */

/*
 * Bitfield flag that controls CRT heap behavior --
 * default is to record all allocations (_CRTDBG_ALLOC_MEM_DF)
 * AND check heap consistency on every alloc/dealloc (_CRTDBG_CHECK_ALWAYS_DF)
 */

extern "C" int _crtDbgFlag = _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_DEFAULT_DF;

/*
 * Size threshold for filling behavior. See _CrtSetDebugFillThreshold.
 */

extern "C" size_t __crtDebugFillThreshold = SIZE_MAX;

extern "C" int __crtDebugCheckCount = FALSE;

/*
 * struct used by _aligned routines as block header.
 */

#define nAlignGapSize sizeof(void *)

typedef struct _AlignMemBlockHdr
{
    void *pHead;
    unsigned char Gap[nAlignGapSize];
} _AlignMemBlockHdr;

#define IS_2_POW_N(x)   (((x)&(x-1)) == 0)

/*
 * Statics governing how often _CrtCheckMemory is called.
 */
static unsigned check_frequency = _CRTDBG_CHECK_DEFAULT_DF >> 16;
static unsigned check_counter;

static long _lRequestCurr = 1;      /* Current request number */

extern "C" _CRTIMP long _crtBreakAlloc = -1L;  /* Break on allocation by request number */

static size_t _lTotalAlloc;         /* Grand total - sum of all allocations */
static size_t _lCurAlloc;           /* Total amount currently allocated */
static size_t _lMaxAlloc;           /* Largest ever allocated at once */

/*
 * The following values are non-zero, constant, odd, large, and atypical
 *      Non-zero values help find bugs assuming zero filled data.
 *      Constant values are good so that memory filling is deterministic
 *          (to help make bugs reproducable).  Of course it is bad if
 *          the constant filling of weird values masks a bug.
 *      Mathematically odd numbers are good for finding bugs assuming a cleared
 *          lower bit.
 *      Large numbers (byte values at least) are less typical, and are good
 *          at finding bad addresses.
 *      Atypical values (i.e. not too often) are good since they typically
 *          cause early detection in code.
 *      For the case of no-man's land and free blocks, if you store to any
 *          of these locations, the memory integrity checker will detect it.
 *
 *      _bAlignLandFill has been changed from 0xBD to 0xED, to ensure that
 *      4 bytes of that (0xEDEDEDED) would give an inaccessible address under 3gb.
 */

static unsigned char _bNoMansLandFill = 0xFD;   /* fill no-man's land with this */
static unsigned char _bAlignLandFill  = 0xED;   /* fill no-man's land for aligned routines */
static unsigned char _bDeadLandFill   = 0xDD;   /* fill free objects with this */
static unsigned char _bCleanLandFill  = 0xCD;   /* fill new objects with this */

static _CrtMemBlockHeader * _pFirstBlock;
static _CrtMemBlockHeader * _pLastBlock;

_CRT_DUMP_CLIENT _pfnDumpClient;

#if _FREE_BLOCK != 0 || _NORMAL_BLOCK != 1 || _CRT_BLOCK != 2 || _IGNORE_BLOCK != 3 || _CLIENT_BLOCK != 4
#error Block numbers have changed !
#endif  /* _FREE_BLOCK != 0 || _NORMAL_BLOCK != 1 || _CRT_BLOCK != 2 || _IGNORE_BLOCK != 3 || _CLIENT_BLOCK != 4 */

static char * szBlockUseName[_MAX_BLOCKS] = {
        "Free",
        "Normal",
        "CRT",
        "Ignore",
        "Client",
        };

extern "C" int __cdecl CheckBytes(unsigned char *, unsigned char, size_t);

/***
*void *malloc() - Get a block of memory from the debug heap
*
*Purpose:
*       Allocate of block of memory of at least size bytes from the heap and
*       return a pointer to it.
*
*       Allocates 'normal' memory block.
*
*Entry:
*       size_t          nSize       - size of block requested
*
*Exit:
*       Success:  Pointer to memory block
*       Failure:  NULL (or some error value)
*
*Exceptions:
*
*******************************************************************************/

extern "C" _CRTIMP void * __cdecl malloc (
        size_t nSize
        )
{
        void *res = _nh_malloc_dbg(nSize, _newmode, _NORMAL_BLOCK, NULL, 0);

        RTCCALLBACK(_RTC_Allocate_hook, (res, nSize, 0));

        return res;
}

/***
*void * _malloc_dbg() - Get a block of memory from the debug heap
*
*Purpose:
*       Allocate of block of memory of at least size bytes from the heap and
*       return a pointer to it.
*
*       Allocates any type of supported memory block.
*
*Entry:
*       size_t          nSize       - size of block requested
*       int             nBlockUse   - block type
*       char *          szFileName  - file name
*       int             nLine       - line number
*
*Exit:
*       Success:  Pointer to memory block
*       Failure:  NULL (or some error value)
*
*Exceptions:
*
*******************************************************************************/

extern "C" _CRTIMP void * __cdecl _malloc_dbg (
        size_t nSize,
        int nBlockUse,
        const char * szFileName,
        int nLine
        )
{
        void *res = _nh_malloc_dbg(nSize, _newmode, nBlockUse, szFileName, nLine);
        RTCCALLBACK(_RTC_Allocate_hook, (res, nSize, 0));
        return res;
}

/***
*void * _nh_malloc() - Get a block of memory from the debug heap
*
*Purpose:
*       Allocate of block of memory of at least size bytes from the debug
*       heap and return a pointer to it. Assumes heap already locked.
*
*       If no blocks available, call new handler.
*
*       Allocates 'normal' memory block.
*
*Entry:
*       size_t          nSize       - size of block requested
*       int             nhFlag      - TRUE if new handler function
*
*Exit:
*       Success:  Pointer to (user portion of) memory block
*       Failure:  NULL
*
*Exceptions:
*
*******************************************************************************/

extern "C" void * __cdecl _nh_malloc (
        size_t nSize,
        int nhFlag
        )
{
        return _nh_malloc_dbg(nSize, nhFlag, _NORMAL_BLOCK, NULL, 0);
}


/***
*void * _nh_malloc_dbg() - Get a block of memory from the debug heap
*
*Purpose:
*       Allocate of block of memory of at least size bytes from the debug
*       heap and return a pointer to it. Assumes heap already locked.
*
*       If no blocks available, call new handler.
*
*       Allocates any type of supported memory block.
*
*Entry:
*       size_t          nSize       - size of block requested
*       int             nhFlag      - TRUE if new handler function
*       int             nBlockUse   - block type
*       char *          szFileName  - file name
*       int             nLine       - line number
*
*Exit:
*       Success:  Pointer to (user portion of) memory block
*       Failure:  NULL
*
*Exceptions:
*
*******************************************************************************/

extern "C" void * __cdecl _nh_malloc_dbg (
        size_t nSize,
        int nhFlag,
        int nBlockUse,
        const char * szFileName,
        int nLine
        )
{
        void * pvBlk;

        for (;;)
        {
            /* do the allocation
             */
            pvBlk = _heap_alloc_dbg(nSize, nBlockUse, szFileName, nLine);

            if (pvBlk)
            {
                return pvBlk;
            }
            if (nhFlag == 0)
            {
                errno = ENOMEM;
                return pvBlk;
            }

            /* call installed new handler */
            if (!_callnewh(nSize))
            {
                errno = ENOMEM;
                return NULL;
            }

            /* new handler was successful -- try to allocate again */
        }
}

/***
*void * _heap_alloc() - does actual allocation
*
*Purpose:
*       Does heap allocation.
*
*       Allocates 'normal' memory block.
*
*Entry:
*       size_t          nSize       - size of block requested
*
*Exit:
*       Success:  Pointer to (user portion of) memory block
*       Failure:  NULL
*
*Exceptions:
*
*******************************************************************************/

extern "C" void * __cdecl _heap_alloc(
        size_t nSize
        )
{
        return _heap_alloc_dbg(nSize, _NORMAL_BLOCK, NULL, 0);
}

/***
*void * _heap_alloc_dbg() - does actual allocation
*
*Purpose:
*       Does heap allocation.
*
*       Allocates any type of supported memory block.
*
*Entry:
*       size_t          nSize       - size of block requested
*       int             nBlockUse   - block type
*       char *          szFileName  - file name
*       int             nLine       - line number
*
*Exit:
*       Success:  Pointer to (user portion of) memory block
*       Failure:  NULL
*
*Exceptions:
*
*******************************************************************************/

extern "C" void * __cdecl _heap_alloc_dbg(
        size_t nSize,
        int nBlockUse,
        const char * szFileName,
        int nLine
        )
{
        long lRequest;
        size_t blockSize;
        int fIgnore = FALSE;
        _CrtMemBlockHeader * pHead;
                void *retval=NULL;

        /* lock the heap
         */
        _mlock(_HEAP_LOCK);
        __try {

                        /* verify heap before allocation */
                        if (check_frequency > 0)
                                if (check_counter == (check_frequency - 1))
                                {
                                        _ASSERTE(_CrtCheckMemory());
                                        check_counter = 0;
                                }
                                else
                                        check_counter++;

                        lRequest = _lRequestCurr;

                        /* break into debugger at specific memory allocation */
                        if (_crtBreakAlloc != -1L && lRequest == _crtBreakAlloc)
                                _CrtDbgBreak();

                        /* forced failure */
                        if ((_pfnAllocHook) && !(*_pfnAllocHook)(_HOOK_ALLOC, NULL, nSize, nBlockUse, lRequest, (const unsigned char *)szFileName, nLine))
                        {
                                if (szFileName)
                                        _RPT2(_CRT_WARN, "Client hook allocation failure at file %hs line %d.\n",
                                                szFileName, nLine);
                                else
                                        _RPT0(_CRT_WARN, "Client hook allocation failure.\n");
                        }
                        else
                        {
                                /* cannot ignore CRT allocations */
                                if (_BLOCK_TYPE(nBlockUse) != _CRT_BLOCK &&
                                        !(_crtDbgFlag & _CRTDBG_ALLOC_MEM_DF))
                                        fIgnore = TRUE;

                                /* Diagnostic memory allocation from this point on */

                                if (nSize > (size_t)(_HEAP_MAXREQ - nNoMansLandSize - sizeof(_CrtMemBlockHeader)))
                                {
                                        _RPT1(_CRT_ERROR, "Invalid allocation size: %Iu bytes.\n", nSize);
                                        errno = ENOMEM;
                                }
                                else
                                {
                                        if (!_BLOCK_TYPE_IS_VALID(nBlockUse))
                                        {
                                                _RPT0(_CRT_ERROR, "Error: memory allocation: bad memory block type.\n");
                                        }

                                        blockSize = sizeof(_CrtMemBlockHeader) + nSize + nNoMansLandSize;

#ifndef WINHEAP
                                        /* round requested size */
                                        blockSize = _ROUND2(blockSize, _GRANULARITY);
#endif  /* WINHEAP */

                                        RTCCALLBACK(_RTC_FuncCheckSet_hook,(0));
                                        pHead = (_CrtMemBlockHeader *)_heap_alloc_base(blockSize);

                                        if (pHead == NULL)
                                        {
                                                errno = ENOMEM;
                                                RTCCALLBACK(_RTC_FuncCheckSet_hook,(1));
                                        }
                                        else
                                        {

                                                /* commit allocation */
                                                ++_lRequestCurr;

                                                if (fIgnore)
                                                {
                                                        pHead->pBlockHeaderNext = NULL;
                                                        pHead->pBlockHeaderPrev = NULL;
                                                        pHead->szFileName = NULL;
                                                        pHead->nLine = IGNORE_LINE;
                                                        pHead->nDataSize = nSize;
                                                        pHead->nBlockUse = _IGNORE_BLOCK;
                                                        pHead->lRequest = IGNORE_REQ;
                                                }
                                                else {
                                                        /* keep track of total amount of memory allocated */
                                                        _lTotalAlloc += nSize;
                                                        _lCurAlloc += nSize;

                                                        if (_lCurAlloc > _lMaxAlloc)
                                                                _lMaxAlloc = _lCurAlloc;

                                                        if (_pFirstBlock)
                                                                _pFirstBlock->pBlockHeaderPrev = pHead;
                                                        else
                                                                _pLastBlock = pHead;

                                                        pHead->pBlockHeaderNext = _pFirstBlock;
                                                        pHead->pBlockHeaderPrev = NULL;
                                                        pHead->szFileName = (char *)szFileName;
                                                        pHead->nLine = nLine;
                                                        pHead->nDataSize = nSize;
                                                        pHead->nBlockUse = nBlockUse;
                                                        pHead->lRequest = lRequest;

                                                        /* link blocks together */
                                                        _pFirstBlock = pHead;
                                                }

                                                /* fill in gap before and after real block */
                                                memset((void *)pHead->gap, _bNoMansLandFill, nNoMansLandSize);
                                                memset((void *)(pbData(pHead) + nSize), _bNoMansLandFill, nNoMansLandSize);

                                                /* fill data with silly value (but non-zero) */
                                                memset((void *)pbData(pHead), _bCleanLandFill, nSize);

                                                RTCCALLBACK(_RTC_FuncCheckSet_hook,(1));

                                                retval=(void *)pbData(pHead);
                                        }
                                }
                        }

        }
        __finally {
            /* unlock the heap
             */
            _munlock(_HEAP_LOCK);
        }
        return retval;
}


/***
*void * calloc() - Get a block of memory from the debug heap, init to 0
*
*Purpose:
*       Allocate of block of memory of at least size bytes from the debug
*       heap and return a pointer to it.
*
*       Allocates 'normal' memory block.
*
*Entry:
*       size_t nNum     - number of elements in the array
*       size_t nSize - size of each element
*
*Exit:
*       Success:  Pointer to (user portion of) memory block
*       Failure:  NULL
*
*Exceptions:
*
*******************************************************************************/
extern "C" _CRTIMP void * __cdecl calloc(
        size_t nNum,
        size_t nSize
        )
{
        void *res = _calloc_dbg(nNum, nSize, _NORMAL_BLOCK, NULL, 0);

        RTCCALLBACK(_RTC_Allocate_hook, (res, nNum * nSize, 0));

        return res;
}


/***
*void * _calloc_dbg() - Get a block of memory from the debug heap, init to 0
*                    - with info
*
*Purpose:
*       Allocate of block of memory of at least size bytes from the debug
*       heap and return a pointer to it.
*
*       Allocates any type of supported memory block.
*
*Entry:
*       size_t          nNum        - number of elements in the array
*       size_t          nSize       - size of each element
*       int             nBlockUse   - block type
*       char *          szFileName  - file name
*       int             nLine       - line number
*
*Exit:
*       Success:  Pointer to (user portion of) memory block
*       Failure:  NULL
*
*Exceptions:
*
*******************************************************************************/

extern "C" _CRTIMP void * __cdecl _calloc_dbg(
        size_t nNum,
        size_t nSize,
        int nBlockUse,
        const char * szFileName,
        int nLine
        )
{
        void * pvBlk;

        /* ensure that (nSize * nNum) does not overflow */
        if (nNum > 0)
        {
            _VALIDATE_RETURN((_HEAP_MAXREQ / nNum) >= nSize, ENOMEM, NULL);
        }

        nSize *= nNum;

        /*
         * try to malloc the requested space
         */

        pvBlk = _malloc_dbg(nSize, nBlockUse, szFileName, nLine);

        /*
         * If malloc() succeeded, initialize the allocated space to zeros.
         * Note that unlike _calloc_base, exactly nNum bytes are set to zero.
         */

        if ( pvBlk != NULL )
        {
            memset(pvBlk, 0, nSize);
        }

        RTCCALLBACK(_RTC_Allocate_hook, (pvBlk, nSize, 0));

        return(pvBlk);
}


/***
*static void * realloc_help() - does all the work for _realloc and _expand
*
*Purpose:
*       Helper function for _realloc and _expand.
*
*Entry:
*       void *          pUserData   - pointer previously allocated block
*       size_t *        pnNewSize   - requested size for the re-allocated block
*                                     changed to actual size during call
*       int             nBlockUse   - block type
*       char *          szFileName  - file name
*       int             nLine       - line number
*       int             fRealloc    - TRUE when _realloc, FALSE when _expand
*
*Exit:
*       Success:  Pointer to (user portion of) memory block
*       Failure:  NULL
*
*Exceptions:
*
*******************************************************************************/

extern "C" static void * __cdecl realloc_help(
        void * pUserData,
        size_t * pnNewSize,
        int nBlockUse,
        const char * szFileName,
        int nLine,
        int fRealloc
        )
{
        long lRequest;
        int fIgnore = FALSE;
        unsigned char *pUserBlock;
        _CrtMemBlockHeader * pOldBlock;
        _CrtMemBlockHeader * pNewBlock;
        size_t nNewSize = *pnNewSize;

        /*
         * ANSI: realloc(NULL, newsize) is equivalent to malloc(newsize)
         */
        if (pUserData == NULL)
        {
            return _malloc_dbg(nNewSize, nBlockUse, szFileName, nLine);
        }

        /*
         * ANSI: realloc(pUserData, 0) is equivalent to free(pUserData)
         * (except that NULL is returned)
         */
        if (fRealloc && nNewSize == 0)
        {
            _free_dbg(pUserData, nBlockUse);
            return NULL;
        }

        /* verify heap before re-allocation */
        if (check_frequency > 0)
            if (check_counter == (check_frequency - 1))
            {
                _ASSERTE(_CrtCheckMemory());
                check_counter = 0;
            }
            else
                check_counter++;

        lRequest = _lRequestCurr;

        if (_crtBreakAlloc != -1L && lRequest == _crtBreakAlloc)
            _CrtDbgBreak(); /* break into debugger at specific memory leak */

        /* forced failure */
        if ((_pfnAllocHook) && !(*_pfnAllocHook)(_HOOK_REALLOC, pUserData, nNewSize, nBlockUse, lRequest, (const unsigned char *)szFileName, nLine))
        {
            if (szFileName)
                _RPT2(_CRT_WARN, "Client hook re-allocation failure at file %hs line %d.\n",
                    szFileName, nLine);
            else
                _RPT0(_CRT_WARN, "Client hook re-allocation failure.\n");

            return NULL;
        }

        /* Diagnostic memory allocation from this point on */

        if (nNewSize > (size_t)(_HEAP_MAXREQ - nNoMansLandSize - sizeof(_CrtMemBlockHeader)))
        {
            _RPT1(_CRT_ERROR, "Invalid allocation size: %Iu bytes.\n", nNewSize);
            errno = ENOMEM;
            return NULL;
        }

        if (nBlockUse != _NORMAL_BLOCK
            && _BLOCK_TYPE(nBlockUse) != _CLIENT_BLOCK
            && _BLOCK_TYPE(nBlockUse) != _CRT_BLOCK)
        {
            _RPT0(_CRT_ERROR, "Error: memory allocation: bad memory block type.\n");
        } else
        {
            if ( CheckBytes((unsigned char*)((uintptr_t)pUserData & ~(sizeof(uintptr_t) -1)) -nAlignGapSize,_bAlignLandFill, nAlignGapSize))
            {
                _RPT1(_CRT_ERROR, "The Block at 0x%p was allocated by aligned routines, use _aligned_realloc()", pUserData);
                errno = EINVAL;
                return NULL;
            }
        }

        /*
         * If this ASSERT fails, a bad pointer has been passed in. It may be
         * totally bogus, or it may have been allocated from another heap.
         * The pointer MUST come from the 'local' heap.
         */
        _ASSERTE(_CrtIsValidHeapPointer(pUserData));

        /* get a pointer to memory block header */
        pOldBlock = pHdr(pUserData);

        if (pOldBlock->nBlockUse == _IGNORE_BLOCK)
            fIgnore = TRUE;

        if (fIgnore)
        {
            _ASSERTE(pOldBlock->nLine == IGNORE_LINE && pOldBlock->lRequest == IGNORE_REQ);
        }
        else {
            /* Error if freeing incorrect memory type */
            /* CRT blocks can be treated as NORMAL blocks */
            if (_BLOCK_TYPE(pOldBlock->nBlockUse) == _CRT_BLOCK && _BLOCK_TYPE(nBlockUse) == _NORMAL_BLOCK)
                nBlockUse = _CRT_BLOCK;
/* The following assertion was prone to false positives - JWM                      */
/*            _ASSERTE(_BLOCK_TYPE(pOldBlock->nBlockUse)==_BLOCK_TYPE(nBlockUse)); */
        }

        /*
         * note that all header values will remain valid
         * and min(nNewSize,nOldSize) bytes of data will also remain valid
         */
        RTCCALLBACK(_RTC_Free_hook, (pUserData, 0));
        RTCCALLBACK(_RTC_FuncCheckSet_hook,(0));

        if (fRealloc)
        {
            if (NULL == (pNewBlock = (_CrtMemBlockHeader *)_realloc_base(pOldBlock,
                sizeof(_CrtMemBlockHeader) + nNewSize + nNoMansLandSize)))
            {
                RTCCALLBACK(_RTC_FuncCheckSet_hook,(1));
                return NULL;
            }
        }
        else {
            if (NULL == (pNewBlock = (_CrtMemBlockHeader *)_expand_base(pOldBlock,
                sizeof(_CrtMemBlockHeader) + nNewSize + nNoMansLandSize)))
            {
                RTCCALLBACK(_RTC_FuncCheckSet_hook,(1));
                return NULL;
            }
#ifdef _WIN64
            /* _WIN64, because of the LFH, doesn't try to resize if the
               block is shrinking. It just returns the original block.
               Make sure our own header tracks that properly. */
            nNewSize = *pnNewSize = (size_t)HeapSize(_crtheap, 0, pNewBlock)
                - sizeof(_CrtMemBlockHeader) - nNoMansLandSize;

#endif  /* _WIN64 */

        }

        /* commit allocation */
        ++_lRequestCurr;

        if (!fIgnore)
        {
            /* keep track of total amount of memory allocated */
            _lTotalAlloc -= pNewBlock->nDataSize;
            _lTotalAlloc += nNewSize;

            _lCurAlloc -= pNewBlock->nDataSize;
            _lCurAlloc += nNewSize;

            if (_lCurAlloc > _lMaxAlloc)
                _lMaxAlloc = _lCurAlloc;
        }

        // Free this thing from RTC - it will be reallocated a bit later (inside realloc_dbg/expand_dbg)
        RTCCALLBACK(_RTC_Free_hook, (pNewBlock, 0));

        pUserBlock = pbData(pNewBlock);

        /* if the block grew, put in special value */
        if (nNewSize > pNewBlock->nDataSize)
            memset(pUserBlock + pNewBlock->nDataSize, _bCleanLandFill,
                nNewSize - pNewBlock->nDataSize);

        /* fill in gap after real block */
        memset(pUserBlock + nNewSize, _bNoMansLandFill, nNoMansLandSize);

        if (!fIgnore)
        {
            pNewBlock->szFileName = (char *)szFileName;
            pNewBlock->nLine = nLine;
            pNewBlock->lRequest = lRequest;
        }

        pNewBlock->nDataSize = nNewSize;

        _ASSERTE(fRealloc || (!fRealloc && pNewBlock == pOldBlock));

        RTCCALLBACK(_RTC_FuncCheckSet_hook,(1));

        /* if block did not move or ignored, we are done */
        if (pNewBlock == pOldBlock || fIgnore)
            return (void *)pUserBlock;

        /* must remove old memory from dbg heap list */
        /* note that new block header pointers still valid */
        if (pNewBlock->pBlockHeaderNext)
        {
            pNewBlock->pBlockHeaderNext->pBlockHeaderPrev
                = pNewBlock->pBlockHeaderPrev;
        }
        else
        {
            _ASSERTE(_pLastBlock == pOldBlock);
            _pLastBlock = pNewBlock->pBlockHeaderPrev;
        }

        if (pNewBlock->pBlockHeaderPrev)
        {
            pNewBlock->pBlockHeaderPrev->pBlockHeaderNext
                = pNewBlock->pBlockHeaderNext;
        }
        else
        {
            _ASSERTE(_pFirstBlock == pOldBlock);
            _pFirstBlock = pNewBlock->pBlockHeaderNext;
        }

        /* put new memory into list */
        if (_pFirstBlock)
            _pFirstBlock->pBlockHeaderPrev = pNewBlock;
        else
            _pLastBlock = pNewBlock;

        pNewBlock->pBlockHeaderNext = _pFirstBlock;
        pNewBlock->pBlockHeaderPrev = NULL;

        /* link blocks together */
        _pFirstBlock = pNewBlock;

        return (void *)pUserBlock;
}


/***
*void * realloc() - reallocate a block of memory in the heap
*
*Purpose:
*       Re-allocates a block in the heap to nNewSize bytes. nNewSize may be
*       either greater or less than the original size of the block. The
*       re-allocation may result in moving the block as well as changing
*       the size. If the block is moved, the contents of the original block
*       are copied over.
*
*       Re-allocates 'normal' memory block.
*
*Entry:
*       void *          pUserData   - pointer to previously allocated block
*       size_t          nNewSize    - requested size for the re-allocated block
*
*Exit:
*       Success:  Pointer to (user portion of) memory block
*       Failure:  NULL
*
*Exceptions:
*
*******************************************************************************/

extern "C" _CRTIMP void * __cdecl realloc(
        void * pUserData,
        size_t nNewSize
        )
{
        void *res = _realloc_dbg(pUserData, nNewSize, _NORMAL_BLOCK, NULL, 0);

        return res;
}

/***
*void * _recalloc() - reallocate items in the heap
*
*Purpose:
*       Re-allocates a block in the heap to nNewSize*count bytes. nNewSize may be
*       either greater or less than the original size of the block. The
*       re-allocation may result in moving the block as well as changing
*       the size. If the block is moved, the contents of the original block
*       are copied over.
*
*       Re-allocates 'normal' memory block.
*
*Entry:
*       void *          pUserData   - pointer to previously allocated block
*       size_t          count       - count of items
*       size_t          nNewSize    - requested size for the items
*
*Exit:
*       Success:  Pointer to (user portion of) memory block
*       Failure:  NULL
*
*Exceptions:
*
*******************************************************************************/

extern "C" _CRTIMP void * __cdecl _recalloc
(
    void * memblock,
    size_t count,
    size_t size
)
{
    size_t  size_orig=0;

    /* ensure that (size * count) does not overflow */
    if (count > 0)
    {
        _VALIDATE_RETURN((_HEAP_MAXREQ / count) >= size, ENOMEM, NULL);
    }
    size_orig = size * count;

    return realloc(memblock, size_orig);
}

/***
*void * _realloc_dbg() - reallocate a block of memory in the heap
*                     - with info
*
*Purpose:
*       Re-allocates a block in the heap to nNewSize bytes. nNewSize may be
*       either greater or less than the original size of the block. The
*       re-allocation may result in moving the block as well as changing
*       the size. If the block is moved, the contents of the original block
*       are copied over.
*
*       Re-allocates any type of supported memory block.
*
*Entry:
*       void *          pUserData   - pointer previously allocated block
*       size_t          nNewSize    - requested size for the re-allocated block
*       int             nBlockUse   - block type
*       char *          szFileName  - file name
*       int             nLine       - line number
*
*Exit:
*       Success:  Pointer to (user portion of) memory block
*       Failure:  NULL
*
*Exceptions:
*
*******************************************************************************/

extern "C" _CRTIMP void * __cdecl _realloc_dbg(
        void * pUserData,
        size_t nNewSize,
        int nBlockUse,
        const char * szFileName,
        int nLine
        )
{
        void * pvBlk;

        _mlock(_HEAP_LOCK);         /* block other threads */
        __try {

        /* allocate the block
         */
        pvBlk = realloc_help(pUserData,
                             &nNewSize,
                             nBlockUse,
                             szFileName,
                             nLine,
                             TRUE);

        }
        __finally {
            _munlock(_HEAP_LOCK);   /* release other threads */
        }
        if (pvBlk)
        {
            RTCCALLBACK(_RTC_Allocate_hook, (pvBlk, nNewSize, 0));
        }
        return pvBlk;
}

/***
*void * _recalloc_dbg() - reallocate items in the heap with info
*
*Purpose:
*       Re-allocates a block in the heap to nNewSize*count bytes. nNewSize may be
*       either greater or less than the original size of the block. The
*       re-allocation may result in moving the block as well as changing
*       the size. If the block is moved, the contents of the original block
*       are copied over.
*
*       Re-allocates any type of supported memory block.
*
*Entry:
*       void *          pUserData   - pointer previously allocated block
*       size_t          count       - count of items
*       size_t          nNewSize    - requested size for the items
*       int             nBlockUse   - block type
*       char *          szFileName  - file name
*       int             nLine       - line number
*
*Exit:
*       Success:  Pointer to (user portion of) memory block
*       Failure:  NULL
*
*Exceptions:
*
*******************************************************************************/

extern "C" _CRTIMP void * __cdecl _recalloc_dbg
(
    void * memblock,
    size_t count,
    size_t size,
    int nBlockUse,
    const char * szFileName,
    int nLine
)
{
    size_t  size_orig=0;

    /* ensure that (size * count) does not overflow */
    if (count > 0)
    {
        _VALIDATE_RETURN((_HEAP_MAXREQ / count) >= size, ENOMEM, NULL);
    }
    size_orig = size * count;

    return _realloc_dbg(memblock, size_orig, nBlockUse, szFileName, nLine);
}


/***
*void * _expand() - expand/contract a block of memory in the heap
*
*Purpose:
*       Resizes a block in the heap to newsize bytes. newsize may be either
*       greater (expansion) or less (contraction) than the original size of
*       the block. The block is NOT moved. In the case of expansion, if the
*       block cannot be expanded to newsize bytes, it is expanded as much as
*       possible.
*
*       Re-allocates 'normal' memory block.
*
*Entry:
*       void * pUserData    - pointer to block in the heap previously allocated
*              by a call to malloc(), realloc() or _expand().
*
*       size_t nNewSize    - requested size for the resized block
*
*Exit:
*       Success:  Pointer to the resized memory block (i.e., pUserData)
*       Failure:  NULL
*
*Uses:
*
*Exceptions:
*       If pUserData does not point to a valid allocation block in the heap,
*       _expand() will behave unpredictably and probably corrupt the heap.
*
*******************************************************************************/

extern "C" _CRTIMP void * __cdecl _expand(
        void * pUserData,
        size_t nNewSize
        )
{
        void *res = _expand_dbg(pUserData, nNewSize, _NORMAL_BLOCK, NULL, 0);

        return res;
}


/***
*void * _expand() - expand/contract a block of memory in the heap
*
*Purpose:
*       Resizes a block in the heap to newsize bytes. newsize may be either
*       greater (expansion) or less (contraction) than the original size of
*       the block. The block is NOT moved. In the case of expansion, if the
*       block cannot be expanded to newsize bytes, it is expanded as much as
*       possible.
*
*       Re-allocates any type of supported memory block.
*
*Entry:
*       void * pUserData   - pointer to block in the heap previously allocated
*              by a call to malloc(), realloc() or _expand().
*
*       size_t nNewSize    - requested size for the resized block
*
*Exit:
*       Success:  Pointer to the resized memory block (i.e., pUserData)
*       Failure:  NULL
*
*Uses:
*
*Exceptions:
*       If pUserData does not point to a valid allocation block in the heap,
*       _expand() will behave unpredictably and probably corrupt the heap.
*
*******************************************************************************/

extern "C" _CRTIMP void * __cdecl _expand_dbg(
        void * pUserData,
        size_t nNewSize,
        int nBlockUse,
        const char * szFileName,
        int nLine
        )
{
        void * pvBlk;

        /* validation section */
        _VALIDATE_RETURN(pUserData != NULL, EINVAL, NULL);
        if (nNewSize > (size_t)(_HEAP_MAXREQ - nNoMansLandSize - sizeof(_CrtMemBlockHeader))) {
            errno = ENOMEM;
            return NULL;
        }

        _mlock(_HEAP_LOCK);         /* block other threads */
        __try {

        /* allocate the block
         */
        pvBlk = realloc_help(pUserData,
                             &nNewSize,
                             nBlockUse,
                             szFileName,
                             nLine,
                             FALSE);

        }
        __finally {
            _munlock(_HEAP_LOCK);   /* release other threads */
        }
        if (pvBlk)
        {
            RTCCALLBACK(_RTC_Allocate_hook, (pUserData, nNewSize, 0));
        }
        return pvBlk;
}

/***
*void free() - free a block in the debug heap
*
*Purpose:
*       Frees a 'normal' memory block.
*
*Entry:
*       void * pUserData -  pointer to a (user portion) of memory block in the
*                       debug heap
*
*Return:
*       <void>
*
*******************************************************************************/
extern "C" _CRTIMP void __cdecl free(
        void * pUserData
        )
{
        _free_dbg(pUserData, _NORMAL_BLOCK);
}


extern "C" void __cdecl _free_nolock(
        void * pUserData
        )
{
        _free_dbg_nolock(pUserData, _NORMAL_BLOCK);
}



/***
*void _free_dbg() - free a block in the debug heap
*
*Purpose:
*       Frees any type of supported block.
*
*Entry:
*       void * pUserData    - pointer to a (user portion) of memory block in the
*                             debug heap
*       int nBlockUse       - block type
*
*Return:
*       <void>
*
*******************************************************************************/


extern "C" _CRTIMP void __cdecl _free_dbg(
        void * pUserData,
        int nBlockUse
        )
{
        /* lock the heap
         */
        _mlock(_HEAP_LOCK);

        __try {
            /* allocate the block
             */
            _free_dbg_nolock(pUserData, nBlockUse);
        }
        __finally {
            /* unlock the heap
             */
            _munlock(_HEAP_LOCK);
        }
}

extern "C" void __cdecl _free_dbg_nolock(


        void * pUserData,
        int nBlockUse
        )
{
        _CrtMemBlockHeader * pHead;

        RTCCALLBACK(_RTC_Free_hook, (pUserData, 0));

        /* verify heap before freeing */

        if (check_frequency > 0)
            if (check_counter == (check_frequency - 1))
            {
                _ASSERTE(_CrtCheckMemory());
                check_counter = 0;
            }
            else
                check_counter++;

        if (pUserData == NULL)
            return;

        /* check if the heap was not allocated by _aligned routines */
        if ( nBlockUse == _NORMAL_BLOCK)
        {
            if ( CheckBytes((unsigned char*)((uintptr_t)pUserData & ~(sizeof(uintptr_t) -1)) -nAlignGapSize,_bAlignLandFill, nAlignGapSize))
            {
                _RPT1(_CRT_ERROR, "The Block at 0x%p was allocated by aligned routines, use _aligned_free()", pUserData);
                errno = EINVAL;
                return;
            }
        }

        /* forced failure */
        if ((_pfnAllocHook) && !(*_pfnAllocHook)(_HOOK_FREE, pUserData, 0, nBlockUse, 0L, NULL, 0))
        {
            _RPT0(_CRT_WARN, "Client hook free failure.\n");

            return;
        }

        /*
         * If this ASSERT fails, a bad pointer has been passed in. It may be
         * totally bogus, or it may have been allocated from another heap.
         * The pointer MUST come from the 'local' heap.
         */
        _ASSERTE(_CrtIsValidHeapPointer(pUserData));

        /* get a pointer to memory block header */
        pHead = pHdr(pUserData);

        /* verify block type */
        _ASSERTE(_BLOCK_TYPE_IS_VALID(pHead->nBlockUse));

        /* if we didn't already check entire heap, at least check this object */
        if (!(_crtDbgFlag & _CRTDBG_CHECK_ALWAYS_DF))
        {
            /* check no-mans-land gaps */
            if (!CheckBytes(pHead->gap, _bNoMansLandFill, nNoMansLandSize))
                _RPT3(_CRT_ERROR, "HEAP CORRUPTION DETECTED: before %hs block (#%d) at 0x%p.\n"
                    "CRT detected that the application wrote to memory before start of heap buffer.\n",
                    szBlockUseName[_BLOCK_TYPE(pHead->nBlockUse)],
                    pHead->lRequest,
                    (BYTE *) pbData(pHead));

            if (!CheckBytes(pbData(pHead) + pHead->nDataSize, _bNoMansLandFill, nNoMansLandSize))
                _RPT3(_CRT_ERROR, "HEAP CORRUPTION DETECTED: after %hs block (#%d) at 0x%p.\n"
                    "CRT detected that the application wrote to memory after end of heap buffer.\n",
                    szBlockUseName[_BLOCK_TYPE(pHead->nBlockUse)],
                    pHead->lRequest,
                    (BYTE *) pbData(pHead));
        }

        RTCCALLBACK(_RTC_FuncCheckSet_hook,(0));

        if (pHead->nBlockUse == _IGNORE_BLOCK)
        {
            _ASSERTE(pHead->nLine == IGNORE_LINE && pHead->lRequest == IGNORE_REQ);
            /* fill the entire block including header with dead-land-fill */
            memset(pHead, _bDeadLandFill,
                sizeof(_CrtMemBlockHeader) + pHead->nDataSize + nNoMansLandSize);
            _free_base(pHead);
            RTCCALLBACK(_RTC_FuncCheckSet_hook,(1));
            return;
        }

        /* CRT blocks can be freed as NORMAL blocks */
        if (pHead->nBlockUse == _CRT_BLOCK && nBlockUse == _NORMAL_BLOCK)
            nBlockUse = _CRT_BLOCK;

        /* Error if freeing incorrect memory type */
        _ASSERTE(pHead->nBlockUse == nBlockUse);

        /* keep track of total amount of memory allocated */
        _lCurAlloc -= pHead->nDataSize;

        /* optionally reclaim memory */
        if (!(_crtDbgFlag & _CRTDBG_DELAY_FREE_MEM_DF))
        {
            /* remove from the linked list */
            if (pHead->pBlockHeaderNext)
            {
                pHead->pBlockHeaderNext->pBlockHeaderPrev = pHead->pBlockHeaderPrev;
            }
            else
            {
                _ASSERTE(_pLastBlock == pHead);
                _pLastBlock = pHead->pBlockHeaderPrev;
            }

            if (pHead->pBlockHeaderPrev)
            {
                pHead->pBlockHeaderPrev->pBlockHeaderNext = pHead->pBlockHeaderNext;
            }
            else
            {
                _ASSERTE(_pFirstBlock == pHead);
                _pFirstBlock = pHead->pBlockHeaderNext;
            }

            /* fill the entire block including header with dead-land-fill */
            memset(pHead, _bDeadLandFill,
                sizeof(_CrtMemBlockHeader) + pHead->nDataSize + nNoMansLandSize);
            _free_base(pHead);
        }
        else
        {
            pHead->nBlockUse = _FREE_BLOCK;

            /* keep memory around as dead space */
            memset(pbData(pHead), _bDeadLandFill, pHead->nDataSize);
        }
        RTCCALLBACK(_RTC_FuncCheckSet_hook,(1));
}

/***
*size_t _msize() - calculate the size of specified block in the heap
*
*Purpose:
*       Calculates the size of memory block (in the heap) pointed to by
*       pUserData.
*
*       For 'normal' memory block.
*
*Entry:
*       void * pUserData - pointer to a memory block in the heap
*
*Return:
*       size of the block
*
*******************************************************************************/

extern "C" _CRTIMP size_t __cdecl _msize (
        void * pUserData
        )
{
        return _msize_dbg(pUserData, _NORMAL_BLOCK);
}


/***
*size_t _msize_dbg() - calculate the size of specified block in the heap
*
*Purpose:
*       Calculates the size of memory block (in the heap) pointed to by
*       pUserData.
*
*Entry:
*       void * pUserData    - pointer to a (user portion) of memory block in the
*                             debug heap
*       int nBlockUse       - block type
*
*       For any type of supported block.
*
*Return:
*       size of the block
*
*******************************************************************************/

extern "C" _CRTIMP size_t __cdecl _msize_dbg (
        void * pUserData,
        int nBlockUse
        )
{
        size_t nSize;
        _CrtMemBlockHeader * pHead;

        /* validation section */
        _VALIDATE_RETURN(pUserData != NULL, EINVAL, -1);

        /* verify heap before getting size */
        if (check_frequency > 0)
            if (check_counter == (check_frequency - 1))
            {
                _ASSERTE(_CrtCheckMemory());
                check_counter = 0;
            }
            else
                check_counter++;

        _mlock(_HEAP_LOCK);         /* block other threads */
        __try {

        /*
         * If this ASSERT fails, a bad pointer has been passed in. It may be
         * totally bogus, or it may have been allocated from another heap.
         * The pointer MUST come from the 'local' heap.
         */
        _ASSERTE(_CrtIsValidHeapPointer(pUserData));

        /* get a pointer to memory block header */
        pHead = pHdr(pUserData);

         /* verify block type */
        _ASSERTE(_BLOCK_TYPE_IS_VALID(pHead->nBlockUse));

        /* CRT blocks can be treated as NORMAL blocks */
        if (pHead->nBlockUse == _CRT_BLOCK && nBlockUse == _NORMAL_BLOCK)
            nBlockUse = _CRT_BLOCK;

/* The following assertion was prone to false positives - JWM */
/*        if (pHead->nBlockUse != _IGNORE_BLOCK)              */
/*            _ASSERTE(pHead->nBlockUse == nBlockUse);        */

        nSize = pHead->nDataSize;

        }
        __finally {
            _munlock(_HEAP_LOCK);   /* release other threads */
        }

        return nSize;
}

/***
*long _CrtSetBreakAlloc() - set allocation on which to break
*
*Purpose:
*       set allocation on which to break
*
*Entry:
*       long lBreakAlloc
*
*Exit:
*       return previous break number
*
*Exceptions:
*
*******************************************************************************/
extern "C" _CRTIMP long __cdecl _CrtSetBreakAlloc(
        long lNewBreakAlloc
        )
{
        long lOldBreakAlloc = _crtBreakAlloc;
        _crtBreakAlloc = lNewBreakAlloc;
        return lOldBreakAlloc;
}

/***
*void _CrtSetDbgBlockType() - change memory block type
*
*Purpose:
*       change memory block type
*
*Entry:
*       void * pUserData    - pointer to a (user portion) of memory block in the
*                             debug heap
*       int nBlockUse       - block type
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/
extern "C" _CRTIMP void __cdecl _CrtSetDbgBlockType(
        void * pUserData,
        int nBlockUse
        )
{
        _CrtMemBlockHeader * pHead;

        _mlock(_HEAP_LOCK);         /* block other threads */
        __try {

        /* If from local heap, then change block type. */
        if (_CrtIsValidHeapPointer(pUserData))
        {
            /* get a pointer to memory block header */
            pHead = pHdr(pUserData);

            /* verify block type */
            _ASSERTE(_BLOCK_TYPE_IS_VALID(pHead->nBlockUse));

            pHead->nBlockUse = nBlockUse;
        }

        }
        __finally {
            _munlock(_HEAP_LOCK);   /* release other threads */
        }

        return;
}

/*---------------------------------------------------------------------------
 *
 * Client-defined allocation hook
 *
 --------------------------------------------------------------------------*/

/***
*_CRT_ALLOC_HOOK _CrtSetAllocHook() - set client allocation hook
*
*Purpose:
*       set client allocation hook
*
*Entry:
*       _CRT_ALLOC_HOOK pfnNewHook - new allocation hook
*
*Exit:
*       return previous hook
*
*Exceptions:
*   Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/
extern "C" _CRTIMP _CRT_ALLOC_HOOK __cdecl _CrtSetAllocHook(
        _CRT_ALLOC_HOOK pfnNewHook
        )
{
        _CRT_ALLOC_HOOK pfnOldHook = _pfnAllocHook;

        _pfnAllocHook = pfnNewHook;
        return pfnOldHook;
}

/***
*_CRT_ALLOC_HOOK _CrtGetAllocHook() - get client allocation hook
*
*Purpose:
*       get client allocation hook
*
*Entry:
*
*Exit:
*       return current hook
*
*Exceptions:
*
*******************************************************************************/
extern "C" _CRTIMP _CRT_ALLOC_HOOK __cdecl _CrtGetAllocHook
(
        void
)
{
        return _pfnAllocHook;
}

/*---------------------------------------------------------------------------
 *
 * Memory management
 *
 --------------------------------------------------------------------------*/

/***
*static int CheckBytes() - verify byte range set to proper value
*
*Purpose:
*       verify byte range set to proper value
*
*Entry:
*       unsigned char *pb       - pointer to start of byte range
*       unsigned char bCheck    - value byte range should be set to
*       size_t nSize            - size of byte range to be checked
*
*Return:
*       TRUE - if all bytes in range equal bcheck
*       FALSE otherwise
*
*******************************************************************************/
extern "C" static int __cdecl CheckBytes(
        unsigned char * pb,
        unsigned char bCheck,
        size_t nSize
        )
{
        int bOkay = TRUE;
        while (nSize--)
        {
            if (*pb++ != bCheck)
            {
/* Internal error report is just noise; calling functions all report results - JWM */
/*                _RPT3(_CRT_WARN, "memory check error at 0x%p = 0x%02X, should be 0x%02X.\n", */
/*                    (BYTE *)(pb-1),*(pb-1), bCheck); */
                bOkay = FALSE;
            }
        }
        return bOkay;
}


/***
*int _CrtCheckMemory() - check heap integrity
*
*Purpose:
*       Confirm integrity of debug heap. Call _heapchk to validate underlying
*       heap.
*
*Entry:
*       void
*
*Return:
*       TRUE - if debug and underlying heap appear valid
*       FALSE otherwise
*
*******************************************************************************/
extern "C" _CRTIMP int __cdecl _CrtCheckMemory(
        void
        )
{
        int allOkay;
        int nHeapCheck;
        _CrtMemBlockHeader * pHead;

        if (!(_crtDbgFlag & _CRTDBG_ALLOC_MEM_DF))
            return TRUE;        /* can't do any checking */

        _mlock(_HEAP_LOCK);  /* block other threads */
        __try {

        /* check underlying heap */

        nHeapCheck = _heapchk();
        if (nHeapCheck != _HEAPEMPTY && nHeapCheck != _HEAPOK)
        {
            switch (nHeapCheck)
            {
            case _HEAPBADBEGIN:
                _RPT0(_CRT_WARN, "_heapchk fails with _HEAPBADBEGIN.\n");
                break;
            case _HEAPBADNODE:
                _RPT0(_CRT_WARN, "_heapchk fails with _HEAPBADNODE.\n");
                break;
            case _HEAPEND:
                _RPT0(_CRT_WARN, "_heapchk fails with _HEAPBADEND.\n");
                break;
            case _HEAPBADPTR:
                _RPT0(_CRT_WARN, "_heapchk fails with _HEAPBADPTR.\n");
                break;
            default:
                _RPT0(_CRT_WARN, "_heapchk fails with unknown return value!\n");
                break;
            }
            allOkay = FALSE;
        }
        else
        {
            allOkay = TRUE;

            /* check all allocated blocks */

            for (pHead = _pFirstBlock; pHead != NULL; pHead = pHead->pBlockHeaderNext)
            {
                int okay = TRUE;       /* this block okay ? */
                unsigned char * blockUse;

                if (_BLOCK_TYPE_IS_VALID(pHead->nBlockUse))
                    blockUse = (unsigned char *)szBlockUseName[_BLOCK_TYPE(pHead->nBlockUse)];
                else
                    blockUse = (unsigned char *)"DAMAGED";


                /* check no-mans-land gaps */
                if (!CheckBytes(pHead->gap, _bNoMansLandFill, nNoMansLandSize))
                {
                    _RPT3(_CRT_WARN, "HEAP CORRUPTION DETECTED: before %hs block (#%d) at 0x%p.\n"
                        "CRT detected that the application wrote to memory before start of heap buffer.\n",
                        blockUse, pHead->lRequest, (BYTE *) pbData(pHead));
                    okay = FALSE;
                }

                if (!CheckBytes(pbData(pHead) + pHead->nDataSize, _bNoMansLandFill,
                nNoMansLandSize))
                {
                    _RPT3(_CRT_WARN, "HEAP CORRUPTION DETECTED: after %hs block (#%d) at 0x%p.\n"
                        "CRT detected that the application wrote to memory after end of heap buffer.\n",
                        blockUse, pHead->lRequest, (BYTE *) pbData(pHead));
                    okay = FALSE;
                }

                /* free blocks should remain undisturbed */
                if (pHead->nBlockUse == _FREE_BLOCK &&
                !CheckBytes(pbData(pHead), _bDeadLandFill, pHead->nDataSize))
                {
                    _RPT1(_CRT_WARN, "HEAP CORRUPTION DETECTED: on top of Free block at 0x%p.\n"
                        "CRT detected that the application wrote to a heap buffer that was freed.\n",
                        (BYTE *) pbData(pHead));
                    okay = FALSE;
                }

                if (!okay)
                {
                    /* report some more statistics about the broken object */

                    if (pHead->szFileName != NULL)
                        _RPT3(_CRT_WARN, "%hs allocated at file %hs(%d).\n",
                            blockUse, pHead->szFileName, pHead->nLine);

                    _RPT3(_CRT_WARN, "%hs located at 0x%p is %Iu bytes long.\n",
                        blockUse, (BYTE *)pbData(pHead), pHead->nDataSize);

                    allOkay = FALSE;
                }
            }
        }

        }
        __finally {
            _munlock( _HEAP_LOCK );     /* release other threads */
        }

        return allOkay;
}


/***
*int _CrtSetDbgFlag() - get/set the _crtDbgFlag
*
*Purpose:
*       get or set the _crtDbgFlag
*
*Entry:
*       int fNewBits - new Flag or _CRTDBG_REPORT_FLAG
*
*Return:
*       previous flag state
*
*Exceptions:
*       The new flag need to use only _CRTDBG_ALLOC_MEM_DF, _CRTDBG_DELAY_FREE_MEM_DF,
*       _CRTDBG_CHECK_ALWAYS_DF, _CRTDBG_CHECK_CRT_DF and _CRTDBG_LEAK_CHECK_DF. Otherwise
*       errno is set to EINVAL and the flag is not changed.
*
*******************************************************************************/
extern "C" _CRTIMP int __cdecl _CrtSetDbgFlag(
        int fNewBits
        )
{
        int fOldBits= _crtDbgFlag;

        /* Make sure the flag uses only _CRTDBG_ALLOC_MEM_DF, _CRTDBG_DELAY_FREE_MEM_DF,
            * _CRTDBG_CHECK_ALWAYS_DF, _CRTDBG_CHECK_CRT_DF and _CRTDBG_LEAK_CHECK_DF
            */
        _VALIDATE_RETURN(       (fNewBits==_CRTDBG_REPORT_FLAG) ||
                                                        ((fNewBits & 0x0ffff &
                                                                ~(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF |
                                                                _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_CHECK_CRT_DF |
                                                                _CRTDBG_LEAK_CHECK_DF)
                                                        ) == 0),
            EINVAL,
            _crtDbgFlag);

        _mlock(_HEAP_LOCK);  /* block other threads */
        __try {

                // deliberate reinit here now we own the lock to ensure we pick up the most recent values
        fOldBits = _crtDbgFlag;

        if ( fNewBits != _CRTDBG_REPORT_FLAG )
        {
            if ( fNewBits & _CRTDBG_CHECK_ALWAYS_DF )
                check_frequency = 1;
            else
                check_frequency = (fNewBits >> 16) & 0x0ffff;

            check_counter = 0;
            _crtDbgFlag = fNewBits;
        }

        }
        __finally {
            _munlock( _HEAP_LOCK );
        }
        return fOldBits;
}


/***
*int _CrtDoForAllClientObjects() - call a client-supplied function for all
*                                  client objects in the heap
*
*Purpose:
*       call a client-supplied function for all client objects in the heap
*
*Entry:
*       void (*pfn)(void *, void *) - pointer to client function to call
*       void * pContext - pointer to user supplied context to pass to function
*
*Return:
*    void
*
*Exceptions:
*   Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/
extern "C" _CRTIMP void __cdecl _CrtDoForAllClientObjects(
        void (*pfn)(void *, void *),
        void * pContext
        )
{
        _CrtMemBlockHeader * pHead;

        /* validation section */
        _VALIDATE_RETURN_VOID(pfn != NULL, EINVAL);

        if (!(_crtDbgFlag & _CRTDBG_ALLOC_MEM_DF))
            return;         /* sorry not enabled */

        _mlock(_HEAP_LOCK);  /* block other threads */
        __try {

        for (pHead = _pFirstBlock; pHead != NULL; pHead = pHead->pBlockHeaderNext)
        {
            if (_BLOCK_TYPE(pHead->nBlockUse) == _CLIENT_BLOCK)
                (*pfn)((void *) pbData(pHead), pContext);
        }

        }
        __finally {
            _munlock(_HEAP_LOCK);  /* release other threads */
        }
}


/***
*int _CrtIsValidPointer() - verify memory range is valid for reading/writing
*
*Purpose:
*       verify memory range range is valid for reading/writing
*
*Entry:
*       const void * pv     - start of memory range to test
*       unsigned int nBytes - size of memory range
*       int bReadWrite      - TRUE if read/write, FALSE if read-only
*
*Return:
*       TRUE - if valid address
*       FALSE otherwise
*
*******************************************************************************/
extern "C" _CRTIMP int __cdecl _CrtIsValidPointer(
        const void * pv,
        unsigned int nBytes,
        int bReadWrite
        )
{
        return (pv != NULL);
}

/***
*int _CrtIsValidHeapPointer() - verify pointer is from 'local' heap
*
*Purpose:
*       Verify pointer is not only a valid pointer but also that it is from
*       the 'local' heap. Pointers from another copy of the C runtime (even in the
*       same process) will be caught.
*
*Entry:
*       const void * pUserData     - pointer of interest
*
*Return:
*       TRUE - if valid and from local heap
*       FALSE otherwise
*
*******************************************************************************/
extern "C" _CRTIMP int __cdecl _CrtIsValidHeapPointer(
        const void * pUserData
        )
{
#ifndef WINHEAP
        int i;
        void * base;
#else  /* WINHEAP */
        unsigned int osver = 0;
#endif  /* WINHEAP */

        if (!pUserData)
            return FALSE;

        if (!_CrtIsValidPointer(pHdr(pUserData), sizeof(_CrtMemBlockHeader), FALSE))
            return FALSE;


#ifdef WINHEAP

        _ERRCHECK(_get_osver(&osver));

#ifndef _WIN64
        if ( __active_heap == __V6_HEAP )
        {
            PHEADER     pHeader;
            if (pHeader = __sbh_find_block(pHdr(pUserData)))
            {
                return __sbh_verify_block(pHeader, pHdr(pUserData));
            }
            else if ( (osver & 0x8000) != 0 )
                return TRUE;
            else
                return HeapValidate( _crtheap, 0, pHdr(pUserData) );
        }
#ifdef CRTDLL
        else if ( __active_heap == __V5_HEAP )
        {
            __old_sbh_region_t * preg;
            __old_sbh_page_t *   ppage;
            __old_page_map_t *   pmap;
            if ( (pmap = __old_sbh_find_block( pHdr(pUserData), &preg, &ppage )) !=
                 NULL )
            {
                if ( *pmap )
                    return TRUE;
                else
                    return FALSE;
            }
            else if ( (osver & 0x8000) != 0 )
                return TRUE;
            else
                return HeapValidate( _crtheap, 0, pHdr(pUserData) );
        }
#endif  /* CRTDLL */
        else    //  __active_heap == _SYSTEM_HEAP
#endif  /* _WIN64 */
        {
            return HeapValidate( _crtheap, 0, pHdr(pUserData) );
        }

#else  /* WINHEAP */

        /*
         * Go through the heap regions and see if the pointer lies within one
         * of the regions of the local heap.
         *
         * Pointers from non-local heaps cannot be handled. For example, a
         * non-local pointer may come from a DLL that has the CRT linked-in.
         *
         */

        for (i = 0; (base = _heap_regions[i]._regbase) != NULL &&
                i < _HEAP_REGIONMAX; i++)
        {
            if (pUserData >= base && pUserData <
                    (void *)(((char *)base)+_heap_regions[i]._currsize))
                return TRUE;
        }

        return FALSE;

#endif  /* WINHEAP */
}


/***
*int _CrtIsMemoryBlock() - verify memory block is debug heap block
*
*Purpose:
*       verify memory block is debug heap block
*
*Entry:
*       const void *    pUserData       - start of memory block
*       unsigned int    nBytes          - size of memory block
*       long * plRequestNumber          - if !NULL, set to request number
*       char **         pszFileName     - if !NULL, set to file name
*       int *           pnLine          - if !NULL, set to line number
*
*Return:
*       TRUE - if debug memory heap address
*       FALSE otherwise
*
*******************************************************************************/
extern "C" _CRTIMP int __cdecl _CrtIsMemoryBlock(
        const void * pUserData,
        unsigned int nBytes,
        long * plRequestNumber,
        char ** pszFileName,
        int * pnLine
        )
{
        _CrtMemBlockHeader * pHead=NULL;
        int retval=FALSE;

        /* pre-init output info with null values */
        if (plRequestNumber != NULL)
        {
            *plRequestNumber = 0;
        }
        if (pszFileName != NULL)
        {
            *pszFileName = NULL;
        }
        if (pnLine != NULL)
        {
            *pnLine = 0;
        }

        if (!_CrtIsValidHeapPointer(pUserData))
        {
            return FALSE;
        }

        _mlock(_HEAP_LOCK);         /* block other threads */
        __try {

        pHead = pHdr(pUserData);

        if (_BLOCK_TYPE_IS_VALID(pHead->nBlockUse) &&
            _CrtIsValidPointer(pUserData, nBytes, TRUE) &&
            pHead->nDataSize == nBytes &&
            pHead->lRequest <= _lRequestCurr
           )
        {
            if (plRequestNumber != NULL)
                *plRequestNumber = pHead->lRequest;
            if (pszFileName != NULL)
                *pszFileName = pHead->szFileName;
            if (pnLine != NULL)
                *pnLine = pHead->nLine;

            retval = TRUE;
        }
        else
            retval = FALSE;

        }
        __finally {
            _munlock(_HEAP_LOCK);   /* release other threads */
        }

        return retval;
}


/***
*
*Purpose:
*       return memory block type for a debug heap block
*
*Entry:
*       const void * pUserData - start of memory block
*
*Return:
*       Block type if pUserData is a valid debug heap block pointer, else -1.
*
*******************************************************************************/
extern "C" _CRTIMP int _CrtReportBlockType(
        const void * pUserData
        )
{
        _CrtMemBlockHeader * pHead;

        if (!_CrtIsValidHeapPointer(pUserData))
            return -1;

        pHead = pHdr(pUserData);
        return pHead->nBlockUse;
}


/*---------------------------------------------------------------------------
 *
 * Memory state
 *
 --------------------------------------------------------------------------*/


/***
*_CRT_DUMP_CLIENT _CrtSetDumpClient() - set client dump routine
*
*Purpose:
*       set client dump routine
*
*Entry:
*       _CRT_DUMP_CLIENT pfnNewDumpClient - new dump routine
*
*Exit:
*       return previous dump routine
*
*Exceptions:
*
*******************************************************************************/
extern "C" _CRTIMP _CRT_DUMP_CLIENT __cdecl _CrtSetDumpClient(
        _CRT_DUMP_CLIENT pfnNewDump
        )
{
        _CRT_DUMP_CLIENT pfnOldDump = _pfnDumpClient;
        _pfnDumpClient = pfnNewDump;
        return pfnOldDump;
}

/***
*_CRT_DUMP_CLIENT _CrtGetDumpClient() - get client dump routine
*
*Purpose:
*       get client dump routine
*
*Entry:
*
*Exit:
*       return current dump routine
*
*Exceptions:
*
*******************************************************************************/
extern "C" _CRTIMP _CRT_DUMP_CLIENT __cdecl _CrtGetDumpClient
(
        void
)
{
        return _pfnDumpClient;
}


/***
*_CrtMemState * _CrtMemStateCheckpoint() - checkpoint current memory state
*
*Purpose:
*       checkpoint current memory state
*
*Entry:
*       _CrtMemState * state - state structure to fill in, will be
*       allocated if NULL
*
*Return:
*       current memory state
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/
extern "C" _CRTIMP void __cdecl _CrtMemCheckpoint(
        _CrtMemState * state
        )
{
        int use;
        _CrtMemBlockHeader * pHead;

        /* validation section */
        _VALIDATE_RETURN_VOID(state != NULL, EINVAL);

        _mlock(_HEAP_LOCK);         /* block other threads */
        __try {

        state->pBlockHeader = _pFirstBlock;
        for (use = 0; use < _MAX_BLOCKS; use++)
            state->lCounts[use] = state->lSizes[use] = 0;

        for (pHead = _pFirstBlock; pHead != NULL; pHead = pHead->pBlockHeaderNext)
        {
            if (_BLOCK_TYPE(pHead->nBlockUse) >= 0 && _BLOCK_TYPE(pHead->nBlockUse) < _MAX_BLOCKS)
            {
                state->lCounts[_BLOCK_TYPE(pHead->nBlockUse)]++;
                state->lSizes[_BLOCK_TYPE(pHead->nBlockUse)] += pHead->nDataSize;
            }
            else
            {
                _RPT1(_CRT_WARN, "Bad memory block found at 0x%p.\n", (BYTE *)pHead);
            }
        }

        state->lHighWaterCount = _lMaxAlloc;
        state->lTotalCount = _lTotalAlloc;

        }
        __finally {
            _munlock(_HEAP_LOCK);   /* release other threads */
        }
}


/***
*int _CrtMemDifference() - compare two memory states
*
*Purpose:
*       compare two memory states
*
*Entry:
*       _CrtMemState * state - return memory state difference
*       _CrtMemState * oldState - earlier memory state
*       _CrtMemState * newState - later memory state
*
*Return:
*       TRUE if difference
*       FALSE otherwise
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/
extern "C" _CRTIMP int __cdecl _CrtMemDifference(
        _CrtMemState * state,
        const _CrtMemState * oldState,
        const _CrtMemState * newState
        )
{
        int use;
        int bSignificantDifference = FALSE;

        /* validation section */
        _VALIDATE_RETURN(state != NULL, EINVAL, FALSE);
        _VALIDATE_RETURN(oldState != NULL, EINVAL, FALSE);
        _VALIDATE_RETURN(newState != NULL, EINVAL, FALSE);

        for (use = 0; use < _MAX_BLOCKS; use++)
        {
            state->lSizes[use] = newState->lSizes[use] - oldState->lSizes[use];
            state->lCounts[use] = newState->lCounts[use] - oldState->lCounts[use];

            if (    (state->lSizes[use] != 0 || state->lCounts[use] != 0) &&
                     use != _FREE_BLOCK &&
                    (use != _CRT_BLOCK ||
                    (use == _CRT_BLOCK && (_crtDbgFlag & _CRTDBG_CHECK_CRT_DF)))
                    )
                bSignificantDifference = TRUE;
        }
        state->lHighWaterCount = newState->lHighWaterCount - oldState->lHighWaterCount;
        state->lTotalCount = newState->lTotalCount - oldState->lTotalCount;
        state->pBlockHeader = NULL;

        return bSignificantDifference;
}

#define MAXPRINT 16

extern "C" static void __cdecl _printMemBlockData(
        _locale_t plocinfo,
        _CrtMemBlockHeader * pHead
        )
{
        int i;
        unsigned char ch;
        unsigned char printbuff[MAXPRINT+1];
        unsigned char valbuff[MAXPRINT*3+1];

        _LocaleUpdate _loc_update(plocinfo);
        for (i = 0; i < min((int)pHead->nDataSize, MAXPRINT); i++)
        {
            ch = pbData(pHead)[i];
            printbuff[i] = _isprint_l(ch, _loc_update.GetLocaleT()) ? ch : ' ';
            _ERRCHECK_SPRINTF(sprintf_s((char *)&valbuff[i*3], _countof(valbuff) - (i * 3) , "%.2X ", ch));
        }
        printbuff[i] = '\0';

        _RPT2(_CRT_WARN, " Data: <%s> %s\n", printbuff, valbuff);
}


/***
*void _CrtMemDumpAllObjectsSince() - dump all objects since memory state
*
*Purpose:
*       dump all objects since memory state
*
*Entry:
*       _CrtMemState * state - dump since this state
*
*Return:
*       void
*
*******************************************************************************/
static void __cdecl _CrtMemDumpAllObjectsSince_stat(
        const _CrtMemState * state,
        _locale_t plocinfo
        )
{
        _CrtMemBlockHeader * pHead;
        _CrtMemBlockHeader * pStopBlock = NULL;

        _mlock(_HEAP_LOCK);         /* block other threads */
        __try {

        _RPT0(_CRT_WARN, "Dumping objects ->\n");

        if (state)
            pStopBlock = state->pBlockHeader;

        for (pHead = _pFirstBlock; pHead != NULL && pHead != pStopBlock;
            pHead = pHead->pBlockHeaderNext)
        {
            if (_BLOCK_TYPE(pHead->nBlockUse) == _IGNORE_BLOCK ||
                _BLOCK_TYPE(pHead->nBlockUse) == _FREE_BLOCK ||
                (_BLOCK_TYPE(pHead->nBlockUse) == _CRT_BLOCK &&
               !(_crtDbgFlag & _CRTDBG_CHECK_CRT_DF))
               )
            {
                /* ignore it for dumping */
            }
            else {
                if (pHead->szFileName != NULL)
                {
#pragma warning(push)
#pragma warning(disable: 4996)
                    if (!_CrtIsValidPointer(pHead->szFileName, 1, FALSE) || IsBadReadPtr(pHead->szFileName,1))
#pragma warning(pop)
                                        {
                        _RPT1(_CRT_WARN, "#File Error#(%d) : ", pHead->nLine);
                                        }
                    else
                                        {
                        _RPT2(_CRT_WARN, "%hs(%d) : ", pHead->szFileName, pHead->nLine);
                                        }
                }

                _RPT1(_CRT_WARN, "{%ld} ", pHead->lRequest);

                if (_BLOCK_TYPE(pHead->nBlockUse) == _CLIENT_BLOCK)
                {
                    _RPT3(_CRT_WARN, "client block at 0x%p, subtype %x, %Iu bytes long.\n",
                        (BYTE *)pbData(pHead), _BLOCK_SUBTYPE(pHead->nBlockUse), pHead->nDataSize);

#pragma warning(push)
#pragma warning(disable: 4996)
                    if (_pfnDumpClient && !IsBadReadPtr(pbData(pHead), 1))
#pragma warning(pop)
                        (*_pfnDumpClient)( (void *) pbData(pHead), pHead->nDataSize);
                    else
                        _printMemBlockData(plocinfo, pHead);
                }
                else if (pHead->nBlockUse == _NORMAL_BLOCK)
                {
                    _RPT2(_CRT_WARN, "normal block at 0x%p, %Iu bytes long.\n",
                        (BYTE *)pbData(pHead), pHead->nDataSize);

                    _printMemBlockData(plocinfo, pHead);
                }
                else if (_BLOCK_TYPE(pHead->nBlockUse) == _CRT_BLOCK)
                {
                    _RPT3(_CRT_WARN, "crt block at 0x%p, subtype %x, %Iu bytes long.\n",
                        (BYTE *)pbData(pHead), _BLOCK_SUBTYPE(pHead->nBlockUse), pHead->nDataSize);

                    _printMemBlockData(plocinfo, pHead);
                }
            }
        }
        }
        __finally {
            _munlock(_HEAP_LOCK);   /* release other threads */
        }

        _RPT0(_CRT_WARN, "Object dump complete.\n");
}
extern "C" _CRTIMP void __cdecl _CrtMemDumpAllObjectsSince(
        const _CrtMemState * state
        )
{
        _locale_t plocinfo = NULL;
        _LocaleUpdate _loc_update(plocinfo);

        _CrtMemDumpAllObjectsSince_stat(state, _loc_update.GetLocaleT());
}


/***
*void _CrtMemDumpMemoryLeaks() - dump all objects still in heap
*
*Purpose:
*       dump all objects still in heap. used to detect memory leaks over the
*       life of a program
*
*Entry:
*       void
*
*Return:
*       TRUE if memory leaks
*       FALSE otherwise
*
*******************************************************************************/
extern "C" _CRTIMP int __cdecl _CrtDumpMemoryLeaks(
        void
        )
{
        /* only dump leaks when there are in fact leaks */
        _CrtMemState msNow;

        _CrtMemCheckpoint(&msNow);

        if (msNow.lCounts[_CLIENT_BLOCK] != 0 ||
            msNow.lCounts[_NORMAL_BLOCK] != 0 ||
            (_crtDbgFlag & _CRTDBG_CHECK_CRT_DF &&
            msNow.lCounts[_CRT_BLOCK] != 0)
           )
        {
            /* difference detected: dump objects since start. */
            _RPT0(_CRT_WARN, "Detected memory leaks!\n");

            _CrtMemDumpAllObjectsSince(NULL);
            return TRUE;
        }

        return FALSE;   /* no leaked objects */
}


/***
*_CrtMemState * _CrtMemDumpStatistics() - dump memory state
*
*Purpose:
*       dump memory state
*
*Entry:
*       _CrtMemState * state - dump this state
*
*Return:
*       void
*
*Exceptions:
*       Input parameters are validated. Refer to the validation section of the function.
*
*******************************************************************************/
extern "C" _CRTIMP void __cdecl _CrtMemDumpStatistics(
        const _CrtMemState * state
        )
{
        int use;

        /* validation section */
        _VALIDATE_RETURN_VOID(state != NULL, EINVAL);

        for (use = 0; use < _MAX_BLOCKS; use++)
        {
            _RPT3(_CRT_WARN, "%Id bytes in %Id %hs Blocks.\n",
                state->lSizes[use], state->lCounts[use], szBlockUseName[use]);
        }

        _RPT1(_CRT_WARN, "Largest number used: %Id bytes.\n", state->lHighWaterCount);
        _RPT1(_CRT_WARN, "Total allocations: %Id bytes.\n", state->lTotalCount);
}


/***
* void *_aligned_malloc(size_t size, size_t alignment)
*       - Get a block of aligned memory from the heap.
*
* Purpose:
*       Allocate of block of aligned memory aligned on the alignment of at least
*       size bytes from the heap and return a pointer to it.
*
* Entry:
*       size_t size - size of block requested
*       size_t alignment - alignment of memory
*
* Exit:
*       Sucess: Pointer to memory block
*       Faliure: Null
*******************************************************************************/

extern "C" _CRTIMP void * __cdecl _aligned_malloc(
        size_t size,
        size_t align
        )
{
    return _aligned_offset_malloc_dbg(size, align, 0, NULL, 0);
}


/***
* void *_aligned_malloc_dbg(size_t size, size_t alignment,
*                           const char *f_name, int line_n)
*       - Get a block of aligned memory from the heap.
*
* Purpose:
*       Allocate of block of aligned memory aligned on the alignment of at least
*       size bytes from the heap and return a pointer to it.
*
* Entry:
*       size_t size - size of block requested
*       size_t alignment - alignment of memory
*       const char * f_name - file name
*       int line_n - line number
*
* Exit:
*       Sucess: Pointer to memory block
*       Faliure: Null
*
*******************************************************************************/


extern "C" _CRTIMP void * __cdecl _aligned_malloc_dbg(
        size_t size,
        size_t align,
        const char * f_name,
        int line_n
        )
{
    return _aligned_offset_malloc_dbg(size, align, 0, f_name, line_n);
}

/***
*
* void *_aligned_realloc(size_t size, size_t alignment)
*       - Reallocate a block of aligned memory from the heap.
*
* Purpose:
*       Reallocates of block of aligned memory aligned on the alignment of at
*       least size bytes from the heap and return a pointer to it. Size can be
*       either greater or less than the original size of the block.
*       The reallocation may result in moving the block as well as changing the
*       size.
*
* Entry:
*       void *memblock - pointer to block in the heap previously allocated by
*               call to _aligned_malloc(), _aligned_offset_malloc(),
*               _aligned_realloc() or _aligned_offset_realloc().
*       size_t size - size of block requested
*       size_t alignment - alignment of memory
*
* Exit:
*       Sucess: Pointer to re-allocated memory block
*       Faliure: Null
*
*******************************************************************************/

extern "C" _CRTIMP void * __cdecl _aligned_realloc(
        void * memblock,
        size_t size,
        size_t align
        )
{
    return _aligned_offset_realloc_dbg(memblock, size, align, 0, NULL, 0);
}

/***
*
* void *_aligned_recalloc(size_t size, size_t alignment)
*       - Reallocate items from the heap.
*
* Purpose:
*       Reallocates of block of aligned memory aligned on the alignment of at
*       least size bytes from the heap and return a pointer to it. Size can be
*       either greater or less than the original size of the block.
*       The reallocation may result in moving the block as well as changing the
*       size.
*
* Entry:
*       void *memblock - pointer to block in the heap previously allocated by
*               call to _aligned_malloc(), _aligned_offset_malloc(),
*               _aligned_realloc() or _aligned_offset_realloc().
*       size_t count - count of items
*       size_t size - size of items
*       size_t alignment - alignment of memory
*
* Exit:
*       Sucess: Pointer to re-allocated memory block
*       Faliure: Null
*
*******************************************************************************/

extern "C" _CRTIMP void * __cdecl _aligned_recalloc(
        void * memblock,
        size_t count,
        size_t size,
        size_t align
        )
{
    return _aligned_offset_recalloc_dbg(memblock, count, size, align, 0, NULL, 0);
}


/***
*
* void *_aligned_realloc_dbg(void * memblock, size_t size, size_t alignment,
*                        const char * f_name, int line_n)
*       - Reallocate a block of aligned memory from the heap.
*
* Purpose:
*       Reallocates of block of aligned memory aligned on the alignment of at
*       least size bytes from the heap and return a pointer to it. Size can be
*       either greater or less than the original size of the block.
*       The reallocation may result in moving the block as well as changing the
*       size.
*
* Entry:
*       void *memblock - pointer to block in the heap previously allocated by
*               call to _aligned_malloc(), _aligned_offset_malloc(),
*               _aligned_realloc() or _aligned_offset_realloc().
*       size_t size - size of block requested
*       size_t alignment - alignment of memory
*       const char * f_name - file name
*       int - line number
*
* Exit:
*       Sucess: Pointer to re-allocated memory block
*       Faliure: Null
*
*******************************************************************************/


extern "C" _CRTIMP void * __cdecl _aligned_realloc_dbg(
        void *memblock,
        size_t size,
        size_t align,
        const char * f_name,
        int line_n
        )
{
    return _aligned_offset_realloc_dbg(memblock, size, align, 0, f_name, line_n);
}

/***
*
* void *_aligned_recalloc_dbg(void * memblock, size_t count, size_t size, size_t alignment, const char * f_name, int line_n)
*       - Reallocate items from the heap.
*
* Purpose:
*       Reallocates of block of aligned memory aligned on the alignment of at
*       least size bytes from the heap and return a pointer to it. Size can be
*       either greater or less than the original size of the block.
*       The reallocation may result in moving the block as well as changing the
*       size.
*
* Entry:
*       void *memblock - pointer to block in the heap previously allocated by
*               call to _aligned_malloc(), _aligned_offset_malloc(),
*               _aligned_realloc() or _aligned_offset_realloc().
*       size_t count - count of items
*       size_t size - size of items
*       size_t alignment - alignment of memory
*       const char * f_name - file name
*       int - line number
*
* Exit:
*       Sucess: Pointer to re-allocated memory block
*       Faliure: Null
*
*******************************************************************************/


extern "C" _CRTIMP void * __cdecl _aligned_recalloc_dbg(
        void *memblock,
        size_t count,
        size_t size,
        size_t align,
        const char * f_name,
        int line_n
        )
{
    return _aligned_offset_recalloc_dbg(memblock, count, size, align, 0, f_name, line_n);
}

/***
*
* void *_aligned_offset_malloc(size_t size, size_t alignment, int offset)
*       - Allocates a block of memory from the heap.
*
* Purpose:
*       Reallocates a block of memory which is shifted by offset from
*       alignment of at least size bytes from the heap and return a pointer
*       to it. Size can be either greater or less than the original size of the
*       block.
*
* Entry:
*       size_t size - size of block of memory
*       size_t alignment - alignment of memory
*       size_t offset - offset of memory from the alignment
*
* Exit:
*       Sucess: Pointer to the re-allocated memory block
*       Faliure: Null
*
*******************************************************************************/

extern "C" _CRTIMP void * __cdecl _aligned_offset_malloc(
        size_t size,
        size_t align,
        size_t offset
        )
{
    return _aligned_offset_malloc_dbg(size, align, offset, NULL, 0);
}



/***
*
* void *_aligned_offset_malloc_dbg(size_t size, size_t alignment, int offset,
*                              const char * f_name, int line_n)
*
* Purpose:
*       Reallocates a block of memory which is shifted by offset from
*       alignment of at least size bytes from the heap and return a pointer
*       to it. Size can be either greater or less than the original size of the
*       block.
*
* Entry:
*       size_t size - size of block of memory
*       size_t alignment - alignment of memory
*       size_t offset - offset of memory from the alignment
*       const char * f_name - file name
*       int line_n - line number
*
* Exit:
*       Sucess: Pointer to the re-allocated memory block
*       Faliure: Null
*
*******************************************************************************/


extern "C" _CRTIMP void * __cdecl _aligned_offset_malloc_dbg(
        size_t size,
        size_t align,
        size_t offset,
        const char * f_name,
        int line_n
        )
{
    uintptr_t ptr, r_ptr, t_ptr;
    _AlignMemBlockHdr *pHdr;

    /* validation section */
    _VALIDATE_RETURN(IS_2_POW_N(align), EINVAL, NULL);
    _VALIDATE_RETURN(offset == 0 || offset < size, EINVAL, NULL);

    align = (align > sizeof(uintptr_t) ? align : sizeof(uintptr_t)) -1;

    t_ptr = (0 -offset)&(sizeof(uintptr_t) -1);

    if ((ptr = (uintptr_t) _malloc_dbg(t_ptr + size + align + sizeof(_AlignMemBlockHdr), _NORMAL_BLOCK, f_name, line_n)) == (uintptr_t)NULL)
        return NULL;

    r_ptr =((ptr +align +t_ptr +sizeof(_AlignMemBlockHdr) +offset)&~align)-offset;
    pHdr = (_AlignMemBlockHdr *)(r_ptr - t_ptr) -1;
    memset((void *)pHdr->Gap, _bAlignLandFill, nAlignGapSize);
    pHdr->pHead = (void *)ptr;
    return (void *) r_ptr;
}

/***
*
* void *_aligned_offset_realloc(size_t size, size_t alignment, int offset)
*       - Reallocate a block of memory from the heap.
*
* Purpose:
*       Reallocates a block of memory which is shifted by offset from
*       alignment of at least size bytes from the heap and return a pointer
*       to it. Size can be either greater or less than the original size of the
*       block.
*
* Entry:
*       void *memblock - pointer to block in the heap previously allocated by
*               call to _aligned_malloc(), _aligned_offset_malloc(),
*               _aligned_realloc() or _aligned_offset_realloc().
*       size_t size - size of block of memory
*       size_t alignment - alignment of memory
*       size_t offset - offset of memory from the alignment
*
* Exit:
*       Sucess: Pointer to the re-allocated memory block
*       Faliure: Null
*
*******************************************************************************/

extern "C" _CRTIMP void * __cdecl _aligned_offset_realloc(
        void * memblock,
        size_t size,
        size_t align,
        size_t offset
        )
{
    return _aligned_offset_realloc_dbg(memblock, size, align, offset, NULL, 0);
}

/***
*
* void *_aligned_offset_recalloc(void * memory, size_t size, size_t count, size_t alignment, int offset)
*       - Reallocate items the heap.
*
* Purpose:
*       Reallocates a block of memory which is shifted by offset from
*       alignment of at least size bytes from the heap and return a pointer
*       to it. Size can be either greater or less than the original size of the
*       block.
*
* Entry:
*       void *memblock - pointer to block in the heap previously allocated by
*               call to _aligned_malloc(), _aligned_offset_malloc(),
*               _aligned_realloc() or _aligned_offset_realloc().
*       size_t count - count of items
*       size_t size - size of items
*       size_t alignment - alignment of memory
*       size_t offset - offset of memory from the alignment
*
* Exit:
*       Sucess: Pointer to the re-allocated memory block
*       Faliure: Null
*
*******************************************************************************/

extern "C" _CRTIMP void * __cdecl _aligned_offset_recalloc(
        void * memblock,
        size_t count,
        size_t size,
        size_t align,
        size_t offset
        )
{
    return _aligned_offset_recalloc_dbg(memblock, count, size, align, offset, NULL, 0);
}



/***
*
* void *_aligned_offset_realloc_dbg(void *memblock, size_t size,
*                                   size_t alignment, int offset,
*                                   const char * f_name, int line_n)
*       - Reallocate a block of memory from the heap.
*
* Purpose:
*       Reallocates a block of memory which is shifted by offset from
*       alignment of at least size bytes from the heap and return a pointer
*       to it. Size can be either greater or less than the original size of the
*       block.
*
* Entry:
*       void *memblock - pointer to block in the heap previously allocated by
*               call to _aligned_malloc(), _aligned_offset_malloc(),
*               _aligned_realloc() or _aligned_offset_realloc().
*       size_t size - size of block of memory
*       size_t alignment - alignment of memory
*       size_t offset - offset of memory from the alignment
*       const char * f_name - file name
*       int line_n - line number
*
* Exit:
*       Sucess: Pointer to the re-allocated memory block
*       Faliure: Null
*
*
*******************************************************************************/


extern "C" _CRTIMP void * __cdecl _aligned_offset_realloc_dbg(
        void * memblock,
        size_t size,
        size_t align,
        size_t offset,
        const char * f_name,
        int line_n
        )
{
    uintptr_t ptr, r_ptr, t_ptr, mov_sz;
    _AlignMemBlockHdr *pHdr, *s_pHdr;

    if ( memblock == NULL)
    {
        return _aligned_offset_malloc_dbg(size, align, offset, f_name, line_n);
    }
    if ( size == 0)
    {
        _aligned_free_dbg(memblock);
        return NULL;
    }

    s_pHdr = (_AlignMemBlockHdr *)((uintptr_t)memblock & ~(sizeof(uintptr_t) -1)) -1;

    if ( CheckBytes((unsigned char *)memblock -nNoMansLandSize, _bNoMansLandFill, nNoMansLandSize))
    {
        _RPT1(_CRT_ERROR, "The block at 0x%p was not allocated by _aligned routines, use realloc()", memblock);
        errno = EINVAL;
        return NULL;
    }

    if(!CheckBytes(s_pHdr->Gap, _bAlignLandFill, nAlignGapSize))
    {
        _RPT1(_CRT_ERROR, "Damage before 0x%p which was allocated by aligned routine\n", memblock);
    }

    /* validation section */
    _VALIDATE_RETURN(IS_2_POW_N(align), EINVAL, NULL);
    _VALIDATE_RETURN(offset == 0 || offset < size, EINVAL, NULL);

    mov_sz = _msize(s_pHdr->pHead) - ((uintptr_t)memblock - (uintptr_t)s_pHdr->pHead);

    align = (align > sizeof(uintptr_t) ? align : sizeof(uintptr_t)) -1;

    t_ptr = (0 -offset)&(sizeof(uintptr_t) -1);

    if ((ptr = (uintptr_t) _malloc_dbg(t_ptr + size + align + sizeof(_AlignMemBlockHdr), _NORMAL_BLOCK, f_name, line_n)) == (uintptr_t)NULL)
        return NULL;

    r_ptr =((ptr +align +t_ptr +sizeof(_AlignMemBlockHdr) +offset)&~align)-offset;
    pHdr = (_AlignMemBlockHdr *)(r_ptr - t_ptr) -1;
    memset((void *)pHdr->Gap, _bAlignLandFill, nAlignGapSize);
    pHdr->pHead = (void *)ptr;

    memcpy((void *)r_ptr, memblock, mov_sz > size ? size : mov_sz);
    _free_dbg(s_pHdr->pHead, _NORMAL_BLOCK);

    return (void *) r_ptr;
}


/***
*
* void *_aligned_offset_recalloc_dbg(void *memblock, size_t count size_t size,
*                                   size_t alignment, int offset,
*                                   const char * f_name, int line_n)
*       - Reallocate items from the heap.
*
* Purpose:
*       Reallocates a block of memory which is shifted by offset from
*       alignment of at least size bytes from the heap and return a pointer
*       to it. Size can be either greater or less than the original size of the
*       block.
*
* Entry:
*       void *memblock - pointer to block in the heap previously allocated by
*               call to _aligned_malloc(), _aligned_offset_malloc(),
*               _aligned_realloc() or _aligned_offset_realloc().
*       size_t count - count of items
*       size_t size - size of items
*       size_t alignment - alignment of memory
*       size_t offset - offset of memory from the alignment
*       const char * f_name - file name
*       int line_n - line number
*
* Exit:
*       Sucess: Pointer to the re-allocated memory block
*       Faliure: Null
*
*
*******************************************************************************/

extern "C" _CRTIMP void * __cdecl _aligned_offset_recalloc_dbg
(
    void * memblock,
    size_t count,
    size_t size,
    size_t align,
    size_t offset,
    const char * f_name,
    int line_n
)
{
    size_t  size_orig=0;

    /* ensure that (size * count) does not overflow */
    if (count > 0)
    {
        _VALIDATE_RETURN((_HEAP_MAXREQ / count) >= size, ENOMEM, NULL);
    }
    size_orig = size * count;

    return _aligned_offset_realloc_dbg(memblock, size_orig, align, offset, f_name, line_n);
}

/***
*
* void *_aligned_free(void *memblock)
*       - Free the memory which was allocated using _aligned_malloc or
*       _aligned_offset_memory
*
* Purpose:
*       Frees the algned memory block which was allocated using _aligned_malloc
*       or _aligned_memory.
*
* Entry:
*       void * memblock - pointer to the block of memory
*
*******************************************************************************/

extern "C" _CRTIMP void __cdecl _aligned_free(
        void *memblock
        )
{
    _aligned_free_dbg(memblock);
}


/***
*
* void *_aligned_free_dbg(void *memblock, const char * file_n, int line_n)
*       - Free the memory which was allocated using _aligned_malloc or
*       _aligned_offset_memory
*
* Purpose:
*       Frees the algned memory block which was allocated using _aligned_malloc
*       or _aligned_memory.
*
* Entry:
*       void * memblock - pointer to the block of memory
*
*******************************************************************************/

extern "C" _CRTIMP void __cdecl _aligned_free_dbg(
        void * memblock
        )
{
    _AlignMemBlockHdr *pHdr;

    if ( memblock == NULL)
        return;

    pHdr = (_AlignMemBlockHdr *)((uintptr_t)memblock & ~(sizeof(uintptr_t) -1)) -1;

    if ( CheckBytes((unsigned char *)memblock -nNoMansLandSize, _bNoMansLandFill, nNoMansLandSize))
    {
        _RPT1(_CRT_ERROR, "The block at 0x%p was not allocated by _aligned routines, use free()", memblock);
        return;
    }

    if(!CheckBytes(pHdr->Gap, _bAlignLandFill, nAlignGapSize))
    {
        _RPT1(_CRT_ERROR, "Damage before 0x%p which was allocated by aligned routine\n", memblock);
    }
    _free_dbg((void *)pHdr->pHead, _NORMAL_BLOCK);
}

/***
*
* size_t _CrtSetDebugFillThreshold(size_t _NewDebugFillThreshold)
*       - Set the new debug fill threshold used in string filling
*
* Purpose:
*       Set the new debug fill threshold used in string filling.
*       Return the previous value of the threshold.
*       Threshold == 0 means no filling.
*       Threshold == SIZE_MAX means alwyas fill to the maximum.
*       At startup, value is SIZE_MAX.
*
* Entry:
*       void * memblock - pointer to the block of memory
*
*******************************************************************************/

extern "C"
_CRTIMP size_t __cdecl _CrtSetDebugFillThreshold(
        size_t _NewDebugFillThreshold
        )
{
    size_t oldThreshold = __crtDebugFillThreshold;

    __crtDebugFillThreshold = _NewDebugFillThreshold;

    return oldThreshold;
}

/***
*int _CrtSetCheckCount(int fCheckCount)
*       - Control buffer count checks for secure "n" functions
*
*Purpose:
*       set __crtDebugCheckCount to the specified value
*
*Entry:
*       int fCheckCount - new value to be set
*
*Exit:
*       return previous flag value
*
*******************************************************************************/

extern "C"
_CRTIMP int __cdecl _CrtSetCheckCount(
        int fCheckCount
        )
{
    int oldCheckCount = __crtDebugCheckCount;
    return oldCheckCount;
}

/***
*int _CrtGetCheckCount() - get current state of buffer count check
*
*Purpose:
*       get the value of __crtDebugCheckCount
*
*Exit:
*       return current flag value
*
*******************************************************************************/

extern "C"
_CRTIMP int __cdecl _CrtGetCheckCount(
        void
        )
{
    return __crtDebugCheckCount;
}

#endif  /* _DEBUG */

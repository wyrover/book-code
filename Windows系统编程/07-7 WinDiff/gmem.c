

/****************************** Module Header *******************************
* Module Name: GMEM.C
*
* Memory utility functions.
*
* Functions:
*
* gmem_panic()
* gmem_init()
* gmem_get()
* gmem_free()
* gmem_freeall()
*
* Comments:
*
* Global heap functions - allocate and free many small
* pieces of memory by calling global alloc for large pieces
* and breaking them up. A heap contains a critical section, so
* multiple simultaneous calls to gmem_get and gmem_free will be
* protected.
*
* gmem_freeall should not be called until all other users have finished
* with the heap.
*
* Out-of-memory is not something we regard as normal.
* If we cannot allocate memory - we put up an abort-retry-ignore
* error, and only return from the function if the user selects ignore.
*
****************************************************************************/

#include <windows.h>
#include <memory.h>

#include "gutils.h"
#include "gutilsrc.h"

int gmem_panic(void);


/* ensure BLKSIZE is multiple of sizeof(DWORD) */
#define BLKSIZE         64               /* blk size in bytes */
#define ALLOCSIZE       32768
#define NBLKS           (ALLOCSIZE / BLKSIZE)
#define MAPSIZE         (NBLKS / 8)
#define MAPLONGS        (MAPSIZE / sizeof(DWORD))
#define TO_BLKS(x)      (((x) + BLKSIZE - 1) / BLKSIZE)

typedef struct seghdr {
        HANDLE hseg;
        CRITICAL_SECTION critsec;
        struct seghdr FAR * pnext;
        long nblocks;
        DWORD segmap[MAPLONGS];
} SEGHDR, FAR * SEGHDRP;


/* anything above this size, we alloc directly from global heap */
#define MAXGALLOC       20000


/***************************************************************************
 * Function: gmem_init
 *
 * Purpose:
 *
 * init heap - create first segment
 */
HANDLE APIENTRY
gmem_init(void)
{
        HANDLE hNew;
        SEGHDRP hp;

        /* retry all memory allocations after calling gmem_panic */
        do {
                hNew = GlobalAlloc(GHND, ALLOCSIZE);
                if (hNew == NULL) {
                        if (gmem_panic() == IDIGNORE) {
                                return(NULL);
                        }
                }
        } while  (hNew == NULL);

        hp = (SEGHDRP) GlobalLock(hNew);
        if (hp == NULL) {
                return(NULL);
        }
        hp->hseg = hNew;
        InitializeCriticalSection(&hp->critsec);
        hp->pnext = NULL;
        gbit_init(hp->segmap, NBLKS);
        gbit_alloc(hp->segmap, 1, TO_BLKS(sizeof(SEGHDR)));
        hp->nblocks = NBLKS - TO_BLKS(sizeof(SEGHDR));

        return(hNew);
}

/***************************************************************************
 * Function: gmem_get
 *
 * Purpose:
 *
 * Get memory from heap
 */
LPSTR APIENTRY
gmem_get(HANDLE hHeap, int len)
{
        SEGHDRP chainp;
        HANDLE hNew;
        SEGHDRP hp;
        LPSTR chp;
        long nblks;
        long start;
        long nfound;


        /* the heap is always locked (in gmem_init)- so having got the
         * pointer, we can always safely unlock it
         */
        chainp = (SEGHDRP) GlobalLock(hHeap);
        GlobalUnlock(hHeap);

        if (len < 1) {
                return(NULL);
        }

        /*
         * too big to be worth allocing from heap - get from globalalloc
         */
        if (len > MAXGALLOC) {
                /* retry all memory allocations after calling gmem_panic */
                do {
                        hNew = GlobalAlloc(GHND, len);
                        if (hNew == NULL) {
                                if (gmem_panic() == IDIGNORE) {
                                        return(NULL);
                                }
                        }
                } while  (hNew == NULL);

                chp = GlobalLock(hNew);
                if (chp == NULL) {
                        return(NULL);
                }
                return(chp);
        }


        /*
         * get critical section during all access to the heap itself
         */
        EnterCriticalSection(&chainp->critsec);

        nblks = TO_BLKS(len + sizeof(HANDLE));

        for (hp = chainp; hp !=NULL; hp = hp->pnext) {
                if (hp->nblocks >= nblks) {
                        nfound = gbit_findfree(hp->segmap, nblks,NBLKS, &start);
                        if (nfound >= nblks) {
                                gbit_alloc(hp->segmap, start, nblks);
                                hp->nblocks -= nblks;

                                /* convert blocknr to pointer
                                 * store seg handle in block
                                 */
                                chp = (LPSTR) hp;
                                chp = &chp[ (start-1) * BLKSIZE];
                                * ( (HANDLE FAR *) chp) = hp->hseg;
                                chp += sizeof(HANDLE);

                                break;
                        }
                }
        }
        if (hp == NULL) {
                /* retry all memory allocations after calling gmem_panic */
                do {
                        hNew = GlobalAlloc(GHND, ALLOCSIZE);
                        if (hNew == NULL) {
                                if (gmem_panic() == IDIGNORE) {
                                        LeaveCriticalSection(&chainp->critsec);
                                        return(NULL);
                                }
                        }
                } while  (hNew == NULL);

                hp = (SEGHDRP) GlobalLock(hNew);
                if (hp == NULL) {
                        LeaveCriticalSection(&chainp->critsec);
                        return(NULL);
                }
                hp->pnext = chainp->pnext;
                hp->hseg = hNew;
                chainp->pnext = hp;
                gbit_init(hp->segmap, NBLKS);
                gbit_alloc(hp->segmap, 1, TO_BLKS(sizeof(SEGHDR)));
                hp->nblocks = NBLKS - TO_BLKS(sizeof(SEGHDR));
                nfound = gbit_findfree(hp->segmap, nblks, NBLKS, &start);
                if (nfound >= nblks) {
                        gbit_alloc(hp->segmap, start, nblks);
                        hp->nblocks -= nblks;

                        /* convert block nr to pointer */
                        chp = (LPSTR) hp;
                        chp = &chp[ (start-1) * BLKSIZE];
                        /* add a handle into the block and skip past */
                        * ( (HANDLE FAR *) chp) = hp->hseg;
                        chp += sizeof(HANDLE);
                }
        }
        LeaveCriticalSection(&chainp->critsec);
        memset(chp, 0, len);
        return(chp);
}

/***************************************************************************
 * Function: gmem_free
 *
 * Purpose:
 *
 * Free memory alloced
 */
void APIENTRY
gmem_free(HANDLE hHeap, LPSTR ptr, int len)
{
        SEGHDRP chainp;
        SEGHDRP hp;
        HANDLE hmem;
        long nblks, blknr;
        LPSTR chp;

        if (len < 1) {
                return;
        }

        /*
         * allocs greater than MAXGALLOC are too big to be worth
         * allocing from the heap - they will have been allocated
         * directly from globalalloc
         */
        if (len > MAXGALLOC) {
                hmem = GlobalHandle( (LPSTR) ptr);
                GlobalUnlock(hmem);
                GlobalFree(hmem);
                return;
        }

        chainp = (SEGHDRP) GlobalLock(hHeap);
        EnterCriticalSection(&chainp->critsec);


        /* just before the ptr we gave the user, is the handle to
         * the block
         */
        chp = (LPSTR) ptr;
        chp -= sizeof(HANDLE);
        hmem = * ((HANDLE FAR *) chp);
        hp = (SEGHDRP) GlobalLock(hmem);

        nblks = TO_BLKS(len + sizeof(HANDLE));

        /* convert ptr to block nr */
        blknr = TO_BLKS( (unsigned) (chp - (LPSTR) hp) ) + 1;

        gbit_free(hp->segmap, blknr, nblks);
        hp->nblocks += nblks;

        GlobalUnlock(hmem);

   LeaveCriticalSection(&chainp->critsec);
        GlobalUnlock(hHeap);

}

/***************************************************************************
 * Function: gmem_freeall
 *
 * Purpose:
 *
 * Free heap
 */
void APIENTRY
gmem_freeall(HANDLE hHeap)
{
        SEGHDRP chainp;
        HANDLE hSeg;

        chainp = (SEGHDRP) GlobalLock(hHeap);
        /* this segment is always locked - so we need to unlock
         * it here as well as below
         */
        GlobalUnlock(hHeap);

        /* finished with the critical section  -
         * caller must ensure that at this point there is no
         * longer any contention
         */
        DeleteCriticalSection(&chainp->critsec);

        while (chainp != NULL) {
                hSeg = chainp->hseg;
                chainp = chainp->pnext;
                GlobalUnlock(hSeg);
                GlobalFree(hSeg);
        }
}

/***************************************************************************
 * Function: gmem_panic
 *
 * Purpose:
 *
 * A memory allocation attempt has failed. Return IDIGNORE to ignore the
 * error and return NULL to the caller, and IDRETRY to retry the allocation
 * attempt.
 */
int
gmem_panic(void)
{
        int code;
    	extern HANDLE hLibInst;
	TCHAR szBuf1[512];
    	TCHAR szBuf2[512];

        LoadString(hLibInst, IDS_MEMORY_ALLOC_FAIL, szBuf1, sizeof(szBuf1));
        LoadString(hLibInst, IDS_OUT_OF_MEMORY, szBuf2, sizeof(szBuf2));

        code = MessageBox(NULL, szBuf1, szBuf2,
                        MB_ICONSTOP|MB_ABORTRETRYIGNORE);
        if (code == IDABORT) {
                /* abort this whole process */
                ExitProcess(1);
        } else {
                return(code);
        }
}


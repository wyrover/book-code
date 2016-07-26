/***
*tidprint.c - Dislpay thread data
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Display the per thread data table.
*
*       [NOTE: This module is NOT included in the C runtime libraries but
*       is maintained for debugging analysis.]
*
*******************************************************************************/

#include <cruntime.h>
#include <stdio.h>
#include <mtdll.h>

void __cdecl _print_tiddata(unsigned long);
void __cdecl _print_tiddata1(_ptiddata);

/***
*void _print_tiddata(unsigned long) - Display data for a thread
*
*Purpose:
*       This routine displays the per thread data for a specific, or all,
*       active threads in the _ptd[] table.
*
*Entry:
*       unsigned long = <n> = ID of the thread to display
*                     = -1  = Display thread data for all threads
*
*Exit:
*       <void>
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _print_tiddata (
        unsigned long tid
        )
{
        int i;                  /* loop index */
        int threadcnt;          /* number of active threads */

        /*
         * lock the _ptd[] table.
         */
        _mlock(_THREADDATA_LOCK);

        /*
         * see if caller want's all threads or just a specific one.
         */
        if (tid == (unsigned long) -1L) {
                /*
                 * caller want's all threads!
                 */
                for ( i = threadcnt = 0 ; i < 1024 ; i++ )
                        /*
                         * print out the fields of *_ptd[i] for each entry
                         * bound to an active thread (i.e., for each i st
                         * _ptd[i] non-NULL). also, count up the total number
                         * of active threads.
                         */
                        if ( _ptd[i] != NULL ) {
                                threadcnt++;
                                _print_tiddata1(_ptd[i]);
                        }

                printf("\nTHERE ARE %d CURRENTLY ACTIVE THREADS!\n", threadcnt);
        }
        else {
                /*
                 * caller just interested in a particular thread. search
                 * the _ptd[] table inline because a call to _getptd[] would
                 * have unpleasant side effects if tid is not (or no longer)
                 * valid.
                 */
                for ( i = 0 ; (i < 1024) && ((_ptd[i] == NULL) ||
                    (_ptd[i] == (_ptiddata)1L) || (_ptd[i]->_tid != tid)) ;
                    i++ ) ;

                if ( i < 1024 )
                        _print_tiddata1(_ptd[i]);
                else
                        printf("\nTID INVALID OR THREAD HAS TERMINATED!\n");
        }

        /*
         * unlock the _ptd[] table.
         */
        _munlock(_THREADDATA_LOCK);

}


/***
* void _print_tiddata1(_ptiddata ptd) - print out _tiddata structure
*
*Purpose:
*       Given a pointer to a thread data structure, print out its contents
*
*Entry:
*       ptd = pointer to thread's data area
*
*Exit:
*       <void>
*
*Exceptions:
*
*******************************************************************************/

void __cdecl _print_tiddata1 (
        _ptiddata ptd
        )
{
        printf("\t_tid            = %lu\n",  ptd->_tid );
        printf("\t_thandle        = %lu\n",  ptd->_thandle );
        printf("\t_terrno         = %d\n",   ptd->_terrno);
        printf("\t_tdoserrno      = %d\n",   ptd->_tdoserrno);
        printf("\t_fpds           = %#x\n",  ptd->_fpds);
        printf("\t_holdrand       = %u\n",   ptd->_holdrand);
        printf("\t_token          = %p\n",   ptd->_token);
        printf("\t_errmsg         = %p\n",   ptd->_errmsg);
        printf("\t_namebuf        = %p\n",   ptd->_namebuf);
        printf("\t_asctimebuf     = %p\n",   ptd->_asctimebuf);
        printf("\t_wasctimebuf    = %p\n",   ptd->_wasctimebuf);
        printf("\t_gmtimebuf      = %p\n",   ptd->_gmtimebuf);
        printf("\t_initaddr       = %p\n",   ptd->_initaddr);
        printf("\t_initarg        = %p\n",   ptd->_initarg);
        printf("\t_pxcptacttab    = %p\n",   ptd->_pxcptacttab);
        printf("\t_tpxcptinfoptrs = %p\n",   ptd->_tpxcptinfoptrs);
        printf("\t_tfpecode       = %p\n\n", ptd->_tfpecode);

}

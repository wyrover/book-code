/***
*winsig.c - C signal support
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines signal(), raise() and supporting functions.
*
*******************************************************************************/


#include <cruntime.h>
#include <errno.h>
#include <float.h>
#include <malloc.h>
#include <mtdll.h>
#include <oscalls.h>
#include <signal.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <dbgint.h>
#include <internal.h>

/*
 * look up the first entry in the exception-action table corresponding to
 * the given signal
 */
static struct _XCPT_ACTION * __cdecl siglookup(int, struct _XCPT_ACTION *);

/*
 * variables holding action codes (and code pointers) for SIGINT, SIGBRK,
 * SIGABRT and SIGTERM.
 *
 * note that the disposition (i.e., action to be taken upon receipt) of
 * these signals is defined on a per-process basis (not per-thread)!!
 */

static _PHNDLR ctrlc_action       = SIG_DFL;    /* SIGINT   */
static _PHNDLR ctrlbreak_action   = SIG_DFL;    /* SIGBREAK */
static _PHNDLR abort_action       = SIG_DFL;    /* SIGABRT  */
static _PHNDLR term_action        = SIG_DFL;    /* SIGTERM  */

/*
 * flag indicated whether or not a handler has been installed to capture
 * ^C and ^Break events.
 */
static int ConsoleCtrlHandler_Installed = 0;

#define _SIGHUP_IGNORE 1
#define _SIGQUIT_IGNORE 3
#define _SIGPIPE_IGNORE 13
#define _SIGIOINT_IGNORE 16
#define _SIGSTOP_IGNORE 17

void __cdecl _initp_misc_winsig(void* enull)
{
    ctrlc_action       = (_PHNDLR) enull;    /* SIGINT   */
    ctrlbreak_action   = (_PHNDLR) enull;    /* SIGBREAK */
    abort_action       = (_PHNDLR) enull;    /* SIGABRT  */
    term_action        = (_PHNDLR) enull;    /* SIGTERM  */
}

/***
*static BOOL WINAPI ctrlevent_capture(DWORD CtrlType) - capture ^C and ^Break events
*
*Purpose:
*       Capture ^C and ^Break events from the console and dispose of them
*       according the values in ctrlc_action and ctrlbreak_action, resp.
*       This is the routine that evokes the user-defined action for SIGINT
*       (^C) or SIGBREAK (^Break) installed by a call to signal().
*
*Entry:
*       DWORD CtrlType  - indicates type of event, two values:
*                               CTRL_C_EVENT
*                               CTRL_BREAK_EVENT
*
*Exit:
*       Returns TRUE to indicate the event (signal) has been handled.
*       Otherwise, returns FALSE.
*
*Exceptions:
*
*******************************************************************************/

static BOOL WINAPI ctrlevent_capture (
        DWORD CtrlType
        )
{
        _PHNDLR ctrl_action;
        _PHNDLR *pctrl_action;
        int sigcode;

        _mlock(_SIGNAL_LOCK);
        __try {

        /*
         * Identify the type of event and fetch the corresponding action
         * description.
         */

        if ( CtrlType == CTRL_C_EVENT ) {
                pctrl_action = &ctrlc_action;
                ctrl_action = (_PHNDLR) _decode_pointer(*pctrl_action);
                sigcode = SIGINT;
        }
        else {
                pctrl_action = &ctrlbreak_action;
                ctrl_action = (_PHNDLR) _decode_pointer(*pctrl_action);
                sigcode = SIGBREAK;
        }

        if ( !(ctrl_action == SIG_DFL) && !(ctrl_action == SIG_IGN) )
                /*
                 * Reset the action to be SIG_DFL
                 */
                *pctrl_action = (_PHNDLR) _encoded_null();

        }
        __finally {
                _munlock(_SIGNAL_LOCK);
        }

        if ( ctrl_action == SIG_DFL )
                /*
                 * return FALSE, indicating the event has NOT been handled
                 */
                return FALSE;

        if ( ctrl_action != SIG_IGN ) {
                (*ctrl_action)(sigcode);
        }

        /*
         * Return TRUE, indicating the event has been handled (which may
         * mean it's being ignored)
         */
        return TRUE;
}



/***
*_PHNDLR signal(signum, sigact) - Define a signal handler
*
*Purpose:
*       The signal routine allows the user to define what action should
*       be taken when various signals occur. The Win32/Dosx32 implementation
*       supports seven signals, divided up into three general groups
*
*       1. Signals corresponding to OS exceptions. These are:
*                       SIGFPE
*                       SIGILL
*                       SIGSEGV
*          Signal actions for these signals are installed by altering the
*          XcptAction and SigAction fields for the appropriate entry in the
*          exception-action table (XcptActTab[]).
*
*       2. Signals corresponding to ^C and ^Break. These are:
*                       SIGINT
*                       SIGBREAK
*          Signal actions for these signals are installed by altering the
*          _ctrlc_action and _ctrlbreak_action variables.
*
*       3. Signals which are implemented only in the runtime. That is, they
*          occur only as the result of a call to raise().
*                       SIGABRT
*                       SIGTERM
*
*
*Entry:
*       int signum      signal type. recognized signal types are:
*
*                       SIGABRT         (ANSI)
*                       SIGBREAK
*                       SIGFPE          (ANSI)
*                       SIGILL          (ANSI)
*                       SIGINT          (ANSI)
*                       SIGSEGV         (ANSI)
*                       SIGTERM         (ANSI)
*                       SIGABRT_COMPAT
*
*       _PHNDLR sigact  signal handling function or action code. the action
*                       codes are:
*
*                       SIG_DFL - take the default action, whatever that may
*                       be, upon receipt of this type type of signal.
*
*                       SIG_DIE - *** ILLEGAL ***
*                       special code used in the XcptAction field of an
*                       XcptActTab[] entry to indicate that the runtime is
*                       to terminate the process upon receipt of the exception.
*                       not accepted as a value for sigact.
*
*                       SIG_IGN - ignore this type of signal
*
*                       [function address] - transfer control to this address
*                       when a signal of this type occurs.
*
*Exit:
*       Good return:
*       Signal returns the previous value of the signal handling function
*       (e.g., SIG_DFL, SIG_IGN, etc., or [function address]). This value is
*       returned in DX:AX.
*
*       Error return:
*       Signal returns -1 and errno is set to EINVAL. The error return is
*       generally taken if the user submits bogus input values.
*
*Exceptions:
*       None.
*
*******************************************************************************/

_PHNDLR __cdecl signal(
        int signum,
        _PHNDLR sigact
        )
{
        struct _XCPT_ACTION *pxcptact;
        _PHNDLR oldsigact;
        int Error=0;
        _ptiddata ptd;
        BOOL SetConsoleCtrlError = FALSE;

        /*
         * Check for values of sigact supported on other platforms but not
         * on this one. Also, make sure sigact is not SIG_DIE
         */
        if ( (sigact == SIG_ACK) || (sigact == SIG_SGE) )
                goto sigreterror;

        /*
         * Take care of all signals which do not correspond to exceptions
         * in the host OS. Those are:
         *
         *                      SIGINT
         *                      SIGBREAK
         *                      SIGABRT
         *                      SIGTERM
         *
         */
        if ( (signum == SIGINT) || (signum == SIGBREAK) || (signum == SIGABRT)
            || (signum == SIGABRT_COMPAT) || (signum == SIGTERM) ) {

                _mlock( _SIGNAL_LOCK );
                __try {

                /*
                 * if SIGINT or SIGBREAK, make sure the handler is installed
                 * to capture ^C and ^Break events.
                 */
                if ( ((signum == SIGINT) || (signum == SIGBREAK)) &&
                    !ConsoleCtrlHandler_Installed )
                                {
                        if ( SetConsoleCtrlHandler(ctrlevent_capture, TRUE)
                            == TRUE )
                                                {
                                ConsoleCtrlHandler_Installed = TRUE;
                                                }
                        else
                                                {
                                _doserrno = GetLastError();
                                                                SetConsoleCtrlError=TRUE;
                        }
                                }

                switch (signum) {

                        case SIGINT:
                                oldsigact = (_PHNDLR) _decode_pointer(ctrlc_action);
                                if(sigact!=SIG_GET)
                                {
                                    ctrlc_action = (_PHNDLR) _encode_pointer(sigact);
                                }
                                break;

                        case SIGBREAK:
                                oldsigact = (_PHNDLR) _decode_pointer(ctrlbreak_action);
                                if(sigact!=SIG_GET)
                                {
                                    ctrlbreak_action = (_PHNDLR) _encode_pointer(sigact);
                                }
                                break;

                        case SIGABRT:
                        case SIGABRT_COMPAT:
                                oldsigact = (_PHNDLR) _decode_pointer(abort_action);
                                if(sigact!=SIG_GET)
                                {
                                    abort_action = (_PHNDLR) _encode_pointer(sigact);
                                }
                                break;

                        case SIGTERM:
                                oldsigact = (_PHNDLR) _decode_pointer(term_action);
                                if(sigact!=SIG_GET)
                                {
                                    term_action = (_PHNDLR) _encode_pointer(sigact);
                                }
                                break;
                }

                }
                __finally {
                        _munlock( _SIGNAL_LOCK );
                }

                if (SetConsoleCtrlError) {
                    goto sigreterror;
                }
                goto sigretok;
        }

        /*
         * If we reach here, signum is supposed to be one the signals which
         * correspond to exceptions in the host OS. Those are:
         *
         *                      SIGFPE
         *                      SIGILL
         *                      SIGSEGV
         */

        /*
         * Make sure signum is one of the remaining supported signals.
         */
        if ( (signum != SIGFPE) && (signum != SIGILL) && (signum != SIGSEGV) )
                goto sigreterror;


        /*
         * Fetch the tid data table entry for this thread
         */
        ptd = _getptd_noexit();
        if (!ptd)
            goto sigreterror;

        /*
         * Check that there a per-thread instance of the exception-action
         * table for this thread. if there isn't, create one.
         */
        if ( ptd->_pxcptacttab == _XcptActTab )
                /*
                 * allocate space for an exception-action table
                 */
                if ( (ptd->_pxcptacttab = _malloc_crt(_XcptActTabSize)) != NULL )
                        /*
                         * initialize the table by copying over the contents
                         * of _XcptActTab[]
                         */
                        (void) memcpy(ptd->_pxcptacttab, _XcptActTab,
                            _XcptActTabSize);
                else
                        /*
                         * cannot create exception-action table, return
                         * error to caller
                         */
                        goto sigreterror;


        /*
         * look up the proper entry in the exception-action table. note that
         * if several exceptions are mapped to the same signal, this returns
         * the pointer to first such entry in the exception action table. it
         * is assumed that the other entries immediately follow this one.
         */
        if ( (pxcptact = siglookup(signum, ptd->_pxcptacttab)) == NULL )
                goto sigreterror;

        /*
         * SIGSEGV, SIGILL and SIGFPE all have more than one exception mapped
         * to them. the code below depends on the exceptions corresponding to
         * the same signal being grouped together in the exception-action
         * table.
         */

        /*
         * store old signal action code for return value
         */
        oldsigact = pxcptact->XcptAction;

        if(sigact!=SIG_GET)
        {
            /*
            * loop through all entries corresponding to the
            * given signal and update the SigAction and XcptAction
            * fields as appropriate
            */

            while ( pxcptact->SigNum == signum ) {
                    /*
                    * take care of the SIG_IGN and SIG_DFL action
                    * codes
                    */
                    pxcptact->XcptAction = sigact;

                    /*
                    * make sure we don't run off the end of the table
                    */
                    if ( ++pxcptact >= ((struct _XCPT_ACTION *)(ptd->_pxcptacttab)
                                       + _XcptActTabCount) )
                        break;
            }
        }

sigretok:
        return(oldsigact);

sigreterror:
        switch(signum)
        {
        case _SIGHUP_IGNORE:
        case _SIGQUIT_IGNORE:
        case _SIGPIPE_IGNORE:
        case _SIGIOINT_IGNORE:
        case _SIGSTOP_IGNORE:
            return SIG_ERR;
        default:
            _VALIDATE_RETURN(("Invalid signal or error", 0), EINVAL, SIG_ERR);
            /* should never happen, but compiler can't tell */
            return SIG_ERR;
        }
}

/***
*int raise(signum) - Raise a signal
*
*Purpose:
*       This routine raises a signal (i.e., performs the action currently
*       defined for this signal). The action associated with the signal is
*       evoked directly without going through intermediate dispatching or
*       handling.
*
*Entry:
*       int signum - signal type (e.g., SIGINT)
*
*Exit:
*       returns 0 on good return, -1 on bad return.
*
*Exceptions:
*       May not return.  Raise has no control over the action
*       routines defined for the various signals.  Those routines may
*       abort, terminate, etc.  In particular, the default actions for
*       certain signals will terminate the program.
*
*******************************************************************************/


int __cdecl raise (
        int signum
        )
{
        _PHNDLR sigact;
        _PHNDLR *psigact;
        PEXCEPTION_POINTERS oldpxcptinfoptrs;
        int oldfpecode;
        int indx;

        int siglock = 0;
        _ptiddata ptd=NULL;

        switch (signum) {

                case SIGINT:
                        sigact = *(psigact = &ctrlc_action);
                        siglock++;
                        break;

                case SIGBREAK:
                        sigact = *(psigact = &ctrlbreak_action);
                        siglock++;
                        break;

                case SIGABRT:
                case SIGABRT_COMPAT:
                        sigact = *(psigact = &abort_action);
                        siglock++;
                        break;

                case SIGTERM:
                        sigact = *(psigact = &term_action);
                        siglock++;
                        break;

                case SIGFPE:
                case SIGILL:
                case SIGSEGV:
                        ptd = _getptd_noexit();
                        if (!ptd)
                            return (-1);
                        sigact = *(psigact = &(siglookup( signum,
                            ptd->_pxcptacttab )->XcptAction));
                        goto decode_done;
                        break;

                default:
                        /*
                         * unsupported signal, return an error
                         */
                        _VALIDATE_RETURN(("Invalid signal or error", 0), EINVAL, -1);
        }
        sigact = (_PHNDLR) _decode_pointer(sigact);

decode_done:

        /*
         * If the current action is SIG_IGN, just return
         */
        if ( sigact == SIG_IGN )
                return(0);

        /*
         * If the current action is SIG_DFL, take the default action
         */
        if ( sigact == SIG_DFL ) {
                /*
                 * The current default action for all of the supported
                 * signals is to terminate with an exit code of 3.
                 */
                _exit(3);
        }

        /*
         * if signum is one of the 'process-wide' signals (i.e., SIGINT,
         * SIGBREAK, SIGABRT or SIGTERM), assert _SIGNAL_LOCK.
         */
        if ( siglock )
                _mlock(_SIGNAL_LOCK);

        __try {


        /*
         * From here on, sigact is assumed to be a pointer to a user-supplied
         * handler.
         */

        /*
         * For signals which correspond to exceptions, set the pointer
         * to the EXCEPTION_POINTERS structure to NULL
         */
        if ( (signum == SIGFPE) || (signum == SIGSEGV) ||
            (signum == SIGILL) ) {
                oldpxcptinfoptrs = ptd->_tpxcptinfoptrs;
                ptd->_tpxcptinfoptrs = NULL;

                 /*
                  * If signum is SIGFPE, also set _fpecode to
                  * _FPE_EXPLICITGEN
                  */
                if ( signum == SIGFPE ) {
                        oldfpecode = ptd->_tfpecode;
                        ptd->_tfpecode = _FPE_EXPLICITGEN;
                }
        }

        /*
         * Reset the action to SIG_DFL and call the user specified handler
         * routine.
         */
        if ( signum == SIGFPE )
                /*
                 * for SIGFPE, must reset the action for all of the floating
                 * point exceptions
                 */
                for ( indx = _First_FPE_Indx ;
                      indx < _First_FPE_Indx + _Num_FPE ;
                      indx++ )
                {
                        ( (struct _XCPT_ACTION *)(ptd->_pxcptacttab) +
                          indx )->XcptAction = SIG_DFL;
                }
        else
                *psigact = (_PHNDLR) _encoded_null();

        }
        __finally {
                if ( siglock )
                        _munlock(_SIGNAL_LOCK);
        }

        if ( signum == SIGFPE )
                /*
                 * Special code to support old SIGFPE handlers which
                 * expect the value of _fpecode as the second argument.
                 */
                (*(void (__cdecl *)(int,int))sigact)(SIGFPE,
                    ptd->_tfpecode);
        else
                (*sigact)(signum);

        /*
         * For signals which correspond to exceptions, restore the pointer
         * to the EXCEPTION_POINTERS structure.
         */
        if ( (signum == SIGFPE) || (signum == SIGSEGV) ||
            (signum == SIGILL) ) {
                ptd->_tpxcptinfoptrs = oldpxcptinfoptrs;

                 /*
                  * If signum is SIGFPE, also restore _fpecode
                  */
                if ( signum == SIGFPE )
                        ptd->_tfpecode = oldfpecode;
        }

        return(0);
}


/***
*struct _XCPT_ACTION *siglookup(int signum) - look up exception-action table
*       entry for signal.
*
*Purpose:
*       Find the first entry int _XcptActTab[] whose SigNum field is signum.
*
*Entry:
*       int signum - C signal type (e.g., SIGINT)
*
*Exit:
*       If successful, pointer to the table entry. If no such entry, NULL is
*       returned.
*
*Exceptions:
*
*******************************************************************************/


static struct _XCPT_ACTION * __cdecl siglookup (
        int signum,
        struct _XCPT_ACTION *pxcptacttab
        )
{
        struct _XCPT_ACTION *pxcptact = pxcptacttab;

        /*
         * walk thru the _xcptactab table looking for the proper entry. note
         * that in the case where more than one exception corresponds to the
         * same signal, the first such instance in the table is the one
         * returned.
         */

        while ( (pxcptact->SigNum != signum) &&
                (++pxcptact < pxcptacttab + _XcptActTabCount) ) ;


        if ( (pxcptact < (pxcptacttab + _XcptActTabCount)) &&
             (pxcptact->SigNum == signum) )
                /*
                 * found a table entry corresponding to the signal
                 */
                return(pxcptact);
        else
                /*
                 * found no table entry corresponding to the signal
                 */
                return(NULL);
}


/***
*_PHNDLR __cdecl __get_sigabrt(void) - return the SIGABRT signal handling function
*
*Purpose:
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

_PHNDLR __cdecl __get_sigabrt(void)
{
    return (_PHNDLR) _decode_pointer(abort_action);
}



/***
*int *__fpecode(void) - return pointer to _fpecode field of the tidtable entry
*       for the current thread
*
*Purpose:
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

int * __cdecl __fpecode (
        void
        )
{
        return( &(_getptd()->_tfpecode) );
}


/***
*void **__pxcptinfoptrs(void) - return pointer to _pxcptinfoptrs field of the
*       tidtable entry for the current thread
*
*Purpose:
*
*Entry:
*
*Exit:
*
*Exceptions:
*
*******************************************************************************/

void ** __cdecl __pxcptinfoptrs (
        void
        )
{
        return( &(_getptd()->_tpxcptinfoptrs) );
}



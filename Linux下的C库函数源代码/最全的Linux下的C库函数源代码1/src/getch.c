/***
*getch.c - contains _getch(), _getche(), _ungetch() and kbhit for Win32
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Defines the "direct console" functions listed above.
*
*       NOTE: The real-mode DOS versions of these functions read from
*       standard input and are therefore redirected when standard input
*       is redirected. However, these versions ALWAYS read from the console,
*       even when standard input is redirected.
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>
#include <conio.h>
#include <internal.h>
#include <mtdll.h>
#include <stdio.h>
#include <stdlib.h>
#include <dbgint.h>
#include <malloc.h>

typedef struct {
        unsigned char LeadChar;
        unsigned char SecondChar;
        } CharPair;

typedef struct {
        unsigned short ScanCode;
        CharPair RegChars;
        CharPair ShiftChars;
        CharPair CtrlChars;
        CharPair AltChars;
        } EnhKeyVals;

typedef struct {
        CharPair RegChars;
        CharPair ShiftChars;
        CharPair CtrlChars;
        CharPair AltChars;
        } NormKeyVals;

/*
 * Table of key values for enhanced keys
 */
const static EnhKeyVals EnhancedKeys[] = {
        { 28, {  13,   0 }, {  13,   0 }, {  10,   0 }, {   0, 166 } },
        { 53, {  47,   0 }, {  63,   0 }, {   0, 149 }, {   0, 164 } },
        { 71, { 224,  71 }, { 224,  71 }, { 224, 119 }, {   0, 151 } },
        { 72, { 224,  72 }, { 224,  72 }, { 224, 141 }, {   0, 152 } },
        { 73, { 224,  73 }, { 224,  73 }, { 224, 134 }, {   0, 153 } },
        { 75, { 224,  75 }, { 224,  75 }, { 224, 115 }, {   0, 155 } },
        { 77, { 224,  77 }, { 224,  77 }, { 224, 116 }, {   0, 157 } },
        { 79, { 224,  79 }, { 224,  79 }, { 224, 117 }, {   0, 159 } },
        { 80, { 224,  80 }, { 224,  80 }, { 224, 145 }, {   0, 160 } },
        { 81, { 224,  81 }, { 224,  81 }, { 224, 118 }, {   0, 161 } },
        { 82, { 224,  82 }, { 224,  82 }, { 224, 146 }, {   0, 162 } },
        { 83, { 224,  83 }, { 224,  83 }, { 224, 147 }, {   0, 163 } }
        };

/*
 * macro for the number of elements of in EnhancedKeys[]
 */
#define NUM_EKA_ELTS    ( sizeof( EnhancedKeys ) / sizeof( EnhKeyVals ) )

/*
 * Table of key values for normal keys. Note that the table is padded so
 * that the key scan code serves as an index into the table.
 */
const static NormKeyVals NormalKeys[] = {

        /* padding */
        { /*  0 */ {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },

        { /*  1 */ {  27,   0 }, {  27,   0 }, {  27,   0 }, {   0,   1 } },
        { /*  2 */ {  49,   0 }, {  33,   0 }, {   0,   0 }, {   0, 120 } },
        { /*  3 */ {  50,   0 }, {  64,   0 }, {   0,   3 }, {   0, 121 } },
        { /*  4 */ {  51,   0 }, {  35,   0 }, {   0,   0 }, {   0, 122 } },
        { /*  5 */ {  52,   0 }, {  36,   0 }, {   0,   0 }, {   0, 123 } },
        { /*  6 */ {  53,   0 }, {  37,   0 }, {   0,   0 }, {   0, 124 } },
        { /*  7 */ {  54,   0 }, {  94,   0 }, {  30,   0 }, {   0, 125 } },
        { /*  8 */ {  55,   0 }, {  38,   0 }, {   0,   0 }, {   0, 126 } },
        { /*  9 */ {  56,   0 }, {  42,   0 }, {   0,   0 }, {   0, 127 } },
        { /* 10 */ {  57,   0 }, {  40,   0 }, {   0,   0 }, {   0, 128 } },
        { /* 11 */ {  48,   0 }, {  41,   0 }, {   0,   0 }, {   0, 129 } },
        { /* 12 */ {  45,   0 }, {  95,   0 }, {  31,   0 }, {   0, 130 } },
        { /* 13 */ {  61,   0 }, {  43,   0 }, {   0,   0 }, {   0, 131 } },
        { /* 14 */ {   8,   0 }, {   8,   0 }, { 127,   0 }, {   0,  14 } },
        { /* 15 */ {   9,   0 }, {   0,  15 }, {   0, 148 }, {   0,  15 } },
        { /* 16 */ { 113,   0 }, {  81,   0 }, {  17,   0 }, {   0,  16 } },
        { /* 17 */ { 119,   0 }, {  87,   0 }, {  23,   0 }, {   0,  17 } },
        { /* 18 */ { 101,   0 }, {  69,   0 }, {   5,   0 }, {   0,  18 } },
        { /* 19 */ { 114,   0 }, {  82,   0 }, {  18,   0 }, {   0,  19 } },
        { /* 20 */ { 116,   0 }, {  84,   0 }, {  20,   0 }, {   0,  20 } },
        { /* 21 */ { 121,   0 }, {  89,   0 }, {  25,   0 }, {   0,  21 } },
        { /* 22 */ { 117,   0 }, {  85,   0 }, {  21,   0 }, {   0,  22 } },
        { /* 23 */ { 105,   0 }, {  73,   0 }, {   9,   0 }, {   0,  23 } },
        { /* 24 */ { 111,   0 }, {  79,   0 }, {  15,   0 }, {   0,  24 } },
        { /* 25 */ { 112,   0 }, {  80,   0 }, {  16,   0 }, {   0,  25 } },
        { /* 26 */ {  91,   0 }, { 123,   0 }, {  27,   0 }, {   0,  26 } },
        { /* 27 */ {  93,   0 }, { 125,   0 }, {  29,   0 }, {   0,  27 } },
        { /* 28 */ {  13,   0 }, {  13,   0 }, {  10,   0 }, {   0,  28 } },

        /* padding */
        { /* 29 */ {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },

        { /* 30 */ {  97,   0 }, {  65,   0 }, {   1,   0 }, {   0,  30 } },
        { /* 31 */ { 115,   0 }, {  83,   0 }, {  19,   0 }, {   0,  31 } },
        { /* 32 */ { 100,   0 }, {  68,   0 }, {   4,   0 }, {   0,  32 } },
        { /* 33 */ { 102,   0 }, {  70,   0 }, {   6,   0 }, {   0,  33 } },
        { /* 34 */ { 103,   0 }, {  71,   0 }, {   7,   0 }, {   0,  34 } },
        { /* 35 */ { 104,   0 }, {  72,   0 }, {   8,   0 }, {   0,  35 } },
        { /* 36 */ { 106,   0 }, {  74,   0 }, {  10,   0 }, {   0,  36 } },
        { /* 37 */ { 107,   0 }, {  75,   0 }, {  11,   0 }, {   0,  37 } },
        { /* 38 */ { 108,   0 }, {  76,   0 }, {  12,   0 }, {   0,  38 } },
        { /* 39 */ {  59,   0 }, {  58,   0 }, {   0,   0 }, {   0,  39 } },
        { /* 40 */ {  39,   0 }, {  34,   0 }, {   0,   0 }, {   0,  40 } },
        { /* 41 */ {  96,   0 }, { 126,   0 }, {   0,   0 }, {   0,  41 } },

        /* padding */
        { /* 42 */ {    0,  0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },

        { /* 43 */ {  92,   0 }, { 124,   0 }, {  28,   0 }, {   0,   0 } },
        { /* 44 */ { 122,   0 }, {  90,   0 }, {  26,   0 }, {   0,  44 } },
        { /* 45 */ { 120,   0 }, {  88,   0 }, {  24,   0 }, {   0,  45 } },
        { /* 46 */ {  99,   0 }, {  67,   0 }, {   3,   0 }, {   0,  46 } },
        { /* 47 */ { 118,   0 }, {  86,   0 }, {  22,   0 }, {   0,  47 } },
        { /* 48 */ {  98,   0 }, {  66,   0 }, {   2,   0 }, {   0,  48 } },
        { /* 49 */ { 110,   0 }, {  78,   0 }, {  14,   0 }, {   0,  49 } },
        { /* 50 */ { 109,   0 }, {  77,   0 }, {  13,   0 }, {   0,  50 } },
        { /* 51 */ {  44,   0 }, {  60,   0 }, {   0,   0 }, {   0,  51 } },
        { /* 52 */ {  46,   0 }, {  62,   0 }, {   0,   0 }, {   0,  52 } },
        { /* 53 */ {  47,   0 }, {  63,   0 }, {   0,   0 }, {   0,  53 } },

        /* padding */
        { /* 54 */ {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },

        { /* 55 */ {  42,   0 }, {   0,   0 }, { 114,   0 }, {   0,   0 } },

        /* padding */
        { /* 56 */ {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },

        { /* 57 */ {  32,   0 }, {  32,   0 }, {  32,   0 }, {  32,   0 } },

        /* padding */
        { /* 58 */ {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },

        { /* 59 */ {   0,  59 }, {   0,  84 }, {   0,  94 }, {   0, 104 } },
        { /* 60 */ {   0,  60 }, {   0,  85 }, {   0,  95 }, {   0, 105 } },
        { /* 61 */ {   0,  61 }, {   0,  86 }, {   0,  96 }, {   0, 106 } },
        { /* 62 */ {   0,  62 }, {   0,  87 }, {   0,  97 }, {   0, 107 } },
        { /* 63 */ {   0,  63 }, {   0,  88 }, {   0,  98 }, {   0, 108 } },
        { /* 64 */ {   0,  64 }, {   0,  89 }, {   0,  99 }, {   0, 109 } },
        { /* 65 */ {   0,  65 }, {   0,  90 }, {   0, 100 }, {   0, 110 } },
        { /* 66 */ {   0,  66 }, {   0,  91 }, {   0, 101 }, {   0, 111 } },
        { /* 67 */ {   0,  67 }, {   0,  92 }, {   0, 102 }, {   0, 112 } },
        { /* 68 */ {   0,  68 }, {   0,  93 }, {   0, 103 }, {   0, 113 } },

        /* padding */
        { /* 69 */ {    0,  0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
        { /* 70 */ {    0,  0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },

        { /* 71 */ {   0,  71 }, {  55,   0 }, {   0, 119 }, {   0,   0 } },
        { /* 72 */ {   0,  72 }, {  56,   0 }, {   0, 141 }, {   0,   0 } },
        { /* 73 */ {   0,  73 }, {  57,   0 }, {   0, 132 }, {   0,   0 } },
        { /* 74 */ {   0,   0 }, {  45,   0 }, {   0,   0 }, {   0,   0 } },
        { /* 75 */ {   0,  75 }, {  52,   0 }, {   0, 115 }, {   0,   0 } },
        { /* 76 */ {   0,   0 }, {  53,   0 }, {   0,   0 }, {   0,   0 } },
        { /* 77 */ {   0,  77 }, {  54,   0 }, {   0, 116 }, {   0,   0 } },
        { /* 78 */ {   0,   0 }, {  43,   0 }, {   0,   0 }, {   0,   0 } },
        { /* 79 */ {   0,  79 }, {  49,   0 }, {   0, 117 }, {   0,   0 } },
        { /* 80 */ {   0,  80 }, {  50,   0 }, {   0, 145 }, {   0,   0 } },
        { /* 81 */ {   0,  81 }, {  51,   0 }, {   0, 118 }, {   0,   0 } },
        { /* 82 */ {   0,  82 }, {  48,   0 }, {   0, 146 }, {   0,   0 } },
        { /* 83 */ {   0,  83 }, {  46,   0 }, {   0, 147 }, {   0,   0 } },

        /* padding */
        { /* 84 */ {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
        { /* 85 */ {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },
        { /* 86 */ {   0,   0 }, {   0,   0 }, {   0,   0 }, {   0,   0 } },

        { /* 87 */ { 224, 133 }, { 224, 135 }, { 224, 137 }, { 224, 139 } },
        { /* 88 */ { 224, 134 }, { 224, 136 }, { 224, 138 }, { 224, 140 } }

};


/*
 * This is the one character push-back buffer used by _getch(), _getche()
 * and _ungetch().
 */
static int chbuf = EOF;


/*
 * Declaration for console handle
 */
extern intptr_t _coninpfh;

/*
 * Function that looks up the extended key code for a given event.
 */
const CharPair * __cdecl _getextendedkeycode(KEY_EVENT_RECORD *);


/***
*int _getch(), _getche() - read one char. from console (without and with echo)
*
*Purpose:
*       If the "_ungetch()" push-back buffer is not empty (empty==-1) Then
*           Mark it empty (-1) and RETURN the value that was in it
*       Read a character using ReadConsole in RAW mode
*       Return the Character Code
*       _getche(): Same as _getch() except that the character value returned
*       is echoed (via "_putch()")
*
*Entry:
*       None, reads from console.
*
*Exit:
*       If an error is returned from the API
*           Then EOF
*       Otherwise
*            next byte from console
*       Static variable "chbuf" may be altered
*
*Exceptions:
*
*******************************************************************************/


int __cdecl _getch (
        void
        )
{
        int ch;

        _mlock(_CONIO_LOCK);            /* secure the console lock */
        __TRY
            ch = _getch_nolock();               /* input the character */
        __FINALLY
            _munlock(_CONIO_LOCK);          /* release the console lock */
        __END_TRY_FINALLY

        return ch;
}

int __cdecl _getche (
        void
        )
{
        int ch;

        _mlock(_CONIO_LOCK);            /* secure the console lock */
        __TRY
            ch = _getche_nolock();              /* input and echo the character */
        __FINALLY
            _munlock(_CONIO_LOCK);          /* unlock the console */
        __END_TRY_FINALLY

        return ch;
}



int __cdecl _getch_nolock (
        void
        )
{
        INPUT_RECORD ConInpRec;
        DWORD NumRead;
        const CharPair *pCP;
        int ch = 0;                     /* single character buffer */
        DWORD oldstate;

        /*
         * check pushback buffer (chbuf) a for character
         */
        if ( chbuf != EOF ) {
            /*
             * something there, clear buffer and return the character.
             */
            ch = (unsigned char)(chbuf & 0xFF);
            chbuf = EOF;
            return ch;
        }

        /*
         * _coninpfh, the handle to the console input, is created the first
         * time that either _getch() or _cgets() or _kbhit() is called.
         */

        if ( _coninpfh == -2 )
            __initconin();

        if (_coninpfh == -1)
            return EOF;

        /*
         * Switch to raw mode (no line input, no echo input)
         */
        GetConsoleMode( (HANDLE)_coninpfh, &oldstate );
        SetConsoleMode( (HANDLE)_coninpfh, 0L );

        for ( ; ; ) {

            /*
             * Get a console input event.
             */
            if ( !ReadConsoleInput( (HANDLE)_coninpfh,
                                    &ConInpRec,
                                    1L,
                                    &NumRead )
                 || (NumRead == 0L) )
            {
                ch = EOF;
                break;
            }

            /*
             * Look for, and decipher, key events.
             */
            if ( (ConInpRec.EventType == KEY_EVENT) &&
                 ConInpRec.Event.KeyEvent.bKeyDown ) {
                /*
                 * Easy case: if uChar.AsciiChar is non-zero, just stuff it
                 * into ch and quit.
                 */

                if ( ch = (unsigned char)ConInpRec.Event.KeyEvent.uChar.AsciiChar )
                    break;

                /*
                 * Hard case: either an extended code or an event which should
                 * not be recognized. let _getextendedkeycode() do the work...
                 */
                if ( pCP = _getextendedkeycode( &(ConInpRec.Event.KeyEvent) ) ) {
                    ch = pCP->LeadChar;
                    chbuf = pCP->SecondChar;
                    break;
                }
            }
        }


        /*
         * Restore previous console mode.
         */
        SetConsoleMode( (HANDLE)_coninpfh, oldstate );

        return ch;
}


/*
 * getche is just getch followed by a putch if no error occurred
 */

int __cdecl _getche_nolock (
        void
        )
{
        int ch;                 /* character read */

        /*
         * check pushback buffer (chbuf) a for character. if found, return
         * it without echoing.
         */
        if ( chbuf != EOF ) {
            /*
             * something there, clear buffer and return the character.
             */
            ch = (unsigned char)(chbuf & 0xFF);
            chbuf = EOF;
            return ch;
        }

        ch = _getch_nolock();       /* read character */

        if (ch != EOF) {
                if (_putch_nolock(ch) != EOF) {
                        return ch;      /* if no error, return char */
                }
        }
        return EOF;                     /* get or put failed, return EOF */
}


/***
*int _kbhit() - return flag if a keystroke is already waiting to be read
*
*Purpose:
*       If the "_ungetch()" push-back buffer is not empty (empty==-1) Then
*           Return TRUE
*       Otherwise get the Keyboard Status (via DOSQUERYFHSTATE)
*
*Entry:
*       None, tests console.
*
*Exit:
*            returns 0 if no key waiting
*                = !0 if key waiting
*
*Exceptions:
*
*******************************************************************************/


int __cdecl _kbhit_nolock(void);

int __cdecl _kbhit (
        void
        )
{
        int retval;

        _mlock(_CONIO_LOCK);            /* secure the console lock */
        __TRY
            retval = _kbhit_nolock();           /* determine if a key is waiting */
        __FINALLY
            _munlock(_CONIO_LOCK);          /* release the console lock */
        __END_TRY_FINALLY

        return retval;
}

int __cdecl _kbhit_nolock (

        void
        )
{
        DWORD NumPending;
        DWORD NumPeeked;
        int ret = FALSE;
        PINPUT_RECORD pIRBuf=NULL;

        /*
         * if a character has been pushed back, return TRUE
         */
        if ( chbuf != -1 )
            return TRUE;

        /*
         * _coninpfh, the handle to the console input, is created the first
         * time that either _getch() or _cgets() or _kbhit() is called.
         */

        if ( _coninpfh == -2 )
            __initconin();

        /*
         * Peek all pending console events
         */
        if ( (_coninpfh == -1) ||

             !GetNumberOfConsoleInputEvents((HANDLE)_coninpfh, &NumPending) ||

             (NumPending == 0))
        {
            return FALSE;
        }

        pIRBuf=(PINPUT_RECORD)_calloca(NumPending, sizeof(INPUT_RECORD));
        if ( pIRBuf == NULL )
        {
            return FALSE;
        }

        if ( PeekConsoleInput( (HANDLE)_coninpfh,
                               pIRBuf,
                               NumPending,
                               &NumPeeked ) &&

             (NumPeeked != 0L) &&

             (NumPeeked <= NumPending) )
        {

            /*
             * Scan all of the peeked events to determine if any is a key event
             * which should be recognized.
             */
            PINPUT_RECORD pIR;
            for ( pIR = pIRBuf ; NumPeeked > 0 ; NumPeeked--, pIR++ ) {

                if ( (pIR->EventType == KEY_EVENT) &&

                     (pIR->Event.KeyEvent.bKeyDown) &&

                     ( pIR->Event.KeyEvent.uChar.AsciiChar ||
                       _getextendedkeycode( &(pIR->Event.KeyEvent) ) ) )
                {
                    /*
                     * Key event corresponding to an ASCII character or an
                     * extended code. In either case, success!
                     */
                    ret = TRUE;
                }
            }
        }

        _freea( pIRBuf );

        return ret;
}


/***
*int _ungetch(c) - push back one character for "_getch()" or "_getche()"
*
*Purpose:
*       If the Push-back buffer "chbuf" is -1 Then
*           Set "chbuf" to the argument and return the argument
*       Else
*           Return EOF to indicate an error
*
*Entry:
*       int c - Character to be pushed back
*
*Exit:
*       If successful
*           returns character that was pushed back
*       Else if error
*           returns EOF
*
*Exceptions:
*
*******************************************************************************/


int __cdecl _ungetch (
        int c
        )
{
        int retval;

        _mlock(_CONIO_LOCK);            /* lock the console */
        __TRY
            retval = _ungetch_nolock(c);        /* pushback character */
        __FINALLY
            _munlock(_CONIO_LOCK);          /* unlock the console */
        __END_TRY_FINALLY

        return retval;
}
int __cdecl _ungetch_nolock (

        int c
        )
{
        /*
         * Fail if the char is EOF or the pushback buffer is non-empty
         */
        if ( (c == EOF) || (chbuf != EOF) )
            return EOF;

        chbuf = (c & 0xFF);
        return chbuf;
}




/***
* static CharPair * _getextendedkeycode(pKE) - return extended code (if any)
*       for key event.
*
*Purpose:
*       Core function for _getch (and getche) and essential to _kbhit. This
*       is the function that determines whether or not a key event NOT
*       accompanied by an ASCII character has an extended code and returns
*       that code.
*
*Entry:
*       None, tests console.
*
*Exit:
*       if successful, returns a pointer to a CharPair value holding the lead
*       and second characters of the extended code.
*
*       if unsuccesful, returns NULL
*
*Exceptions:
*
*******************************************************************************/


const CharPair * __cdecl _getextendedkeycode (
        KEY_EVENT_RECORD *pKE
        )
{
        DWORD CKS;              /* hold dwControlKeyState value */
        const CharPair *pCP;    /* pointer to CharPair containing extended
                                   code */
        int i;

        if ( (CKS = pKE->dwControlKeyState) & ENHANCED_KEY ) {

            /*
             * Find the appropriate entry in EnhancedKeys[]
             */
            for ( pCP = NULL, i = 0 ; i < NUM_EKA_ELTS ; i++ ) {

                if ( EnhancedKeys[i].ScanCode == pKE->wVirtualScanCode ) {

                    if ( CKS & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED) )

                        pCP = &(EnhancedKeys[i].AltChars);

                    else if ( CKS & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED) )

                        pCP = &(EnhancedKeys[i].CtrlChars);

                    else if ( CKS & SHIFT_PRESSED)

                        pCP = &(EnhancedKeys[i].ShiftChars);

                    else

                        pCP = &(EnhancedKeys[i].RegChars);

                    break;

                }
            }

        }

        else {

            /*
             * Regular key or a keyboard event which shouldn't be recognized.
             * Determine which by getting the proper field of the proper
             * entry in NormalKeys[], and examining the extended code.
             */
            if ( CKS & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED) )

                pCP = &(NormalKeys[pKE->wVirtualScanCode].AltChars);

            else if ( CKS & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED) )

                pCP = &(NormalKeys[pKE->wVirtualScanCode].CtrlChars);

            else if ( CKS & SHIFT_PRESSED)

                pCP = &(NormalKeys[pKE->wVirtualScanCode].ShiftChars);

            else

                pCP = &(NormalKeys[pKE->wVirtualScanCode].RegChars);

            if ( ((pCP->LeadChar != 0) && (pCP->LeadChar != 224)) ||
                 (pCP->SecondChar == 0) )
                /*
                 * Must be a keyboard event which should not be recognized
                 * (e.g., shift key was pressed)
                 */
                pCP = NULL;

        }

        return(pCP);
}

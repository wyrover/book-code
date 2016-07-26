/***
*fgetwc_nolock.inl - get a wide character from a stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines fgetwc_nolock() - read a wide character from a stream
*
*******************************************************************************/

/***
*_getwc_nolock() -  getwc() core routine (locked version)
*
*Purpose:
*       Core getwc() routine; assumes stream is already locked.
*
*       [See getwc() above for more info.]
*
*Entry: [See getwc()]
*
*Exit:  [See getwc()]
*
*Exceptions:
*
*******************************************************************************/


wint_t __cdecl _fgetwc_nolock (
        REG1 FILE *stream
        )
{



        if(!(stream->_flag & _IOSTRG) && _textmode_safe(_fileno(stream)) != __IOINFO_TM_ANSI)
        {
            /* This is for files open with "u" - Unicode Mode. We need to read
            2 chars instead of 1. This is required even for UTF8, because read
            converts UTF8 to a regular Wide Char */

            int ch;
            wchar_t wch;

            char * p = (char *)&wch;

            ch = _getc_nolock(stream);
            if(ch == EOF)
                return WEOF;

            *p = (char)ch;
            ++p;

            ch = _getc_nolock(stream);
            if(ch == EOF)
                return WEOF;

            *p = (char)ch;

            return wch;

        }

        if (!(stream->_flag & _IOSTRG) && (_osfile_safe(_fileno(stream)) &
              FTEXT))
        {
                int size = 1;
                int ch;
                char mbc[4];
                wchar_t wch;

                /* text (multi-byte) mode */
                if ((ch = _getc_nolock(stream)) == EOF)
                        return WEOF;

                mbc[0] = (char)ch;

                if (isleadbyte((unsigned char)mbc[0]))
                {
                        if ((ch = _getc_nolock(stream)) == EOF)
                        {
                                ungetc(mbc[0], stream);
                                return WEOF;
                        }
                        mbc[1] = (char)ch;
                        size = 2;
                }
                if (mbtowc(&wch, mbc, size) == -1)
                {
                        /*
                         * Conversion failed! Set errno and return
                         * failure.
                         */
                        errno = EILSEQ;
                        return WEOF;
                }
                return wch;
        }
        /* binary (Unicode) mode */
        if ((stream->_cnt -= sizeof(wchar_t)) >= 0)
                return *((wchar_t *)(stream->_ptr))++;
        else
                return (wint_t) _filwbuf(stream);
}

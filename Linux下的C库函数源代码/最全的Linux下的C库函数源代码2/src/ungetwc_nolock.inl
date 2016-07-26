/***
*ungetwc.c - unget a wide character from a stream
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines ungetwc() - pushes a wide character back onto an input stream
*
*******************************************************************************/

/***
*_ungetwc_nolock() -  Ungetwc() core routine (locked version)
*
*Purpose:
*       Core ungetwc() routine; assumes stream is already locked.
*
*       [See ungetwc() above for more info.]
*
*Entry: [See ungetwc()]
*
*Exit:  [See ungetwc()]
*
*Exceptions:
*
*******************************************************************************/


wint_t __cdecl _ungetwc_nolock (
        wint_t ch,
        FILE *str
        )
{



        /*
         * Requirements for success:
         *
         * 1. Character to be pushed back on the stream must not be WEOF.
         *
         * 2. The stream must currently be in read mode, or must be open for
         *    update (i.e., read/write) and must NOT currently be in write
         *    mode.
         */
        if ( (ch != WEOF) &&
             ( (str->_flag & _IOREAD) || ((str->_flag & _IORW) &&
                !(str->_flag & _IOWRT))
             )
           )
        {
#ifndef _SAFECRT_IMPL
                /* This is always called in safecrt with base!= NULL - hence
                not needed */

                /* If stream is unbuffered, get one. */
                if (str->_base == NULL)
                        _getbuf(str);
#endif  /* _SAFECRT_IMPL */

                if (!(str->_flag & _IOSTRG) && (_osfile_safe(_fileno(str)) &
                    FTEXT))
                {
                        /*
                         * Text mode, sigh... Convert the wc to a mbc.
                         */
                        int size, i;
                        char mbc[MB_LEN_MAX];

#ifndef _SAFECRT_IMPL
                        if(_textmode_safe(_fileno(str)) != __IOINFO_TM_ANSI)
                        {
                            /* This is for files open with "u" - Unicode Mode.
                            The chars in the stream were originally UNICODE & not
                            MultiByte. Hence we don't need to do the wctomb. This
                            also holds for UTF-8, because read converts UTF8 to
                            regular UNICODE strings */

                            char *p = (char *)&ch;
                            mbc[0] = *p;
                            ++p;
                            mbc[1] = *p;

                            size = 2;

                        }
                        else
                        {
                            if (wctomb_s(&size, mbc, MB_LEN_MAX, ch) != 0)
                            {
                                /*
                                * Conversion failed; errno is set by wctomb_s;
                                * we return WEOF to indicate failure.
                                */
                                return WEOF;
                            }
                        }
#else  /* _SAFECRT_IMPL */
                        /* safecrt does not have wctombs_s yet */
_BEGIN_SECURE_CRT_DEPRECATION_DISABLE
                        if ((size = wctomb(mbc, ch)) == -1)
_END_SECURE_CRT_DEPRECATION_DISABLE
                        {
                                /*
                                    * Conversion failed! Set errno and return
                                    * failure.
                                    */
                                errno = EILSEQ;
                                return WEOF;
                        }
#endif  /* _SAFECRT_IMPL */

                        /* we know _base != NULL; since file is buffered */
                        if (str->_ptr < str->_base + size)
                        {
                                if (str->_cnt)
                                    /* my back is against the wall; i've already done
                                     * ungetwc, and there's no room for this one
                                     */
                                    return WEOF;
                                if (size > str->_bufsiz)
                                    return WEOF;
                                str->_ptr = size + str->_base;
                        }

                        for ( i = size -1; i >= 0; i--)
                        {
                                *--str->_ptr = mbc[i];
                        }
                        str->_cnt += size;

                        str->_flag &= ~_IOEOF;
                        str->_flag |= _IOREAD;  /* may already be set */
                        return (wint_t) (0x0ffff & ch);
                }
                /*
                 * Binary mode or a string (from swscanf) - push back the wide
                 * character
                 */

                /* we know _base != NULL; since file is buffered */
                if (str->_ptr < str->_base + sizeof(wchar_t))
                {
                        if (str->_cnt)
                                /* my back is against the wall; i've already done
                                 * ungetc, and there's no room for this one
                                 */
                                return WEOF;
                        if (sizeof(wchar_t) > str->_bufsiz)
                            return WEOF;
                        str->_ptr = sizeof(wchar_t) + str->_base;
                }

                if (str->_flag & _IOSTRG) {
                        /* If stream opened by swscanf do not modify buffer */
                        if (*--((wchar_t *)(str->_ptr)) != (wchar_t)ch) {
                                ++((wchar_t *)(str->_ptr));
                                return WEOF;
                        }
                } else
                        *--((wchar_t *)(str->_ptr)) = (wchar_t)(ch & 0xffff);

                str->_cnt += sizeof(wchar_t);

                str->_flag &= ~_IOEOF;
                str->_flag |= _IOREAD;  /* may already be set */

                return (wint_t)(ch & 0xffff);

        }
        return WEOF;
}

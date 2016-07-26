/***
*wcsncat.c - append n chars of string to new string
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       defines wcsncat() - appends n characters of string onto
*       end of other string
*
*******************************************************************************/


#include <cruntime.h>
#include <string.h>

/***
*wchar_t *wcsncat(front, back, count) - append count chars of back onto front
*
*Purpose:
*       Appends at most count characters of the string back onto the
*       end of front, and ALWAYS terminates with a null character.
*       If count is greater than the length of back, the length of back
*       is used instead.  (Unlike wcsncpy, this routine does not pad out
*       to count characters).
*
*Entry:
*       wchar_t *front - string to append onto
*       wchar_t *back - string to append
*       size_t count - count of max characters to append
*
*Exit:
*       returns a pointer to string appended onto (front).
*
*Uses:
*
*Exceptions:
*
*******************************************************************************/

wchar_t * __cdecl wcsncat (
        wchar_t * front,
        const wchar_t * back,
        size_t count
        )
{
        wchar_t *start = front;

        while (*front++)
                ;
        front--;

        while (count--)
                if (!(*front++ = *back++))
                        return(start);

        *front = L'\0';
        return(start);
}


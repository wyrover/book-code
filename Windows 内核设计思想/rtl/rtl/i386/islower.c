/* Copyright (C) 1995 DJ Delorie, see COPYING.DJ for details */
#include <ntoskrnl.h>
//#include <msvcrt/ctype.h>

#undef islower
int islower(int c)
{
    return _isctype(c, _LOWER);
}
#undef iswlower
int iswlower(wint_t c)
{
    return iswctype(c, _LOWER);
}

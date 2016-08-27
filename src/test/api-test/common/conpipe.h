#ifndef conpipe_h__
    #define conpipe_h__

    #include <stdio.h>

    int     pclose(FILE *);
    FILE    *openpipe(wchar_t *, unsigned short);

#endif // conpipe_h__

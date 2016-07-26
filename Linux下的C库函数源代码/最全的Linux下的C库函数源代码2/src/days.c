/***
*days.c - static arrays with days from beg of year for each month
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       contains static arrays used by gmtime and statconv to determine
*       date and time values.  Shows days from beg of year.
*
*******************************************************************************/

#include <internal.h>

int _lpdays[] = {
        -1, 30, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
};

int _days[] = {
        -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364
};

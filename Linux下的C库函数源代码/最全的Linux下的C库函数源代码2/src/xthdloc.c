/***
*xthdloc.c - sets the __globallocalestatus flag to disable per thread locale
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Sets the __globallocalestatus flag to enable per thread locale.
*       Link with this obj to enable per thread locale.
*
*******************************************************************************/

#include <cruntime.h>
#include <mtdll.h>
#include <internal.h>
#include <locale.h>
#include <setlocal.h>

int __globallocalestatus = (~_GLOBAL_LOCALE_BIT) ;


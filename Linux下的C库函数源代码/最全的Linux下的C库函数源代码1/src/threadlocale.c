/***
*threadlocale.c - sets the __globallocalestatus flag to enable per thread locale
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Sets the __globallocalestatus flag to enable per thread locale.
*       Link with this obj to enable per thread locale.
*
*******************************************************************************/

int __globallocalestatus = -1 ;

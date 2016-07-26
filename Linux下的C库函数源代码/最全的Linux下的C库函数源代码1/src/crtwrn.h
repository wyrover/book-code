/***
*crtwrn.h
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This file contains the IDs and messages for warnings
*       in the CRT headers.
*
****/

#pragma once

#ifndef _INC_CRTWRN
#define _INC_CRTWRN

#include <crtdefs.h>

#define __CRT_WARNING( _Number, _Description ) \
    message("" __FILE__ "(" _CRT_STRINGIZE(__LINE__) ") : " \
    "warning CRT" _CRT_STRINGIZE(_Number) ": " _CRT_STRINGIZE(_Description))

#define _CRT_WARNING( _Id ) \
    __CRT_WARNING( _CRTWRN_WNUMBER_##_Id, _CRTWRN_MESSAGE_##_Id )

/*
A warning is a 4-digit ID number (_CRTWRN_WNUMBER_*)
followed by a message (_CRTWRN_MESSAGE_*)
Emit a warning by adding the following code to the header file:
    #pragma _CRT_WARNING( id )
*/

/* NAME */
/* #pragma _CRT_WARNING( NAME ) */
/* #define _CRTWRN_WNUMBER_NAME  9999 */
/* #define _CRTWRN_MESSAGE_NAME  description */

/* _VCCLRIT_DEPRECATED */
/* #pragma _CRT_WARNING( _VCCLRIT_DEPRECATED ) */
#define _CRTWRN_WNUMBER__VCCLRIT_DEPRECATED  1001
#define _CRTWRN_MESSAGE__VCCLRIT_DEPRECATED  _vcclrit.h is deprecated

/* _EXPLICIT_KEYWORD_ */
/* #pragma _CRT_WARNING( _EXPLICIT_KEYWORD ) */
#define _CRTWRN_WNUMBER__EXPLICIT_KEYWORD    1002
#define _CRTWRN_MESSAGE__EXPLICIT_KEYWORD    \
       'explicit' keyword not recognized

/* _CANNOT_USE_32BIT_TIME_T */
/* #pragma _CRT_WARNING( _NO_32BIT_TIME_T ) */
#define _CRTWRN_WNUMBER__NO_32BIT_TIME_T  1003
#define _CRTWRN_MESSAGE__NO_32BIT_TIME_T  _USE_32BIT_TIME_T ignored. \
        You cannot use 32-bit time_t with _WIN64

/* _NO_SPECIAL_TRANSFER */
/* #pragma _CRT_WARNING( _NO_SPECIAL_TRANSFER ) */
#define _CRTWRN_WNUMBER__NO_SPECIAL_TRANSFER  1004
#define _CRTWRN_MESSAGE__NO_SPECIAL_TRANSFER Special transfer of \
        control routines not defined for this platform

/* _NO_ITERATOR_DEBUGGING */
/* #pragma _CRT_WARNING( _NO_ITERATOR_DEBUGGING ) */
#define _CRTWRN_WNUMBER__NO_ITERATOR_DEBUGGING  1005
#define _CRTWRN_MESSAGE__NO_ITERATOR_DEBUGGING Iterator debugging not \
        supported in retail builds.\n  \
        Setting _HAS_ITERA##TOR_DEBUGGING=0

#endif  /* _INC_CRTWRN */

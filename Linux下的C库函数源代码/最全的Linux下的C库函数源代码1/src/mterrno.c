/***
*mterrno.c - provide function versions of errno & _doserrno for LIBC.LIB
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Sometimes users want to compile code (such as for use in a library)
*       for both single-thread and multi-thread applications.  Currently the
*       one major stumbling block to doing this is the fact that errno &
*       _doserrno are defined in different ways in LIBC.LIB and LIBCMT.LIB.
*       Code that should otherwise be usable with both LIBC.LIB and LIBCMT.LIB
*       but which accesses errno and/or _doserrno is not usable with both.
*       By providing the function versions of errno & _doserrno in LIBC.LIB,
*       users can compile their code for both LIBCMT.LIB and LIBC.LIB.
*       Note that this does not magically make single-thread code safe in a
*       multi-threaded environment, it merely makes it easier to use the
*       same code with LIBC.LIB and LIBCMT.LIB.
*
*******************************************************************************/


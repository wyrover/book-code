/***
*stubs.c - extdef stubs
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       This module resolves external references made by the libs
*       in the "non-SYSCALL" version (i.e., the stripped down library
*       that has only routines that don't make system calls).
*
*******************************************************************************/

#include <cruntime.h>
#include <oscalls.h>

/*
 * referenced by crt (output)
 */

int _fltused = 0x9875;
int _ldused = 0x9873;
int __fastflag = 0;
int _iob;
char _osfile[20];
int errno;

void __cdecl fflush( void ){}
void __cdecl fprintf( void ){}
void __cdecl abort( void ){}
void __cdecl read( void ){}
void __cdecl _read( void ){}
void __cdecl _assert( void ) {}
void __cdecl _amsg_exit( void ) {}

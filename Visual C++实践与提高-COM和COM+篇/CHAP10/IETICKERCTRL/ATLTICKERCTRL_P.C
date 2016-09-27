

/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 6.00.0361 */
/* at Thu Jun 30 17:38:47 2005
 */
/* Compiler settings for E:\Software\BookCode\VCCOM\CHAP10\IETICKERCTRL\AtlTickerCtrl.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data
    VC __declspec() decoration level:
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
    #pragma warning(push)
#endif
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */
#pragma warning( disable: 4211 )  /* redefine extent to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
    #define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
    #error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "AtlTickerCtrl.h"

#define TYPE_FORMAT_STRING_SIZE   33
#define PROC_FORMAT_STRING_SIZE   493
#define TRANSMIT_AS_TABLE_SIZE    0
#define WIRE_MARSHAL_TABLE_SIZE   0

typedef struct _MIDL_TYPE_FORMAT_STRING {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
} MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
} MIDL_PROC_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax =
{{0x8A885D04, 0x1CEB, 0x11C9, {0x9F, 0xE8, 0x08, 0x00, 0x2B, 0x10, 0x48, 0x60}}, {2, 0}};


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ITicker_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ITicker_ProxyInfo;



#if !defined(__RPC_WIN32__)
    #error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
    #error You need a Windows 2000 or later to run this stub because it uses these features:
    #error   /robust command line switch.
    #error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
    #error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString = {
    0,
    {

        /* Procedure put_BackColor */

        0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /*  2 */    NdrFcLong(0x0),      /* 0 */
        /*  6 */    NdrFcShort(0x7),     /* 7 */
        /*  8 */    NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 10 */    NdrFcShort(0x8),     /* 8 */
        /* 12 */    NdrFcShort(0x8),     /* 8 */
        /* 14 */    0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 16 */    0x8,        /* 8 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 18 */    NdrFcShort(0x0),     /* 0 */
        /* 20 */    NdrFcShort(0x0),     /* 0 */
        /* 22 */    NdrFcShort(0x0),     /* 0 */

        /* Parameter clr */

        /* 24 */    NdrFcShort(0x48),    /* Flags:  in, base type, */
        /* 26 */    NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 28 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 30 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 32 */    NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 34 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_BackColor */

        /* 36 */    0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 38 */    NdrFcLong(0x0),      /* 0 */
        /* 42 */    NdrFcShort(0x8),     /* 8 */
        /* 44 */    NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 46 */    NdrFcShort(0x0),     /* 0 */
        /* 48 */    NdrFcShort(0x24),    /* 36 */
        /* 50 */    0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 52 */    0x8,        /* 8 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 54 */    NdrFcShort(0x0),     /* 0 */
        /* 56 */    NdrFcShort(0x0),     /* 0 */
        /* 58 */    NdrFcShort(0x0),     /* 0 */

        /* Parameter pclr */

        /* 60 */    NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
        /* 62 */    NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 64 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 66 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 68 */    NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 70 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_BorderColor */

        /* 72 */    0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 74 */    NdrFcLong(0x0),      /* 0 */
        /* 78 */    NdrFcShort(0x9),     /* 9 */
        /* 80 */    NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 82 */    NdrFcShort(0x8),     /* 8 */
        /* 84 */    NdrFcShort(0x8),     /* 8 */
        /* 86 */    0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 88 */    0x8,        /* 8 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 90 */    NdrFcShort(0x0),     /* 0 */
        /* 92 */    NdrFcShort(0x0),     /* 0 */
        /* 94 */    NdrFcShort(0x0),     /* 0 */

        /* Parameter clr */

        /* 96 */    NdrFcShort(0x48),    /* Flags:  in, base type, */
        /* 98 */    NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 100 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 102 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 104 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 106 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_BorderColor */

        /* 108 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 110 */   NdrFcLong(0x0),      /* 0 */
        /* 114 */   NdrFcShort(0xa),     /* 10 */
        /* 116 */   NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 118 */   NdrFcShort(0x0),     /* 0 */
        /* 120 */   NdrFcShort(0x24),    /* 36 */
        /* 122 */   0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 124 */   0x8,        /* 8 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 126 */   NdrFcShort(0x0),     /* 0 */
        /* 128 */   NdrFcShort(0x0),     /* 0 */
        /* 130 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter pclr */

        /* 132 */   NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
        /* 134 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 136 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 138 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 140 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 142 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure putref_Font */

        /* 144 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 146 */   NdrFcLong(0x0),      /* 0 */
        /* 150 */   NdrFcShort(0xb),     /* 11 */
        /* 152 */   NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 154 */   NdrFcShort(0x0),     /* 0 */
        /* 156 */   NdrFcShort(0x8),     /* 8 */
        /* 158 */   0x46,       /* Oi2 Flags:  clt must size, has return, has ext, */
        0x2,        /* 2 */
        /* 160 */   0x8,        /* 8 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 162 */   NdrFcShort(0x0),     /* 0 */
        /* 164 */   NdrFcShort(0x0),     /* 0 */
        /* 166 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter pFont */

        /* 168 */   NdrFcShort(0xb),     /* Flags:  must size, must free, in, */
        /* 170 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 172 */   NdrFcShort(0x6),     /* Type Offset=6 */

        /* Return value */

        /* 174 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 176 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 178 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_Font */

        /* 180 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 182 */   NdrFcLong(0x0),      /* 0 */
        /* 186 */   NdrFcShort(0xc),     /* 12 */
        /* 188 */   NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 190 */   NdrFcShort(0x0),     /* 0 */
        /* 192 */   NdrFcShort(0x8),     /* 8 */
        /* 194 */   0x46,       /* Oi2 Flags:  clt must size, has return, has ext, */
        0x2,        /* 2 */
        /* 196 */   0x8,        /* 8 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 198 */   NdrFcShort(0x0),     /* 0 */
        /* 200 */   NdrFcShort(0x0),     /* 0 */
        /* 202 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter pFont */

        /* 204 */   NdrFcShort(0xb),     /* Flags:  must size, must free, in, */
        /* 206 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 208 */   NdrFcShort(0x6),     /* Type Offset=6 */

        /* Return value */

        /* 210 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 212 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 214 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_Font */

        /* 216 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 218 */   NdrFcLong(0x0),      /* 0 */
        /* 222 */   NdrFcShort(0xd),     /* 13 */
        /* 224 */   NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 226 */   NdrFcShort(0x0),     /* 0 */
        /* 228 */   NdrFcShort(0x8),     /* 8 */
        /* 230 */   0x45,       /* Oi2 Flags:  srv must size, has return, has ext, */
        0x2,        /* 2 */
        /* 232 */   0x8,        /* 8 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 234 */   NdrFcShort(0x0),     /* 0 */
        /* 236 */   NdrFcShort(0x0),     /* 0 */
        /* 238 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter ppFont */

        /* 240 */   NdrFcShort(0x13),    /* Flags:  must size, must free, out, */
        /* 242 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 244 */   NdrFcShort(0x18),    /* Type Offset=24 */

        /* Return value */

        /* 246 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 248 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 250 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_ForeColor */

        /* 252 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 254 */   NdrFcLong(0x0),      /* 0 */
        /* 258 */   NdrFcShort(0xe),     /* 14 */
        /* 260 */   NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 262 */   NdrFcShort(0x8),     /* 8 */
        /* 264 */   NdrFcShort(0x8),     /* 8 */
        /* 266 */   0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 268 */   0x8,        /* 8 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 270 */   NdrFcShort(0x0),     /* 0 */
        /* 272 */   NdrFcShort(0x0),     /* 0 */
        /* 274 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter clr */

        /* 276 */   NdrFcShort(0x48),    /* Flags:  in, base type, */
        /* 278 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 280 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 282 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 284 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 286 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_ForeColor */

        /* 288 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 290 */   NdrFcLong(0x0),      /* 0 */
        /* 294 */   NdrFcShort(0xf),     /* 15 */
        /* 296 */   NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 298 */   NdrFcShort(0x0),     /* 0 */
        /* 300 */   NdrFcShort(0x24),    /* 36 */
        /* 302 */   0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 304 */   0x8,        /* 8 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 306 */   NdrFcShort(0x0),     /* 0 */
        /* 308 */   NdrFcShort(0x0),     /* 0 */
        /* 310 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter pclr */

        /* 312 */   NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
        /* 314 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 316 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 318 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 320 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 322 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure StartScroll */

        /* 324 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 326 */   NdrFcLong(0x0),      /* 0 */
        /* 330 */   NdrFcShort(0x10),    /* 16 */
        /* 332 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 334 */   NdrFcShort(0x0),     /* 0 */
        /* 336 */   NdrFcShort(0x8),     /* 8 */
        /* 338 */   0x44,       /* Oi2 Flags:  has return, has ext, */
        0x1,        /* 1 */
        /* 340 */   0x8,        /* 8 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 342 */   NdrFcShort(0x0),     /* 0 */
        /* 344 */   NdrFcShort(0x0),     /* 0 */
        /* 346 */   NdrFcShort(0x0),     /* 0 */

        /* Return value */

        /* 348 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 350 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 352 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure StopScroll */

        /* 354 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 356 */   NdrFcLong(0x0),      /* 0 */
        /* 360 */   NdrFcShort(0x11),    /* 17 */
        /* 362 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 364 */   NdrFcShort(0x0),     /* 0 */
        /* 366 */   NdrFcShort(0x8),     /* 8 */
        /* 368 */   0x44,       /* Oi2 Flags:  has return, has ext, */
        0x1,        /* 1 */
        /* 370 */   0x8,        /* 8 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 372 */   NdrFcShort(0x0),     /* 0 */
        /* 374 */   NdrFcShort(0x0),     /* 0 */
        /* 376 */   NdrFcShort(0x0),     /* 0 */

        /* Return value */

        /* 378 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 380 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 382 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure GetQuotes */

        /* 384 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 386 */   NdrFcLong(0x0),      /* 0 */
        /* 390 */   NdrFcShort(0x12),    /* 18 */
        /* 392 */   NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 394 */   NdrFcShort(0x0),     /* 0 */
        /* 396 */   NdrFcShort(0x22),    /* 34 */
        /* 398 */   0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 400 */   0x8,        /* 8 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 402 */   NdrFcShort(0x0),     /* 0 */
        /* 404 */   NdrFcShort(0x0),     /* 0 */
        /* 406 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter pbSuccess */

        /* 408 */   NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
        /* 410 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 412 */   0x6,        /* FC_SHORT */
        0x0,        /* 0 */

        /* Return value */

        /* 414 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 416 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 418 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_Speed */

        /* 420 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 422 */   NdrFcLong(0x0),      /* 0 */
        /* 426 */   NdrFcShort(0x13),    /* 19 */
        /* 428 */   NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 430 */   NdrFcShort(0x0),     /* 0 */
        /* 432 */   NdrFcShort(0x24),    /* 36 */
        /* 434 */   0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 436 */   0x8,        /* 8 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 438 */   NdrFcShort(0x0),     /* 0 */
        /* 440 */   NdrFcShort(0x0),     /* 0 */
        /* 442 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter pVal */

        /* 444 */   NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
        /* 446 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 448 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 450 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 452 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 454 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_Speed */

        /* 456 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 458 */   NdrFcLong(0x0),      /* 0 */
        /* 462 */   NdrFcShort(0x14),    /* 20 */
        /* 464 */   NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 466 */   NdrFcShort(0x8),     /* 8 */
        /* 468 */   NdrFcShort(0x8),     /* 8 */
        /* 470 */   0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 472 */   0x8,        /* 8 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 474 */   NdrFcShort(0x0),     /* 0 */
        /* 476 */   NdrFcShort(0x0),     /* 0 */
        /* 478 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter newVal */

        /* 480 */   NdrFcShort(0x48),    /* Flags:  in, base type, */
        /* 482 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 484 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 486 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 488 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 490 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        0x0
    }
};

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString = {
    0,
    {
        NdrFcShort(0x0),     /* 0 */
        /*  2 */
        0x11, 0xc,  /* FC_RP [alloced_on_stack] [simple_pointer] */
        /*  4 */    0x8,        /* FC_LONG */
        0x5c,       /* FC_PAD */
        /*  6 */
        0x2f,       /* FC_IP */
        0x5a,       /* FC_CONSTANT_IID */
        /*  8 */    NdrFcLong(0xbef6e003),   /* -1091117053 */
        /* 12 */    NdrFcShort(0xa874),      /* -22412 */
        /* 14 */    NdrFcShort(0x101a),      /* 4122 */
        /* 16 */    0x8b,       /* 139 */
        0xba,       /* 186 */
        /* 18 */    0x0,        /* 0 */
        0xaa,       /* 170 */
        /* 20 */    0x0,        /* 0 */
        0x30,       /* 48 */
        /* 22 */    0xc,        /* 12 */
        0xab,       /* 171 */
        /* 24 */
        0x11, 0x10, /* FC_RP [pointer_deref] */
        /* 26 */    NdrFcShort(0xffec),      /* Offset= -20 (6) */
        /* 28 */
        0x11, 0xc,  /* FC_RP [alloced_on_stack] [simple_pointer] */
        /* 30 */    0x6,        /* FC_SHORT */
        0x5c,       /* FC_PAD */

        0x0
    }
};


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ITicker, ver. 0.0,
   GUID={0x48727BC4,0xB815,0x4EBB,{0x97,0x18,0x84,0xEF,0xF6,0xD5,0x95,0x05}} */

#pragma code_seg(".orpc")
static const unsigned short ITicker_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    0,
    36,
    72,
    108,
    144,
    180,
    216,
    252,
    288,
    324,
    354,
    384,
    420,
    456
};

static const MIDL_STUBLESS_PROXY_INFO ITicker_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &ITicker_FormatStringOffsetTable[-3],
    0,
    0,
    0
};


static const MIDL_SERVER_INFO ITicker_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &ITicker_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};
CINTERFACE_PROXY_VTABLE(21) _ITickerProxyVtbl = {
    &ITicker_ProxyInfo,
    &IID_ITicker,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *)(INT_PTR) - 1 /* ITicker::put_BackColor */ ,
    (void *)(INT_PTR) - 1 /* ITicker::get_BackColor */ ,
    (void *)(INT_PTR) - 1 /* ITicker::put_BorderColor */ ,
    (void *)(INT_PTR) - 1 /* ITicker::get_BorderColor */ ,
    (void *)(INT_PTR) - 1 /* ITicker::putref_Font */ ,
    (void *)(INT_PTR) - 1 /* ITicker::put_Font */ ,
    (void *)(INT_PTR) - 1 /* ITicker::get_Font */ ,
    (void *)(INT_PTR) - 1 /* ITicker::put_ForeColor */ ,
    (void *)(INT_PTR) - 1 /* ITicker::get_ForeColor */ ,
    (void *)(INT_PTR) - 1 /* ITicker::StartScroll */ ,
    (void *)(INT_PTR) - 1 /* ITicker::StopScroll */ ,
    (void *)(INT_PTR) - 1 /* ITicker::GetQuotes */ ,
    (void *)(INT_PTR) - 1 /* ITicker::get_Speed */ ,
    (void *)(INT_PTR) - 1 /* ITicker::put_Speed */
};


static const PRPC_STUB_FUNCTION ITicker_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _ITickerStubVtbl = {
    &IID_ITicker,
    &ITicker_ServerInfo,
    21,
    &ITicker_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x6000169, /* MIDL Version 6.0.361 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
};

const CInterfaceProxyVtbl * _AtlTickerCtrl_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_ITickerProxyVtbl,
    0
};

const CInterfaceStubVtbl * _AtlTickerCtrl_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_ITickerStubVtbl,
    0
};

PCInterfaceName const _AtlTickerCtrl_InterfaceNamesList[] = {
    "ITicker",
    0
};

const IID *  _AtlTickerCtrl_BaseIIDList[] = {
    &IID_IDispatch,
    0
};


#define _AtlTickerCtrl_CHECK_IID(n) IID_GENERIC_CHECK_IID( _AtlTickerCtrl, pIID, n)

int __stdcall _AtlTickerCtrl_IID_Lookup(const IID * pIID, int * pIndex)
{
    if (!_AtlTickerCtrl_CHECK_IID(0)) {
        *pIndex = 0;
        return 1;
    }

    return 0;
}

const ExtendedProxyFileInfo AtlTickerCtrl_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _AtlTickerCtrl_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _AtlTickerCtrl_StubVtblList,
    (const PCInterfaceName *) & _AtlTickerCtrl_InterfaceNamesList,
    (const IID **) & _AtlTickerCtrl_BaseIIDList,
    & _AtlTickerCtrl_IID_Lookup,
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
    #pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/



/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 6.00.0361 */
/* at Thu Jun 30 17:38:47 2005
 */
/* Compiler settings for E:\Software\BookCode\VCCOM\CHAP10\IETICKERCTRL\AtlTickerCtrl.idl:
    Oicf, W1, Zp8, env=Win64 (32b run,appending)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data
    VC __declspec() decoration level:
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if defined(_M_IA64) || defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
    #pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extent to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
    #define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
    #error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "AtlTickerCtrl.h"

#define TYPE_FORMAT_STRING_SIZE   33
#define PROC_FORMAT_STRING_SIZE   521
#define TRANSMIT_AS_TABLE_SIZE    0
#define WIRE_MARSHAL_TABLE_SIZE   0

typedef struct _MIDL_TYPE_FORMAT_STRING {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
} MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
} MIDL_PROC_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax =
{{0x8A885D04, 0x1CEB, 0x11C9, {0x9F, 0xE8, 0x08, 0x00, 0x2B, 0x10, 0x48, 0x60}}, {2, 0}};


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ITicker_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ITicker_ProxyInfo;



#if !defined(__RPC_WIN64__)
    #error  Invalid build platform for this stub.
#endif

static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString = {
    0,
    {

        /* Procedure put_BackColor */

        0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /*  2 */    NdrFcLong(0x0),      /* 0 */
        /*  6 */    NdrFcShort(0x7),     /* 7 */
        /*  8 */    NdrFcShort(0x18),    /* ia64 Stack size/offset = 24 */
        /* 10 */    NdrFcShort(0x8),     /* 8 */
        /* 12 */    NdrFcShort(0x8),     /* 8 */
        /* 14 */    0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 16 */    0xa,        /* 10 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 18 */    NdrFcShort(0x0),     /* 0 */
        /* 20 */    NdrFcShort(0x0),     /* 0 */
        /* 22 */    NdrFcShort(0x0),     /* 0 */
        /* 24 */    NdrFcShort(0x0),     /* 0 */

        /* Parameter clr */

        /* 26 */    NdrFcShort(0x48),    /* Flags:  in, base type, */
        /* 28 */    NdrFcShort(0x8),     /* ia64 Stack size/offset = 8 */
        /* 30 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 32 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 34 */    NdrFcShort(0x10),    /* ia64 Stack size/offset = 16 */
        /* 36 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_BackColor */

        /* 38 */    0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 40 */    NdrFcLong(0x0),      /* 0 */
        /* 44 */    NdrFcShort(0x8),     /* 8 */
        /* 46 */    NdrFcShort(0x18),    /* ia64 Stack size/offset = 24 */
        /* 48 */    NdrFcShort(0x0),     /* 0 */
        /* 50 */    NdrFcShort(0x24),    /* 36 */
        /* 52 */    0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 54 */    0xa,        /* 10 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 56 */    NdrFcShort(0x0),     /* 0 */
        /* 58 */    NdrFcShort(0x0),     /* 0 */
        /* 60 */    NdrFcShort(0x0),     /* 0 */
        /* 62 */    NdrFcShort(0x0),     /* 0 */

        /* Parameter pclr */

        /* 64 */    NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
        /* 66 */    NdrFcShort(0x8),     /* ia64 Stack size/offset = 8 */
        /* 68 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 70 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 72 */    NdrFcShort(0x10),    /* ia64 Stack size/offset = 16 */
        /* 74 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_BorderColor */

        /* 76 */    0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 78 */    NdrFcLong(0x0),      /* 0 */
        /* 82 */    NdrFcShort(0x9),     /* 9 */
        /* 84 */    NdrFcShort(0x18),    /* ia64 Stack size/offset = 24 */
        /* 86 */    NdrFcShort(0x8),     /* 8 */
        /* 88 */    NdrFcShort(0x8),     /* 8 */
        /* 90 */    0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 92 */    0xa,        /* 10 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 94 */    NdrFcShort(0x0),     /* 0 */
        /* 96 */    NdrFcShort(0x0),     /* 0 */
        /* 98 */    NdrFcShort(0x0),     /* 0 */
        /* 100 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter clr */

        /* 102 */   NdrFcShort(0x48),    /* Flags:  in, base type, */
        /* 104 */   NdrFcShort(0x8),     /* ia64 Stack size/offset = 8 */
        /* 106 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 108 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 110 */   NdrFcShort(0x10),    /* ia64 Stack size/offset = 16 */
        /* 112 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_BorderColor */

        /* 114 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 116 */   NdrFcLong(0x0),      /* 0 */
        /* 120 */   NdrFcShort(0xa),     /* 10 */
        /* 122 */   NdrFcShort(0x18),    /* ia64 Stack size/offset = 24 */
        /* 124 */   NdrFcShort(0x0),     /* 0 */
        /* 126 */   NdrFcShort(0x24),    /* 36 */
        /* 128 */   0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 130 */   0xa,        /* 10 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 132 */   NdrFcShort(0x0),     /* 0 */
        /* 134 */   NdrFcShort(0x0),     /* 0 */
        /* 136 */   NdrFcShort(0x0),     /* 0 */
        /* 138 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter pclr */

        /* 140 */   NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
        /* 142 */   NdrFcShort(0x8),     /* ia64 Stack size/offset = 8 */
        /* 144 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 146 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 148 */   NdrFcShort(0x10),    /* ia64 Stack size/offset = 16 */
        /* 150 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure putref_Font */

        /* 152 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 154 */   NdrFcLong(0x0),      /* 0 */
        /* 158 */   NdrFcShort(0xb),     /* 11 */
        /* 160 */   NdrFcShort(0x18),    /* ia64 Stack size/offset = 24 */
        /* 162 */   NdrFcShort(0x0),     /* 0 */
        /* 164 */   NdrFcShort(0x8),     /* 8 */
        /* 166 */   0x46,       /* Oi2 Flags:  clt must size, has return, has ext, */
        0x2,        /* 2 */
        /* 168 */   0xa,        /* 10 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 170 */   NdrFcShort(0x0),     /* 0 */
        /* 172 */   NdrFcShort(0x0),     /* 0 */
        /* 174 */   NdrFcShort(0x0),     /* 0 */
        /* 176 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter pFont */

        /* 178 */   NdrFcShort(0xb),     /* Flags:  must size, must free, in, */
        /* 180 */   NdrFcShort(0x8),     /* ia64 Stack size/offset = 8 */
        /* 182 */   NdrFcShort(0x6),     /* Type Offset=6 */

        /* Return value */

        /* 184 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 186 */   NdrFcShort(0x10),    /* ia64 Stack size/offset = 16 */
        /* 188 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_Font */

        /* 190 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 192 */   NdrFcLong(0x0),      /* 0 */
        /* 196 */   NdrFcShort(0xc),     /* 12 */
        /* 198 */   NdrFcShort(0x18),    /* ia64 Stack size/offset = 24 */
        /* 200 */   NdrFcShort(0x0),     /* 0 */
        /* 202 */   NdrFcShort(0x8),     /* 8 */
        /* 204 */   0x46,       /* Oi2 Flags:  clt must size, has return, has ext, */
        0x2,        /* 2 */
        /* 206 */   0xa,        /* 10 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 208 */   NdrFcShort(0x0),     /* 0 */
        /* 210 */   NdrFcShort(0x0),     /* 0 */
        /* 212 */   NdrFcShort(0x0),     /* 0 */
        /* 214 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter pFont */

        /* 216 */   NdrFcShort(0xb),     /* Flags:  must size, must free, in, */
        /* 218 */   NdrFcShort(0x8),     /* ia64 Stack size/offset = 8 */
        /* 220 */   NdrFcShort(0x6),     /* Type Offset=6 */

        /* Return value */

        /* 222 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 224 */   NdrFcShort(0x10),    /* ia64 Stack size/offset = 16 */
        /* 226 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_Font */

        /* 228 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 230 */   NdrFcLong(0x0),      /* 0 */
        /* 234 */   NdrFcShort(0xd),     /* 13 */
        /* 236 */   NdrFcShort(0x18),    /* ia64 Stack size/offset = 24 */
        /* 238 */   NdrFcShort(0x0),     /* 0 */
        /* 240 */   NdrFcShort(0x8),     /* 8 */
        /* 242 */   0x45,       /* Oi2 Flags:  srv must size, has return, has ext, */
        0x2,        /* 2 */
        /* 244 */   0xa,        /* 10 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 246 */   NdrFcShort(0x0),     /* 0 */
        /* 248 */   NdrFcShort(0x0),     /* 0 */
        /* 250 */   NdrFcShort(0x0),     /* 0 */
        /* 252 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter ppFont */

        /* 254 */   NdrFcShort(0x13),    /* Flags:  must size, must free, out, */
        /* 256 */   NdrFcShort(0x8),     /* ia64 Stack size/offset = 8 */
        /* 258 */   NdrFcShort(0x18),    /* Type Offset=24 */

        /* Return value */

        /* 260 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 262 */   NdrFcShort(0x10),    /* ia64 Stack size/offset = 16 */
        /* 264 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_ForeColor */

        /* 266 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 268 */   NdrFcLong(0x0),      /* 0 */
        /* 272 */   NdrFcShort(0xe),     /* 14 */
        /* 274 */   NdrFcShort(0x18),    /* ia64 Stack size/offset = 24 */
        /* 276 */   NdrFcShort(0x8),     /* 8 */
        /* 278 */   NdrFcShort(0x8),     /* 8 */
        /* 280 */   0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 282 */   0xa,        /* 10 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 284 */   NdrFcShort(0x0),     /* 0 */
        /* 286 */   NdrFcShort(0x0),     /* 0 */
        /* 288 */   NdrFcShort(0x0),     /* 0 */
        /* 290 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter clr */

        /* 292 */   NdrFcShort(0x48),    /* Flags:  in, base type, */
        /* 294 */   NdrFcShort(0x8),     /* ia64 Stack size/offset = 8 */
        /* 296 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 298 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 300 */   NdrFcShort(0x10),    /* ia64 Stack size/offset = 16 */
        /* 302 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_ForeColor */

        /* 304 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 306 */   NdrFcLong(0x0),      /* 0 */
        /* 310 */   NdrFcShort(0xf),     /* 15 */
        /* 312 */   NdrFcShort(0x18),    /* ia64 Stack size/offset = 24 */
        /* 314 */   NdrFcShort(0x0),     /* 0 */
        /* 316 */   NdrFcShort(0x24),    /* 36 */
        /* 318 */   0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 320 */   0xa,        /* 10 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 322 */   NdrFcShort(0x0),     /* 0 */
        /* 324 */   NdrFcShort(0x0),     /* 0 */
        /* 326 */   NdrFcShort(0x0),     /* 0 */
        /* 328 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter pclr */

        /* 330 */   NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
        /* 332 */   NdrFcShort(0x8),     /* ia64 Stack size/offset = 8 */
        /* 334 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 336 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 338 */   NdrFcShort(0x10),    /* ia64 Stack size/offset = 16 */
        /* 340 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure StartScroll */

        /* 342 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 344 */   NdrFcLong(0x0),      /* 0 */
        /* 348 */   NdrFcShort(0x10),    /* 16 */
        /* 350 */   NdrFcShort(0x10),    /* ia64 Stack size/offset = 16 */
        /* 352 */   NdrFcShort(0x0),     /* 0 */
        /* 354 */   NdrFcShort(0x8),     /* 8 */
        /* 356 */   0x44,       /* Oi2 Flags:  has return, has ext, */
        0x1,        /* 1 */
        /* 358 */   0xa,        /* 10 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 360 */   NdrFcShort(0x0),     /* 0 */
        /* 362 */   NdrFcShort(0x0),     /* 0 */
        /* 364 */   NdrFcShort(0x0),     /* 0 */
        /* 366 */   NdrFcShort(0x0),     /* 0 */

        /* Return value */

        /* 368 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 370 */   NdrFcShort(0x8),     /* ia64 Stack size/offset = 8 */
        /* 372 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure StopScroll */

        /* 374 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 376 */   NdrFcLong(0x0),      /* 0 */
        /* 380 */   NdrFcShort(0x11),    /* 17 */
        /* 382 */   NdrFcShort(0x10),    /* ia64 Stack size/offset = 16 */
        /* 384 */   NdrFcShort(0x0),     /* 0 */
        /* 386 */   NdrFcShort(0x8),     /* 8 */
        /* 388 */   0x44,       /* Oi2 Flags:  has return, has ext, */
        0x1,        /* 1 */
        /* 390 */   0xa,        /* 10 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 392 */   NdrFcShort(0x0),     /* 0 */
        /* 394 */   NdrFcShort(0x0),     /* 0 */
        /* 396 */   NdrFcShort(0x0),     /* 0 */
        /* 398 */   NdrFcShort(0x0),     /* 0 */

        /* Return value */

        /* 400 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 402 */   NdrFcShort(0x8),     /* ia64 Stack size/offset = 8 */
        /* 404 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure GetQuotes */

        /* 406 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 408 */   NdrFcLong(0x0),      /* 0 */
        /* 412 */   NdrFcShort(0x12),    /* 18 */
        /* 414 */   NdrFcShort(0x18),    /* ia64 Stack size/offset = 24 */
        /* 416 */   NdrFcShort(0x0),     /* 0 */
        /* 418 */   NdrFcShort(0x22),    /* 34 */
        /* 420 */   0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 422 */   0xa,        /* 10 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 424 */   NdrFcShort(0x0),     /* 0 */
        /* 426 */   NdrFcShort(0x0),     /* 0 */
        /* 428 */   NdrFcShort(0x0),     /* 0 */
        /* 430 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter pbSuccess */

        /* 432 */   NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
        /* 434 */   NdrFcShort(0x8),     /* ia64 Stack size/offset = 8 */
        /* 436 */   0x6,        /* FC_SHORT */
        0x0,        /* 0 */

        /* Return value */

        /* 438 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 440 */   NdrFcShort(0x10),    /* ia64 Stack size/offset = 16 */
        /* 442 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_Speed */

        /* 444 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 446 */   NdrFcLong(0x0),      /* 0 */
        /* 450 */   NdrFcShort(0x13),    /* 19 */
        /* 452 */   NdrFcShort(0x18),    /* ia64 Stack size/offset = 24 */
        /* 454 */   NdrFcShort(0x0),     /* 0 */
        /* 456 */   NdrFcShort(0x24),    /* 36 */
        /* 458 */   0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 460 */   0xa,        /* 10 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 462 */   NdrFcShort(0x0),     /* 0 */
        /* 464 */   NdrFcShort(0x0),     /* 0 */
        /* 466 */   NdrFcShort(0x0),     /* 0 */
        /* 468 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter pVal */

        /* 470 */   NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
        /* 472 */   NdrFcShort(0x8),     /* ia64 Stack size/offset = 8 */
        /* 474 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 476 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 478 */   NdrFcShort(0x10),    /* ia64 Stack size/offset = 16 */
        /* 480 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_Speed */

        /* 482 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 484 */   NdrFcLong(0x0),      /* 0 */
        /* 488 */   NdrFcShort(0x14),    /* 20 */
        /* 490 */   NdrFcShort(0x18),    /* ia64 Stack size/offset = 24 */
        /* 492 */   NdrFcShort(0x8),     /* 8 */
        /* 494 */   NdrFcShort(0x8),     /* 8 */
        /* 496 */   0x44,       /* Oi2 Flags:  has return, has ext, */
        0x2,        /* 2 */
        /* 498 */   0xa,        /* 10 */
        0x1,        /* Ext Flags:  new corr desc, */
        /* 500 */   NdrFcShort(0x0),     /* 0 */
        /* 502 */   NdrFcShort(0x0),     /* 0 */
        /* 504 */   NdrFcShort(0x0),     /* 0 */
        /* 506 */   NdrFcShort(0x0),     /* 0 */

        /* Parameter newVal */

        /* 508 */   NdrFcShort(0x48),    /* Flags:  in, base type, */
        /* 510 */   NdrFcShort(0x8),     /* ia64 Stack size/offset = 8 */
        /* 512 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 514 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 516 */   NdrFcShort(0x10),    /* ia64 Stack size/offset = 16 */
        /* 518 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        0x0
    }
};

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString = {
    0,
    {
        NdrFcShort(0x0),     /* 0 */
        /*  2 */
        0x11, 0xc,  /* FC_RP [alloced_on_stack] [simple_pointer] */
        /*  4 */    0x8,        /* FC_LONG */
        0x5c,       /* FC_PAD */
        /*  6 */
        0x2f,       /* FC_IP */
        0x5a,       /* FC_CONSTANT_IID */
        /*  8 */    NdrFcLong(0xbef6e003),   /* -1091117053 */
        /* 12 */    NdrFcShort(0xa874),      /* -22412 */
        /* 14 */    NdrFcShort(0x101a),      /* 4122 */
        /* 16 */    0x8b,       /* 139 */
        0xba,       /* 186 */
        /* 18 */    0x0,        /* 0 */
        0xaa,       /* 170 */
        /* 20 */    0x0,        /* 0 */
        0x30,       /* 48 */
        /* 22 */    0xc,        /* 12 */
        0xab,       /* 171 */
        /* 24 */
        0x11, 0x10, /* FC_RP [pointer_deref] */
        /* 26 */    NdrFcShort(0xffec),      /* Offset= -20 (6) */
        /* 28 */
        0x11, 0xc,  /* FC_RP [alloced_on_stack] [simple_pointer] */
        /* 30 */    0x6,        /* FC_SHORT */
        0x5c,       /* FC_PAD */

        0x0
    }
};


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ITicker, ver. 0.0,
   GUID={0x48727BC4,0xB815,0x4EBB,{0x97,0x18,0x84,0xEF,0xF6,0xD5,0x95,0x05}} */

#pragma code_seg(".orpc")
static const unsigned short ITicker_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    0,
    38,
    76,
    114,
    152,
    190,
    228,
    266,
    304,
    342,
    374,
    406,
    444,
    482
};

static const MIDL_STUBLESS_PROXY_INFO ITicker_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &ITicker_FormatStringOffsetTable[-3],
    0,
    0,
    0
};


static const MIDL_SERVER_INFO ITicker_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &ITicker_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};
CINTERFACE_PROXY_VTABLE(21) _ITickerProxyVtbl = {
    &ITicker_ProxyInfo,
    &IID_ITicker,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *)(INT_PTR) - 1 /* ITicker::put_BackColor */ ,
    (void *)(INT_PTR) - 1 /* ITicker::get_BackColor */ ,
    (void *)(INT_PTR) - 1 /* ITicker::put_BorderColor */ ,
    (void *)(INT_PTR) - 1 /* ITicker::get_BorderColor */ ,
    (void *)(INT_PTR) - 1 /* ITicker::putref_Font */ ,
    (void *)(INT_PTR) - 1 /* ITicker::put_Font */ ,
    (void *)(INT_PTR) - 1 /* ITicker::get_Font */ ,
    (void *)(INT_PTR) - 1 /* ITicker::put_ForeColor */ ,
    (void *)(INT_PTR) - 1 /* ITicker::get_ForeColor */ ,
    (void *)(INT_PTR) - 1 /* ITicker::StartScroll */ ,
    (void *)(INT_PTR) - 1 /* ITicker::StopScroll */ ,
    (void *)(INT_PTR) - 1 /* ITicker::GetQuotes */ ,
    (void *)(INT_PTR) - 1 /* ITicker::get_Speed */ ,
    (void *)(INT_PTR) - 1 /* ITicker::put_Speed */
};


static const PRPC_STUB_FUNCTION ITicker_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _ITickerStubVtbl = {
    &IID_ITicker,
    &ITicker_ServerInfo,
    21,
    &ITicker_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x6000169, /* MIDL Version 6.0.361 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
};

const CInterfaceProxyVtbl * _AtlTickerCtrl_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_ITickerProxyVtbl,
    0
};

const CInterfaceStubVtbl * _AtlTickerCtrl_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_ITickerStubVtbl,
    0
};

PCInterfaceName const _AtlTickerCtrl_InterfaceNamesList[] = {
    "ITicker",
    0
};

const IID *  _AtlTickerCtrl_BaseIIDList[] = {
    &IID_IDispatch,
    0
};


#define _AtlTickerCtrl_CHECK_IID(n) IID_GENERIC_CHECK_IID( _AtlTickerCtrl, pIID, n)

int __stdcall _AtlTickerCtrl_IID_Lookup(const IID * pIID, int * pIndex)
{
    if (!_AtlTickerCtrl_CHECK_IID(0)) {
        *pIndex = 0;
        return 1;
    }

    return 0;
}

const ExtendedProxyFileInfo AtlTickerCtrl_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _AtlTickerCtrl_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _AtlTickerCtrl_StubVtblList,
    (const PCInterfaceName *) & _AtlTickerCtrl_InterfaceNamesList,
    (const IID **) & _AtlTickerCtrl_BaseIIDList,
    & _AtlTickerCtrl_IID_Lookup,
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
    #pragma warning(pop)
#endif


#endif /* defined(_M_IA64) || defined(_M_AMD64)*/


/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Dec 19 16:51:31 1999
 */
/* Compiler settings for C:\projects\Books\atl\2ed\Examples\Chap08\PostIt\PostIt.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data
*/
//@@MIDL_FILE_HEADING(  )

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
    #define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
    #error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "PostIt.h"

#define TYPE_FORMAT_STRING_SIZE   85
#define PROC_FORMAT_STRING_SIZE   365

typedef struct _MIDL_TYPE_FORMAT_STRING {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
} MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
} MIDL_PROC_FORMAT_STRING;


extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IPostItCtl, ver. 0.0,
   GUID={0xBF31419D,0xB661,0x11D3,{0x83,0x91,0x00,0x60,0x08,0x1A,0xEB,0x5F}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IPostItCtl_ServerInfo;

#pragma code_seg(".orpc")
extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[1];

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
    0x20000, /* Ndr library version */
    0,
    0x50100a4, /* MIDL Version 5.1.164 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    1,  /* Flags */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
};

static const unsigned short IPostItCtl_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    0,
    28,
    56,
    84,
    112,
    140,
    168,
    196,
    224,
    252,
    280,
    308,
    336
};

static const MIDL_SERVER_INFO IPostItCtl_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IPostItCtl_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IPostItCtl_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IPostItCtl_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(20) _IPostItCtlProxyVtbl = {
    &IPostItCtl_ProxyInfo,
    &IID_IPostItCtl,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) - 1 /* IPostItCtl::put_BackColor */ ,
    (void *) - 1 /* IPostItCtl::get_BackColor */ ,
    (void *) - 1 /* IPostItCtl::put_BorderStyle */ ,
    (void *) - 1 /* IPostItCtl::get_BorderStyle */ ,
    (void *) - 1 /* IPostItCtl::putref_Font */ ,
    (void *) - 1 /* IPostItCtl::put_Font */ ,
    (void *) - 1 /* IPostItCtl::get_Font */ ,
    (void *) - 1 /* IPostItCtl::put_ForeColor */ ,
    (void *) - 1 /* IPostItCtl::get_ForeColor */ ,
    (void *) - 1 /* IPostItCtl::put_Text */ ,
    (void *) - 1 /* IPostItCtl::get_Text */ ,
    (void *) - 1 /* IPostItCtl::put_Appearance */ ,
    (void *) - 1 /* IPostItCtl::get_Appearance */
};


static const PRPC_STUB_FUNCTION IPostItCtl_table[] = {
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
    NdrStubCall2
};

CInterfaceStubVtbl _IPostItCtlStubVtbl = {
    &IID_IPostItCtl,
    &IPostItCtl_ServerInfo,
    20,
    &IPostItCtl_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

#pragma data_seg(".rdata")

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[1] = {

    {
        BSTR_UserSize
        , BSTR_UserMarshal
        , BSTR_UserUnmarshal
        , BSTR_UserFree
    }

};


#if !defined(__RPC_WIN32__)
    #error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
    #error You need a Windows NT 4.0 or later to run this stub because it uses these features:
    #error   -Oif or -Oicf, [wire_marshal] or [user_marshal] attribute, more than 32 methods in the interface.
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
#ifndef _ALPHA_
        /*  8 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 10 */    NdrFcShort(0x8),     /* 8 */
        /* 12 */    NdrFcShort(0x8),     /* 8 */
        /* 14 */    0x4,        /* Oi2 Flags:  has return, */
        0x2,        /* 2 */

        /* Parameter clr */

        /* 16 */    NdrFcShort(0x48),    /* Flags:  in, base type, */
#ifndef _ALPHA_
        /* 18 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 20 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 22 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 24 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 26 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_BackColor */

        /* 28 */    0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 30 */    NdrFcLong(0x0),      /* 0 */
        /* 34 */    NdrFcShort(0x8),     /* 8 */
#ifndef _ALPHA_
        /* 36 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 38 */    NdrFcShort(0x0),     /* 0 */
        /* 40 */    NdrFcShort(0x10),    /* 16 */
        /* 42 */    0x4,        /* Oi2 Flags:  has return, */
        0x2,        /* 2 */

        /* Parameter pclr */

        /* 44 */    NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
        /* 46 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 48 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 50 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 52 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 54 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_BorderStyle */

        /* 56 */    0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 58 */    NdrFcLong(0x0),      /* 0 */
        /* 62 */    NdrFcShort(0x9),     /* 9 */
#ifndef _ALPHA_
        /* 64 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 66 */    NdrFcShort(0x8),     /* 8 */
        /* 68 */    NdrFcShort(0x8),     /* 8 */
        /* 70 */    0x4,        /* Oi2 Flags:  has return, */
        0x2,        /* 2 */

        /* Parameter style */

        /* 72 */    NdrFcShort(0x48),    /* Flags:  in, base type, */
#ifndef _ALPHA_
        /* 74 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 76 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 78 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 80 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 82 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_BorderStyle */

        /* 84 */    0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 86 */    NdrFcLong(0x0),      /* 0 */
        /* 90 */    NdrFcShort(0xa),     /* 10 */
#ifndef _ALPHA_
        /* 92 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 94 */    NdrFcShort(0x0),     /* 0 */
        /* 96 */    NdrFcShort(0x10),    /* 16 */
        /* 98 */    0x4,        /* Oi2 Flags:  has return, */
        0x2,        /* 2 */

        /* Parameter pstyle */

        /* 100 */   NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
        /* 102 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 104 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 106 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 108 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 110 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure putref_Font */

        /* 112 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 114 */   NdrFcLong(0x0),      /* 0 */
        /* 118 */   NdrFcShort(0xb),     /* 11 */
#ifndef _ALPHA_
        /* 120 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 122 */   NdrFcShort(0x0),     /* 0 */
        /* 124 */   NdrFcShort(0x8),     /* 8 */
        /* 126 */   0x6,        /* Oi2 Flags:  clt must size, has return, */
        0x2,        /* 2 */

        /* Parameter pFont */

        /* 128 */   NdrFcShort(0xb),     /* Flags:  must size, must free, in, */
#ifndef _ALPHA_
        /* 130 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 132 */   NdrFcShort(0x6),     /* Type Offset=6 */

        /* Return value */

        /* 134 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 136 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 138 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_Font */

        /* 140 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 142 */   NdrFcLong(0x0),      /* 0 */
        /* 146 */   NdrFcShort(0xc),     /* 12 */
#ifndef _ALPHA_
        /* 148 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 150 */   NdrFcShort(0x0),     /* 0 */
        /* 152 */   NdrFcShort(0x8),     /* 8 */
        /* 154 */   0x6,        /* Oi2 Flags:  clt must size, has return, */
        0x2,        /* 2 */

        /* Parameter pFont */

        /* 156 */   NdrFcShort(0xb),     /* Flags:  must size, must free, in, */
#ifndef _ALPHA_
        /* 158 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 160 */   NdrFcShort(0x6),     /* Type Offset=6 */

        /* Return value */

        /* 162 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 164 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 166 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_Font */

        /* 168 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 170 */   NdrFcLong(0x0),      /* 0 */
        /* 174 */   NdrFcShort(0xd),     /* 13 */
#ifndef _ALPHA_
        /* 176 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 178 */   NdrFcShort(0x0),     /* 0 */
        /* 180 */   NdrFcShort(0x8),     /* 8 */
        /* 182 */   0x5,        /* Oi2 Flags:  srv must size, has return, */
        0x2,        /* 2 */

        /* Parameter ppFont */

        /* 184 */   NdrFcShort(0x13),    /* Flags:  must size, must free, out, */
#ifndef _ALPHA_
        /* 186 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 188 */   NdrFcShort(0x18),    /* Type Offset=24 */

        /* Return value */

        /* 190 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 192 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 194 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_ForeColor */

        /* 196 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 198 */   NdrFcLong(0x0),      /* 0 */
        /* 202 */   NdrFcShort(0xe),     /* 14 */
#ifndef _ALPHA_
        /* 204 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 206 */   NdrFcShort(0x8),     /* 8 */
        /* 208 */   NdrFcShort(0x8),     /* 8 */
        /* 210 */   0x4,        /* Oi2 Flags:  has return, */
        0x2,        /* 2 */

        /* Parameter clr */

        /* 212 */   NdrFcShort(0x48),    /* Flags:  in, base type, */
#ifndef _ALPHA_
        /* 214 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 216 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 218 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 220 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 222 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_ForeColor */

        /* 224 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 226 */   NdrFcLong(0x0),      /* 0 */
        /* 230 */   NdrFcShort(0xf),     /* 15 */
#ifndef _ALPHA_
        /* 232 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 234 */   NdrFcShort(0x0),     /* 0 */
        /* 236 */   NdrFcShort(0x10),    /* 16 */
        /* 238 */   0x4,        /* Oi2 Flags:  has return, */
        0x2,        /* 2 */

        /* Parameter pclr */

        /* 240 */   NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
        /* 242 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 244 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 246 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 248 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 250 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_Text */

        /* 252 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 254 */   NdrFcLong(0x0),      /* 0 */
        /* 258 */   NdrFcShort(0x10),    /* 16 */
#ifndef _ALPHA_
        /* 260 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 262 */   NdrFcShort(0x0),     /* 0 */
        /* 264 */   NdrFcShort(0x8),     /* 8 */
        /* 266 */   0x6,        /* Oi2 Flags:  clt must size, has return, */
        0x2,        /* 2 */

        /* Parameter strText */

        /* 268 */   NdrFcShort(0x8b),    /* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
        /* 270 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 272 */   NdrFcShort(0x34),    /* Type Offset=52 */

        /* Return value */

        /* 274 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 276 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 278 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_Text */

        /* 280 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 282 */   NdrFcLong(0x0),      /* 0 */
        /* 286 */   NdrFcShort(0x11),    /* 17 */
#ifndef _ALPHA_
        /* 288 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 290 */   NdrFcShort(0x0),     /* 0 */
        /* 292 */   NdrFcShort(0x8),     /* 8 */
        /* 294 */   0x5,        /* Oi2 Flags:  srv must size, has return, */
        0x2,        /* 2 */

        /* Parameter pstrText */

        /* 296 */   NdrFcShort(0x2113),      /* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
        /* 298 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 300 */   NdrFcShort(0x46),    /* Type Offset=70 */

        /* Return value */

        /* 302 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 304 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 306 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_Appearance */

        /* 308 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 310 */   NdrFcLong(0x0),      /* 0 */
        /* 314 */   NdrFcShort(0x12),    /* 18 */
#ifndef _ALPHA_
        /* 316 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 318 */   NdrFcShort(0x6),     /* 6 */
        /* 320 */   NdrFcShort(0x8),     /* 8 */
        /* 322 */   0x4,        /* Oi2 Flags:  has return, */
        0x2,        /* 2 */

        /* Parameter appearance */

        /* 324 */   NdrFcShort(0x48),    /* Flags:  in, base type, */
#ifndef _ALPHA_
        /* 326 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 328 */   0x6,        /* FC_SHORT */
        0x0,        /* 0 */

        /* Return value */

        /* 330 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 332 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 334 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_Appearance */

        /* 336 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 338 */   NdrFcLong(0x0),      /* 0 */
        /* 342 */   NdrFcShort(0x13),    /* 19 */
#ifndef _ALPHA_
        /* 344 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 346 */   NdrFcShort(0x0),     /* 0 */
        /* 348 */   NdrFcShort(0xe),     /* 14 */
        /* 350 */   0x4,        /* Oi2 Flags:  has return, */
        0x2,        /* 2 */

        /* Parameter pappearance */

        /* 352 */   NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
        /* 354 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 356 */   0x6,        /* FC_SHORT */
        0x0,        /* 0 */

        /* Return value */

        /* 358 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 360 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 362 */   0x8,        /* FC_LONG */
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
        0x11, 0x10, /* FC_RP */
        /* 26 */    NdrFcShort(0xffffffec),      /* Offset= -20 (6) */
        /* 28 */
        0x12, 0x0,  /* FC_UP */
        /* 30 */    NdrFcShort(0xc),     /* Offset= 12 (42) */
        /* 32 */
        0x1b,       /* FC_CARRAY */
        0x1,        /* 1 */
        /* 34 */    NdrFcShort(0x2),     /* 2 */
        /* 36 */    0x9,        /* Corr desc: FC_ULONG */
        0x0,        /*  */
        /* 38 */    NdrFcShort(0xfffc),      /* -4 */
        /* 40 */    0x6,        /* FC_SHORT */
        0x5b,       /* FC_END */
        /* 42 */
        0x17,       /* FC_CSTRUCT */
        0x3,        /* 3 */
        /* 44 */    NdrFcShort(0x8),     /* 8 */
        /* 46 */    NdrFcShort(0xfffffff2),      /* Offset= -14 (32) */
        /* 48 */    0x8,        /* FC_LONG */
        0x8,        /* FC_LONG */
        /* 50 */    0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 52 */    0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 54 */    NdrFcShort(0x0),     /* 0 */
        /* 56 */    NdrFcShort(0x4),     /* 4 */
        /* 58 */    NdrFcShort(0x0),     /* 0 */
        /* 60 */    NdrFcShort(0xffffffe0),      /* Offset= -32 (28) */
        /* 62 */
        0x11, 0x4,  /* FC_RP [alloced_on_stack] */
        /* 64 */    NdrFcShort(0x6),     /* Offset= 6 (70) */
        /* 66 */
        0x13, 0x0,  /* FC_OP */
        /* 68 */    NdrFcShort(0xffffffe6),      /* Offset= -26 (42) */
        /* 70 */    0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 72 */    NdrFcShort(0x0),     /* 0 */
        /* 74 */    NdrFcShort(0x4),     /* 4 */
        /* 76 */    NdrFcShort(0x0),     /* 0 */
        /* 78 */    NdrFcShort(0xfffffff4),      /* Offset= -12 (66) */
        /* 80 */
        0x11, 0xc,  /* FC_RP [alloced_on_stack] [simple_pointer] */
        /* 82 */    0x6,        /* FC_SHORT */
        0x5c,       /* FC_PAD */

        0x0
    }
};

const CInterfaceProxyVtbl * _PostIt_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_IPostItCtlProxyVtbl,
    0
};

const CInterfaceStubVtbl * _PostIt_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_IPostItCtlStubVtbl,
    0
};

PCInterfaceName const _PostIt_InterfaceNamesList[] = {
    "IPostItCtl",
    0
};

const IID *  _PostIt_BaseIIDList[] = {
    &IID_IDispatch,
    0
};


#define _PostIt_CHECK_IID(n)    IID_GENERIC_CHECK_IID( _PostIt, pIID, n)

int __stdcall _PostIt_IID_Lookup(const IID * pIID, int * pIndex)
{
    if (!_PostIt_CHECK_IID(0)) {
        *pIndex = 0;
        return 1;
    }

    return 0;
}

const ExtendedProxyFileInfo PostIt_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _PostIt_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _PostIt_StubVtblList,
    (const PCInterfaceName *) & _PostIt_InterfaceNamesList,
    (const IID **) & _PostIt_BaseIIDList,
    & _PostIt_IID_Lookup,
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};

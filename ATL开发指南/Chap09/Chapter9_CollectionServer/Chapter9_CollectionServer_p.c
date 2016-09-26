/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Feb 23 21:51:23 2000
 */
/* Compiler settings for C:\projects\Books\atl\2ed\Examples\Chap09\Chapter9_CollectionServer\Chapter9_CollectionServer.idl:
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


#include "Chapter9_CollectionServer.h"

#define TYPE_FORMAT_STRING_SIZE   125
#define PROC_FORMAT_STRING_SIZE   337

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


/* Object interface: IMovie, ver. 0.0,
   GUID={0xF579C5C1,0x3777,0x11d2,{0x88,0x3A,0x44,0x45,0x53,0x54,0x00,0x00}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IMovie_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IMovie_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    0,
    28,
    56,
    84
};

static const MIDL_SERVER_INFO IMovie_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IMovie_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IMovie_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IMovie_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(11) _IMovieProxyVtbl = {
    &IMovie_ProxyInfo,
    &IID_IMovie,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) - 1 /* IMovie::get_Director */ ,
    (void *) - 1 /* IMovie::put_Director */ ,
    (void *) - 1 /* IMovie::get_Title */ ,
    (void *) - 1 /* IMovie::put_Title */
};


static const PRPC_STUB_FUNCTION IMovie_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IMovieStubVtbl = {
    &IID_IMovie,
    &IMovie_ServerInfo,
    11,
    &IMovie_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IMovieCollection, ver. 0.0,
   GUID={0xF579C5C4,0x3777,0x11d2,{0x88,0x3A,0x44,0x45,0x53,0x54,0x00,0x00}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IMovieCollection_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IMovieCollection_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    112,
    140,
    168,
    196,
    218,
    252
};

static const MIDL_SERVER_INFO IMovieCollection_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IMovieCollection_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IMovieCollection_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IMovieCollection_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(13) _IMovieCollectionProxyVtbl = {
    &IMovieCollection_ProxyInfo,
    &IID_IMovieCollection,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) - 1 /* IMovieCollection::get_Count */ ,
    (void *) - 1 /* IMovieCollection::Add */ ,
    (void *) - 1 /* IMovieCollection::Remove */ ,
    (void *) - 1 /* IMovieCollection::RemoveAll */ ,
    (void *) - 1 /* IMovieCollection::get_Item */ ,
    (void *) - 1 /* IMovieCollection::get__NewEnum */
};


static const PRPC_STUB_FUNCTION IMovieCollection_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IMovieCollectionStubVtbl = {
    &IID_IMovieCollection,
    &IMovieCollection_ServerInfo,
    13,
    &IMovieCollection_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IApplication, ver. 0.0,
   GUID={0x56E642CF,0xEA5F,0x11D3,{0x83,0xD5,0x00,0x60,0x08,0x1A,0xEB,0x5F}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IApplication_ServerInfo;

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

static const unsigned short IApplication_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    280,
    308
};

static const MIDL_SERVER_INFO IApplication_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IApplication_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IApplication_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IApplication_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(9) _IApplicationProxyVtbl = {
    &IApplication_ProxyInfo,
    &IID_IApplication,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) - 1 /* IApplication::get_Movies */ ,
    (void *) - 1 /* IApplication::CreateMovie */
};


static const PRPC_STUB_FUNCTION IApplication_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IApplicationStubVtbl = {
    &IID_IApplication,
    &IApplication_ServerInfo,
    9,
    &IApplication_table[-3],
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

        /* Procedure get_Director */

        0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /*  2 */    NdrFcLong(0x0),      /* 0 */
        /*  6 */    NdrFcShort(0x7),     /* 7 */
#ifndef _ALPHA_
        /*  8 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 10 */    NdrFcShort(0x0),     /* 0 */
        /* 12 */    NdrFcShort(0x8),     /* 8 */
        /* 14 */    0x5,        /* Oi2 Flags:  srv must size, has return, */
        0x2,        /* 2 */

        /* Parameter pVal */

        /* 16 */    NdrFcShort(0x2113),      /* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
        /* 18 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 20 */    NdrFcShort(0x1e),    /* Type Offset=30 */

        /* Return value */

        /* 22 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 24 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 26 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_Director */

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
        /* 40 */    NdrFcShort(0x8),     /* 8 */
        /* 42 */    0x6,        /* Oi2 Flags:  clt must size, has return, */
        0x2,        /* 2 */

        /* Parameter newVal */

        /* 44 */    NdrFcShort(0x8b),    /* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
        /* 46 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 48 */    NdrFcShort(0x2c),    /* Type Offset=44 */

        /* Return value */

        /* 50 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 52 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 54 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_Title */

        /* 56 */    0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 58 */    NdrFcLong(0x0),      /* 0 */
        /* 62 */    NdrFcShort(0x9),     /* 9 */
#ifndef _ALPHA_
        /* 64 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 66 */    NdrFcShort(0x0),     /* 0 */
        /* 68 */    NdrFcShort(0x8),     /* 8 */
        /* 70 */    0x5,        /* Oi2 Flags:  srv must size, has return, */
        0x2,        /* 2 */

        /* Parameter pVal */

        /* 72 */    NdrFcShort(0x2113),      /* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
        /* 74 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 76 */    NdrFcShort(0x1e),    /* Type Offset=30 */

        /* Return value */

        /* 78 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 80 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 82 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_Title */

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
        /* 96 */    NdrFcShort(0x8),     /* 8 */
        /* 98 */    0x6,        /* Oi2 Flags:  clt must size, has return, */
        0x2,        /* 2 */

        /* Parameter newVal */

        /* 100 */   NdrFcShort(0x8b),    /* Flags:  must size, must free, in, by val, */
#ifndef _ALPHA_
        /* 102 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 104 */   NdrFcShort(0x2c),    /* Type Offset=44 */

        /* Return value */

        /* 106 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 108 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 110 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_Count */

        /* 112 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 114 */   NdrFcLong(0x0),      /* 0 */
        /* 118 */   NdrFcShort(0x7),     /* 7 */
#ifndef _ALPHA_
        /* 120 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 122 */   NdrFcShort(0x0),     /* 0 */
        /* 124 */   NdrFcShort(0x10),    /* 16 */
        /* 126 */   0x4,        /* Oi2 Flags:  has return, */
        0x2,        /* 2 */

        /* Parameter pVal */

        /* 128 */   NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
#ifndef _ALPHA_
        /* 130 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 132 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 134 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 136 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 138 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Add */

        /* 140 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 142 */   NdrFcLong(0x0),      /* 0 */
        /* 146 */   NdrFcShort(0x8),     /* 8 */
#ifndef _ALPHA_
        /* 148 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 150 */   NdrFcShort(0x0),     /* 0 */
        /* 152 */   NdrFcShort(0x8),     /* 8 */
        /* 154 */   0x6,        /* Oi2 Flags:  clt must size, has return, */
        0x2,        /* 2 */

        /* Parameter pMovie */

        /* 156 */   NdrFcShort(0xb),     /* Flags:  must size, must free, in, */
#ifndef _ALPHA_
        /* 158 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 160 */   NdrFcShort(0x3a),    /* Type Offset=58 */

        /* Return value */

        /* 162 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 164 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 166 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Remove */

        /* 168 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 170 */   NdrFcLong(0x0),      /* 0 */
        /* 174 */   NdrFcShort(0x9),     /* 9 */
#ifndef _ALPHA_
        /* 176 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 178 */   NdrFcShort(0x8),     /* 8 */
        /* 180 */   NdrFcShort(0x8),     /* 8 */
        /* 182 */   0x4,        /* Oi2 Flags:  has return, */
        0x2,        /* 2 */

        /* Parameter inIndex */

        /* 184 */   NdrFcShort(0x48),    /* Flags:  in, base type, */
#ifndef _ALPHA_
        /* 186 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 188 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 190 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 192 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 194 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure RemoveAll */

        /* 196 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 198 */   NdrFcLong(0x0),      /* 0 */
        /* 202 */   NdrFcShort(0xa),     /* 10 */
#ifndef _ALPHA_
        /* 204 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 206 */   NdrFcShort(0x0),     /* 0 */
        /* 208 */   NdrFcShort(0x8),     /* 8 */
        /* 210 */   0x4,        /* Oi2 Flags:  has return, */
        0x1,        /* 1 */

        /* Return value */

        /* 212 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 214 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 216 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_Item */

        /* 218 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 220 */   NdrFcLong(0x0),      /* 0 */
        /* 224 */   NdrFcShort(0xb),     /* 11 */
#ifndef _ALPHA_
        /* 226 */   NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 228 */   NdrFcShort(0x8),     /* 8 */
        /* 230 */   NdrFcShort(0x8),     /* 8 */
        /* 232 */   0x5,        /* Oi2 Flags:  srv must size, has return, */
        0x3,        /* 3 */

        /* Parameter inIndex */

        /* 234 */   NdrFcShort(0x48),    /* Flags:  in, base type, */
#ifndef _ALPHA_
        /* 236 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 238 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter ppMovie */

        /* 240 */   NdrFcShort(0x13),    /* Flags:  must size, must free, out, */
#ifndef _ALPHA_
        /* 242 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 244 */   NdrFcShort(0x4c),    /* Type Offset=76 */

        /* Return value */

        /* 246 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 248 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 250 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get__NewEnum */

        /* 252 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 254 */   NdrFcLong(0x0),      /* 0 */
        /* 258 */   NdrFcShort(0xc),     /* 12 */
#ifndef _ALPHA_
        /* 260 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 262 */   NdrFcShort(0x0),     /* 0 */
        /* 264 */   NdrFcShort(0x8),     /* 8 */
        /* 266 */   0x5,        /* Oi2 Flags:  srv must size, has return, */
        0x2,        /* 2 */

        /* Parameter pVal */

        /* 268 */   NdrFcShort(0x13),    /* Flags:  must size, must free, out, */
#ifndef _ALPHA_
        /* 270 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 272 */   NdrFcShort(0x50),    /* Type Offset=80 */

        /* Return value */

        /* 274 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 276 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 278 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_Movies */

        /* 280 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 282 */   NdrFcLong(0x0),      /* 0 */
        /* 286 */   NdrFcShort(0x7),     /* 7 */
#ifndef _ALPHA_
        /* 288 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 290 */   NdrFcShort(0x0),     /* 0 */
        /* 292 */   NdrFcShort(0x8),     /* 8 */
        /* 294 */   0x5,        /* Oi2 Flags:  srv must size, has return, */
        0x2,        /* 2 */

        /* Parameter ppVal */

        /* 296 */   NdrFcShort(0x13),    /* Flags:  must size, must free, out, */
#ifndef _ALPHA_
        /* 298 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 300 */   NdrFcShort(0x66),    /* Type Offset=102 */

        /* Return value */

        /* 302 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 304 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 306 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure CreateMovie */

        /* 308 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 310 */   NdrFcLong(0x0),      /* 0 */
        /* 314 */   NdrFcShort(0x8),     /* 8 */
#ifndef _ALPHA_
        /* 316 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 318 */   NdrFcShort(0x0),     /* 0 */
        /* 320 */   NdrFcShort(0x8),     /* 8 */
        /* 322 */   0x5,        /* Oi2 Flags:  srv must size, has return, */
        0x2,        /* 2 */

        /* Parameter ppVal */

        /* 324 */   NdrFcShort(0x13),    /* Flags:  must size, must free, out, */
#ifndef _ALPHA_
        /* 326 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 328 */   NdrFcShort(0x66),    /* Type Offset=102 */

        /* Return value */

        /* 330 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 332 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 334 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        0x0
    }
};

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString = {
    0,
    {
        NdrFcShort(0x0),     /* 0 */
        /*  2 */
        0x11, 0x4,  /* FC_RP [alloced_on_stack] */
        /*  4 */    NdrFcShort(0x1a),    /* Offset= 26 (30) */
        /*  6 */
        0x13, 0x0,  /* FC_OP */
        /*  8 */    NdrFcShort(0xc),     /* Offset= 12 (20) */
        /* 10 */
        0x1b,       /* FC_CARRAY */
        0x1,        /* 1 */
        /* 12 */    NdrFcShort(0x2),     /* 2 */
        /* 14 */    0x9,        /* Corr desc: FC_ULONG */
        0x0,        /*  */
        /* 16 */    NdrFcShort(0xfffc),      /* -4 */
        /* 18 */    0x6,        /* FC_SHORT */
        0x5b,       /* FC_END */
        /* 20 */
        0x17,       /* FC_CSTRUCT */
        0x3,        /* 3 */
        /* 22 */    NdrFcShort(0x8),     /* 8 */
        /* 24 */    NdrFcShort(0xfffffff2),      /* Offset= -14 (10) */
        /* 26 */    0x8,        /* FC_LONG */
        0x8,        /* FC_LONG */
        /* 28 */    0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 30 */    0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 32 */    NdrFcShort(0x0),     /* 0 */
        /* 34 */    NdrFcShort(0x4),     /* 4 */
        /* 36 */    NdrFcShort(0x0),     /* 0 */
        /* 38 */    NdrFcShort(0xffffffe0),      /* Offset= -32 (6) */
        /* 40 */
        0x12, 0x0,  /* FC_UP */
        /* 42 */    NdrFcShort(0xffffffea),      /* Offset= -22 (20) */
        /* 44 */    0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 46 */    NdrFcShort(0x0),     /* 0 */
        /* 48 */    NdrFcShort(0x4),     /* 4 */
        /* 50 */    NdrFcShort(0x0),     /* 0 */
        /* 52 */    NdrFcShort(0xfffffff4),      /* Offset= -12 (40) */
        /* 54 */
        0x11, 0xc,  /* FC_RP [alloced_on_stack] [simple_pointer] */
        /* 56 */    0x8,        /* FC_LONG */
        0x5c,       /* FC_PAD */
        /* 58 */
        0x2f,       /* FC_IP */
        0x5a,       /* FC_CONSTANT_IID */
        /* 60 */    NdrFcLong(0xf579c5c1),   /* -176568895 */
        /* 64 */    NdrFcShort(0x3777),      /* 14199 */
        /* 66 */    NdrFcShort(0x11d2),      /* 4562 */
        /* 68 */    0x88,       /* 136 */
        0x3a,       /* 58 */
        /* 70 */    0x44,       /* 68 */
        0x45,       /* 69 */
        /* 72 */    0x53,       /* 83 */
        0x54,       /* 84 */
        /* 74 */    0x0,        /* 0 */
        0x0,        /* 0 */
        /* 76 */
        0x11, 0x10, /* FC_RP */
        /* 78 */    NdrFcShort(0xffffffec),      /* Offset= -20 (58) */
        /* 80 */
        0x11, 0x10, /* FC_RP */
        /* 82 */    NdrFcShort(0x2),     /* Offset= 2 (84) */
        /* 84 */
        0x2f,       /* FC_IP */
        0x5a,       /* FC_CONSTANT_IID */
        /* 86 */    NdrFcLong(0x0),      /* 0 */
        /* 90 */    NdrFcShort(0x0),     /* 0 */
        /* 92 */    NdrFcShort(0x0),     /* 0 */
        /* 94 */    0xc0,       /* 192 */
        0x0,        /* 0 */
        /* 96 */    0x0,        /* 0 */
        0x0,        /* 0 */
        /* 98 */    0x0,        /* 0 */
        0x0,        /* 0 */
        /* 100 */   0x0,        /* 0 */
        0x46,       /* 70 */
        /* 102 */
        0x11, 0x10, /* FC_RP */
        /* 104 */   NdrFcShort(0x2),     /* Offset= 2 (106) */
        /* 106 */
        0x2f,       /* FC_IP */
        0x5a,       /* FC_CONSTANT_IID */
        /* 108 */   NdrFcLong(0x20400),      /* 132096 */
        /* 112 */   NdrFcShort(0x0),     /* 0 */
        /* 114 */   NdrFcShort(0x0),     /* 0 */
        /* 116 */   0xc0,       /* 192 */
        0x0,        /* 0 */
        /* 118 */   0x0,        /* 0 */
        0x0,        /* 0 */
        /* 120 */   0x0,        /* 0 */
        0x0,        /* 0 */
        /* 122 */   0x0,        /* 0 */
        0x46,       /* 70 */

        0x0
    }
};

const CInterfaceProxyVtbl * _Chapter9_CollectionServer_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_IMovieProxyVtbl,
    (CInterfaceProxyVtbl *) &_IMovieCollectionProxyVtbl,
    (CInterfaceProxyVtbl *) &_IApplicationProxyVtbl,
    0
};

const CInterfaceStubVtbl * _Chapter9_CollectionServer_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_IMovieStubVtbl,
    (CInterfaceStubVtbl *) &_IMovieCollectionStubVtbl,
    (CInterfaceStubVtbl *) &_IApplicationStubVtbl,
    0
};

PCInterfaceName const _Chapter9_CollectionServer_InterfaceNamesList[] = {
    "IMovie",
    "IMovieCollection",
    "IApplication",
    0
};

const IID *  _Chapter9_CollectionServer_BaseIIDList[] = {
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _Chapter9_CollectionServer_CHECK_IID(n) IID_GENERIC_CHECK_IID( _Chapter9_CollectionServer, pIID, n)

int __stdcall _Chapter9_CollectionServer_IID_Lookup(const IID * pIID, int * pIndex)
{
    IID_BS_LOOKUP_SETUP
    IID_BS_LOOKUP_INITIAL_TEST(_Chapter9_CollectionServer, 3, 2)
    IID_BS_LOOKUP_NEXT_TEST(_Chapter9_CollectionServer, 1)
    IID_BS_LOOKUP_RETURN_RESULT(_Chapter9_CollectionServer, 3, *pIndex)
}

const ExtendedProxyFileInfo Chapter9_CollectionServer_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _Chapter9_CollectionServer_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _Chapter9_CollectionServer_StubVtblList,
    (const PCInterfaceName *) & _Chapter9_CollectionServer_InterfaceNamesList,
    (const IID **) & _Chapter9_CollectionServer_BaseIIDList,
    & _Chapter9_CollectionServer_IID_Lookup,
    3,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};

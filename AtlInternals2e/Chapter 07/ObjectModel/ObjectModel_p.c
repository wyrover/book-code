/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Nov 23 01:11:40 1998
 */
/* Compiler settings for D:\ATLBook\src\Chapter 08\ObjectModel\ObjectModel.idl:
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


#include "ObjectModel.h"

#define TYPE_FORMAT_STRING_SIZE   125
#define PROC_FORMAT_STRING_SIZE   203

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


/* Object interface: IDocument, ver. 0.0,
   GUID={0xCB693B6C,0x80E2,0x11D2,{0x98,0x7D,0x00,0x60,0x08,0x23,0xCF,0xFB}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IDocument_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IDocument_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    0,
    28
};

static const MIDL_SERVER_INFO IDocument_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IDocument_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IDocument_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IDocument_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(9) _IDocumentProxyVtbl = {
    &IDocument_ProxyInfo,
    &IID_IDocument,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) - 1 /* IDocument::get_Data */ ,
    (void *) - 1 /* IDocument::put_Data */
};


static const PRPC_STUB_FUNCTION IDocument_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IDocumentStubVtbl = {
    &IID_IDocument,
    &IDocument_ServerInfo,
    9,
    &IDocument_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IDocuments, ver. 0.0,
   GUID={0xCB693B6E,0x80E2,0x11D2,{0x98,0x7D,0x00,0x60,0x08,0x23,0xCF,0xFB}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IDocuments_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IDocuments_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    56,
    84,
    112,
    146
};

static const MIDL_SERVER_INFO IDocuments_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IDocuments_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IDocuments_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IDocuments_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(11) _IDocumentsProxyVtbl = {
    &IDocuments_ProxyInfo,
    &IID_IDocuments,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) - 1 /* IDocuments::AddDocument */ ,
    (void *) - 1 /* IDocuments::get_Count */ ,
    (void *) - 1 /* IDocuments::get_Item */ ,
    (void *) - 1 /* IDocuments::get__NewEnum */
};


static const PRPC_STUB_FUNCTION IDocuments_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IDocumentsStubVtbl = {
    &IID_IDocuments,
    &IDocuments_ServerInfo,
    11,
    &IDocuments_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IApplication, ver. 0.0,
   GUID={0xCB693B70,0x80E2,0x11D2,{0x98,0x7D,0x00,0x60,0x08,0x23,0xCF,0xFB}} */


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
    174
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

CINTERFACE_PROXY_VTABLE(8) _IApplicationProxyVtbl = {
    &IApplication_ProxyInfo,
    &IID_IApplication,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *)-1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *)-1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) - 1 /* IApplication::get_Documents */
};


static const PRPC_STUB_FUNCTION IApplication_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2
};

CInterfaceStubVtbl _IApplicationStubVtbl = {
    &IID_IApplication,
    &IApplication_ServerInfo,
    8,
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

        /* Procedure get_Data */

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

        /* Procedure put_Data */

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

        /* Procedure AddDocument */

        /* 56 */    0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 58 */    NdrFcLong(0x0),      /* 0 */
        /* 62 */    NdrFcShort(0x7),     /* 7 */
#ifndef _ALPHA_
        /* 64 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 66 */    NdrFcShort(0x0),     /* 0 */
        /* 68 */    NdrFcShort(0x8),     /* 8 */
        /* 70 */    0x5,        /* Oi2 Flags:  srv must size, has return, */
        0x2,        /* 2 */

        /* Parameter ppDocument */

        /* 72 */    NdrFcShort(0x13),    /* Flags:  must size, must free, out, */
#ifndef _ALPHA_
        /* 74 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 76 */    NdrFcShort(0x36),    /* Type Offset=54 */

        /* Return value */

        /* 78 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 80 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 82 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_Count */

        /* 84 */    0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 86 */    NdrFcLong(0x0),      /* 0 */
        /* 90 */    NdrFcShort(0x8),     /* 8 */
#ifndef _ALPHA_
        /* 92 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 94 */    NdrFcShort(0x0),     /* 0 */
        /* 96 */    NdrFcShort(0x10),    /* 16 */
        /* 98 */    0x4,        /* Oi2 Flags:  has return, */
        0x2,        /* 2 */

        /* Parameter pnCount */

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

        /* Procedure get_Item */

        /* 112 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 114 */   NdrFcLong(0x0),      /* 0 */
        /* 118 */   NdrFcShort(0x9),     /* 9 */
#ifndef _ALPHA_
        /* 120 */   NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 122 */   NdrFcShort(0x8),     /* 8 */
        /* 124 */   NdrFcShort(0x8),     /* 8 */
        /* 126 */   0x5,        /* Oi2 Flags:  srv must size, has return, */
        0x3,        /* 3 */

        /* Parameter n */

        /* 128 */   NdrFcShort(0x48),    /* Flags:  in, base type, */
#ifndef _ALPHA_
        /* 130 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 132 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter ppdoc */

        /* 134 */   NdrFcShort(0x13),    /* Flags:  must size, must free, out, */
#ifndef _ALPHA_
        /* 136 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 138 */   NdrFcShort(0x36),    /* Type Offset=54 */

        /* Return value */

        /* 140 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 142 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 144 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get__NewEnum */

        /* 146 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 148 */   NdrFcLong(0x0),      /* 0 */
        /* 152 */   NdrFcShort(0xa),     /* 10 */
#ifndef _ALPHA_
        /* 154 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 156 */   NdrFcShort(0x0),     /* 0 */
        /* 158 */   NdrFcShort(0x8),     /* 8 */
        /* 160 */   0x5,        /* Oi2 Flags:  srv must size, has return, */
        0x2,        /* 2 */

        /* Parameter ppEnum */

        /* 162 */   NdrFcShort(0x13),    /* Flags:  must size, must free, out, */
#ifndef _ALPHA_
        /* 164 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 166 */   NdrFcShort(0x50),    /* Type Offset=80 */

        /* Return value */

        /* 168 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 170 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 172 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_Documents */

        /* 174 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 176 */   NdrFcLong(0x0),      /* 0 */
        /* 180 */   NdrFcShort(0x7),     /* 7 */
#ifndef _ALPHA_
        /* 182 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 184 */   NdrFcShort(0x0),     /* 0 */
        /* 186 */   NdrFcShort(0x8),     /* 8 */
        /* 188 */   0x5,        /* Oi2 Flags:  srv must size, has return, */
        0x2,        /* 2 */

        /* Parameter pVal */

        /* 190 */   NdrFcShort(0x13),    /* Flags:  must size, must free, out, */
#ifndef _ALPHA_
        /* 192 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 194 */   NdrFcShort(0x66),    /* Type Offset=102 */

        /* Return value */

        /* 196 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
#ifndef _ALPHA_
        /* 198 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 200 */   0x8,        /* FC_LONG */
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
        0x11, 0x10, /* FC_RP */
        /* 56 */    NdrFcShort(0x2),     /* Offset= 2 (58) */
        /* 58 */
        0x2f,       /* FC_IP */
        0x5a,       /* FC_CONSTANT_IID */
        /* 60 */    NdrFcLong(0xcb693b6c),   /* -882295956 */
        /* 64 */    NdrFcShort(0x80e2),      /* -32542 */
        /* 66 */    NdrFcShort(0x11d2),      /* 4562 */
        /* 68 */    0x98,       /* 152 */
        0x7d,       /* 125 */
        /* 70 */    0x0,        /* 0 */
        0x60,       /* 96 */
        /* 72 */    0x8,        /* 8 */
        0x23,       /* 35 */
        /* 74 */    0xcf,       /* 207 */
        0xfb,       /* 251 */
        /* 76 */
        0x11, 0xc,  /* FC_RP [alloced_on_stack] [simple_pointer] */
        /* 78 */    0x8,        /* FC_LONG */
        0x5c,       /* FC_PAD */
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
        /* 108 */   NdrFcLong(0xcb693b6e),   /* -882295954 */
        /* 112 */   NdrFcShort(0x80e2),      /* -32542 */
        /* 114 */   NdrFcShort(0x11d2),      /* 4562 */
        /* 116 */   0x98,       /* 152 */
        0x7d,       /* 125 */
        /* 118 */   0x0,        /* 0 */
        0x60,       /* 96 */
        /* 120 */   0x8,        /* 8 */
        0x23,       /* 35 */
        /* 122 */   0xcf,       /* 207 */
        0xfb,       /* 251 */

        0x0
    }
};

const CInterfaceProxyVtbl * _ObjectModel_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_IDocumentProxyVtbl,
    (CInterfaceProxyVtbl *) &_IDocumentsProxyVtbl,
    (CInterfaceProxyVtbl *) &_IApplicationProxyVtbl,
    0
};

const CInterfaceStubVtbl * _ObjectModel_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_IDocumentStubVtbl,
    (CInterfaceStubVtbl *) &_IDocumentsStubVtbl,
    (CInterfaceStubVtbl *) &_IApplicationStubVtbl,
    0
};

PCInterfaceName const _ObjectModel_InterfaceNamesList[] = {
    "IDocument",
    "IDocuments",
    "IApplication",
    0
};

const IID *  _ObjectModel_BaseIIDList[] = {
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _ObjectModel_CHECK_IID(n)   IID_GENERIC_CHECK_IID( _ObjectModel, pIID, n)

int __stdcall _ObjectModel_IID_Lookup(const IID * pIID, int * pIndex)
{
    IID_BS_LOOKUP_SETUP
    IID_BS_LOOKUP_INITIAL_TEST(_ObjectModel, 3, 2)
    IID_BS_LOOKUP_NEXT_TEST(_ObjectModel, 1)
    IID_BS_LOOKUP_RETURN_RESULT(_ObjectModel, 3, *pIndex)
}

const ExtendedProxyFileInfo ObjectModel_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _ObjectModel_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _ObjectModel_StubVtblList,
    (const PCInterfaceName *) & _ObjectModel_InterfaceNamesList,
    (const IID **) & _ObjectModel_BaseIIDList,
    & _ObjectModel_IID_Lookup,
    3,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};

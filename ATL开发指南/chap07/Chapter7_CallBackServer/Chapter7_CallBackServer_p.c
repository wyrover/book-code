/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 3.01.75 */
/* at Tue Nov 04 17:55:33 1997
 */
/* Compiler settings for Chapter7_CallBackServer.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )

#define USE_STUBLESS_PROXY

#include "rpcproxy.h"
#include "Chapter7_CallBackServer.h"

#define TYPE_FORMAT_STRING_SIZE   19
#define PROC_FORMAT_STRING_SIZE   187

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


/* Object interface: ICallBack, ver. 0.0,
   GUID={0x48CD3740,0x50A3,0x11d1,{0xB5,0xEC,0x00,0x04,0xAC,0xFF,0x17,0x1C}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ICallBack_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short ICallBack_FormatStringOffsetTable[] = {
    0
};

static const MIDL_SERVER_INFO ICallBack_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &ICallBack_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO ICallBack_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &ICallBack_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(4) _ICallBackProxyVtbl = {
    &ICallBack_ProxyInfo,
    &IID_ICallBack,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) - 1 /* ICallBack::ComputationComplete */
};

const CInterfaceStubVtbl _ICallBackStubVtbl = {
    &IID_ICallBack,
    &ICallBack_ServerInfo,
    4,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IMath, ver. 0.0,
   GUID={0xDB5CCD8E,0x5489,0x11D1,{0x88,0x3A,0x44,0x45,0x53,0x54,0x00,0x00}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IMath_ServerInfo;

#pragma code_seg(".orpc")

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
    0, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x301004b, /* MIDL Version 3.1.75 */
    0,
    0,
    0,  /* Reserved1 */
    0,  /* Reserved2 */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
};

static const unsigned short IMath_FormatStringOffsetTable[] = {
    24,
    54,
    84,
    114,
    144,
    168
};

static const MIDL_SERVER_INFO IMath_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IMath_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IMath_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IMath_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(9) _IMathProxyVtbl = {
    &IMath_ProxyInfo,
    &IID_IMath,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) - 1 /* IMath::Add */ ,
    (void *) - 1 /* IMath::Subtract */ ,
    (void *) - 1 /* IMath::Multiply */ ,
    (void *) - 1 /* IMath::Divide */ ,
    (void *) - 1 /* IMath::Advise */ ,
    (void *) - 1 /* IMath::UnAdvise */
};

const CInterfaceStubVtbl _IMathStubVtbl = {
    &IID_IMath,
    &IMath_ServerInfo,
    9,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};

#pragma data_seg(".rdata")

#if !defined(__RPC_WIN32__)
    #error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT40_OR_LATER)
    #error You need a Windows NT 4.0 or later to run this stub because it uses these features:
    #error   -Oif or -Oicf.
    #error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
    #error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString = {
    0,
    {

        /* Procedure ComputationComplete */

        0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /*  2 */    NdrFcShort(0x3),     /* 3 */
#ifndef _ALPHA_
        /*  4 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /*  6 */    NdrFcShort(0x8),     /* 8 */
        /*  8 */    NdrFcShort(0x8),     /* 8 */
        /* 10 */    0x4,        /* 4 */
        0x2,        /* 2 */

        /* Parameter lResult */

        /* 12 */    NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 14 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 16 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 18 */    NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 20 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 22 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Add */

        /* 24 */    0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 26 */    NdrFcShort(0x3),     /* 3 */
#ifndef _ALPHA_
        /* 28 */    NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 30 */    NdrFcShort(0x10),    /* 16 */
        /* 32 */    NdrFcShort(0x8),     /* 8 */
        /* 34 */    0x4,        /* 4 */
        0x3,        /* 3 */

        /* Parameter lOp1 */

        /* 36 */    NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 38 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 40 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter lOp2 */

        /* 42 */    NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 44 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 46 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 48 */    NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 50 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 52 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Subtract */

        /* 54 */    0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 56 */    NdrFcShort(0x4),     /* 4 */
#ifndef _ALPHA_
        /* 58 */    NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 60 */    NdrFcShort(0x10),    /* 16 */
        /* 62 */    NdrFcShort(0x8),     /* 8 */
        /* 64 */    0x4,        /* 4 */
        0x3,        /* 3 */

        /* Parameter lOp1 */

        /* 66 */    NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 68 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 70 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter lOp2 */

        /* 72 */    NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 74 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 76 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 78 */    NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 80 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 82 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Multiply */

        /* 84 */    0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 86 */    NdrFcShort(0x5),     /* 5 */
#ifndef _ALPHA_
        /* 88 */    NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 90 */    NdrFcShort(0x10),    /* 16 */
        /* 92 */    NdrFcShort(0x8),     /* 8 */
        /* 94 */    0x4,        /* 4 */
        0x3,        /* 3 */

        /* Parameter lOp1 */

        /* 96 */    NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 98 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 100 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter lOp2 */

        /* 102 */   NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 104 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 106 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 108 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 110 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 112 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Divide */

        /* 114 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 116 */   NdrFcShort(0x6),     /* 6 */
#ifndef _ALPHA_
        /* 118 */   NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 120 */   NdrFcShort(0x10),    /* 16 */
        /* 122 */   NdrFcShort(0x8),     /* 8 */
        /* 124 */   0x4,        /* 4 */
        0x3,        /* 3 */

        /* Parameter lOp1 */

        /* 126 */   NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 128 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 130 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter lOp2 */

        /* 132 */   NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 134 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 136 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 138 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 140 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 142 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Advise */

        /* 144 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 146 */   NdrFcShort(0x7),     /* 7 */
#ifndef _ALPHA_
        /* 148 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 150 */   NdrFcShort(0x0),     /* 0 */
        /* 152 */   NdrFcShort(0x8),     /* 8 */
        /* 154 */   0x6,        /* 6 */
        0x2,        /* 2 */

        /* Parameter pCallBack */

        /* 156 */   NdrFcShort(0xb),     /* 11 */
#ifndef _ALPHA_
        /* 158 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 160 */   NdrFcShort(0x0),     /* Type Offset=0 */

        /* Return value */

        /* 162 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 164 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 166 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure UnAdvise */

        /* 168 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 170 */   NdrFcShort(0x8),     /* 8 */
#ifndef _ALPHA_
        /* 172 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 174 */   NdrFcShort(0x0),     /* 0 */
        /* 176 */   NdrFcShort(0x8),     /* 8 */
        /* 178 */   0x4,        /* 4 */
        0x1,        /* 1 */

        /* Return value */

        /* 180 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 182 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 184 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        0x0
    }
};

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString = {
    0,
    {

        0x2f,       /* FC_IP */
        0x5a,       /* FC_CONSTANT_IID */
        /*  2 */    NdrFcLong(0x48cd3740),   /* 1221408576 */
        /*  6 */    NdrFcShort(0x50a3),      /* 20643 */
        /*  8 */    NdrFcShort(0x11d1),      /* 4561 */
        /* 10 */    0xb5,       /* 181 */
        0xec,       /* 236 */
        /* 12 */    0x0,        /* 0 */
        0x4,        /* 4 */
        /* 14 */    0xac,       /* 172 */
        0xff,       /* 255 */
        /* 16 */    0x17,       /* 23 */
        0x1c,       /* 28 */

        0x0
    }
};

const CInterfaceProxyVtbl * _Chapter7_CallBackServer_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_ICallBackProxyVtbl,
    (CInterfaceProxyVtbl *) &_IMathProxyVtbl,
    0
};

const CInterfaceStubVtbl * _Chapter7_CallBackServer_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_ICallBackStubVtbl,
    (CInterfaceStubVtbl *) &_IMathStubVtbl,
    0
};

PCInterfaceName const _Chapter7_CallBackServer_InterfaceNamesList[] = {
    "ICallBack",
    "IMath",
    0
};


#define _Chapter7_CallBackServer_CHECK_IID(n)   IID_GENERIC_CHECK_IID( _Chapter7_CallBackServer, pIID, n)

int __stdcall _Chapter7_CallBackServer_IID_Lookup(const IID * pIID, int * pIndex)
{
    IID_BS_LOOKUP_SETUP
    IID_BS_LOOKUP_INITIAL_TEST(_Chapter7_CallBackServer, 2, 1)
    IID_BS_LOOKUP_RETURN_RESULT(_Chapter7_CallBackServer, 2, *pIndex)
}

const ExtendedProxyFileInfo Chapter7_CallBackServer_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _Chapter7_CallBackServer_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _Chapter7_CallBackServer_StubVtblList,
    (const PCInterfaceName *) & _Chapter7_CallBackServer_InterfaceNamesList,
    0, // no delegation
    & _Chapter7_CallBackServer_IID_Lookup,
    2,
    2
};

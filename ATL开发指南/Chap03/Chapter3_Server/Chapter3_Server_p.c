/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 3.01.75 */
/* at Sun Oct 19 18:54:28 1997
 */
/* Compiler settings for Chapter3_Server.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )

#define USE_STUBLESS_PROXY

#include "rpcproxy.h"
#include "Chapter3_Server.h"

#define TYPE_FORMAT_STRING_SIZE   5
#define PROC_FORMAT_STRING_SIZE   205

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


/* Object interface: IMath, ver. 0.0,
   GUID={0x8812699C,0x1CC8,0x11D1,{0x88,0x3A,0x44,0x45,0x53,0x54,0x00,0x00}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IMath_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IMath_FormatStringOffsetTable[] = {
    0,
    36,
    72,
    108
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

CINTERFACE_PROXY_VTABLE(7) _IMathProxyVtbl = {
    &IMath_ProxyInfo,
    &IID_IMath,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) - 1 /* IMath::Add */ ,
    (void *) - 1 /* IMath::Subtract */ ,
    (void *) - 1 /* IMath::Multiply */ ,
    (void *) - 1 /* IMath::Divide */
};

const CInterfaceStubVtbl _IMathStubVtbl = {
    &IID_IMath,
    &IMath_ServerInfo,
    7,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IAdvancedMath, ver. 0.0,
   GUID={0x6AF3DF1E,0xC48F,0x11D0,{0xA7,0x69,0xD4,0x77,0xA4,0x00,0x00,0x00}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAdvancedMath_ServerInfo;

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

static const unsigned short IAdvancedMath_FormatStringOffsetTable[] = {
    144,
    174
};

static const MIDL_SERVER_INFO IAdvancedMath_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IAdvancedMath_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IAdvancedMath_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IAdvancedMath_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(5) _IAdvancedMathProxyVtbl = {
    &IAdvancedMath_ProxyInfo,
    &IID_IAdvancedMath,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) - 1 /* IAdvancedMath::Factorial */ ,
    (void *) - 1 /* IAdvancedMath::Fibonacci */
};

const CInterfaceStubVtbl _IAdvancedMathStubVtbl = {
    &IID_IAdvancedMath,
    &IAdvancedMath_ServerInfo,
    5,
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

        /* Procedure Add */

        0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /*  2 */    NdrFcShort(0x3),     /* 3 */
#ifndef _ALPHA_
        /*  4 */    NdrFcShort(0x14),    /* x86, MIPS, PPC Stack size/offset = 20 */
#else
        NdrFcShort(0x28),    /* Alpha Stack size/offset = 40 */
#endif
        /*  6 */    NdrFcShort(0x10),    /* 16 */
        /*  8 */    NdrFcShort(0x10),    /* 16 */
        /* 10 */    0x4,        /* 4 */
        0x4,        /* 4 */

        /* Parameter lOp1 */

        /* 12 */    NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 14 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 16 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter lOp2 */

        /* 18 */    NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 20 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 22 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter plResult */

        /* 24 */    NdrFcShort(0x2150),      /* 8528 */
#ifndef _ALPHA_
        /* 26 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 28 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 30 */    NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 32 */    NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 34 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Subtract */

        /* 36 */    0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 38 */    NdrFcShort(0x4),     /* 4 */
#ifndef _ALPHA_
        /* 40 */    NdrFcShort(0x14),    /* x86, MIPS, PPC Stack size/offset = 20 */
#else
        NdrFcShort(0x28),    /* Alpha Stack size/offset = 40 */
#endif
        /* 42 */    NdrFcShort(0x10),    /* 16 */
        /* 44 */    NdrFcShort(0x10),    /* 16 */
        /* 46 */    0x4,        /* 4 */
        0x4,        /* 4 */

        /* Parameter lOp1 */

        /* 48 */    NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 50 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 52 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter lOp2 */

        /* 54 */    NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 56 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 58 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter plResult */

        /* 60 */    NdrFcShort(0x2150),      /* 8528 */
#ifndef _ALPHA_
        /* 62 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 64 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 66 */    NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 68 */    NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 70 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Multiply */

        /* 72 */    0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 74 */    NdrFcShort(0x5),     /* 5 */
#ifndef _ALPHA_
        /* 76 */    NdrFcShort(0x14),    /* x86, MIPS, PPC Stack size/offset = 20 */
#else
        NdrFcShort(0x28),    /* Alpha Stack size/offset = 40 */
#endif
        /* 78 */    NdrFcShort(0x10),    /* 16 */
        /* 80 */    NdrFcShort(0x10),    /* 16 */
        /* 82 */    0x4,        /* 4 */
        0x4,        /* 4 */

        /* Parameter lOp1 */

        /* 84 */    NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 86 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 88 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter lOp2 */

        /* 90 */    NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 92 */    NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 94 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter plResult */

        /* 96 */    NdrFcShort(0x2150),      /* 8528 */
#ifndef _ALPHA_
        /* 98 */    NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 100 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 102 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 104 */   NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 106 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Divide */

        /* 108 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 110 */   NdrFcShort(0x6),     /* 6 */
#ifndef _ALPHA_
        /* 112 */   NdrFcShort(0x14),    /* x86, MIPS, PPC Stack size/offset = 20 */
#else
        NdrFcShort(0x28),    /* Alpha Stack size/offset = 40 */
#endif
        /* 114 */   NdrFcShort(0x10),    /* 16 */
        /* 116 */   NdrFcShort(0x10),    /* 16 */
        /* 118 */   0x4,        /* 4 */
        0x4,        /* 4 */

        /* Parameter lOp1 */

        /* 120 */   NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 122 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 124 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter lOp2 */

        /* 126 */   NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 128 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 130 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter plResult */

        /* 132 */   NdrFcShort(0x2150),      /* 8528 */
#ifndef _ALPHA_
        /* 134 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 136 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 138 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 140 */   NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 142 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Factorial */

        /* 144 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 146 */   NdrFcShort(0x3),     /* 3 */
#ifndef _ALPHA_
        /* 148 */   NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 150 */   NdrFcShort(0x6),     /* 6 */
        /* 152 */   NdrFcShort(0x10),    /* 16 */
        /* 154 */   0x4,        /* 4 */
        0x3,        /* 3 */

        /* Parameter sFact */

        /* 156 */   NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 158 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 160 */   0x6,        /* FC_SHORT */
        0x0,        /* 0 */

        /* Parameter pResult */

        /* 162 */   NdrFcShort(0x2150),      /* 8528 */
#ifndef _ALPHA_
        /* 164 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 166 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 168 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 170 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 172 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Fibonacci */

        /* 174 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 176 */   NdrFcShort(0x4),     /* 4 */
#ifndef _ALPHA_
        /* 178 */   NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 180 */   NdrFcShort(0x6),     /* 6 */
        /* 182 */   NdrFcShort(0x10),    /* 16 */
        /* 184 */   0x4,        /* 4 */
        0x3,        /* 3 */

        /* Parameter sFib */

        /* 186 */   NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 188 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 190 */   0x6,        /* FC_SHORT */
        0x0,        /* 0 */

        /* Parameter pResult */

        /* 192 */   NdrFcShort(0x2150),      /* 8528 */
#ifndef _ALPHA_
        /* 194 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 196 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 198 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 200 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 202 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        0x0
    }
};

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString = {
    0,
    {
        0x11, 0xc,  /* FC_RP [alloced_on_stack] [simple_pointer] */
        /*  2 */    0x8,        /* FC_LONG */
        0x5c,       /* FC_PAD */

        0x0
    }
};

const CInterfaceProxyVtbl * _Chapter3_Server_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_IAdvancedMathProxyVtbl,
    (CInterfaceProxyVtbl *) &_IMathProxyVtbl,
    0
};

const CInterfaceStubVtbl * _Chapter3_Server_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_IAdvancedMathStubVtbl,
    (CInterfaceStubVtbl *) &_IMathStubVtbl,
    0
};

PCInterfaceName const _Chapter3_Server_InterfaceNamesList[] = {
    "IAdvancedMath",
    "IMath",
    0
};


#define _Chapter3_Server_CHECK_IID(n)   IID_GENERIC_CHECK_IID( _Chapter3_Server, pIID, n)

int __stdcall _Chapter3_Server_IID_Lookup(const IID * pIID, int * pIndex)
{
    IID_BS_LOOKUP_SETUP
    IID_BS_LOOKUP_INITIAL_TEST(_Chapter3_Server, 2, 1)
    IID_BS_LOOKUP_RETURN_RESULT(_Chapter3_Server, 2, *pIndex)
}

const ExtendedProxyFileInfo Chapter3_Server_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _Chapter3_Server_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _Chapter3_Server_StubVtblList,
    (const PCInterfaceName *) & _Chapter3_Server_InterfaceNamesList,
    0, // no delegation
    & _Chapter3_Server_IID_Lookup,
    2,
    2
};

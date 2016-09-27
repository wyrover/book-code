/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 3.01.75 */
/* at Mon Oct 06 22:26:09 1997
 */
/* Compiler settings for Chapter4_Server.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )

#define USE_STUBLESS_PROXY

#include "rpcproxy.h"
#include "Chapter4_Server.h"

#define TYPE_FORMAT_STRING_SIZE   113
#define PROC_FORMAT_STRING_SIZE   355

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


/* Standard interface: __MIDL_itf_Chapter4_Server_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IMath, ver. 0.0,
   GUID={0x5FB0C22E,0x3343,0x11D1,{0x88,0x3A,0x44,0x45,0x53,0x54,0x00,0x00}} */


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
   GUID={0x984D09A1,0x3379,0x11d1,{0x88,0x3A,0x44,0x45,0x53,0x54,0x00,0x00}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IAdvancedMath_ServerInfo;

#pragma code_seg(".orpc")
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


/* Standard interface: __MIDL_itf_Chapter4_Server_0137, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IComponentInfo, ver. 0.0,
   GUID={0x1E405AA0,0x3396,0x11d1,{0x88,0x3A,0x44,0x45,0x53,0x54,0x00,0x00}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IComponentInfo_ServerInfo;

#pragma code_seg(".orpc")
static const unsigned short IComponentInfo_FormatStringOffsetTable[] = {
    204,
    228
};

static const MIDL_SERVER_INFO IComponentInfo_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IComponentInfo_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IComponentInfo_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IComponentInfo_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(5) _IComponentInfoProxyVtbl = {
    &IComponentInfo_ProxyInfo,
    &IID_IComponentInfo,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) - 1 /* IComponentInfo::get_Info */ ,
    (void *) - 1 /* IComponentInfo::get_Name */
};

const CInterfaceStubVtbl _IComponentInfoStubVtbl = {
    &IID_IComponentInfo,
    &IComponentInfo_ServerInfo,
    5,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
};


/* Object interface: IMath2, ver. 0.0,
   GUID={0x984D09A2,0x3379,0x11d1,{0x88,0x3A,0x44,0x45,0x53,0x54,0x00,0x00}} */


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IMath2_ServerInfo;

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
    0, /* -error bounds_check flag */
    0x20000, /* Ndr library version */
    0,
    0x301004b, /* MIDL Version 3.1.75 */
    0,
    UserMarshalRoutines,
    0,  /* Reserved1 */
    0,  /* Reserved2 */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
};

static const unsigned short IMath2_FormatStringOffsetTable[] = {
    0,
    36,
    72,
    108,
    252,
    288,
    330
};

static const MIDL_SERVER_INFO IMath2_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IMath2_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};

static const MIDL_STUBLESS_PROXY_INFO IMath2_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IMath2_FormatStringOffsetTable[-3],
    0,
    0,
    0
};

CINTERFACE_PROXY_VTABLE(10) _IMath2ProxyVtbl = {
    &IMath2_ProxyInfo,
    &IID_IMath2,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    (void *) - 1 /* IMath2::Add */ ,
    (void *) - 1 /* IMath2::Subtract */ ,
    (void *) - 1 /* IMath2::Multiply */ ,
    (void *) - 1 /* IMath2::Divide */ ,
    (void *) - 1 /* IMath2::Sum */ ,
    (void *) - 1 /* IMath2::Compute */ ,
    (void *) - 1 /* IMath2::get_AdvancedMath */
};

const CInterfaceStubVtbl _IMath2StubVtbl = {
    &IID_IMath2,
    &IMath2_ServerInfo,
    10,
    0, /* pure interpreted */
    CStdStubBuffer_METHODS
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
    #error   -Oif or -Oicf, [wire_marshal] or [user_marshal] attribute.
    #error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
    #error This app will die there with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString = {
    0,
    {

        /* Procedure Add */


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

        /* Procedure get_Info */

        /* 204 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 206 */   NdrFcShort(0x3),     /* 3 */
#ifndef _ALPHA_
        /* 208 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 210 */   NdrFcShort(0x0),     /* 0 */
        /* 212 */   NdrFcShort(0x8),     /* 8 */
        /* 214 */   0x5,        /* 5 */
        0x2,        /* 2 */

        /* Parameter pInfo */

        /* 216 */   NdrFcShort(0x2013),      /* 8211 */
#ifndef _ALPHA_
        /* 218 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 220 */   NdrFcShort(0x4),     /* Type Offset=4 */

        /* Return value */

        /* 222 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 224 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 226 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_Name */

        /* 228 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 230 */   NdrFcShort(0x4),     /* 4 */
#ifndef _ALPHA_
        /* 232 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 234 */   NdrFcShort(0x0),     /* 0 */
        /* 236 */   NdrFcShort(0x8),     /* 8 */
        /* 238 */   0x5,        /* 5 */
        0x2,        /* 2 */

        /* Parameter bstrName */

        /* 240 */   NdrFcShort(0x2113),      /* 8467 */
#ifndef _ALPHA_
        /* 242 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 244 */   NdrFcShort(0x46),    /* Type Offset=70 */

        /* Return value */

        /* 246 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 248 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 250 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Sum */

        /* 252 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 254 */   NdrFcShort(0x7),     /* 7 */
#ifndef _ALPHA_
        /* 256 */   NdrFcShort(0x14),    /* x86, MIPS, PPC Stack size/offset = 20 */
#else
        NdrFcShort(0x28),    /* Alpha Stack size/offset = 40 */
#endif
        /* 258 */   NdrFcShort(0x6),     /* 6 */
        /* 260 */   NdrFcShort(0x10),    /* 16 */
        /* 262 */   0x6,        /* 6 */
        0x4,        /* 4 */

        /* Parameter sArraySize */

        /* 264 */   NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 266 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 268 */   0x6,        /* FC_SHORT */
        0x0,        /* 0 */

        /* Parameter sArray */

        /* 270 */   NdrFcShort(0xb),     /* 11 */
#ifndef _ALPHA_
        /* 272 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 274 */   NdrFcShort(0x50),    /* Type Offset=80 */

        /* Parameter lResult */

        /* 276 */   NdrFcShort(0x2150),      /* 8528 */
#ifndef _ALPHA_
        /* 278 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 280 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 282 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 284 */   NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 286 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Compute */

        /* 288 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 290 */   NdrFcShort(0x8),     /* 8 */
#ifndef _ALPHA_
        /* 292 */   NdrFcShort(0x18),    /* x86, MIPS, PPC Stack size/offset = 24 */
#else
        NdrFcShort(0x30),    /* Alpha Stack size/offset = 48 */
#endif
        /* 294 */   NdrFcShort(0x16),    /* 22 */
        /* 296 */   NdrFcShort(0x10),    /* 16 */
        /* 298 */   0x4,        /* 4 */
        0x5,        /* 5 */

        /* Parameter enumOp */

        /* 300 */   NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 302 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 304 */   0xd,        /* FC_ENUM16 */
        0x0,        /* 0 */

        /* Parameter lOp1 */

        /* 306 */   NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 308 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 310 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter lOp2 */

        /* 312 */   NdrFcShort(0x48),    /* 72 */
#ifndef _ALPHA_
        /* 314 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 316 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Parameter plResult */

        /* 318 */   NdrFcShort(0x2150),      /* 8528 */
#ifndef _ALPHA_
        /* 320 */   NdrFcShort(0x10),    /* x86, MIPS, PPC Stack size/offset = 16 */
#else
        NdrFcShort(0x20),    /* Alpha Stack size/offset = 32 */
#endif
        /* 322 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 324 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 326 */   NdrFcShort(0x14),    /* x86, MIPS, PPC Stack size/offset = 20 */
#else
        NdrFcShort(0x28),    /* Alpha Stack size/offset = 40 */
#endif
        /* 328 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_AdvancedMath */

        /* 330 */   0x33,       /* FC_AUTO_HANDLE */
        0x64,       /* 100 */
        /* 332 */   NdrFcShort(0x9),     /* 9 */
#ifndef _ALPHA_
        /* 334 */   NdrFcShort(0xc),     /* x86, MIPS, PPC Stack size/offset = 12 */
#else
        NdrFcShort(0x18),    /* Alpha Stack size/offset = 24 */
#endif
        /* 336 */   NdrFcShort(0x0),     /* 0 */
        /* 338 */   NdrFcShort(0x8),     /* 8 */
        /* 340 */   0x5,        /* 5 */
        0x2,        /* 2 */

        /* Parameter ppVal */

        /* 342 */   NdrFcShort(0x13),    /* 19 */
#ifndef _ALPHA_
        /* 344 */   NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 346 */   NdrFcShort(0x5a),    /* Type Offset=90 */

        /* Return value */

        /* 348 */   NdrFcShort(0x70),    /* 112 */
#ifndef _ALPHA_
        /* 350 */   NdrFcShort(0x8),     /* x86, MIPS, PPC Stack size/offset = 8 */
#else
        NdrFcShort(0x10),    /* Alpha Stack size/offset = 16 */
#endif
        /* 352 */   0x8,        /* FC_LONG */
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
        /*  4 */
        0x11, 0x14, /* FC_RP [alloced_on_stack] */
        /*  6 */    NdrFcShort(0x2),     /* Offset= 2 (8) */
        /*  8 */
        0x13, 0x0,  /* FC_OP */
        /* 10 */    NdrFcShort(0x24),    /* Offset= 36 (46) */
        /* 12 */
        0x13, 0x0,  /* FC_OP */
        /* 14 */    NdrFcShort(0xc),     /* Offset= 12 (26) */
        /* 16 */
        0x1b,       /* FC_CARRAY */
        0x1,        /* 1 */
        /* 18 */    NdrFcShort(0x2),     /* 2 */
        /* 20 */    0x9,        /* 9 */
        0x0,        /*  */
        /* 22 */    NdrFcShort(0xfffffffc),      /* -4 */
        /* 24 */    0x6,        /* FC_SHORT */
        0x5b,       /* FC_END */
        /* 26 */
        0x17,       /* FC_CSTRUCT */
        0x3,        /* 3 */
        /* 28 */    NdrFcShort(0x8),     /* 8 */
        /* 30 */    NdrFcShort(0xfffffff2),      /* Offset= -14 (16) */
        /* 32 */    0x8,        /* FC_LONG */
        0x8,        /* FC_LONG */
        /* 34 */    0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 36 */    0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 38 */    NdrFcShort(0x0),     /* 0 */
        /* 40 */    NdrFcShort(0x4),     /* 4 */
        /* 42 */    NdrFcShort(0x0),     /* 0 */
        /* 44 */    NdrFcShort(0xffffffe0),      /* Offset= -32 (12) */
        /* 46 */
        0x1a,       /* FC_BOGUS_STRUCT */
        0x3,        /* 3 */
        /* 48 */    NdrFcShort(0xc),     /* 12 */
        /* 50 */    NdrFcShort(0x0),     /* 0 */
        /* 52 */    NdrFcShort(0xa),     /* Offset= 10 (62) */
        /* 54 */    0x36,       /* FC_POINTER */
        0x6,        /* FC_SHORT */
        /* 56 */    0x6,        /* FC_SHORT */
        0x4c,       /* FC_EMBEDDED_COMPLEX */
        /* 58 */    0x0,        /* 0 */
        NdrFcShort(0xffffffe9),      /* Offset= -23 (36) */
        0x5b,       /* FC_END */
        /* 62 */
        0x13, 0x8,  /* FC_OP [simple_pointer] */
        /* 64 */
        0x22,       /* FC_C_CSTRING */
        0x5c,       /* FC_PAD */
        /* 66 */
        0x11, 0x4,  /* FC_RP [alloced_on_stack] */
        /* 68 */    NdrFcShort(0x2),     /* Offset= 2 (70) */
        /* 70 */    0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 72 */    NdrFcShort(0x0),     /* 0 */
        /* 74 */    NdrFcShort(0x4),     /* 4 */
        /* 76 */    NdrFcShort(0x0),     /* 0 */
        /* 78 */    NdrFcShort(0xffffffbe),      /* Offset= -66 (12) */
        /* 80 */
        0x1b,       /* FC_CARRAY */
        0x1,        /* 1 */
        /* 82 */    NdrFcShort(0x2),     /* 2 */
        /* 84 */    0x26,       /* 38 */
        0x0,        /*  */
#ifndef _ALPHA_
        /* 86 */    NdrFcShort(0x4),     /* x86, MIPS, PPC Stack size/offset = 4 */
#else
        NdrFcShort(0x8),     /* Alpha Stack size/offset = 8 */
#endif
        /* 88 */    0x6,        /* FC_SHORT */
        0x5b,       /* FC_END */
        /* 90 */
        0x11, 0x10, /* FC_RP */
        /* 92 */    NdrFcShort(0x2),     /* Offset= 2 (94) */
        /* 94 */
        0x2f,       /* FC_IP */
        0x5a,       /* FC_CONSTANT_IID */
        /* 96 */    NdrFcLong(0x984d09a1),   /* -1739781727 */
        /* 100 */   NdrFcShort(0x3379),      /* 13177 */
        /* 102 */   NdrFcShort(0x11d1),      /* 4561 */
        /* 104 */   0x88,       /* 136 */
        0x3a,       /* 58 */
        /* 106 */   0x44,       /* 68 */
        0x45,       /* 69 */
        /* 108 */   0x53,       /* 83 */
        0x54,       /* 84 */
        /* 110 */   0x0,        /* 0 */
        0x0,        /* 0 */

        0x0
    }
};

const CInterfaceProxyVtbl * _Chapter4_Server_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_IMathProxyVtbl,
    (CInterfaceProxyVtbl *) &_IComponentInfoProxyVtbl,
    (CInterfaceProxyVtbl *) &_IAdvancedMathProxyVtbl,
    (CInterfaceProxyVtbl *) &_IMath2ProxyVtbl,
    0
};

const CInterfaceStubVtbl * _Chapter4_Server_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_IMathStubVtbl,
    (CInterfaceStubVtbl *) &_IComponentInfoStubVtbl,
    (CInterfaceStubVtbl *) &_IAdvancedMathStubVtbl,
    (CInterfaceStubVtbl *) &_IMath2StubVtbl,
    0
};

PCInterfaceName const _Chapter4_Server_InterfaceNamesList[] = {
    "IMath",
    "IComponentInfo",
    "IAdvancedMath",
    "IMath2",
    0
};


#define _Chapter4_Server_CHECK_IID(n)   IID_GENERIC_CHECK_IID( _Chapter4_Server, pIID, n)

int __stdcall _Chapter4_Server_IID_Lookup(const IID * pIID, int * pIndex)
{
    IID_BS_LOOKUP_SETUP
    IID_BS_LOOKUP_INITIAL_TEST(_Chapter4_Server, 4, 2)
    IID_BS_LOOKUP_NEXT_TEST(_Chapter4_Server, 1)
    IID_BS_LOOKUP_RETURN_RESULT(_Chapter4_Server, 4, *pIndex)
}

const ExtendedProxyFileInfo Chapter4_Server_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _Chapter4_Server_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _Chapter4_Server_StubVtblList,
    (const PCInterfaceName *) & _Chapter4_Server_InterfaceNamesList,
    0, // no delegation
    & _Chapter4_Server_IID_Lookup,
    4,
    2
};

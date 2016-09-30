
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 6.00.0347 */
/* at Thu Nov 21 08:29:25 2002
 */
/* Compiler settings for SpeakerNoAttr.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data
    VC __declspec() decoration level:
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if !defined(_M_IA64) && !defined(_M_AMD64)
#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
    #define __REQUIRED_RPCPROXY_H_VERSION__ 440
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
    #error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "SpeakerNoAttr.h"

#define TYPE_FORMAT_STRING_SIZE   77
#define PROC_FORMAT_STRING_SIZE   253
#define TRANSMIT_AS_TABLE_SIZE    0
#define WIRE_MARSHAL_TABLE_SIZE   1

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


extern const MIDL_SERVER_INFO ISpeaker_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ISpeaker_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IListener_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IListener_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO INamedObject_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO INamedObject_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

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

        /* Procedure get_Volume */

        0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /*  2 */    NdrFcLong(0x0),      /* 0 */
        /*  6 */    NdrFcShort(0x7),     /* 7 */
        /*  8 */    NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 10 */    NdrFcShort(0x0),     /* 0 */
        /* 12 */    NdrFcShort(0x24),    /* 36 */
        /* 14 */    0x4,        /* Oi2 Flags:  has return, */
        0x2,        /* 2 */

        /* Parameter pVal */

        /* 16 */    NdrFcShort(0x2150),      /* Flags:  out, base type, simple ref, srv alloc size=8 */
        /* 18 */    NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 20 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 22 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 24 */    NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 26 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_Volume */

        /* 28 */    0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 30 */    NdrFcLong(0x0),      /* 0 */
        /* 34 */    NdrFcShort(0x8),     /* 8 */
        /* 36 */    NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 38 */    NdrFcShort(0x8),     /* 8 */
        /* 40 */    NdrFcShort(0x8),     /* 8 */
        /* 42 */    0x4,        /* Oi2 Flags:  has return, */
        0x2,        /* 2 */

        /* Parameter newVal */

        /* 44 */    NdrFcShort(0x48),    /* Flags:  in, base type, */
        /* 46 */    NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 48 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Return value */

        /* 50 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 52 */    NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 54 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_Speech */

        /* 56 */    0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 58 */    NdrFcLong(0x0),      /* 0 */
        /* 62 */    NdrFcShort(0x9),     /* 9 */
        /* 64 */    NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 66 */    NdrFcShort(0x0),     /* 0 */
        /* 68 */    NdrFcShort(0x8),     /* 8 */
        /* 70 */    0x5,        /* Oi2 Flags:  srv must size, has return, */
        0x2,        /* 2 */

        /* Parameter pVal */

        /* 72 */    NdrFcShort(0x2113),      /* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
        /* 74 */    NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 76 */    NdrFcShort(0x22),    /* Type Offset=34 */

        /* Return value */

        /* 78 */    NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 80 */    NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 82 */    0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_Speech */

        /* 84 */    0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 86 */    NdrFcLong(0x0),      /* 0 */
        /* 90 */    NdrFcShort(0xa),     /* 10 */
        /* 92 */    NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 94 */    NdrFcShort(0x0),     /* 0 */
        /* 96 */    NdrFcShort(0x8),     /* 8 */
        /* 98 */    0x6,        /* Oi2 Flags:  clt must size, has return, */
        0x2,        /* 2 */

        /* Parameter newVal */

        /* 100 */   NdrFcShort(0x8b),    /* Flags:  must size, must free, in, by val, */
        /* 102 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 104 */   NdrFcShort(0x30),    /* Type Offset=48 */

        /* Return value */

        /* 106 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 108 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 110 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure Speak */

        /* 112 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 114 */   NdrFcLong(0x0),      /* 0 */
        /* 118 */   NdrFcShort(0xb),     /* 11 */
        /* 120 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 122 */   NdrFcShort(0x0),     /* 0 */
        /* 124 */   NdrFcShort(0x8),     /* 8 */
        /* 126 */   0x4,        /* Oi2 Flags:  has return, */
        0x1,        /* 1 */

        /* Return value */

        /* 128 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 130 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 132 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure ListenTo */

        /* 134 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 136 */   NdrFcLong(0x0),      /* 0 */
        /* 140 */   NdrFcShort(0x7),     /* 7 */
        /* 142 */   NdrFcShort(0x10),    /* x86 Stack size/offset = 16 */
        /* 144 */   NdrFcShort(0x6),     /* 6 */
        /* 146 */   NdrFcShort(0x8),     /* 8 */
        /* 148 */   0x6,        /* Oi2 Flags:  clt must size, has return, */
        0x3,        /* 3 */

        /* Parameter role */

        /* 150 */   NdrFcShort(0x48),    /* Flags:  in, base type, */
        /* 152 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 154 */   0xd,        /* FC_ENUM16 */
        0x0,        /* 0 */

        /* Parameter pSpeaker */

        /* 156 */   NdrFcShort(0xb),     /* Flags:  must size, must free, in, */
        /* 158 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 160 */   NdrFcShort(0x3a),    /* Type Offset=58 */

        /* Return value */

        /* 162 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 164 */   NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 166 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure StopListening */

        /* 168 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 170 */   NdrFcLong(0x0),      /* 0 */
        /* 174 */   NdrFcShort(0x8),     /* 8 */
        /* 176 */   NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 178 */   NdrFcShort(0x6),     /* 6 */
        /* 180 */   NdrFcShort(0x8),     /* 8 */
        /* 182 */   0x4,        /* Oi2 Flags:  has return, */
        0x2,        /* 2 */

        /* Parameter role */

        /* 184 */   NdrFcShort(0x48),    /* Flags:  in, base type, */
        /* 186 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 188 */   0xd,        /* FC_ENUM16 */
        0x0,        /* 0 */

        /* Return value */

        /* 190 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 192 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 194 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure get_Name */

        /* 196 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 198 */   NdrFcLong(0x0),      /* 0 */
        /* 202 */   NdrFcShort(0x7),     /* 7 */
        /* 204 */   NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 206 */   NdrFcShort(0x0),     /* 0 */
        /* 208 */   NdrFcShort(0x8),     /* 8 */
        /* 210 */   0x5,        /* Oi2 Flags:  srv must size, has return, */
        0x2,        /* 2 */

        /* Parameter pVal */

        /* 212 */   NdrFcShort(0x2113),      /* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
        /* 214 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 216 */   NdrFcShort(0x22),    /* Type Offset=34 */

        /* Return value */

        /* 218 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 220 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 222 */   0x8,        /* FC_LONG */
        0x0,        /* 0 */

        /* Procedure put_Name */

        /* 224 */   0x33,       /* FC_AUTO_HANDLE */
        0x6c,       /* Old Flags:  object, Oi2 */
        /* 226 */   NdrFcLong(0x0),      /* 0 */
        /* 230 */   NdrFcShort(0x8),     /* 8 */
        /* 232 */   NdrFcShort(0xc),     /* x86 Stack size/offset = 12 */
        /* 234 */   NdrFcShort(0x0),     /* 0 */
        /* 236 */   NdrFcShort(0x8),     /* 8 */
        /* 238 */   0x6,        /* Oi2 Flags:  clt must size, has return, */
        0x2,        /* 2 */

        /* Parameter newVal */

        /* 240 */   NdrFcShort(0x8b),    /* Flags:  must size, must free, in, by val, */
        /* 242 */   NdrFcShort(0x4),     /* x86 Stack size/offset = 4 */
        /* 244 */   NdrFcShort(0x30),    /* Type Offset=48 */

        /* Return value */

        /* 246 */   NdrFcShort(0x70),    /* Flags:  out, return, base type, */
        /* 248 */   NdrFcShort(0x8),     /* x86 Stack size/offset = 8 */
        /* 250 */   0x8,        /* FC_LONG */
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
        0x11, 0x4,  /* FC_RP [alloced_on_stack] */
        /*  8 */    NdrFcShort(0x1a),    /* Offset= 26 (34) */
        /* 10 */
        0x13, 0x0,  /* FC_OP */
        /* 12 */    NdrFcShort(0xc),     /* Offset= 12 (24) */
        /* 14 */
        0x1b,       /* FC_CARRAY */
        0x1,        /* 1 */
        /* 16 */    NdrFcShort(0x2),     /* 2 */
        /* 18 */    0x9,        /* Corr desc: FC_ULONG */
        0x0,        /*  */
        /* 20 */    NdrFcShort(0xfffc),      /* -4 */
        /* 22 */    0x6,        /* FC_SHORT */
        0x5b,       /* FC_END */
        /* 24 */
        0x17,       /* FC_CSTRUCT */
        0x3,        /* 3 */
        /* 26 */    NdrFcShort(0x8),     /* 8 */
        /* 28 */    NdrFcShort(0xfffffff2),      /* Offset= -14 (14) */
        /* 30 */    0x8,        /* FC_LONG */
        0x8,        /* FC_LONG */
        /* 32 */    0x5c,       /* FC_PAD */
        0x5b,       /* FC_END */
        /* 34 */    0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 36 */    NdrFcShort(0x0),     /* 0 */
        /* 38 */    NdrFcShort(0x4),     /* 4 */
        /* 40 */    NdrFcShort(0x0),     /* 0 */
        /* 42 */    NdrFcShort(0xffffffe0),      /* Offset= -32 (10) */
        /* 44 */
        0x12, 0x0,  /* FC_UP */
        /* 46 */    NdrFcShort(0xffffffea),      /* Offset= -22 (24) */
        /* 48 */    0xb4,       /* FC_USER_MARSHAL */
        0x83,       /* 131 */
        /* 50 */    NdrFcShort(0x0),     /* 0 */
        /* 52 */    NdrFcShort(0x4),     /* 4 */
        /* 54 */    NdrFcShort(0x0),     /* 0 */
        /* 56 */    NdrFcShort(0xfffffff4),      /* Offset= -12 (44) */
        /* 58 */
        0x2f,       /* FC_IP */
        0x5a,       /* FC_CONSTANT_IID */
        /* 60 */    NdrFcLong(0x0),      /* 0 */
        /* 64 */    NdrFcShort(0x0),     /* 0 */
        /* 66 */    NdrFcShort(0x0),     /* 0 */
        /* 68 */    0xc0,       /* 192 */
        0x0,        /* 0 */
        /* 70 */    0x0,        /* 0 */
        0x0,        /* 0 */
        /* 72 */    0x0,        /* 0 */
        0x0,        /* 0 */
        /* 74 */    0x0,        /* 0 */
        0x46,       /* 70 */

        0x0
    }
};

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = {

    {
        BSTR_UserSize
        , BSTR_UserMarshal
        , BSTR_UserUnmarshal
        , BSTR_UserFree
    }

};



/* Standard interface: __MIDL_itf_SpeakerNoAttr_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ISpeaker, ver. 0.0,
   GUID={0xCE00527D,0xF8E3,0x46A3,{0x8B,0xC8,0xA2,0x53,0x45,0xAD,0x6C,0xAA}} */

#pragma code_seg(".orpc")
static const unsigned short ISpeaker_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    0,
    28,
    56,
    84,
    112
};

static const MIDL_STUBLESS_PROXY_INFO ISpeaker_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &ISpeaker_FormatStringOffsetTable[-3],
    0,
    0,
    0
};


static const MIDL_SERVER_INFO ISpeaker_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &ISpeaker_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};
CINTERFACE_PROXY_VTABLE(12) _ISpeakerProxyVtbl = {
    &ISpeaker_ProxyInfo,
    &IID_ISpeaker,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *)(INT_PTR) - 1 /* ISpeaker::get_Volume */ ,
    (void *)(INT_PTR) - 1 /* ISpeaker::put_Volume */ ,
    (void *)(INT_PTR) - 1 /* ISpeaker::get_Speech */ ,
    (void *)(INT_PTR) - 1 /* ISpeaker::put_Speech */ ,
    (void *)(INT_PTR) - 1 /* ISpeaker::Speak */
};


static const PRPC_STUB_FUNCTION ISpeaker_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _ISpeakerStubVtbl = {
    &IID_ISpeaker,
    &ISpeaker_ServerInfo,
    12,
    &ISpeaker_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IListener, ver. 0.0,
   GUID={0xBECC670E,0x7A07,0x4032,{0xA5,0xA3,0x07,0xD1,0x55,0x8A,0xE6,0x5D}} */

#pragma code_seg(".orpc")
static const unsigned short IListener_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    134,
    168
};

static const MIDL_STUBLESS_PROXY_INFO IListener_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &IListener_FormatStringOffsetTable[-3],
    0,
    0,
    0
};


static const MIDL_SERVER_INFO IListener_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &IListener_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};
CINTERFACE_PROXY_VTABLE(9) _IListenerProxyVtbl = {
    &IListener_ProxyInfo,
    &IID_IListener,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *)(INT_PTR) - 1 /* IListener::ListenTo */ ,
    (void *)(INT_PTR) - 1 /* IListener::StopListening */
};


static const PRPC_STUB_FUNCTION IListener_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IListenerStubVtbl = {
    &IID_IListener,
    &IListener_ServerInfo,
    9,
    &IListener_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: INamedObject, ver. 0.0,
   GUID={0x450D6780,0xFC73,0x11d1,{0x8C,0xC3,0x00,0xA0,0xC9,0xC8,0xE5,0x0D}} */

#pragma code_seg(".orpc")
static const unsigned short INamedObject_FormatStringOffsetTable[] = {
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    (unsigned short) - 1,
    196,
    224
};

static const MIDL_STUBLESS_PROXY_INFO INamedObject_ProxyInfo = {
    &Object_StubDesc,
    __MIDL_ProcFormatString.Format,
    &INamedObject_FormatStringOffsetTable[-3],
    0,
    0,
    0
};


static const MIDL_SERVER_INFO INamedObject_ServerInfo = {
    &Object_StubDesc,
    0,
    __MIDL_ProcFormatString.Format,
    &INamedObject_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0
};
CINTERFACE_PROXY_VTABLE(9) _INamedObjectProxyVtbl = {
    &INamedObject_ProxyInfo,
    &IID_INamedObject,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *)(INT_PTR) - 1 /* INamedObject::get_Name */ ,
    (void *)(INT_PTR) - 1 /* INamedObject::put_Name */
};


static const PRPC_STUB_FUNCTION INamedObject_table[] = {
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _INamedObjectStubVtbl = {
    &IID_INamedObject,
    &INamedObject_ServerInfo,
    9,
    &INamedObject_table[-3],
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
    0x20000, /* Ndr library version */
    0,
    0x600015b, /* MIDL Version 6.0.347 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0   /* Reserved5 */
};

const CInterfaceProxyVtbl * _SpeakerNoAttr_ProxyVtblList[] = {
    (CInterfaceProxyVtbl *) &_IListenerProxyVtbl,
    (CInterfaceProxyVtbl *) &_ISpeakerProxyVtbl,
    (CInterfaceProxyVtbl *) &_INamedObjectProxyVtbl,
    0
};

const CInterfaceStubVtbl * _SpeakerNoAttr_StubVtblList[] = {
    (CInterfaceStubVtbl *) &_IListenerStubVtbl,
    (CInterfaceStubVtbl *) &_ISpeakerStubVtbl,
    (CInterfaceStubVtbl *) &_INamedObjectStubVtbl,
    0
};

PCInterfaceName const _SpeakerNoAttr_InterfaceNamesList[] = {
    "IListener",
    "ISpeaker",
    "INamedObject",
    0
};

const IID *  _SpeakerNoAttr_BaseIIDList[] = {
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _SpeakerNoAttr_CHECK_IID(n) IID_GENERIC_CHECK_IID( _SpeakerNoAttr, pIID, n)

int __stdcall _SpeakerNoAttr_IID_Lookup(const IID * pIID, int * pIndex)
{
    IID_BS_LOOKUP_SETUP
    IID_BS_LOOKUP_INITIAL_TEST(_SpeakerNoAttr, 3, 2)
    IID_BS_LOOKUP_NEXT_TEST(_SpeakerNoAttr, 1)
    IID_BS_LOOKUP_RETURN_RESULT(_SpeakerNoAttr, 3, *pIndex)
}

const ExtendedProxyFileInfo SpeakerNoAttr_ProxyFileInfo = {
    (PCInterfaceProxyVtblList *) & _SpeakerNoAttr_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _SpeakerNoAttr_StubVtblList,
    (const PCInterfaceName *) & _SpeakerNoAttr_InterfaceNamesList,
    (const IID **) & _SpeakerNoAttr_BaseIIDList,
    & _SpeakerNoAttr_IID_Lookup,
    3,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/


/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Jan 01 23:07:12 2003
 */
/* Compiler settings for D:\MyWorkspace\ConnectionCOM\ConnectionCOM.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
    #define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
    #error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
    #include "windows.h"
    #include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ConnectionCOM_h__
#define __ConnectionCOM_h__

#ifdef __cplusplus
extern "C" {
#endif

/* Forward Declarations */

#ifndef __IAdd_FWD_DEFINED__
#define __IAdd_FWD_DEFINED__
typedef interface IAdd IAdd;
#endif  /* __IAdd_FWD_DEFINED__ */


#ifndef ___IAddEvents_FWD_DEFINED__
#define ___IAddEvents_FWD_DEFINED__
typedef interface _IAddEvents _IAddEvents;
#endif  /* ___IAddEvents_FWD_DEFINED__ */


#ifndef __Add_FWD_DEFINED__
#define __Add_FWD_DEFINED__

#ifdef __cplusplus
typedef class Add Add;
#else
typedef struct Add Add;
#endif /* __cplusplus */

#endif  /* __Add_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void __RPC_FAR *);

#ifndef __IAdd_INTERFACE_DEFINED__
#define __IAdd_INTERFACE_DEFINED__

/* interface IAdd */
/* [unique][helpstring][dual][uuid][object] */


EXTERN_C const IID IID_IAdd;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("295D9570-C579-4696-8BE2-C069DEC06F36")
IAdd :
public IDispatch {
public:
    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add(
        int a,
        int b) = 0;

};

#else   /* C style interface */

typedef struct IAddVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IAdd __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IAdd __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IAdd __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        IAdd __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        IAdd __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        IAdd __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        IAdd __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Add)(
        IAdd __RPC_FAR * This,
        int a,
        int b);

    END_INTERFACE
} IAddVtbl;

interface IAdd {
    CONST_VTBL struct IAddVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IAdd_QueryInterface(This,riid,ppvObject)    \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAdd_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define IAdd_Release(This)  \
    (This)->lpVtbl -> Release(This)


#define IAdd_GetTypeInfoCount(This,pctinfo) \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAdd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)  \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAdd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAdd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAdd_Add(This,a,b)  \
    (This)->lpVtbl -> Add(This,a,b)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAdd_Add_Proxy(
    IAdd __RPC_FAR * This,
    int a,
    int b);


void __RPC_STUB IAdd_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IAdd_INTERFACE_DEFINED__ */



#ifndef __CONNECTIONCOMLib_LIBRARY_DEFINED__
#define __CONNECTIONCOMLib_LIBRARY_DEFINED__

/* library CONNECTIONCOMLib */
/* [helpstring][version][uuid] */


EXTERN_C const IID LIBID_CONNECTIONCOMLib;

#ifndef ___IAddEvents_INTERFACE_DEFINED__
#define ___IAddEvents_INTERFACE_DEFINED__

/* interface _IAddEvents */
/* [object][helpstring][uuid] */


EXTERN_C const IID IID__IAddEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("AFE854B0-246F-4B66-B26F-A1060225C71C")
_IAddEvents :
public IUnknown {
public:
    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ExecutionOver(
        int Result) = 0;

};

#else   /* C style interface */

typedef struct _IAddEventsVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        _IAddEvents __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        _IAddEvents __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        _IAddEvents __RPC_FAR * This);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *ExecutionOver)(
        _IAddEvents __RPC_FAR * This,
        int Result);

    END_INTERFACE
} _IAddEventsVtbl;

interface _IAddEvents {
    CONST_VTBL struct _IAddEventsVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define _IAddEvents_QueryInterface(This,riid,ppvObject) \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IAddEvents_AddRef(This)    \
    (This)->lpVtbl -> AddRef(This)

#define _IAddEvents_Release(This)   \
    (This)->lpVtbl -> Release(This)


#define _IAddEvents_ExecutionOver(This,Result)  \
    (This)->lpVtbl -> ExecutionOver(This,Result)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE _IAddEvents_ExecutionOver_Proxy(
    _IAddEvents __RPC_FAR * This,
    int Result);


void __RPC_STUB _IAddEvents_ExecutionOver_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* ___IAddEvents_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Add;

#ifdef __cplusplus

class DECLSPEC_UUID("630B3CD3-DDB1-43CE-AD2F-4F57DC54D5D0")
    Add;
#endif
#endif /* __CONNECTIONCOMLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif

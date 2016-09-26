/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Sep 28 11:57:05 2000
 */
/* Compiler settings for C:\Vccom\chap10\ASPComSamp\ASPComSamp.idl:
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

#ifndef __ASPComSamp_h__
#define __ASPComSamp_h__

#ifdef __cplusplus
extern "C" {
#endif

/* Forward Declarations */

#ifndef __IASPComp_FWD_DEFINED__
#define __IASPComp_FWD_DEFINED__
typedef interface IASPComp IASPComp;
#endif  /* __IASPComp_FWD_DEFINED__ */


#ifndef __ASPComp_FWD_DEFINED__
#define __ASPComp_FWD_DEFINED__

#ifdef __cplusplus
typedef class ASPComp ASPComp;
#else
typedef struct ASPComp ASPComp;
#endif /* __cplusplus */

#endif  /* __ASPComp_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void __RPC_FAR *);

#ifndef __IASPComp_INTERFACE_DEFINED__
#define __IASPComp_INTERFACE_DEFINED__

/* interface IASPComp */
/* [unique][helpstring][dual][uuid][object] */


EXTERN_C const IID IID_IASPComp;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("1474A2ED-951F-11D4-896A-FD2AD650DB79")
IASPComp :
public IDispatch {
public:
    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ProcessRequest(void) = 0;

};

#else   /* C style interface */

typedef struct IASPCompVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IASPComp __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IASPComp __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IASPComp __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        IASPComp __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        IASPComp __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        IASPComp __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        IASPComp __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *ProcessRequest)(
        IASPComp __RPC_FAR * This);

    END_INTERFACE
} IASPCompVtbl;

interface IASPComp {
    CONST_VTBL struct IASPCompVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IASPComp_QueryInterface(This,riid,ppvObject)    \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IASPComp_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define IASPComp_Release(This)  \
    (This)->lpVtbl -> Release(This)


#define IASPComp_GetTypeInfoCount(This,pctinfo) \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IASPComp_GetTypeInfo(This,iTInfo,lcid,ppTInfo)  \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IASPComp_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IASPComp_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IASPComp_ProcessRequest(This)   \
    (This)->lpVtbl -> ProcessRequest(This)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IASPComp_ProcessRequest_Proxy(
    IASPComp __RPC_FAR * This);


void __RPC_STUB IASPComp_ProcessRequest_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IASPComp_INTERFACE_DEFINED__ */



#ifndef __ASPCOMSAMPLib_LIBRARY_DEFINED__
#define __ASPCOMSAMPLib_LIBRARY_DEFINED__

/* library ASPCOMSAMPLib */
/* [helpstring][version][uuid] */


EXTERN_C const IID LIBID_ASPCOMSAMPLib;

EXTERN_C const CLSID CLSID_ASPComp;

#ifdef __cplusplus

class DECLSPEC_UUID("1474A2EE-951F-11D4-896A-FD2AD650DB79")
    ASPComp;
#endif
#endif /* __ASPCOMSAMPLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif

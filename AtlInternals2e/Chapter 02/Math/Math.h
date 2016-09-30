
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 6.00.0347 */
/* at Mon Sep 23 10:15:08 2002
 */
/* Compiler settings for Math.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data
    VC __declspec() decoration level:
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
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

#ifndef __Math_h__
#define __Math_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
    #pragma once
#endif

/* Forward Declarations */

#ifndef __ICalc_FWD_DEFINED__
    #define __ICalc_FWD_DEFINED__
    typedef interface ICalc ICalc;
#endif  /* __ICalc_FWD_DEFINED__ */


#ifndef __Calc_FWD_DEFINED__
    #define __Calc_FWD_DEFINED__

    #ifdef __cplusplus
        typedef class Calc Calc;
    #else
        typedef struct Calc Calc;
    #endif /* __cplusplus */

#endif  /* __Calc_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C" {
#endif

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void *);

#ifndef __ICalc_INTERFACE_DEFINED__
#define __ICalc_INTERFACE_DEFINED__

/* interface ICalc */
/* [unique][helpstring][nonextensible][dual][uuid][object] */


EXTERN_C const IID IID_ICalc;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("2F6C88D7-C2BF-4933-81FA-3FBAFC3FC34B")
ICalc :
public IDispatch {
public:
    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add(
        /* [in] */ DOUBLE Op1,
        /* [in] */ DOUBLE Op2,
        /* [retval][out] */ DOUBLE * Result) = 0;

};

#else   /* C style interface */

typedef struct ICalcVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE *QueryInterface)(
        ICalc * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject);

    ULONG(STDMETHODCALLTYPE *AddRef)(
        ICalc * This);

    ULONG(STDMETHODCALLTYPE *Release)(
        ICalc * This);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfoCount)(
        ICalc * This,
        /* [out] */ UINT *pctinfo);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfo)(
        ICalc * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo **ppTInfo);

    HRESULT(STDMETHODCALLTYPE *GetIDsOfNames)(
        ICalc * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE *Invoke)(
        ICalc * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS *pDispParams,
        /* [out] */ VARIANT *pVarResult,
        /* [out] */ EXCEPINFO *pExcepInfo,
        /* [out] */ UINT *puArgErr);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *Add)(
        ICalc * This,
        /* [in] */ DOUBLE Op1,
        /* [in] */ DOUBLE Op2,
        /* [retval][out] */ DOUBLE *Result);

    END_INTERFACE
} ICalcVtbl;

interface ICalc {
    CONST_VTBL struct ICalcVtbl *lpVtbl;
};



#ifdef COBJMACROS


#define ICalc_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICalc_AddRef(This)  \
    (This)->lpVtbl -> AddRef(This)

#define ICalc_Release(This) \
    (This)->lpVtbl -> Release(This)


#define ICalc_GetTypeInfoCount(This,pctinfo)    \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICalc_GetTypeInfo(This,iTInfo,lcid,ppTInfo) \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICalc_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICalc_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ICalc_Add(This,Op1,Op2,Result)  \
    (This)->lpVtbl -> Add(This,Op1,Op2,Result)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ICalc_Add_Proxy(
    ICalc * This,
    /* [in] */ DOUBLE Op1,
    /* [in] */ DOUBLE Op2,
    /* [retval][out] */ DOUBLE *Result);


void __RPC_STUB ICalc_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __ICalc_INTERFACE_DEFINED__ */



#ifndef __MathLib_LIBRARY_DEFINED__
#define __MathLib_LIBRARY_DEFINED__

/* library MathLib */
/* [helpstring][version][uuid] */


EXTERN_C const IID LIBID_MathLib;

EXTERN_C const CLSID CLSID_Calc;

#ifdef __cplusplus

class DECLSPEC_UUID("3718FAF2-7F65-46DC-AFFD-F44E469804DA")
    Calc;
#endif
#endif /* __MathLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



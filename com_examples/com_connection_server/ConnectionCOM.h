

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 7.00.0555 */
/* at Mon Sep 26 18:23:05 2016
 */
/* Compiler settings for .\ConnectionCOM.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data
    VC __declspec() decoration level:
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
    #define __REQUIRED_RPCNDR_H_VERSION__ 475
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

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
    #pragma once
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

#ifdef __cplusplus
extern "C" {
#endif


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

    HRESULT(STDMETHODCALLTYPE *QueryInterface)(
        IAdd * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        __RPC__deref_out  void **ppvObject);

    ULONG(STDMETHODCALLTYPE *AddRef)(
        IAdd * This);

    ULONG(STDMETHODCALLTYPE *Release)(
        IAdd * This);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfoCount)(
        IAdd * This,
        /* [out] */ UINT *pctinfo);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfo)(
        IAdd * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo **ppTInfo);

    HRESULT(STDMETHODCALLTYPE *GetIDsOfNames)(
        IAdd * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR *rgszNames,
        /* [range][in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE *Invoke)(
        IAdd * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS *pDispParams,
        /* [out] */ VARIANT *pVarResult,
        /* [out] */ EXCEPINFO *pExcepInfo,
        /* [out] */ UINT *puArgErr);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *Add)(
        IAdd * This,
        int a,
        int b);

    END_INTERFACE
} IAddVtbl;

interface IAdd {
    CONST_VTBL struct IAddVtbl *lpVtbl;
};



#ifdef COBJMACROS


#define IAdd_QueryInterface(This,riid,ppvObject)    \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )

#define IAdd_AddRef(This)   \
    ( (This)->lpVtbl -> AddRef(This) )

#define IAdd_Release(This)  \
    ( (This)->lpVtbl -> Release(This) )


#define IAdd_GetTypeInfoCount(This,pctinfo) \
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) )

#define IAdd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)  \
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) )

#define IAdd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) )

#define IAdd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  \
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) )


#define IAdd_Add(This,a,b)  \
    ( (This)->lpVtbl -> Add(This,a,b) )

#endif /* COBJMACROS */


#endif  /* C style interface */




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

    HRESULT(STDMETHODCALLTYPE *QueryInterface)(
        _IAddEvents * This,
        /* [in] */ REFIID riid,
        /* [annotation][iid_is][out] */
        __RPC__deref_out  void **ppvObject);

    ULONG(STDMETHODCALLTYPE *AddRef)(
        _IAddEvents * This);

    ULONG(STDMETHODCALLTYPE *Release)(
        _IAddEvents * This);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *ExecutionOver)(
        _IAddEvents * This,
        int Result);

    END_INTERFACE
} _IAddEventsVtbl;

interface _IAddEvents {
    CONST_VTBL struct _IAddEventsVtbl *lpVtbl;
};



#ifdef COBJMACROS


#define _IAddEvents_QueryInterface(This,riid,ppvObject) \
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) )

#define _IAddEvents_AddRef(This)    \
    ( (This)->lpVtbl -> AddRef(This) )

#define _IAddEvents_Release(This)   \
    ( (This)->lpVtbl -> Release(This) )


#define _IAddEvents_ExecutionOver(This,Result)  \
    ( (This)->lpVtbl -> ExecutionOver(This,Result) )

#endif /* COBJMACROS */


#endif  /* C style interface */




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



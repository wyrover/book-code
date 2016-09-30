
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 6.00.0347 */
/* at Sat Dec 07 11:11:50 2002
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

#ifndef __SpeakerNoAttr_h__
#define __SpeakerNoAttr_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
    #pragma once
#endif

/* Forward Declarations */

#ifndef __ISpeaker_FWD_DEFINED__
    #define __ISpeaker_FWD_DEFINED__
    typedef interface ISpeaker ISpeaker;
#endif  /* __ISpeaker_FWD_DEFINED__ */


#ifndef __IListener_FWD_DEFINED__
    #define __IListener_FWD_DEFINED__
    typedef interface IListener IListener;
#endif  /* __IListener_FWD_DEFINED__ */


#ifndef __INamedObject_FWD_DEFINED__
    #define __INamedObject_FWD_DEFINED__
    typedef interface INamedObject INamedObject;
#endif  /* __INamedObject_FWD_DEFINED__ */


#ifndef ___ISpeakerEvents_FWD_DEFINED__
    #define ___ISpeakerEvents_FWD_DEFINED__
    typedef interface _ISpeakerEvents _ISpeakerEvents;
#endif  /* ___ISpeakerEvents_FWD_DEFINED__ */


#ifndef __Demagogue_FWD_DEFINED__
    #define __Demagogue_FWD_DEFINED__

    #ifdef __cplusplus
        typedef class Demagogue Demagogue;
    #else
        typedef struct Demagogue Demagogue;
    #endif /* __cplusplus */

#endif  /* __Demagogue_FWD_DEFINED__ */


#ifndef __EarPolitic_FWD_DEFINED__
    #define __EarPolitic_FWD_DEFINED__

    #ifdef __cplusplus
        typedef class EarPolitic EarPolitic;
    #else
        typedef struct EarPolitic EarPolitic;
    #endif /* __cplusplus */

#endif  /* __EarPolitic_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C" {
#endif

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void *);

/* interface __MIDL_itf_SpeakerNoAttr_0000 */
/* [local] */

#define DISPID_ONWHISPER    ( 1 )

#define DISPID_ONTALK   ( 2 )

#define DISPID_ONYELL   ( 3 )



extern RPC_IF_HANDLE __MIDL_itf_SpeakerNoAttr_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_SpeakerNoAttr_0000_v0_0_s_ifspec;

#ifndef __ISpeaker_INTERFACE_DEFINED__
#define __ISpeaker_INTERFACE_DEFINED__

/* interface ISpeaker */
/* [unique][helpstring][nonextensible][dual][uuid][object] */

#define DISPID_VOLUME   ( 1 )

#define DISPID_SPEECH   ( 2 )

#define DISPID_SPEAK    ( 3 )


EXTERN_C const IID IID_ISpeaker;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("CE00527D-F8E3-46A3-8BC8-A25345AD6CAA")
ISpeaker :
public IDispatch {
public:
    virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Volume(
        /* [retval][out] */ LONG * pVal) = 0;

    virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Volume(
        /* [in] */ LONG newVal) = 0;

    virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Speech(
        /* [retval][out] */ BSTR * pVal) = 0;

    virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Speech(
        /* [in] */ BSTR newVal) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Speak(void) = 0;

};

#else   /* C style interface */

typedef struct ISpeakerVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE *QueryInterface)(
        ISpeaker * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject);

    ULONG(STDMETHODCALLTYPE *AddRef)(
        ISpeaker * This);

    ULONG(STDMETHODCALLTYPE *Release)(
        ISpeaker * This);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfoCount)(
        ISpeaker * This,
        /* [out] */ UINT *pctinfo);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfo)(
        ISpeaker * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo **ppTInfo);

    HRESULT(STDMETHODCALLTYPE *GetIDsOfNames)(
        ISpeaker * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE *Invoke)(
        ISpeaker * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS *pDispParams,
        /* [out] */ VARIANT *pVarResult,
        /* [out] */ EXCEPINFO *pExcepInfo,
        /* [out] */ UINT *puArgErr);

    /* [helpstring][id][propget] */ HRESULT(STDMETHODCALLTYPE *get_Volume)(
        ISpeaker * This,
        /* [retval][out] */ LONG *pVal);

    /* [helpstring][id][propput] */ HRESULT(STDMETHODCALLTYPE *put_Volume)(
        ISpeaker * This,
        /* [in] */ LONG newVal);

    /* [helpstring][id][propget] */ HRESULT(STDMETHODCALLTYPE *get_Speech)(
        ISpeaker * This,
        /* [retval][out] */ BSTR *pVal);

    /* [helpstring][id][propput] */ HRESULT(STDMETHODCALLTYPE *put_Speech)(
        ISpeaker * This,
        /* [in] */ BSTR newVal);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *Speak)(
        ISpeaker * This);

    END_INTERFACE
} ISpeakerVtbl;

interface ISpeaker {
    CONST_VTBL struct ISpeakerVtbl *lpVtbl;
};



#ifdef COBJMACROS


#define ISpeaker_QueryInterface(This,riid,ppvObject)    \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISpeaker_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define ISpeaker_Release(This)  \
    (This)->lpVtbl -> Release(This)


#define ISpeaker_GetTypeInfoCount(This,pctinfo) \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISpeaker_GetTypeInfo(This,iTInfo,lcid,ppTInfo)  \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISpeaker_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISpeaker_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISpeaker_get_Volume(This,pVal)  \
    (This)->lpVtbl -> get_Volume(This,pVal)

#define ISpeaker_put_Volume(This,newVal)    \
    (This)->lpVtbl -> put_Volume(This,newVal)

#define ISpeaker_get_Speech(This,pVal)  \
    (This)->lpVtbl -> get_Speech(This,pVal)

#define ISpeaker_put_Speech(This,newVal)    \
    (This)->lpVtbl -> put_Speech(This,newVal)

#define ISpeaker_Speak(This)    \
    (This)->lpVtbl -> Speak(This)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISpeaker_get_Volume_Proxy(
    ISpeaker * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB ISpeaker_get_Volume_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISpeaker_put_Volume_Proxy(
    ISpeaker * This,
    /* [in] */ LONG newVal);


void __RPC_STUB ISpeaker_put_Volume_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ISpeaker_get_Speech_Proxy(
    ISpeaker * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ISpeaker_get_Speech_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ISpeaker_put_Speech_Proxy(
    ISpeaker * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISpeaker_put_Speech_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISpeaker_Speak_Proxy(
    ISpeaker * This);


void __RPC_STUB ISpeaker_Speak_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __ISpeaker_INTERFACE_DEFINED__ */


#ifndef __IListener_INTERFACE_DEFINED__
#define __IListener_INTERFACE_DEFINED__

/* interface IListener */
/* [unique][helpstring][nonextensible][dual][uuid][object] */

typedef
enum SpeakerRole {
    Defendant   = 0,
    Plaintiff   = Defendant + 1
}   SpeakerRole;


EXTERN_C const IID IID_IListener;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("BECC670E-7A07-4032-A5A3-07D1558AE65D")
IListener :
public IDispatch {
public:
    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ListenTo(
        /* [in] */ SpeakerRole role,
        /* [in] */ IUnknown * pSpeaker) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopListening(
        /* [in] */ SpeakerRole role) = 0;

};

#else   /* C style interface */

typedef struct IListenerVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE *QueryInterface)(
        IListener * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject);

    ULONG(STDMETHODCALLTYPE *AddRef)(
        IListener * This);

    ULONG(STDMETHODCALLTYPE *Release)(
        IListener * This);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfoCount)(
        IListener * This,
        /* [out] */ UINT *pctinfo);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfo)(
        IListener * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo **ppTInfo);

    HRESULT(STDMETHODCALLTYPE *GetIDsOfNames)(
        IListener * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE *Invoke)(
        IListener * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS *pDispParams,
        /* [out] */ VARIANT *pVarResult,
        /* [out] */ EXCEPINFO *pExcepInfo,
        /* [out] */ UINT *puArgErr);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *ListenTo)(
        IListener * This,
        /* [in] */ SpeakerRole role,
        /* [in] */ IUnknown *pSpeaker);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *StopListening)(
        IListener * This,
        /* [in] */ SpeakerRole role);

    END_INTERFACE
} IListenerVtbl;

interface IListener {
    CONST_VTBL struct IListenerVtbl *lpVtbl;
};



#ifdef COBJMACROS


#define IListener_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IListener_AddRef(This)  \
    (This)->lpVtbl -> AddRef(This)

#define IListener_Release(This) \
    (This)->lpVtbl -> Release(This)


#define IListener_GetTypeInfoCount(This,pctinfo)    \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IListener_GetTypeInfo(This,iTInfo,lcid,ppTInfo) \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IListener_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IListener_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IListener_ListenTo(This,role,pSpeaker)  \
    (This)->lpVtbl -> ListenTo(This,role,pSpeaker)

#define IListener_StopListening(This,role)  \
    (This)->lpVtbl -> StopListening(This,role)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IListener_ListenTo_Proxy(
    IListener * This,
    /* [in] */ SpeakerRole role,
    /* [in] */ IUnknown *pSpeaker);


void __RPC_STUB IListener_ListenTo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IListener_StopListening_Proxy(
    IListener * This,
    /* [in] */ SpeakerRole role);


void __RPC_STUB IListener_StopListening_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IListener_INTERFACE_DEFINED__ */


#ifndef __INamedObject_INTERFACE_DEFINED__
#define __INamedObject_INTERFACE_DEFINED__

/* interface INamedObject */
/* [unique][helpstring][dual][uuid][object] */

#define DISPID_NAME ( 1 )


EXTERN_C const IID IID_INamedObject;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("450D6780-FC73-11d1-8CC3-00A0C9C8E50D")
INamedObject :
public IDispatch {
public:
    virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name(
        /* [retval][out] */ BSTR * pVal) = 0;

    virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Name(
        /* [in] */ BSTR newVal) = 0;

};

#else   /* C style interface */

typedef struct INamedObjectVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE *QueryInterface)(
        INamedObject * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject);

    ULONG(STDMETHODCALLTYPE *AddRef)(
        INamedObject * This);

    ULONG(STDMETHODCALLTYPE *Release)(
        INamedObject * This);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfoCount)(
        INamedObject * This,
        /* [out] */ UINT *pctinfo);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfo)(
        INamedObject * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo **ppTInfo);

    HRESULT(STDMETHODCALLTYPE *GetIDsOfNames)(
        INamedObject * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE *Invoke)(
        INamedObject * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS *pDispParams,
        /* [out] */ VARIANT *pVarResult,
        /* [out] */ EXCEPINFO *pExcepInfo,
        /* [out] */ UINT *puArgErr);

    /* [helpstring][id][propget] */ HRESULT(STDMETHODCALLTYPE *get_Name)(
        INamedObject * This,
        /* [retval][out] */ BSTR *pVal);

    /* [helpstring][id][propput] */ HRESULT(STDMETHODCALLTYPE *put_Name)(
        INamedObject * This,
        /* [in] */ BSTR newVal);

    END_INTERFACE
} INamedObjectVtbl;

interface INamedObject {
    CONST_VTBL struct INamedObjectVtbl *lpVtbl;
};



#ifdef COBJMACROS


#define INamedObject_QueryInterface(This,riid,ppvObject)    \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define INamedObject_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define INamedObject_Release(This)  \
    (This)->lpVtbl -> Release(This)


#define INamedObject_GetTypeInfoCount(This,pctinfo) \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define INamedObject_GetTypeInfo(This,iTInfo,lcid,ppTInfo)  \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define INamedObject_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define INamedObject_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define INamedObject_get_Name(This,pVal)    \
    (This)->lpVtbl -> get_Name(This,pVal)

#define INamedObject_put_Name(This,newVal)  \
    (This)->lpVtbl -> put_Name(This,newVal)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE INamedObject_get_Name_Proxy(
    INamedObject * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB INamedObject_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE INamedObject_put_Name_Proxy(
    INamedObject * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB INamedObject_put_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __INamedObject_INTERFACE_DEFINED__ */



#ifndef __SpeakerNoAttrLib_LIBRARY_DEFINED__
#define __SpeakerNoAttrLib_LIBRARY_DEFINED__

/* library SpeakerNoAttrLib */
/* [helpstring][version][uuid] */


EXTERN_C const IID LIBID_SpeakerNoAttrLib;

#ifndef ___ISpeakerEvents_DISPINTERFACE_DEFINED__
#define ___ISpeakerEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ISpeakerEvents */
/* [helpstring][uuid] */


EXTERN_C const IID DIID__ISpeakerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("9CB7F78F-EF31-4702-9510-72B0952DF487")
_ISpeakerEvents :
public IDispatch {
};

#else   /* C style interface */

typedef struct _ISpeakerEventsVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE *QueryInterface)(
        _ISpeakerEvents * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject);

    ULONG(STDMETHODCALLTYPE *AddRef)(
        _ISpeakerEvents * This);

    ULONG(STDMETHODCALLTYPE *Release)(
        _ISpeakerEvents * This);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfoCount)(
        _ISpeakerEvents * This,
        /* [out] */ UINT *pctinfo);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfo)(
        _ISpeakerEvents * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo **ppTInfo);

    HRESULT(STDMETHODCALLTYPE *GetIDsOfNames)(
        _ISpeakerEvents * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE *Invoke)(
        _ISpeakerEvents * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS *pDispParams,
        /* [out] */ VARIANT *pVarResult,
        /* [out] */ EXCEPINFO *pExcepInfo,
        /* [out] */ UINT *puArgErr);

    END_INTERFACE
} _ISpeakerEventsVtbl;

interface _ISpeakerEvents {
    CONST_VTBL struct _ISpeakerEventsVtbl *lpVtbl;
};



#ifdef COBJMACROS


#define _ISpeakerEvents_QueryInterface(This,riid,ppvObject) \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _ISpeakerEvents_AddRef(This)    \
    (This)->lpVtbl -> AddRef(This)

#define _ISpeakerEvents_Release(This)   \
    (This)->lpVtbl -> Release(This)


#define _ISpeakerEvents_GetTypeInfoCount(This,pctinfo)  \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _ISpeakerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)   \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _ISpeakerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _ISpeakerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)   \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif  /* C style interface */


#endif  /* ___ISpeakerEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Demagogue;

#ifdef __cplusplus

class DECLSPEC_UUID("BA5A5622-05DF-45CF-A80A-E09DC145B01C")
    Demagogue;
#endif

EXTERN_C const CLSID CLSID_EarPolitic;

#ifdef __cplusplus

class DECLSPEC_UUID("3BEA68EC-4B6D-46D7-A3EE-792CA10EB0F6")
    EarPolitic;
#endif
#endif /* __SpeakerNoAttrLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(unsigned long *, unsigned long            , BSTR *);
unsigned char * __RPC_USER  BSTR_UserMarshal(unsigned long *, unsigned char *, BSTR *);
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR *);
void                      __RPC_USER  BSTR_UserFree(unsigned long *, BSTR *);

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif





/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 6.00.0361 */
/* at Thu Jun 30 17:38:47 2005
 */
/* Compiler settings for E:\Software\BookCode\VCCOM\CHAP10\IETICKERCTRL\AtlTickerCtrl.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data
    VC __declspec() decoration level:
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

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

#ifndef __AtlTickerCtrl_h__
#define __AtlTickerCtrl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
    #pragma once
#endif

/* Forward Declarations */

#ifndef __ITicker_FWD_DEFINED__
    #define __ITicker_FWD_DEFINED__
    typedef interface ITicker ITicker;
#endif  /* __ITicker_FWD_DEFINED__ */


#ifndef ___ITickerEvents_FWD_DEFINED__
    #define ___ITickerEvents_FWD_DEFINED__
    typedef interface _ITickerEvents _ITickerEvents;
#endif  /* ___ITickerEvents_FWD_DEFINED__ */


#ifndef __Ticker_FWD_DEFINED__
    #define __Ticker_FWD_DEFINED__

    #ifdef __cplusplus
        typedef class Ticker Ticker;
    #else
        typedef struct Ticker Ticker;
    #endif /* __cplusplus */

#endif  /* __Ticker_FWD_DEFINED__ */


#ifndef __TickerPP_FWD_DEFINED__
    #define __TickerPP_FWD_DEFINED__

    #ifdef __cplusplus
        typedef class TickerPP TickerPP;
    #else
        typedef struct TickerPP TickerPP;
    #endif /* __cplusplus */

#endif  /* __TickerPP_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C" {
#endif

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void *);

#ifndef __ITicker_INTERFACE_DEFINED__
#define __ITicker_INTERFACE_DEFINED__

/* interface ITicker */
/* [unique][helpstring][dual][uuid][object] */


EXTERN_C const IID IID_ITicker;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("48727BC4-B815-4EBB-9718-84EFF6D59505")
ITicker :
public IDispatch {
public:
    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_BackColor(
        /* [in] */ OLE_COLOR clr) = 0;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_BackColor(
        /* [retval][out] */ OLE_COLOR * pclr) = 0;

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_BorderColor(
        /* [in] */ OLE_COLOR clr) = 0;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_BorderColor(
        /* [retval][out] */ OLE_COLOR * pclr) = 0;

    virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_Font(
        /* [in] */ IFontDisp * pFont) = 0;

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Font(
        /* [in] */ IFontDisp * pFont) = 0;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Font(
        /* [retval][out] */ IFontDisp **ppFont) = 0;

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ForeColor(
        /* [in] */ OLE_COLOR clr) = 0;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ForeColor(
        /* [retval][out] */ OLE_COLOR * pclr) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartScroll(void) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopScroll(void) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetQuotes(
        /* [retval][out] */ VARIANT_BOOL * pbSuccess) = 0;

    virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Speed(
        /* [retval][out] */ long * pVal) = 0;

    virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Speed(
        /* [in] */ long newVal) = 0;

};

#else   /* C style interface */

typedef struct ITickerVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE *QueryInterface)(
        ITicker * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject);

    ULONG(STDMETHODCALLTYPE *AddRef)(
        ITicker * This);

    ULONG(STDMETHODCALLTYPE *Release)(
        ITicker * This);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfoCount)(
        ITicker * This,
        /* [out] */ UINT *pctinfo);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfo)(
        ITicker * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo **ppTInfo);

    HRESULT(STDMETHODCALLTYPE *GetIDsOfNames)(
        ITicker * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE *Invoke)(
        ITicker * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS *pDispParams,
        /* [out] */ VARIANT *pVarResult,
        /* [out] */ EXCEPINFO *pExcepInfo,
        /* [out] */ UINT *puArgErr);

    /* [id][propput] */ HRESULT(STDMETHODCALLTYPE *put_BackColor)(
        ITicker * This,
        /* [in] */ OLE_COLOR clr);

    /* [id][propget] */ HRESULT(STDMETHODCALLTYPE *get_BackColor)(
        ITicker * This,
        /* [retval][out] */ OLE_COLOR *pclr);

    /* [id][propput] */ HRESULT(STDMETHODCALLTYPE *put_BorderColor)(
        ITicker * This,
        /* [in] */ OLE_COLOR clr);

    /* [id][propget] */ HRESULT(STDMETHODCALLTYPE *get_BorderColor)(
        ITicker * This,
        /* [retval][out] */ OLE_COLOR *pclr);

    /* [id][propputref] */ HRESULT(STDMETHODCALLTYPE *putref_Font)(
        ITicker * This,
        /* [in] */ IFontDisp *pFont);

    /* [id][propput] */ HRESULT(STDMETHODCALLTYPE *put_Font)(
        ITicker * This,
        /* [in] */ IFontDisp *pFont);

    /* [id][propget] */ HRESULT(STDMETHODCALLTYPE *get_Font)(
        ITicker * This,
        /* [retval][out] */ IFontDisp **ppFont);

    /* [id][propput] */ HRESULT(STDMETHODCALLTYPE *put_ForeColor)(
        ITicker * This,
        /* [in] */ OLE_COLOR clr);

    /* [id][propget] */ HRESULT(STDMETHODCALLTYPE *get_ForeColor)(
        ITicker * This,
        /* [retval][out] */ OLE_COLOR *pclr);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *StartScroll)(
        ITicker * This);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *StopScroll)(
        ITicker * This);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE *GetQuotes)(
        ITicker * This,
        /* [retval][out] */ VARIANT_BOOL *pbSuccess);

    /* [helpstring][id][propget] */ HRESULT(STDMETHODCALLTYPE *get_Speed)(
        ITicker * This,
        /* [retval][out] */ long *pVal);

    /* [helpstring][id][propput] */ HRESULT(STDMETHODCALLTYPE *put_Speed)(
        ITicker * This,
        /* [in] */ long newVal);

    END_INTERFACE
} ITickerVtbl;

interface ITicker {
    CONST_VTBL struct ITickerVtbl *lpVtbl;
};



#ifdef COBJMACROS


#define ITicker_QueryInterface(This,riid,ppvObject) \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITicker_AddRef(This)    \
    (This)->lpVtbl -> AddRef(This)

#define ITicker_Release(This)   \
    (This)->lpVtbl -> Release(This)


#define ITicker_GetTypeInfoCount(This,pctinfo)  \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITicker_GetTypeInfo(This,iTInfo,lcid,ppTInfo)   \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITicker_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITicker_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)   \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITicker_put_BackColor(This,clr) \
    (This)->lpVtbl -> put_BackColor(This,clr)

#define ITicker_get_BackColor(This,pclr)    \
    (This)->lpVtbl -> get_BackColor(This,pclr)

#define ITicker_put_BorderColor(This,clr)   \
    (This)->lpVtbl -> put_BorderColor(This,clr)

#define ITicker_get_BorderColor(This,pclr)  \
    (This)->lpVtbl -> get_BorderColor(This,pclr)

#define ITicker_putref_Font(This,pFont) \
    (This)->lpVtbl -> putref_Font(This,pFont)

#define ITicker_put_Font(This,pFont)    \
    (This)->lpVtbl -> put_Font(This,pFont)

#define ITicker_get_Font(This,ppFont)   \
    (This)->lpVtbl -> get_Font(This,ppFont)

#define ITicker_put_ForeColor(This,clr) \
    (This)->lpVtbl -> put_ForeColor(This,clr)

#define ITicker_get_ForeColor(This,pclr)    \
    (This)->lpVtbl -> get_ForeColor(This,pclr)

#define ITicker_StartScroll(This)   \
    (This)->lpVtbl -> StartScroll(This)

#define ITicker_StopScroll(This)    \
    (This)->lpVtbl -> StopScroll(This)

#define ITicker_GetQuotes(This,pbSuccess)   \
    (This)->lpVtbl -> GetQuotes(This,pbSuccess)

#define ITicker_get_Speed(This,pVal)    \
    (This)->lpVtbl -> get_Speed(This,pVal)

#define ITicker_put_Speed(This,newVal)  \
    (This)->lpVtbl -> put_Speed(This,newVal)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [id][propput] */ HRESULT STDMETHODCALLTYPE ITicker_put_BackColor_Proxy(
    ITicker * This,
    /* [in] */ OLE_COLOR clr);


void __RPC_STUB ITicker_put_BackColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ITicker_get_BackColor_Proxy(
    ITicker * This,
    /* [retval][out] */ OLE_COLOR *pclr);


void __RPC_STUB ITicker_get_BackColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ITicker_put_BorderColor_Proxy(
    ITicker * This,
    /* [in] */ OLE_COLOR clr);


void __RPC_STUB ITicker_put_BorderColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ITicker_get_BorderColor_Proxy(
    ITicker * This,
    /* [retval][out] */ OLE_COLOR *pclr);


void __RPC_STUB ITicker_get_BorderColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ITicker_putref_Font_Proxy(
    ITicker * This,
    /* [in] */ IFontDisp *pFont);


void __RPC_STUB ITicker_putref_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ITicker_put_Font_Proxy(
    ITicker * This,
    /* [in] */ IFontDisp *pFont);


void __RPC_STUB ITicker_put_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ITicker_get_Font_Proxy(
    ITicker * This,
    /* [retval][out] */ IFontDisp **ppFont);


void __RPC_STUB ITicker_get_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ITicker_put_ForeColor_Proxy(
    ITicker * This,
    /* [in] */ OLE_COLOR clr);


void __RPC_STUB ITicker_put_ForeColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ITicker_get_ForeColor_Proxy(
    ITicker * This,
    /* [retval][out] */ OLE_COLOR *pclr);


void __RPC_STUB ITicker_get_ForeColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITicker_StartScroll_Proxy(
    ITicker * This);


void __RPC_STUB ITicker_StartScroll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITicker_StopScroll_Proxy(
    ITicker * This);


void __RPC_STUB ITicker_StopScroll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITicker_GetQuotes_Proxy(
    ITicker * This,
    /* [retval][out] */ VARIANT_BOOL *pbSuccess);


void __RPC_STUB ITicker_GetQuotes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITicker_get_Speed_Proxy(
    ITicker * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB ITicker_get_Speed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITicker_put_Speed_Proxy(
    ITicker * This,
    /* [in] */ long newVal);


void __RPC_STUB ITicker_put_Speed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __ITicker_INTERFACE_DEFINED__ */



#ifndef __ATLTICKERCTRLLib_LIBRARY_DEFINED__
#define __ATLTICKERCTRLLib_LIBRARY_DEFINED__

/* library ATLTICKERCTRLLib */
/* [helpstring][version][uuid] */


EXTERN_C const IID LIBID_ATLTICKERCTRLLib;

#ifndef ___ITickerEvents_DISPINTERFACE_DEFINED__
#define ___ITickerEvents_DISPINTERFACE_DEFINED__

/* dispinterface _ITickerEvents */
/* [helpstring][uuid] */


EXTERN_C const IID DIID__ITickerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("43401211-FF77-41E0-B659-17636F119F57")
_ITickerEvents :
public IDispatch {
};

#else   /* C style interface */

typedef struct _ITickerEventsVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE *QueryInterface)(
        _ITickerEvents * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject);

    ULONG(STDMETHODCALLTYPE *AddRef)(
        _ITickerEvents * This);

    ULONG(STDMETHODCALLTYPE *Release)(
        _ITickerEvents * This);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfoCount)(
        _ITickerEvents * This,
        /* [out] */ UINT *pctinfo);

    HRESULT(STDMETHODCALLTYPE *GetTypeInfo)(
        _ITickerEvents * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo **ppTInfo);

    HRESULT(STDMETHODCALLTYPE *GetIDsOfNames)(
        _ITickerEvents * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE *Invoke)(
        _ITickerEvents * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS *pDispParams,
        /* [out] */ VARIANT *pVarResult,
        /* [out] */ EXCEPINFO *pExcepInfo,
        /* [out] */ UINT *puArgErr);

    END_INTERFACE
} _ITickerEventsVtbl;

interface _ITickerEvents {
    CONST_VTBL struct _ITickerEventsVtbl *lpVtbl;
};



#ifdef COBJMACROS


#define _ITickerEvents_QueryInterface(This,riid,ppvObject)  \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _ITickerEvents_AddRef(This) \
    (This)->lpVtbl -> AddRef(This)

#define _ITickerEvents_Release(This)    \
    (This)->lpVtbl -> Release(This)


#define _ITickerEvents_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _ITickerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _ITickerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)  \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _ITickerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif  /* C style interface */


#endif  /* ___ITickerEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Ticker;

#ifdef __cplusplus

class DECLSPEC_UUID("CB9E8D82-BC1E-4865-ADD3-7CC1678EA712")
    Ticker;
#endif

EXTERN_C const CLSID CLSID_TickerPP;

#ifdef __cplusplus

class DECLSPEC_UUID("A5ACC474-4C00-4988-B215-09541C5740C8")
    TickerPP;
#endif
#endif /* __ATLTICKERCTRLLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



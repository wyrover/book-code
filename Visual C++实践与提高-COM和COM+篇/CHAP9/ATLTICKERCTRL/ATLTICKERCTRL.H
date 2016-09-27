/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Sep 19 14:54:19 2000
 */
/* Compiler settings for C:\VcCom\chap9\AtlTickerCtrl\AtlTickerCtrl.idl:
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

#ifndef __AtlTickerCtrl_h__
#define __AtlTickerCtrl_h__

#ifdef __cplusplus
extern "C" {
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

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void __RPC_FAR *);

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
        /* [retval][out] */ OLE_COLOR __RPC_FAR * pclr) = 0;

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_BorderColor(
        /* [in] */ OLE_COLOR clr) = 0;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_BorderColor(
        /* [retval][out] */ OLE_COLOR __RPC_FAR * pclr) = 0;

    virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_Font(
        /* [in] */ IFontDisp __RPC_FAR * pFont) = 0;

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Font(
        /* [in] */ IFontDisp __RPC_FAR * pFont) = 0;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Font(
        /* [retval][out] */ IFontDisp __RPC_FAR *__RPC_FAR * ppFont) = 0;

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ForeColor(
        /* [in] */ OLE_COLOR clr) = 0;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ForeColor(
        /* [retval][out] */ OLE_COLOR __RPC_FAR * pclr) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartScroll(void) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StopScroll(void) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetQuotes(
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR * pbSuccess) = 0;

    virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Speed(
        /* [retval][out] */ long __RPC_FAR * pVal) = 0;

    virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Speed(
        /* [in] */ long newVal) = 0;

};

#else   /* C style interface */

typedef struct ITickerVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        ITicker __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        ITicker __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        ITicker __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        ITicker __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        ITicker __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        ITicker __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        ITicker __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    /* [id][propput] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *put_BackColor)(
        ITicker __RPC_FAR * This,
        /* [in] */ OLE_COLOR clr);

    /* [id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_BackColor)(
        ITicker __RPC_FAR * This,
        /* [retval][out] */ OLE_COLOR __RPC_FAR *pclr);

    /* [id][propput] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *put_BorderColor)(
        ITicker __RPC_FAR * This,
        /* [in] */ OLE_COLOR clr);

    /* [id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_BorderColor)(
        ITicker __RPC_FAR * This,
        /* [retval][out] */ OLE_COLOR __RPC_FAR *pclr);

    /* [id][propputref] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *putref_Font)(
        ITicker __RPC_FAR * This,
        /* [in] */ IFontDisp __RPC_FAR *pFont);

    /* [id][propput] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *put_Font)(
        ITicker __RPC_FAR * This,
        /* [in] */ IFontDisp __RPC_FAR *pFont);

    /* [id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Font)(
        ITicker __RPC_FAR * This,
        /* [retval][out] */ IFontDisp __RPC_FAR *__RPC_FAR *ppFont);

    /* [id][propput] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *put_ForeColor)(
        ITicker __RPC_FAR * This,
        /* [in] */ OLE_COLOR clr);

    /* [id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_ForeColor)(
        ITicker __RPC_FAR * This,
        /* [retval][out] */ OLE_COLOR __RPC_FAR *pclr);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *StartScroll)(
        ITicker __RPC_FAR * This);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *StopScroll)(
        ITicker __RPC_FAR * This);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetQuotes)(
        ITicker __RPC_FAR * This,
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbSuccess);

    /* [helpstring][id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Speed)(
        ITicker __RPC_FAR * This,
        /* [retval][out] */ long __RPC_FAR *pVal);

    /* [helpstring][id][propput] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *put_Speed)(
        ITicker __RPC_FAR * This,
        /* [in] */ long newVal);

    END_INTERFACE
} ITickerVtbl;

interface ITicker {
    CONST_VTBL struct ITickerVtbl __RPC_FAR *lpVtbl;
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
    ITicker __RPC_FAR * This,
    /* [in] */ OLE_COLOR clr);


void __RPC_STUB ITicker_put_BackColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ITicker_get_BackColor_Proxy(
    ITicker __RPC_FAR * This,
    /* [retval][out] */ OLE_COLOR __RPC_FAR *pclr);


void __RPC_STUB ITicker_get_BackColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ITicker_put_BorderColor_Proxy(
    ITicker __RPC_FAR * This,
    /* [in] */ OLE_COLOR clr);


void __RPC_STUB ITicker_put_BorderColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ITicker_get_BorderColor_Proxy(
    ITicker __RPC_FAR * This,
    /* [retval][out] */ OLE_COLOR __RPC_FAR *pclr);


void __RPC_STUB ITicker_get_BorderColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ITicker_putref_Font_Proxy(
    ITicker __RPC_FAR * This,
    /* [in] */ IFontDisp __RPC_FAR *pFont);


void __RPC_STUB ITicker_putref_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ITicker_put_Font_Proxy(
    ITicker __RPC_FAR * This,
    /* [in] */ IFontDisp __RPC_FAR *pFont);


void __RPC_STUB ITicker_put_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ITicker_get_Font_Proxy(
    ITicker __RPC_FAR * This,
    /* [retval][out] */ IFontDisp __RPC_FAR *__RPC_FAR *ppFont);


void __RPC_STUB ITicker_get_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ITicker_put_ForeColor_Proxy(
    ITicker __RPC_FAR * This,
    /* [in] */ OLE_COLOR clr);


void __RPC_STUB ITicker_put_ForeColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ITicker_get_ForeColor_Proxy(
    ITicker __RPC_FAR * This,
    /* [retval][out] */ OLE_COLOR __RPC_FAR *pclr);


void __RPC_STUB ITicker_get_ForeColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITicker_StartScroll_Proxy(
    ITicker __RPC_FAR * This);


void __RPC_STUB ITicker_StartScroll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITicker_StopScroll_Proxy(
    ITicker __RPC_FAR * This);


void __RPC_STUB ITicker_StopScroll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITicker_GetQuotes_Proxy(
    ITicker __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbSuccess);


void __RPC_STUB ITicker_GetQuotes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITicker_get_Speed_Proxy(
    ITicker __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ITicker_get_Speed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITicker_put_Speed_Proxy(
    ITicker __RPC_FAR * This,
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

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        _ITickerEvents __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        _ITickerEvents __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        _ITickerEvents __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        _ITickerEvents __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        _ITickerEvents __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        _ITickerEvents __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        _ITickerEvents __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    END_INTERFACE
} _ITickerEventsVtbl;

interface _ITickerEvents {
    CONST_VTBL struct _ITickerEventsVtbl __RPC_FAR *lpVtbl;
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

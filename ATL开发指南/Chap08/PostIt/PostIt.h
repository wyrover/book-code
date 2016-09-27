/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Dec 19 16:51:31 1999
 */
/* Compiler settings for C:\projects\Books\atl\2ed\Examples\Chap08\PostIt\PostIt.idl:
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

#ifndef __PostIt_h__
#define __PostIt_h__

#ifdef __cplusplus
extern "C" {
#endif

/* Forward Declarations */

#ifndef __IPostItCtl_FWD_DEFINED__
#define __IPostItCtl_FWD_DEFINED__
typedef interface IPostItCtl IPostItCtl;
#endif  /* __IPostItCtl_FWD_DEFINED__ */


#ifndef ___IPostItCtlEvents_FWD_DEFINED__
#define ___IPostItCtlEvents_FWD_DEFINED__
typedef interface _IPostItCtlEvents _IPostItCtlEvents;
#endif  /* ___IPostItCtlEvents_FWD_DEFINED__ */


#ifndef __PostItCtl_FWD_DEFINED__
#define __PostItCtl_FWD_DEFINED__

#ifdef __cplusplus
typedef class PostItCtl PostItCtl;
#else
typedef struct PostItCtl PostItCtl;
#endif /* __cplusplus */

#endif  /* __PostItCtl_FWD_DEFINED__ */


#ifndef __PostItPpg_FWD_DEFINED__
#define __PostItPpg_FWD_DEFINED__

#ifdef __cplusplus
typedef class PostItPpg PostItPpg;
#else
typedef struct PostItPpg PostItPpg;
#endif /* __cplusplus */

#endif  /* __PostItPpg_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void __RPC_FAR *);

#ifndef __IPostItCtl_INTERFACE_DEFINED__
#define __IPostItCtl_INTERFACE_DEFINED__

/* interface IPostItCtl */
/* [unique][helpstring][dual][uuid][object] */


EXTERN_C const IID IID_IPostItCtl;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("BF31419D-B661-11D3-8391-0060081AEB5F")
IPostItCtl :
public IDispatch {
public:
    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_BackColor(
        /* [in] */ OLE_COLOR clr) = 0;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_BackColor(
        /* [retval][out] */ OLE_COLOR __RPC_FAR * pclr) = 0;

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_BorderStyle(
        /* [in] */ long style) = 0;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_BorderStyle(
        /* [retval][out] */ long __RPC_FAR * pstyle) = 0;

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

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Text(
        /* [in] */ BSTR strText) = 0;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Text(
        /* [retval][out] */ BSTR __RPC_FAR * pstrText) = 0;

    virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Appearance(
        /* [in] */ short appearance) = 0;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Appearance(
        /* [retval][out] */ short __RPC_FAR * pappearance) = 0;

};

#else   /* C style interface */

typedef struct IPostItCtlVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IPostItCtl __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IPostItCtl __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IPostItCtl __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        IPostItCtl __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        IPostItCtl __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        IPostItCtl __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        IPostItCtl __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    /* [id][propput] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *put_BackColor)(
        IPostItCtl __RPC_FAR * This,
        /* [in] */ OLE_COLOR clr);

    /* [id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_BackColor)(
        IPostItCtl __RPC_FAR * This,
        /* [retval][out] */ OLE_COLOR __RPC_FAR *pclr);

    /* [id][propput] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *put_BorderStyle)(
        IPostItCtl __RPC_FAR * This,
        /* [in] */ long style);

    /* [id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_BorderStyle)(
        IPostItCtl __RPC_FAR * This,
        /* [retval][out] */ long __RPC_FAR *pstyle);

    /* [id][propputref] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *putref_Font)(
        IPostItCtl __RPC_FAR * This,
        /* [in] */ IFontDisp __RPC_FAR *pFont);

    /* [id][propput] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *put_Font)(
        IPostItCtl __RPC_FAR * This,
        /* [in] */ IFontDisp __RPC_FAR *pFont);

    /* [id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Font)(
        IPostItCtl __RPC_FAR * This,
        /* [retval][out] */ IFontDisp __RPC_FAR *__RPC_FAR *ppFont);

    /* [id][propput] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *put_ForeColor)(
        IPostItCtl __RPC_FAR * This,
        /* [in] */ OLE_COLOR clr);

    /* [id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_ForeColor)(
        IPostItCtl __RPC_FAR * This,
        /* [retval][out] */ OLE_COLOR __RPC_FAR *pclr);

    /* [id][propput] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *put_Text)(
        IPostItCtl __RPC_FAR * This,
        /* [in] */ BSTR strText);

    /* [id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Text)(
        IPostItCtl __RPC_FAR * This,
        /* [retval][out] */ BSTR __RPC_FAR *pstrText);

    /* [id][propput] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *put_Appearance)(
        IPostItCtl __RPC_FAR * This,
        /* [in] */ short appearance);

    /* [id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Appearance)(
        IPostItCtl __RPC_FAR * This,
        /* [retval][out] */ short __RPC_FAR *pappearance);

    END_INTERFACE
} IPostItCtlVtbl;

interface IPostItCtl {
    CONST_VTBL struct IPostItCtlVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IPostItCtl_QueryInterface(This,riid,ppvObject)  \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPostItCtl_AddRef(This) \
    (This)->lpVtbl -> AddRef(This)

#define IPostItCtl_Release(This)    \
    (This)->lpVtbl -> Release(This)


#define IPostItCtl_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPostItCtl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPostItCtl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)  \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPostItCtl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPostItCtl_put_BackColor(This,clr)  \
    (This)->lpVtbl -> put_BackColor(This,clr)

#define IPostItCtl_get_BackColor(This,pclr) \
    (This)->lpVtbl -> get_BackColor(This,pclr)

#define IPostItCtl_put_BorderStyle(This,style)  \
    (This)->lpVtbl -> put_BorderStyle(This,style)

#define IPostItCtl_get_BorderStyle(This,pstyle) \
    (This)->lpVtbl -> get_BorderStyle(This,pstyle)

#define IPostItCtl_putref_Font(This,pFont)  \
    (This)->lpVtbl -> putref_Font(This,pFont)

#define IPostItCtl_put_Font(This,pFont) \
    (This)->lpVtbl -> put_Font(This,pFont)

#define IPostItCtl_get_Font(This,ppFont)    \
    (This)->lpVtbl -> get_Font(This,ppFont)

#define IPostItCtl_put_ForeColor(This,clr)  \
    (This)->lpVtbl -> put_ForeColor(This,clr)

#define IPostItCtl_get_ForeColor(This,pclr) \
    (This)->lpVtbl -> get_ForeColor(This,pclr)

#define IPostItCtl_put_Text(This,strText)   \
    (This)->lpVtbl -> put_Text(This,strText)

#define IPostItCtl_get_Text(This,pstrText)  \
    (This)->lpVtbl -> get_Text(This,pstrText)

#define IPostItCtl_put_Appearance(This,appearance)  \
    (This)->lpVtbl -> put_Appearance(This,appearance)

#define IPostItCtl_get_Appearance(This,pappearance) \
    (This)->lpVtbl -> get_Appearance(This,pappearance)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPostItCtl_put_BackColor_Proxy(
    IPostItCtl __RPC_FAR * This,
    /* [in] */ OLE_COLOR clr);


void __RPC_STUB IPostItCtl_put_BackColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPostItCtl_get_BackColor_Proxy(
    IPostItCtl __RPC_FAR * This,
    /* [retval][out] */ OLE_COLOR __RPC_FAR *pclr);


void __RPC_STUB IPostItCtl_get_BackColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPostItCtl_put_BorderStyle_Proxy(
    IPostItCtl __RPC_FAR * This,
    /* [in] */ long style);


void __RPC_STUB IPostItCtl_put_BorderStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPostItCtl_get_BorderStyle_Proxy(
    IPostItCtl __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pstyle);


void __RPC_STUB IPostItCtl_get_BorderStyle_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE IPostItCtl_putref_Font_Proxy(
    IPostItCtl __RPC_FAR * This,
    /* [in] */ IFontDisp __RPC_FAR *pFont);


void __RPC_STUB IPostItCtl_putref_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPostItCtl_put_Font_Proxy(
    IPostItCtl __RPC_FAR * This,
    /* [in] */ IFontDisp __RPC_FAR *pFont);


void __RPC_STUB IPostItCtl_put_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPostItCtl_get_Font_Proxy(
    IPostItCtl __RPC_FAR * This,
    /* [retval][out] */ IFontDisp __RPC_FAR *__RPC_FAR *ppFont);


void __RPC_STUB IPostItCtl_get_Font_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPostItCtl_put_ForeColor_Proxy(
    IPostItCtl __RPC_FAR * This,
    /* [in] */ OLE_COLOR clr);


void __RPC_STUB IPostItCtl_put_ForeColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPostItCtl_get_ForeColor_Proxy(
    IPostItCtl __RPC_FAR * This,
    /* [retval][out] */ OLE_COLOR __RPC_FAR *pclr);


void __RPC_STUB IPostItCtl_get_ForeColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPostItCtl_put_Text_Proxy(
    IPostItCtl __RPC_FAR * This,
    /* [in] */ BSTR strText);


void __RPC_STUB IPostItCtl_put_Text_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPostItCtl_get_Text_Proxy(
    IPostItCtl __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pstrText);


void __RPC_STUB IPostItCtl_get_Text_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE IPostItCtl_put_Appearance_Proxy(
    IPostItCtl __RPC_FAR * This,
    /* [in] */ short appearance);


void __RPC_STUB IPostItCtl_put_Appearance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IPostItCtl_get_Appearance_Proxy(
    IPostItCtl __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pappearance);


void __RPC_STUB IPostItCtl_get_Appearance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IPostItCtl_INTERFACE_DEFINED__ */



#ifndef __POSTITLib_LIBRARY_DEFINED__
#define __POSTITLib_LIBRARY_DEFINED__

/* library POSTITLib */
/* [helpstring][version][uuid] */


EXTERN_C const IID LIBID_POSTITLib;

#ifndef ___IPostItCtlEvents_DISPINTERFACE_DEFINED__
#define ___IPostItCtlEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IPostItCtlEvents */
/* [helpstring][uuid] */


EXTERN_C const IID DIID__IPostItCtlEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("BF31419F-B661-11D3-8391-0060081AEB5F")
_IPostItCtlEvents :
public IDispatch {
};

#else   /* C style interface */

typedef struct _IPostItCtlEventsVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        _IPostItCtlEvents __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        _IPostItCtlEvents __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        _IPostItCtlEvents __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        _IPostItCtlEvents __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        _IPostItCtlEvents __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        _IPostItCtlEvents __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        _IPostItCtlEvents __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    END_INTERFACE
} _IPostItCtlEventsVtbl;

interface _IPostItCtlEvents {
    CONST_VTBL struct _IPostItCtlEventsVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define _IPostItCtlEvents_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IPostItCtlEvents_AddRef(This)  \
    (This)->lpVtbl -> AddRef(This)

#define _IPostItCtlEvents_Release(This) \
    (This)->lpVtbl -> Release(This)


#define _IPostItCtlEvents_GetTypeInfoCount(This,pctinfo)    \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IPostItCtlEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo) \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IPostItCtlEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IPostItCtlEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif  /* C style interface */


#endif  /* ___IPostItCtlEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PostItCtl;

#ifdef __cplusplus

class DECLSPEC_UUID("BF31419E-B661-11D3-8391-0060081AEB5F")
    PostItCtl;
#endif

EXTERN_C const CLSID CLSID_PostItPpg;

#ifdef __cplusplus

class DECLSPEC_UUID("BF3141A2-B661-11D3-8391-0060081AEB5F")
    PostItPpg;
#endif
#endif /* __POSTITLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR *);
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR *);
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR *);
void                      __RPC_USER  BSTR_UserFree(unsigned long __RPC_FAR *, BSTR __RPC_FAR *);

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Feb 23 21:51:23 2000
 */
/* Compiler settings for C:\projects\Books\atl\2ed\Examples\Chap09\Chapter9_CollectionServer\Chapter9_CollectionServer.idl:
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

#ifndef __Chapter9_CollectionServer_h__
#define __Chapter9_CollectionServer_h__

#ifdef __cplusplus
extern "C" {
#endif

/* Forward Declarations */

#ifndef __IMovie_FWD_DEFINED__
#define __IMovie_FWD_DEFINED__
typedef interface IMovie IMovie;
#endif  /* __IMovie_FWD_DEFINED__ */


#ifndef __IMovieCollection_FWD_DEFINED__
#define __IMovieCollection_FWD_DEFINED__
typedef interface IMovieCollection IMovieCollection;
#endif  /* __IMovieCollection_FWD_DEFINED__ */


#ifndef __IApplication_FWD_DEFINED__
#define __IApplication_FWD_DEFINED__
typedef interface IApplication IApplication;
#endif  /* __IApplication_FWD_DEFINED__ */


#ifndef __IMovieCollection_FWD_DEFINED__
#define __IMovieCollection_FWD_DEFINED__
typedef interface IMovieCollection IMovieCollection;
#endif  /* __IMovieCollection_FWD_DEFINED__ */


#ifndef __IMovie_FWD_DEFINED__
#define __IMovie_FWD_DEFINED__
typedef interface IMovie IMovie;
#endif  /* __IMovie_FWD_DEFINED__ */


#ifndef __Application_FWD_DEFINED__
#define __Application_FWD_DEFINED__

#ifdef __cplusplus
typedef class Application Application;
#else
typedef struct Application Application;
#endif /* __cplusplus */

#endif  /* __Application_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void __RPC_FAR *);

#ifndef __IMovie_INTERFACE_DEFINED__
#define __IMovie_INTERFACE_DEFINED__

/* interface IMovie */
/* [unique][helpstring][dual][uuid][object] */


EXTERN_C const IID IID_IMovie;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("F579C5C1-3777-11d2-883A-444553540000")
IMovie :
public IDispatch {
public:
    virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Director(
        /* [retval][out] */ BSTR __RPC_FAR * pVal) = 0;

    virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Director(
        /* [in] */ BSTR newVal) = 0;

    virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Title(
        /* [retval][out] */ BSTR __RPC_FAR * pVal) = 0;

    virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Title(
        /* [in] */ BSTR newVal) = 0;

};

#else   /* C style interface */

typedef struct IMovieVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IMovie __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IMovie __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IMovie __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        IMovie __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        IMovie __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        IMovie __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        IMovie __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    /* [helpstring][id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Director)(
        IMovie __RPC_FAR * This,
        /* [retval][out] */ BSTR __RPC_FAR *pVal);

    /* [helpstring][id][propput] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *put_Director)(
        IMovie __RPC_FAR * This,
        /* [in] */ BSTR newVal);

    /* [helpstring][id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Title)(
        IMovie __RPC_FAR * This,
        /* [retval][out] */ BSTR __RPC_FAR *pVal);

    /* [helpstring][id][propput] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *put_Title)(
        IMovie __RPC_FAR * This,
        /* [in] */ BSTR newVal);

    END_INTERFACE
} IMovieVtbl;

interface IMovie {
    CONST_VTBL struct IMovieVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IMovie_QueryInterface(This,riid,ppvObject)  \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMovie_AddRef(This) \
    (This)->lpVtbl -> AddRef(This)

#define IMovie_Release(This)    \
    (This)->lpVtbl -> Release(This)


#define IMovie_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMovie_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMovie_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)  \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMovie_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMovie_get_Director(This,pVal)  \
    (This)->lpVtbl -> get_Director(This,pVal)

#define IMovie_put_Director(This,newVal)    \
    (This)->lpVtbl -> put_Director(This,newVal)

#define IMovie_get_Title(This,pVal) \
    (This)->lpVtbl -> get_Title(This,pVal)

#define IMovie_put_Title(This,newVal)   \
    (This)->lpVtbl -> put_Title(This,newVal)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMovie_get_Director_Proxy(
    IMovie __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IMovie_get_Director_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMovie_put_Director_Proxy(
    IMovie __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IMovie_put_Director_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMovie_get_Title_Proxy(
    IMovie __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IMovie_get_Title_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMovie_put_Title_Proxy(
    IMovie __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IMovie_put_Title_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IMovie_INTERFACE_DEFINED__ */


#ifndef __IMovieCollection_INTERFACE_DEFINED__
#define __IMovieCollection_INTERFACE_DEFINED__

/* interface IMovieCollection */
/* [unique][helpstring][dual][uuid][object] */


EXTERN_C const IID IID_IMovieCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("F579C5C4-3777-11d2-883A-444553540000")
IMovieCollection :
public IDispatch {
public:
    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Count(
        /* [retval][out] */ long __RPC_FAR * pVal) = 0;

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Add(
        /* [in] */ IMovie __RPC_FAR * pMovie) = 0;

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE Remove(
        /* [in] */ long inIndex) = 0;

    virtual /* [id] */ HRESULT STDMETHODCALLTYPE RemoveAll(void) = 0;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Item(
        /* [in] */ long inIndex,
        /* [retval][out] */ IMovie __RPC_FAR *__RPC_FAR * ppMovie) = 0;

    virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum(
        /* [retval][out] */ LPUNKNOWN __RPC_FAR * pVal) = 0;

};

#else   /* C style interface */

typedef struct IMovieCollectionVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IMovieCollection __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IMovieCollection __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IMovieCollection __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        IMovieCollection __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        IMovieCollection __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        IMovieCollection __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        IMovieCollection __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    /* [id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Count)(
        IMovieCollection __RPC_FAR * This,
        /* [retval][out] */ long __RPC_FAR *pVal);

    /* [id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Add)(
        IMovieCollection __RPC_FAR * This,
        /* [in] */ IMovie __RPC_FAR *pMovie);

    /* [id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Remove)(
        IMovieCollection __RPC_FAR * This,
        /* [in] */ long inIndex);

    /* [id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *RemoveAll)(
        IMovieCollection __RPC_FAR * This);

    /* [id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Item)(
        IMovieCollection __RPC_FAR * This,
        /* [in] */ long inIndex,
        /* [retval][out] */ IMovie __RPC_FAR *__RPC_FAR *ppMovie);

    /* [id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get__NewEnum)(
        IMovieCollection __RPC_FAR * This,
        /* [retval][out] */ LPUNKNOWN __RPC_FAR *pVal);

    END_INTERFACE
} IMovieCollectionVtbl;

interface IMovieCollection {
    CONST_VTBL struct IMovieCollectionVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IMovieCollection_QueryInterface(This,riid,ppvObject)    \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMovieCollection_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define IMovieCollection_Release(This)  \
    (This)->lpVtbl -> Release(This)


#define IMovieCollection_GetTypeInfoCount(This,pctinfo) \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMovieCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)  \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMovieCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMovieCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMovieCollection_get_Count(This,pVal)   \
    (This)->lpVtbl -> get_Count(This,pVal)

#define IMovieCollection_Add(This,pMovie)   \
    (This)->lpVtbl -> Add(This,pMovie)

#define IMovieCollection_Remove(This,inIndex)   \
    (This)->lpVtbl -> Remove(This,inIndex)

#define IMovieCollection_RemoveAll(This)    \
    (This)->lpVtbl -> RemoveAll(This)

#define IMovieCollection_get_Item(This,inIndex,ppMovie) \
    (This)->lpVtbl -> get_Item(This,inIndex,ppMovie)

#define IMovieCollection_get__NewEnum(This,pVal)    \
    (This)->lpVtbl -> get__NewEnum(This,pVal)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IMovieCollection_get_Count_Proxy(
    IMovieCollection __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB IMovieCollection_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IMovieCollection_Add_Proxy(
    IMovieCollection __RPC_FAR * This,
    /* [in] */ IMovie __RPC_FAR *pMovie);


void __RPC_STUB IMovieCollection_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IMovieCollection_Remove_Proxy(
    IMovieCollection __RPC_FAR * This,
    /* [in] */ long inIndex);


void __RPC_STUB IMovieCollection_Remove_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IMovieCollection_RemoveAll_Proxy(
    IMovieCollection __RPC_FAR * This);


void __RPC_STUB IMovieCollection_RemoveAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IMovieCollection_get_Item_Proxy(
    IMovieCollection __RPC_FAR * This,
    /* [in] */ long inIndex,
    /* [retval][out] */ IMovie __RPC_FAR *__RPC_FAR *ppMovie);


void __RPC_STUB IMovieCollection_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IMovieCollection_get__NewEnum_Proxy(
    IMovieCollection __RPC_FAR * This,
    /* [retval][out] */ LPUNKNOWN __RPC_FAR *pVal);


void __RPC_STUB IMovieCollection_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IMovieCollection_INTERFACE_DEFINED__ */


#ifndef __IApplication_INTERFACE_DEFINED__
#define __IApplication_INTERFACE_DEFINED__

/* interface IApplication */
/* [unique][helpstring][dual][uuid][object] */


EXTERN_C const IID IID_IApplication;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("56E642CF-EA5F-11D3-83D5-0060081AEB5F")
IApplication :
public IDispatch {
public:
    virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Movies(
        /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR * ppVal) = 0;

    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateMovie(
        /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR * ppVal) = 0;

};

#else   /* C style interface */

typedef struct IApplicationVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IApplication __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IApplication __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IApplication __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        IApplication __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        IApplication __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        IApplication __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        IApplication __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    /* [helpstring][id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Movies)(
        IApplication __RPC_FAR * This,
        /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppVal);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *CreateMovie)(
        IApplication __RPC_FAR * This,
        /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppVal);

    END_INTERFACE
} IApplicationVtbl;

interface IApplication {
    CONST_VTBL struct IApplicationVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IApplication_QueryInterface(This,riid,ppvObject)    \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IApplication_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define IApplication_Release(This)  \
    (This)->lpVtbl -> Release(This)


#define IApplication_GetTypeInfoCount(This,pctinfo) \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IApplication_GetTypeInfo(This,iTInfo,lcid,ppTInfo)  \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IApplication_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IApplication_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IApplication_get_Movies(This,ppVal) \
    (This)->lpVtbl -> get_Movies(This,ppVal)

#define IApplication_CreateMovie(This,ppVal)    \
    (This)->lpVtbl -> CreateMovie(This,ppVal)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IApplication_get_Movies_Proxy(
    IApplication __RPC_FAR * This,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppVal);


void __RPC_STUB IApplication_get_Movies_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IApplication_CreateMovie_Proxy(
    IApplication __RPC_FAR * This,
    /* [retval][out] */ IDispatch __RPC_FAR *__RPC_FAR *ppVal);


void __RPC_STUB IApplication_CreateMovie_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IApplication_INTERFACE_DEFINED__ */



#ifndef __CHAPTER9_COLLECTIONSERVERLib_LIBRARY_DEFINED__
#define __CHAPTER9_COLLECTIONSERVERLib_LIBRARY_DEFINED__

/* library CHAPTER9_COLLECTIONSERVERLib */
/* [helpstring][version][uuid] */




EXTERN_C const IID LIBID_CHAPTER9_COLLECTIONSERVERLib;

EXTERN_C const CLSID CLSID_Application;

#ifdef __cplusplus

class DECLSPEC_UUID("56E642D0-EA5F-11D3-83D5-0060081AEB5F")
    Application;
#endif
#endif /* __CHAPTER9_COLLECTIONSERVERLib_LIBRARY_DEFINED__ */

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

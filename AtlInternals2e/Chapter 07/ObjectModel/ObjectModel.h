/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Nov 23 01:11:40 1998
 */
/* Compiler settings for D:\ATLBook\src\Chapter 08\ObjectModel\ObjectModel.idl:
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

#ifndef __ObjectModel_h__
#define __ObjectModel_h__

#ifdef __cplusplus
extern "C" {
#endif

/* Forward Declarations */

#ifndef __IDocument_FWD_DEFINED__
#define __IDocument_FWD_DEFINED__
typedef interface IDocument IDocument;
#endif  /* __IDocument_FWD_DEFINED__ */


#ifndef __IDocuments_FWD_DEFINED__
#define __IDocuments_FWD_DEFINED__
typedef interface IDocuments IDocuments;
#endif  /* __IDocuments_FWD_DEFINED__ */


#ifndef __IApplication_FWD_DEFINED__
#define __IApplication_FWD_DEFINED__
typedef interface IApplication IApplication;
#endif  /* __IApplication_FWD_DEFINED__ */


#ifndef __Document_FWD_DEFINED__
#define __Document_FWD_DEFINED__

#ifdef __cplusplus
typedef class Document Document;
#else
typedef struct Document Document;
#endif /* __cplusplus */

#endif  /* __Document_FWD_DEFINED__ */


#ifndef __Documents_FWD_DEFINED__
#define __Documents_FWD_DEFINED__

#ifdef __cplusplus
typedef class Documents Documents;
#else
typedef struct Documents Documents;
#endif /* __cplusplus */

#endif  /* __Documents_FWD_DEFINED__ */


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

#ifndef __IDocument_INTERFACE_DEFINED__
#define __IDocument_INTERFACE_DEFINED__

/* interface IDocument */
/* [unique][helpstring][dual][uuid][object] */


EXTERN_C const IID IID_IDocument;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("CB693B6C-80E2-11D2-987D-00600823CFFB")
IDocument :
public IDispatch {
public:
    virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_Data(
        /* [retval][out] */ BSTR __RPC_FAR * pVal) = 0;

    virtual /* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE put_Data(
        /* [in] */ BSTR newVal) = 0;

};

#else   /* C style interface */

typedef struct IDocumentVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IDocument __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IDocument __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IDocument __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        IDocument __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        IDocument __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        IDocument __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        IDocument __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    /* [helpstring][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Data)(
        IDocument __RPC_FAR * This,
        /* [retval][out] */ BSTR __RPC_FAR *pVal);

    /* [helpstring][propput] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *put_Data)(
        IDocument __RPC_FAR * This,
        /* [in] */ BSTR newVal);

    END_INTERFACE
} IDocumentVtbl;

interface IDocument {
    CONST_VTBL struct IDocumentVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IDocument_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDocument_AddRef(This)  \
    (This)->lpVtbl -> AddRef(This)

#define IDocument_Release(This) \
    (This)->lpVtbl -> Release(This)


#define IDocument_GetTypeInfoCount(This,pctinfo)    \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDocument_GetTypeInfo(This,iTInfo,lcid,ppTInfo) \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDocument_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)   \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDocument_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDocument_get_Data(This,pVal)   \
    (This)->lpVtbl -> get_Data(This,pVal)

#define IDocument_put_Data(This,newVal) \
    (This)->lpVtbl -> put_Data(This,newVal)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IDocument_get_Data_Proxy(
    IDocument __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB IDocument_get_Data_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propput] */ HRESULT STDMETHODCALLTYPE IDocument_put_Data_Proxy(
    IDocument __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IDocument_put_Data_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IDocument_INTERFACE_DEFINED__ */


#ifndef __IDocuments_INTERFACE_DEFINED__
#define __IDocuments_INTERFACE_DEFINED__

/* interface IDocuments */
/* [unique][helpstring][dual][uuid][object] */


EXTERN_C const IID IID_IDocuments;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("CB693B6E-80E2-11D2-987D-00600823CFFB")
IDocuments :
public IDispatch {
public:
    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddDocument(
        /* [retval][out] */ IDocument __RPC_FAR *__RPC_FAR * ppDocument) = 0;

    virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Count(
        /* [retval][out] */ long __RPC_FAR * pnCount) = 0;

    virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get_Item(
        /* [in] */ long n,
        /* [retval][out] */ IDocument __RPC_FAR *__RPC_FAR * ppdoc) = 0;

    virtual /* [propget][id] */ HRESULT STDMETHODCALLTYPE get__NewEnum(
        /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR * ppEnum) = 0;

};

#else   /* C style interface */

typedef struct IDocumentsVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IDocuments __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IDocuments __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IDocuments __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        IDocuments __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        IDocuments __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        IDocuments __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        IDocuments __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *AddDocument)(
        IDocuments __RPC_FAR * This,
        /* [retval][out] */ IDocument __RPC_FAR *__RPC_FAR *ppDocument);

    /* [propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Count)(
        IDocuments __RPC_FAR * This,
        /* [retval][out] */ long __RPC_FAR *pnCount);

    /* [propget][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Item)(
        IDocuments __RPC_FAR * This,
        /* [in] */ long n,
        /* [retval][out] */ IDocument __RPC_FAR *__RPC_FAR *ppdoc);

    /* [propget][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get__NewEnum)(
        IDocuments __RPC_FAR * This,
        /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppEnum);

    END_INTERFACE
} IDocumentsVtbl;

interface IDocuments {
    CONST_VTBL struct IDocumentsVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IDocuments_QueryInterface(This,riid,ppvObject)  \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDocuments_AddRef(This) \
    (This)->lpVtbl -> AddRef(This)

#define IDocuments_Release(This)    \
    (This)->lpVtbl -> Release(This)


#define IDocuments_GetTypeInfoCount(This,pctinfo)   \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDocuments_GetTypeInfo(This,iTInfo,lcid,ppTInfo)    \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDocuments_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)  \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDocuments_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)    \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDocuments_AddDocument(This,ppDocument) \
    (This)->lpVtbl -> AddDocument(This,ppDocument)

#define IDocuments_get_Count(This,pnCount)  \
    (This)->lpVtbl -> get_Count(This,pnCount)

#define IDocuments_get_Item(This,n,ppdoc)   \
    (This)->lpVtbl -> get_Item(This,n,ppdoc)

#define IDocuments_get__NewEnum(This,ppEnum)    \
    (This)->lpVtbl -> get__NewEnum(This,ppEnum)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDocuments_AddDocument_Proxy(
    IDocuments __RPC_FAR * This,
    /* [retval][out] */ IDocument __RPC_FAR *__RPC_FAR *ppDocument);


void __RPC_STUB IDocuments_AddDocument_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IDocuments_get_Count_Proxy(
    IDocuments __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pnCount);


void __RPC_STUB IDocuments_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IDocuments_get_Item_Proxy(
    IDocuments __RPC_FAR * This,
    /* [in] */ long n,
    /* [retval][out] */ IDocument __RPC_FAR *__RPC_FAR *ppdoc);


void __RPC_STUB IDocuments_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget][id] */ HRESULT STDMETHODCALLTYPE IDocuments_get__NewEnum_Proxy(
    IDocuments __RPC_FAR * This,
    /* [retval][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppEnum);


void __RPC_STUB IDocuments_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IDocuments_INTERFACE_DEFINED__ */


#ifndef __IApplication_INTERFACE_DEFINED__
#define __IApplication_INTERFACE_DEFINED__

/* interface IApplication */
/* [unique][helpstring][dual][uuid][object] */


EXTERN_C const IID IID_IApplication;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("CB693B70-80E2-11D2-987D-00600823CFFB")
IApplication :
public IDispatch {
public:
    virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Documents(
        /* [retval][out] */ IDocuments __RPC_FAR *__RPC_FAR * pVal) = 0;

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

    /* [helpstring][id][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Documents)(
        IApplication __RPC_FAR * This,
        /* [retval][out] */ IDocuments __RPC_FAR *__RPC_FAR *pVal);

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


#define IApplication_get_Documents(This,pVal)   \
    (This)->lpVtbl -> get_Documents(This,pVal)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IApplication_get_Documents_Proxy(
    IApplication __RPC_FAR * This,
    /* [retval][out] */ IDocuments __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB IApplication_get_Documents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IApplication_INTERFACE_DEFINED__ */



#ifndef __OBJECTMODELLib_LIBRARY_DEFINED__
#define __OBJECTMODELLib_LIBRARY_DEFINED__

/* library OBJECTMODELLib */
/* [helpstring][version][uuid] */


EXTERN_C const IID LIBID_OBJECTMODELLib;

EXTERN_C const CLSID CLSID_Document;

#ifdef __cplusplus

class DECLSPEC_UUID("CB693B6D-80E2-11D2-987D-00600823CFFB")
    Document;
#endif

EXTERN_C const CLSID CLSID_Documents;

#ifdef __cplusplus

class DECLSPEC_UUID("CB693B6F-80E2-11D2-987D-00600823CFFB")
    Documents;
#endif

EXTERN_C const CLSID CLSID_Application;

#ifdef __cplusplus

class DECLSPEC_UUID("CB693B71-80E2-11D2-987D-00600823CFFB")
    Application;
#endif
#endif /* __OBJECTMODELLib_LIBRARY_DEFINED__ */

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

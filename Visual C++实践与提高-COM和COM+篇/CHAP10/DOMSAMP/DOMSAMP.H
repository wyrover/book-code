/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Sep 28 10:45:33 2000
 */
/* Compiler settings for C:\VCCOM\chap10\DOMSamp\DOMSamp.idl:
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

#ifndef __DOMSamp_h__
#define __DOMSamp_h__

#ifdef __cplusplus
extern "C" {
#endif

/* Forward Declarations */

#ifndef __IDOMCtrl_FWD_DEFINED__
#define __IDOMCtrl_FWD_DEFINED__
typedef interface IDOMCtrl IDOMCtrl;
#endif  /* __IDOMCtrl_FWD_DEFINED__ */


#ifndef __DOMCtrl_FWD_DEFINED__
#define __DOMCtrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class DOMCtrl DOMCtrl;
#else
typedef struct DOMCtrl DOMCtrl;
#endif /* __cplusplus */

#endif  /* __DOMCtrl_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void __RPC_FAR *);

#ifndef __IDOMCtrl_INTERFACE_DEFINED__
#define __IDOMCtrl_INTERFACE_DEFINED__

/* interface IDOMCtrl */
/* [unique][helpstring][dual][uuid][object] */


EXTERN_C const IID IID_IDOMCtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("63D6A83B-94C4-11D4-896A-80FF100AAD79")
IDOMCtrl :
public IDispatch {
public:
};

#else   /* C style interface */

typedef struct IDOMCtrlVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IDOMCtrl __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IDOMCtrl __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IDOMCtrl __RPC_FAR * This);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount)(
        IDOMCtrl __RPC_FAR * This,
        /* [out] */ UINT __RPC_FAR *pctinfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo)(
        IDOMCtrl __RPC_FAR * This,
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames)(
        IDOMCtrl __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
        /* [in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);

    /* [local] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Invoke)(
        IDOMCtrl __RPC_FAR * This,
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
        /* [out] */ VARIANT __RPC_FAR *pVarResult,
        /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
        /* [out] */ UINT __RPC_FAR *puArgErr);

    END_INTERFACE
} IDOMCtrlVtbl;

interface IDOMCtrl {
    CONST_VTBL struct IDOMCtrlVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IDOMCtrl_QueryInterface(This,riid,ppvObject)    \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDOMCtrl_AddRef(This)   \
    (This)->lpVtbl -> AddRef(This)

#define IDOMCtrl_Release(This)  \
    (This)->lpVtbl -> Release(This)


#define IDOMCtrl_GetTypeInfoCount(This,pctinfo) \
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDOMCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)  \
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDOMCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)    \
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDOMCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)  \
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif  /* C style interface */




#endif  /* __IDOMCtrl_INTERFACE_DEFINED__ */



#ifndef __DOMSAMPLib_LIBRARY_DEFINED__
#define __DOMSAMPLib_LIBRARY_DEFINED__

/* library DOMSAMPLib */
/* [helpstring][version][uuid] */


EXTERN_C const IID LIBID_DOMSAMPLib;

EXTERN_C const CLSID CLSID_DOMCtrl;

#ifdef __cplusplus

class DECLSPEC_UUID("63D6A83C-94C4-11D4-896A-80FF100AAD79")
    DOMCtrl;
#endif
#endif /* __DOMSAMPLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif

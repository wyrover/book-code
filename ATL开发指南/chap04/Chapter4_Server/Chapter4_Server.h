/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.01.75 */
/* at Mon Oct 06 22:26:09 1997
 */
/* Compiler settings for Chapter4_Server.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )
#include "rpc.h"
#include "rpcndr.h"
#ifndef COM_NO_WINDOWS_H
    #include "windows.h"
    #include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Chapter4_Server_h__
#define __Chapter4_Server_h__

#ifdef __cplusplus
extern "C" {
#endif

/* Forward Declarations */

#ifndef __IMath_FWD_DEFINED__
#define __IMath_FWD_DEFINED__
typedef interface IMath IMath;
#endif  /* __IMath_FWD_DEFINED__ */


#ifndef __IAdvancedMath_FWD_DEFINED__
#define __IAdvancedMath_FWD_DEFINED__
typedef interface IAdvancedMath IAdvancedMath;
#endif  /* __IAdvancedMath_FWD_DEFINED__ */


#ifndef __IComponentInfo_FWD_DEFINED__
#define __IComponentInfo_FWD_DEFINED__
typedef interface IComponentInfo IComponentInfo;
#endif  /* __IComponentInfo_FWD_DEFINED__ */


#ifndef __IMath2_FWD_DEFINED__
#define __IMath2_FWD_DEFINED__
typedef interface IMath2 IMath2;
#endif  /* __IMath2_FWD_DEFINED__ */


#ifndef __Math_FWD_DEFINED__
#define __Math_FWD_DEFINED__

#ifdef __cplusplus
typedef class Math Math;
#else
typedef struct Math Math;
#endif /* __cplusplus */

#endif  /* __Math_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void __RPC_FAR *);

/****************************************
 * Generated header for interface: __MIDL_itf_Chapter4_Server_0000
 * at Mon Oct 06 22:26:09 1997
 * using MIDL 3.01.75
 ****************************************/
/* [local] */


typedef /* [helpstring][uuid] */
enum mathOPERATION {
    mathAdd = 0x1,
    mathSubtract    = 0x2,
    mathMultiply    = 0x3,
    mathDivide  = 0x4
}   mathOPERATION;



extern RPC_IF_HANDLE __MIDL_itf_Chapter4_Server_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Chapter4_Server_0000_v0_0_s_ifspec;

#ifndef __IMath_INTERFACE_DEFINED__
#define __IMath_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IMath
 * at Mon Oct 06 22:26:09 1997
 * using MIDL 3.01.75
 ****************************************/
/* [object][unique][helpstring][uuid] */



EXTERN_C const IID IID_IMath;

#if defined(__cplusplus) && !defined(CINTERFACE)

interface DECLSPEC_UUID("5FB0C22E-3343-11D1-883A-444553540000")
IMath :
public IUnknown {
public:
    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Add(
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR * plResult) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Subtract(
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR * plResult) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Multiply(
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR * plResult) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Divide(
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR * plResult) = 0;

};

#else   /* C style interface */

typedef struct IMathVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IMath __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IMath __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IMath __RPC_FAR * This);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Add)(
        IMath __RPC_FAR * This,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR *plResult);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Subtract)(
        IMath __RPC_FAR * This,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR *plResult);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Multiply)(
        IMath __RPC_FAR * This,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR *plResult);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Divide)(
        IMath __RPC_FAR * This,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR *plResult);

    END_INTERFACE
} IMathVtbl;

interface IMath {
    CONST_VTBL struct IMathVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IMath_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMath_AddRef(This)  \
    (This)->lpVtbl -> AddRef(This)

#define IMath_Release(This) \
    (This)->lpVtbl -> Release(This)


#define IMath_Add(This,lOp1,lOp2,plResult)  \
    (This)->lpVtbl -> Add(This,lOp1,lOp2,plResult)

#define IMath_Subtract(This,lOp1,lOp2,plResult) \
    (This)->lpVtbl -> Subtract(This,lOp1,lOp2,plResult)

#define IMath_Multiply(This,lOp1,lOp2,plResult) \
    (This)->lpVtbl -> Multiply(This,lOp1,lOp2,plResult)

#define IMath_Divide(This,lOp1,lOp2,plResult)   \
    (This)->lpVtbl -> Divide(This,lOp1,lOp2,plResult)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath_Add_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ long lOp1,
    /* [in] */ long lOp2,
    /* [retval][out] */ long __RPC_FAR *plResult);


void __RPC_STUB IMath_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath_Subtract_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ long lOp1,
    /* [in] */ long lOp2,
    /* [retval][out] */ long __RPC_FAR *plResult);


void __RPC_STUB IMath_Subtract_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath_Multiply_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ long lOp1,
    /* [in] */ long lOp2,
    /* [retval][out] */ long __RPC_FAR *plResult);


void __RPC_STUB IMath_Multiply_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath_Divide_Proxy(
    IMath __RPC_FAR * This,
    /* [in] */ long lOp1,
    /* [in] */ long lOp2,
    /* [retval][out] */ long __RPC_FAR *plResult);


void __RPC_STUB IMath_Divide_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IMath_INTERFACE_DEFINED__ */


#ifndef __IAdvancedMath_INTERFACE_DEFINED__
#define __IAdvancedMath_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IAdvancedMath
 * at Mon Oct 06 22:26:09 1997
 * using MIDL 3.01.75
 ****************************************/
/* [object][unique][helpstring][uuid] */



EXTERN_C const IID IID_IAdvancedMath;

#if defined(__cplusplus) && !defined(CINTERFACE)

interface DECLSPEC_UUID("984D09A1-3379-11d1-883A-444553540000")
IAdvancedMath :
public IUnknown {
public:
    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Factorial(
        /* [in] */ short sFact,
        /* [retval][out] */ long __RPC_FAR * pResult) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Fibonacci(
        /* [in] */ short sFib,
        /* [retval][out] */ long __RPC_FAR * pResult) = 0;

};

#else   /* C style interface */

typedef struct IAdvancedMathVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IAdvancedMath __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IAdvancedMath __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IAdvancedMath __RPC_FAR * This);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Factorial)(
        IAdvancedMath __RPC_FAR * This,
        /* [in] */ short sFact,
        /* [retval][out] */ long __RPC_FAR *pResult);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Fibonacci)(
        IAdvancedMath __RPC_FAR * This,
        /* [in] */ short sFib,
        /* [retval][out] */ long __RPC_FAR *pResult);

    END_INTERFACE
} IAdvancedMathVtbl;

interface IAdvancedMath {
    CONST_VTBL struct IAdvancedMathVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IAdvancedMath_QueryInterface(This,riid,ppvObject)   \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAdvancedMath_AddRef(This)  \
    (This)->lpVtbl -> AddRef(This)

#define IAdvancedMath_Release(This) \
    (This)->lpVtbl -> Release(This)


#define IAdvancedMath_Factorial(This,sFact,pResult) \
    (This)->lpVtbl -> Factorial(This,sFact,pResult)

#define IAdvancedMath_Fibonacci(This,sFib,pResult)  \
    (This)->lpVtbl -> Fibonacci(This,sFib,pResult)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAdvancedMath_Factorial_Proxy(
    IAdvancedMath __RPC_FAR * This,
    /* [in] */ short sFact,
    /* [retval][out] */ long __RPC_FAR *pResult);


void __RPC_STUB IAdvancedMath_Factorial_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IAdvancedMath_Fibonacci_Proxy(
    IAdvancedMath __RPC_FAR * This,
    /* [in] */ short sFib,
    /* [retval][out] */ long __RPC_FAR *pResult);


void __RPC_STUB IAdvancedMath_Fibonacci_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IAdvancedMath_INTERFACE_DEFINED__ */


/****************************************
 * Generated header for interface: __MIDL_itf_Chapter4_Server_0137
 * at Mon Oct 06 22:26:09 1997
 * using MIDL 3.01.75
 ****************************************/
/* [local] */


typedef struct  COMPONENT_INFO {
    /* [string] */ unsigned char __RPC_FAR *pstrAuthor;
    short sMajor;
    short sMinor;
    BSTR bstrName;
}   COMPONENT_INFO;



extern RPC_IF_HANDLE __MIDL_itf_Chapter4_Server_0137_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Chapter4_Server_0137_v0_0_s_ifspec;

#ifndef __IComponentInfo_INTERFACE_DEFINED__
#define __IComponentInfo_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IComponentInfo
 * at Mon Oct 06 22:26:09 1997
 * using MIDL 3.01.75
 ****************************************/
/* [object][unique][helpstring][uuid] */



EXTERN_C const IID IID_IComponentInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)

interface DECLSPEC_UUID("1E405AA0-3396-11d1-883A-444553540000")
IComponentInfo :
public IUnknown {
public:
    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE get_Info(
        /* [out] */ COMPONENT_INFO __RPC_FAR *__RPC_FAR * pInfo) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE get_Name(
        /* [out] */ BSTR __RPC_FAR * bstrName) = 0;

};

#else   /* C style interface */

typedef struct IComponentInfoVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IComponentInfo __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IComponentInfo __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IComponentInfo __RPC_FAR * This);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Info)(
        IComponentInfo __RPC_FAR * This,
        /* [out] */ COMPONENT_INFO __RPC_FAR *__RPC_FAR *pInfo);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Name)(
        IComponentInfo __RPC_FAR * This,
        /* [out] */ BSTR __RPC_FAR *bstrName);

    END_INTERFACE
} IComponentInfoVtbl;

interface IComponentInfo {
    CONST_VTBL struct IComponentInfoVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IComponentInfo_QueryInterface(This,riid,ppvObject)  \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IComponentInfo_AddRef(This) \
    (This)->lpVtbl -> AddRef(This)

#define IComponentInfo_Release(This)    \
    (This)->lpVtbl -> Release(This)


#define IComponentInfo_get_Info(This,pInfo) \
    (This)->lpVtbl -> get_Info(This,pInfo)

#define IComponentInfo_get_Name(This,bstrName)  \
    (This)->lpVtbl -> get_Name(This,bstrName)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IComponentInfo_get_Info_Proxy(
    IComponentInfo __RPC_FAR * This,
    /* [out] */ COMPONENT_INFO __RPC_FAR *__RPC_FAR *pInfo);


void __RPC_STUB IComponentInfo_get_Info_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IComponentInfo_get_Name_Proxy(
    IComponentInfo __RPC_FAR * This,
    /* [out] */ BSTR __RPC_FAR *bstrName);


void __RPC_STUB IComponentInfo_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IComponentInfo_INTERFACE_DEFINED__ */


#ifndef __IMath2_INTERFACE_DEFINED__
#define __IMath2_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IMath2
 * at Mon Oct 06 22:26:09 1997
 * using MIDL 3.01.75
 ****************************************/
/* [object][unique][helpstring][uuid] */



EXTERN_C const IID IID_IMath2;

#if defined(__cplusplus) && !defined(CINTERFACE)

interface DECLSPEC_UUID("984D09A2-3379-11d1-883A-444553540000")
IMath2 :
public IUnknown {
public:
    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Add(
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR * plResult) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Subtract(
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR * plResult) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Multiply(
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR * plResult) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Divide(
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR * plResult) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Sum(
        /* [in] */ short sArraySize,
        /* [size_is][in] */ short __RPC_FAR sArray[  ],
        /* [retval][out] */ long __RPC_FAR * lResult) = 0;

    virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Compute(
        /* [in] */ mathOPERATION enumOp,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR * plResult) = 0;

    virtual /* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE get_AdvancedMath(
        /* [retval][out] */ IAdvancedMath __RPC_FAR *__RPC_FAR * ppVal) = 0;

};

#else   /* C style interface */

typedef struct IMath2Vtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IMath2 __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IMath2 __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IMath2 __RPC_FAR * This);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Add)(
        IMath2 __RPC_FAR * This,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR *plResult);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Subtract)(
        IMath2 __RPC_FAR * This,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR *plResult);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Multiply)(
        IMath2 __RPC_FAR * This,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR *plResult);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Divide)(
        IMath2 __RPC_FAR * This,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR *plResult);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Sum)(
        IMath2 __RPC_FAR * This,
        /* [in] */ short sArraySize,
        /* [size_is][in] */ short __RPC_FAR sArray[  ],
        /* [retval][out] */ long __RPC_FAR *lResult);

    /* [helpstring] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *Compute)(
        IMath2 __RPC_FAR * This,
        /* [in] */ mathOPERATION enumOp,
        /* [in] */ long lOp1,
        /* [in] */ long lOp2,
        /* [retval][out] */ long __RPC_FAR *plResult);

    /* [helpstring][propget] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_AdvancedMath)(
        IMath2 __RPC_FAR * This,
        /* [retval][out] */ IAdvancedMath __RPC_FAR *__RPC_FAR *ppVal);

    END_INTERFACE
} IMath2Vtbl;

interface IMath2 {
    CONST_VTBL struct IMath2Vtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IMath2_QueryInterface(This,riid,ppvObject)  \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMath2_AddRef(This) \
    (This)->lpVtbl -> AddRef(This)

#define IMath2_Release(This)    \
    (This)->lpVtbl -> Release(This)


#define IMath2_Add(This,lOp1,lOp2,plResult) \
    (This)->lpVtbl -> Add(This,lOp1,lOp2,plResult)

#define IMath2_Subtract(This,lOp1,lOp2,plResult)    \
    (This)->lpVtbl -> Subtract(This,lOp1,lOp2,plResult)

#define IMath2_Multiply(This,lOp1,lOp2,plResult)    \
    (This)->lpVtbl -> Multiply(This,lOp1,lOp2,plResult)

#define IMath2_Divide(This,lOp1,lOp2,plResult)  \
    (This)->lpVtbl -> Divide(This,lOp1,lOp2,plResult)

#define IMath2_Sum(This,sArraySize,sArray,lResult)  \
    (This)->lpVtbl -> Sum(This,sArraySize,sArray,lResult)

#define IMath2_Compute(This,enumOp,lOp1,lOp2,plResult)  \
    (This)->lpVtbl -> Compute(This,enumOp,lOp1,lOp2,plResult)

#define IMath2_get_AdvancedMath(This,ppVal) \
    (This)->lpVtbl -> get_AdvancedMath(This,ppVal)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath2_Add_Proxy(
    IMath2 __RPC_FAR * This,
    /* [in] */ long lOp1,
    /* [in] */ long lOp2,
    /* [retval][out] */ long __RPC_FAR *plResult);


void __RPC_STUB IMath2_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath2_Subtract_Proxy(
    IMath2 __RPC_FAR * This,
    /* [in] */ long lOp1,
    /* [in] */ long lOp2,
    /* [retval][out] */ long __RPC_FAR *plResult);


void __RPC_STUB IMath2_Subtract_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath2_Multiply_Proxy(
    IMath2 __RPC_FAR * This,
    /* [in] */ long lOp1,
    /* [in] */ long lOp2,
    /* [retval][out] */ long __RPC_FAR *plResult);


void __RPC_STUB IMath2_Multiply_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath2_Divide_Proxy(
    IMath2 __RPC_FAR * This,
    /* [in] */ long lOp1,
    /* [in] */ long lOp2,
    /* [retval][out] */ long __RPC_FAR *plResult);


void __RPC_STUB IMath2_Divide_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath2_Sum_Proxy(
    IMath2 __RPC_FAR * This,
    /* [in] */ short sArraySize,
    /* [size_is][in] */ short __RPC_FAR sArray[  ],
    /* [retval][out] */ long __RPC_FAR *lResult);


void __RPC_STUB IMath2_Sum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE IMath2_Compute_Proxy(
    IMath2 __RPC_FAR * This,
    /* [in] */ mathOPERATION enumOp,
    /* [in] */ long lOp1,
    /* [in] */ long lOp2,
    /* [retval][out] */ long __RPC_FAR *plResult);


void __RPC_STUB IMath2_Compute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][propget] */ HRESULT STDMETHODCALLTYPE IMath2_get_AdvancedMath_Proxy(
    IMath2 __RPC_FAR * This,
    /* [retval][out] */ IAdvancedMath __RPC_FAR *__RPC_FAR *ppVal);


void __RPC_STUB IMath2_get_AdvancedMath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IMath2_INTERFACE_DEFINED__ */



#ifndef __CHAPTER4_SERVERLib_LIBRARY_DEFINED__
#define __CHAPTER4_SERVERLib_LIBRARY_DEFINED__

/****************************************
 * Generated header for library: CHAPTER4_SERVERLib
 * at Mon Oct 06 22:26:09 1997
 * using MIDL 3.01.75
 ****************************************/
/* [helpstring][version][uuid] */



EXTERN_C const IID LIBID_CHAPTER4_SERVERLib;

#ifdef __cplusplus
EXTERN_C const CLSID CLSID_Math;

class DECLSPEC_UUID("5FB0C22F-3343-11D1-883A-444553540000")
    Math;
#endif
#endif /* __CHAPTER4_SERVERLib_LIBRARY_DEFINED__ */

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

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.01.75 */
/* at Sun Oct 19 18:54:28 1997
 */
/* Compiler settings for Chapter3_Server.idl:
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

#ifndef __Chapter3_Server_h__
#define __Chapter3_Server_h__

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

#ifndef __IMath_INTERFACE_DEFINED__
#define __IMath_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: IMath
 * at Sun Oct 19 18:54:28 1997
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][uuid][object] */



EXTERN_C const IID IID_IMath;

#if defined(__cplusplus) && !defined(CINTERFACE)

interface DECLSPEC_UUID("8812699C-1CC8-11D1-883A-444553540000")
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
 * at Sun Oct 19 18:54:28 1997
 * using MIDL 3.01.75
 ****************************************/
/* [unique][helpstring][uuid][object] */



EXTERN_C const IID IID_IAdvancedMath;

#if defined(__cplusplus) && !defined(CINTERFACE)

interface DECLSPEC_UUID("6AF3DF1E-C48F-11D0-A769-D477A4000000")
IAdvancedMath :
public IUnknown {
public:
    virtual HRESULT STDMETHODCALLTYPE Factorial(
        /* [in] */ short sFact,
        /* [retval][out] */ long __RPC_FAR * pResult) = 0;

    virtual HRESULT STDMETHODCALLTYPE Fibonacci(
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

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *Factorial)(
        IAdvancedMath __RPC_FAR * This,
        /* [in] */ short sFact,
        /* [retval][out] */ long __RPC_FAR *pResult);

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *Fibonacci)(
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



HRESULT STDMETHODCALLTYPE IAdvancedMath_Factorial_Proxy(
    IAdvancedMath __RPC_FAR * This,
    /* [in] */ short sFact,
    /* [retval][out] */ long __RPC_FAR *pResult);


void __RPC_STUB IAdvancedMath_Factorial_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IAdvancedMath_Fibonacci_Proxy(
    IAdvancedMath __RPC_FAR * This,
    /* [in] */ short sFib,
    /* [retval][out] */ long __RPC_FAR *pResult);


void __RPC_STUB IAdvancedMath_Fibonacci_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IAdvancedMath_INTERFACE_DEFINED__ */



#ifndef __CHAPTER3_SERVERLib_LIBRARY_DEFINED__
#define __CHAPTER3_SERVERLib_LIBRARY_DEFINED__

/****************************************
 * Generated header for library: CHAPTER3_SERVERLib
 * at Sun Oct 19 18:54:28 1997
 * using MIDL 3.01.75
 ****************************************/
/* [helpstring][version][uuid] */



EXTERN_C const IID LIBID_CHAPTER3_SERVERLib;

#ifdef __cplusplus
EXTERN_C const CLSID CLSID_Math;

class DECLSPEC_UUID("8812699D-1CC8-11D1-883A-444553540000")
    Math;
#endif
#endif /* __CHAPTER3_SERVERLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Feb 06 15:56:08 2000
 */
/* Compiler settings for C:\projects\Books\atl\2ed\Examples\Chap09\Chapter9_EnumServer\Chapter9_EnumServer.idl:
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

#ifndef __Chapter9_EnumServer_h__
#define __Chapter9_EnumServer_h__

#ifdef __cplusplus
extern "C" {
#endif

/* Forward Declarations */

#ifndef __IBasicEnum_FWD_DEFINED__
#define __IBasicEnum_FWD_DEFINED__
typedef interface IBasicEnum IBasicEnum;
#endif  /* __IBasicEnum_FWD_DEFINED__ */


#ifndef __BasicEnum_FWD_DEFINED__
#define __BasicEnum_FWD_DEFINED__

#ifdef __cplusplus
typedef class BasicEnum BasicEnum;
#else
typedef struct BasicEnum BasicEnum;
#endif /* __cplusplus */

#endif  /* __BasicEnum_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void __RPC_FAR *);

#ifndef __IBasicEnum_INTERFACE_DEFINED__
#define __IBasicEnum_INTERFACE_DEFINED__

/* interface IBasicEnum */
/* [unique][helpstring][uuid][object] */


EXTERN_C const IID IID_IBasicEnum;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("DBD4D8EF-DCD4-11D3-83C0-0060081AEB5F")
IBasicEnum :
public IUnknown {
public:
    virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE get_Enum(
        /* [retval][out] */ IEnumString __RPC_FAR *__RPC_FAR * ppVal) = 0;

};

#else   /* C style interface */

typedef struct IBasicEnumVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE __RPC_FAR *QueryInterface)(
        IBasicEnum __RPC_FAR * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *AddRef)(
        IBasicEnum __RPC_FAR * This);

    ULONG(STDMETHODCALLTYPE __RPC_FAR *Release)(
        IBasicEnum __RPC_FAR * This);

    /* [helpstring][id] */ HRESULT(STDMETHODCALLTYPE __RPC_FAR *get_Enum)(
        IBasicEnum __RPC_FAR * This,
        /* [retval][out] */ IEnumString __RPC_FAR *__RPC_FAR *ppVal);

    END_INTERFACE
} IBasicEnumVtbl;

interface IBasicEnum {
    CONST_VTBL struct IBasicEnumVtbl __RPC_FAR *lpVtbl;
};



#ifdef COBJMACROS


#define IBasicEnum_QueryInterface(This,riid,ppvObject)  \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IBasicEnum_AddRef(This) \
    (This)->lpVtbl -> AddRef(This)

#define IBasicEnum_Release(This)    \
    (This)->lpVtbl -> Release(This)


#define IBasicEnum_get_Enum(This,ppVal) \
    (This)->lpVtbl -> get_Enum(This,ppVal)

#endif /* COBJMACROS */


#endif  /* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IBasicEnum_get_Enum_Proxy(
    IBasicEnum __RPC_FAR * This,
    /* [retval][out] */ IEnumString __RPC_FAR *__RPC_FAR *ppVal);


void __RPC_STUB IBasicEnum_get_Enum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif  /* __IBasicEnum_INTERFACE_DEFINED__ */



#ifndef __CHAPTER9_ENUMSERVERLib_LIBRARY_DEFINED__
#define __CHAPTER9_ENUMSERVERLib_LIBRARY_DEFINED__

/* library CHAPTER9_ENUMSERVERLib */
/* [helpstring][version][uuid] */


EXTERN_C const IID LIBID_CHAPTER9_ENUMSERVERLib;

EXTERN_C const CLSID CLSID_BasicEnum;

#ifdef __cplusplus

class DECLSPEC_UUID("DBD4D8F0-DCD4-11D3-83C0-0060081AEB5F")
    BasicEnum;
#endif
#endif /* __CHAPTER9_ENUMSERVERLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif

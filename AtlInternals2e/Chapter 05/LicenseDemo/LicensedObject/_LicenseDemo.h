
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 6.00.0347 */
/* at Sun Nov 10 13:41:53 2002
 */
/* Compiler settings for _LicenseDemo.idl:
    Os, W1, Zp8, env=Win32 (32b run)
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

#ifndef ___LicenseDemo_h__
#define ___LicenseDemo_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
    #pragma once
#endif

/* Forward Declarations */

#ifndef __IEarPolitic_FWD_DEFINED__
    #define __IEarPolitic_FWD_DEFINED__
    typedef interface IEarPolitic IEarPolitic;
#endif  /* __IEarPolitic_FWD_DEFINED__ */


#ifndef __CEarPolitic_FWD_DEFINED__
    #define __CEarPolitic_FWD_DEFINED__

    #ifdef __cplusplus
        typedef class CEarPolitic CEarPolitic;
    #else
        typedef struct CEarPolitic CEarPolitic;
    #endif /* __cplusplus */

#endif  /* __CEarPolitic_FWD_DEFINED__ */


/* header files for imported files */
#include "prsht.h"
#include "mshtml.h"
#include "mshtmhst.h"
#include "exdisp.h"
#include "objsafe.h"

#ifdef __cplusplus
extern "C" {
#endif

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free(void *);

#ifndef __IEarPolitic_INTERFACE_DEFINED__
#define __IEarPolitic_INTERFACE_DEFINED__

/* interface IEarPolitic */
/* [unique][helpstring][oleautomation][uuid][object] */


EXTERN_C const IID IID_IEarPolitic;

#if defined(__cplusplus) && !defined(CINTERFACE)

MIDL_INTERFACE("525DE96A-ABF6-46DF-8EF5-7E677A3F128C")
IEarPolitic :
public IUnknown {
public:
};

#else   /* C style interface */

typedef struct IEarPoliticVtbl {
    BEGIN_INTERFACE

    HRESULT(STDMETHODCALLTYPE *QueryInterface)(
        IEarPolitic * This,
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject);

    ULONG(STDMETHODCALLTYPE *AddRef)(
        IEarPolitic * This);

    ULONG(STDMETHODCALLTYPE *Release)(
        IEarPolitic * This);

    END_INTERFACE
} IEarPoliticVtbl;

interface IEarPolitic {
    CONST_VTBL struct IEarPoliticVtbl *lpVtbl;
};



#ifdef COBJMACROS


#define IEarPolitic_QueryInterface(This,riid,ppvObject) \
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEarPolitic_AddRef(This)    \
    (This)->lpVtbl -> AddRef(This)

#define IEarPolitic_Release(This)   \
    (This)->lpVtbl -> Release(This)


#endif /* COBJMACROS */


#endif  /* C style interface */




#endif  /* __IEarPolitic_INTERFACE_DEFINED__ */



#ifndef __LicenseDemo_LIBRARY_DEFINED__
#define __LicenseDemo_LIBRARY_DEFINED__

/* library LicenseDemo */
/* [helpstring][uuid][version] */


EXTERN_C const IID LIBID_LicenseDemo;

EXTERN_C const CLSID CLSID_CEarPolitic;

#ifdef __cplusplus

class DECLSPEC_UUID("90B1E18D-2976-41A3-A4CC-7DA344318C58")
    CEarPolitic;
#endif
#endif /* __LicenseDemo_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif



//
// Math.cpp
//
//

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "math.h"


//
// Math class implementation
//
// Constructors
Math::Math()
{
    m_dwRef = 0;
}

Math::~Math()
{
    g_dwObjs--;
}


STDMETHODIMP Math::QueryInterface(REFIID riid, void** ppv)
{
    *ppv = NULL;

    if (riid == IID_IUnknown ||
        riid == IID_IDispatch)
        *ppv = this;

    if (*ppv) {
        ((LPUNKNOWN)*ppv)->AddRef();
        return (S_OK);
    }

    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) Math::AddRef()
{
    return ++m_dwRef;
}

STDMETHODIMP_(ULONG) Math::Release()
{
    if (--m_dwRef)
        return m_dwRef;

    delete this;
    return 0;
}

STDMETHODIMP Math::GetTypeInfoCount(UINT* pctinfo)
{
    *pctinfo = 0;
    return S_OK;
}

STDMETHODIMP Math::GetTypeInfo(UINT itinfo,
                               LCID lcid,
                               ITypeInfo** pptinfo)
{
    // Not implemented
    if (pptinfo)
        *pptinfo = 0;

    return (E_NOTIMPL);
}

STDMETHODIMP Math::GetIDsOfNames(REFIID riid,
                                 OLECHAR** rgszNames,
                                 UINT cNames,
                                 LCID lcid,
                                 DISPID* rgdispid)
{
    // To make things simple, we only support 1 name at a time
    if (cNames > 1)
        return (E_INVALIDARG);

    // Convert the member name to ANSI
    CHAR  szAnsi[128];
    long lLen = WideCharToMultiByte(CP_ACP,
                                    0,
                                    rgszNames[0],
                                    wcslen(rgszNames[0]),
                                    szAnsi,
                                    sizeof(szAnsi),
                                    0,
                                    0);
    szAnsi[lLen] = '\0';

    // Compare the member name to see if it's one that we have
    // and return the correct DISPID
    if (strncmp("Add", szAnsi, 3) == 0)
        rgdispid[0] = DISPID_ADD;
    else if (strncmp("Subtract", szAnsi, 8) == 0)
        rgdispid[0] = DISPID_SUBTRACT;
    else if (strncmp("Multiply", szAnsi, 8) == 0)
        rgdispid[0] = DISPID_MULTIPLY;
    else if (strncmp("Divide", szAnsi, 6) == 0)
        rgdispid[0] = DISPID_DIVIDE;
    else
        return (DISPID_UNKNOWN);

    return S_OK;
}

STDMETHODIMP Math::Invoke(DISPID dispid,
                          REFIID riid,
                          LCID lcid,
                          WORD wFlags,
                          DISPPARAMS FAR* pDispParams,
                          VARIANT FAR* pvarResult,
                          EXCEPINFO FAR* pExcepInfo,
                          unsigned int FAR* puArgErr)
{
    // All of our methods take two parameters
    if (!pDispParams ||
        pDispParams->cArgs != 2)
        return (DISP_E_BADPARAMCOUNT);

    // We don't support named arguments
    if (pDispParams->cNamedArgs > 0)
        return (DISP_E_NONAMEDARGS);

    // Break out the parameters and coerce them
    // to the proper type
    HRESULT hr;
    VARIANT varOp1;
    VARIANT varOp2;
    // Coerce the variant into the desired type
    // In this case we would like a long
    VariantInit(&varOp1);
    hr = VariantChangeType(&varOp1,
                           &(pDispParams->rgvarg[1]),
                           0,
                           VT_I4);

    // If we can't get a long return invalidate argument
    if (FAILED(hr))
        return (DISP_E_TYPEMISMATCH);

    // Coerce the variant into the desired type
    // In this case we would like a long
    VariantInit(&varOp2);
    hr = VariantChangeType(&varOp2,
                           &(pDispParams->rgvarg[0]),
                           0,
                           VT_I4);

    // If we can't get a long return invalidate argument
    if (FAILED(hr))
        return (DISP_E_TYPEMISMATCH);

    // Initialize the return value
    // If there isn't one, then just return
    if (pvarResult) {
        VariantInit(pvarResult);
        pvarResult->vt = VT_I4;
    } else
        return S_OK;

    switch (dispid) {
    case DISPID_ADD:
        pvarResult->lVal = varOp1.lVal + varOp2.lVal;
        return S_OK;

    case DISPID_SUBTRACT:
        pvarResult->lVal = varOp1.lVal - varOp2.lVal;
        return S_OK;

    case DISPID_MULTIPLY:
        pvarResult->lVal = varOp1.lVal * varOp2.lVal;
        return S_OK;

    case DISPID_DIVIDE:
        pvarResult->lVal = varOp1.lVal / varOp2.lVal;
        return S_OK;

    default:
        return (DISP_E_MEMBERNOTFOUND);
    }
}

MathClassFactory::MathClassFactory()
{
    m_dwRef = 0;
}

MathClassFactory::~MathClassFactory()
{
}

STDMETHODIMP MathClassFactory::QueryInterface(REFIID riid, void** ppv)
{
    *ppv = NULL;

    if (riid == IID_IUnknown || riid == IID_IClassFactory)
        *ppv = this;

    if (*ppv) {
        ((LPUNKNOWN)*ppv)->AddRef();
        return S_OK;
    }

    return (E_NOINTERFACE);
}

STDMETHODIMP_(ULONG) MathClassFactory::AddRef()
{
    return ++m_dwRef;
}

STDMETHODIMP_(ULONG) MathClassFactory::Release()
{
    if (--m_dwRef)
        return m_dwRef;
    else
        delete this;

    return 0;
}

STDMETHODIMP MathClassFactory::CreateInstance
(LPUNKNOWN pUnkOuter, REFIID riid, void** ppvObj)
{
    Math* pMath;
    HRESULT     hr;
    *ppvObj = NULL;
    pMath = new Math;

    if (pMath == NULL)
        return (E_OUTOFMEMORY);

    hr = pMath->QueryInterface(riid, ppvObj);

    if (FAILED(hr))
        delete pMath;
    else
        g_dwObjs++;  // Increment the global object count

    return hr;
}

STDMETHODIMP MathClassFactory::LockServer(BOOL fLock)
{
    if (fLock)
        g_dwLocks++;
    else
        g_dwLocks--;

    return S_OK;
}


// CoPrimeNumbers.h : Declaration of the CPrimeNumbers

#pragma once
#include "resource.h"       // main symbols
#include <vector>
#include "IsPrime.h"

using namespace std;

__interface IEnumPrimes;
[idl_quote("interface IEnumPrimes;")];

// IPrimeNumbers
[
    object,
    uuid("EA0E07FA-0E03-44DC-BAB3-3080F4C533AE"),
    dual,   helpstring("IPrimeNumbers Interface"),
    pointer_default(unique)
]
__interface IPrimeNumbers :
IDispatch {
    HRESULT CalcPrimes([in] long min, [in] long max);

    [propget]
    HRESULT Count([out, retval] long * pnCount);

    [propget]
    HRESULT Item([in] long n, [out, retval] long * pnPrime);

    [propget]
    HRESULT Enum([out, retval] IEnumPrimes** ppEnum);
};

[
    object,
    uuid(E64EDDDF - 2B2C - 11D2 - 983C - 00600823CFFB),
    helpstring("IEnumPrimes Interface"),
    pointer_default(unique)
]
__interface IEnumPrimes :
IUnknown {
    [local]
    HRESULT Next([in] ULONG celt,
    [out] long * rgelt,
    [out] ULONG * pceltFetched);

    [call_as(Next)]
    HRESULT RemoteNext([in] ULONG celt,
    [out, size_is(celt), length_is(*pceltFetched)] long * rgelt,
    [out] ULONG * pceltFetched);

    HRESULT Skip([in] ULONG celt);
    HRESULT Reset();
    HRESULT Clone([out] IEnumPrimes **ppenum);
};

// Implement local/call_as for IEnumPrimes
[idl_quote("[cpp_quote(\"#ifdef __midl_proxy\")]")];
[idl_quote("[cpp_quote(\"static HRESULT STDMETHODCALLTYPE\")]")];
[idl_quote("[cpp_quote(\"IEnumPrimes_Next_Proxy(IEnumPrimes* This, ULONG celt, long* rgelt, ULONG* pceltFetched)\")]")];
[idl_quote("[cpp_quote(\"{\")]")];
[idl_quote("[cpp_quote(\"  ULONG cFetched;\")]")];
[idl_quote("[cpp_quote(\"  if( !pceltFetched && celt != 1 ) return E_INVALIDARG;\")]")];
[idl_quote("[cpp_quote(\"  return IEnumPrimes_RemoteNext_Proxy(This, celt, rgelt, pceltFetched ? pceltFetched : &cFetched);\")]")];
[idl_quote("[cpp_quote(\"}\")]")];
[idl_quote("[cpp_quote(\"\")]")];
[idl_quote("[cpp_quote(\"static HRESULT STDMETHODCALLTYPE\")]")];
[idl_quote("[cpp_quote(\"IEnumPrimes_Next_Stub(IEnumPrimes* This, ULONG celt, long* rgelt, ULONG* pceltFetched)\")]")];
[idl_quote("[cpp_quote(\"{\")]")];
[idl_quote("[cpp_quote(\"  HRESULT hr = This->lpVtbl->Next(This, celt, rgelt, pceltFetched);\")]")];
[idl_quote("[cpp_quote(\"  if( hr == S_OK && celt == 1 ) *pceltFetched = 1;\")]")];
[idl_quote("[cpp_quote(\"  return hr;\")]")];
[idl_quote("[cpp_quote(\"}\")]")];
[idl_quote("[cpp_quote(\"#endif // __midl_proxy\")]")];


struct _CopyVariantFromLong {
    static HRESULT copy(VARIANT* p1, long* p2)
    {
        p1->vt = VT_I4;
        p1->lVal = *p2;
        return S_OK;
    }

    static void init(VARIANT* p)
    {
        VariantInit(p);
    }
    static void destroy(VARIANT* p)
    {
        VariantClear(p);
    }
};

typedef CComEnumOnSTL<IEnumPrimes, &__uuidof(IEnumPrimes), long, _Copy<long>, vector<long> >
CComEnumPrimesOnVectorOfLongs;


// CPrimeNumbers

[
    coclass,
    threading("apartment"),
    vi_progid("PrimeSvr.PrimeNumbers"),
    progid("PrimeSvr.PrimeNumbers.1"),
    version(1.0),
    uuid("0EBFFA7F-D33E-4718-B19C-8FABEE9488D5"),
    helpstring("PrimeNumbers Class")
]
class ATL_NO_VTABLE CPrimeNumbers :
//public IPrimeNumbers
    public IDispatchImpl<IPrimeNumbers, &__uuidof(IPrimeNumbers), &CAtlModule::m_libid, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
    CPrimeNumbers()
    {
    }


    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

    STDMETHODIMP CalcPrimes(long min, long max)
    {
        m_rgPrimes.clear();

        for (long n = min; n <= max; n++) {
            if (IsPrime(n)) {
                m_rgPrimes.push_back(n);
            }
        }

        return S_OK;
    }

    STDMETHODIMP get_Count(long* pnCount)
    {
        *pnCount = m_rgPrimes.size();
        return S_OK;
    }

    STDMETHODIMP get_Item(long n, long* pnPrime)
    {
        if (n >= 1 && n <= m_rgPrimes.size()) {
            *pnPrime = m_rgPrimes[n - 1];
            return S_OK;
        }

        return E_INVALIDARG;
    }

    // w/ STL
    STDMETHODIMP get_Enum(IEnumPrimes** ppEnum)
    {
        *ppEnum = 0;
        CComObject<CComEnumPrimesOnVectorOfLongs>* pe = NULL;
        HRESULT hr = pe->CreateInstance(&pe);

        if (SUCCEEDED(hr)) {
            pe->AddRef();
            hr = pe->Init(this->GetUnknown(), m_rgPrimes);

            if (SUCCEEDED(hr)) {
                hr = pe->QueryInterface(ppEnum);
            }

            pe->Release();
        } else {
            hr = E_OUTOFMEMORY;
        }

        return hr;
    }

private:
    vector<long>    m_rgPrimes;

};


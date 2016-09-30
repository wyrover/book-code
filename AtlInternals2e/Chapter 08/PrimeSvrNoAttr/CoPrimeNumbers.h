// CoPrimeNumbers.h : Declaration of the CPrimeNumbers

#pragma once
#include "resource.h"       // main symbols

#include "PrimeSvrNoAttr.h"
#include <vector>
#include "IsPrime.h"

using namespace std;

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

class ATL_NO_VTABLE CPrimeNumbers :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CPrimeNumbers, &CLSID_PrimeNumbers>,
    public IDispatchImpl<IPrimeNumbers, &IID_IPrimeNumbers, &LIBID_PrimeSvrNoAttrLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
    CPrimeNumbers()
    {
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_PRIMENUMBERS)


    BEGIN_COM_MAP(CPrimeNumbers)
    COM_INTERFACE_ENTRY(IPrimeNumbers)
    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()


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
        *pnCount = (long)m_rgPrimes.size();
        return S_OK;
    }

    STDMETHODIMP get_Item(long n, long* pnPrime)
    {
        if (n >= 1 && n <= (long)m_rgPrimes.size()) {
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

OBJECT_ENTRY_AUTO(__uuidof(PrimeNumbers), CPrimeNumbers)

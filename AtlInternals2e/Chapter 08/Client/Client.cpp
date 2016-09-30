// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

#include "..\\PrimeSvrNoAttr\\PrimeSvrNoAttr.h"
#include "..\\PrimeSvrNoAttr\\PrimeSvrNoAttr_i.c"
#include "enum_iterator.h"

struct OutputPrime {
    void operator()(const long& nPrime)
    {
        cout << nPrime << " ";
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
    ::CoInitialize(NULL);
    CComPtr<IPrimeNumbers>  spPrimes;

    if (SUCCEEDED(spPrimes.CoCreateInstance(CLSID_PrimeNumbers))) {
        // Populate the collection
        HRESULT hr = spPrimes->CalcPrimes(0, 1000);
        // Count the number of items in the collection
        long  nPrimes;
        hr = spPrimes->get_Count(&nPrimes);
        cout << "Primes: " << nPrimes << endl;
        // Enumerate over the collection using sequencial access
        CComPtr<IEnumPrimes>  spEnum;
        hr = spPrimes->get_Enum(&spEnum);
        // Using an STL algorithm
        //typedef enum_iterator<IEnumPrimes, &IID_IEnumPrimes, long> enum_primes;
        //for_each(enum_primes(spEnum), enum_primes(), OutputPrime());
        // Using an STL-like iterator
        //typedef enum_iterator<IEnumPrimes, &IID_IEnumPrimes, long> enum_primes;
        //for (enum_primes it = enum_primes(spEnum, 64); it != enum_primes(); it++)
        //{
        //  cout << *it << " ";
        //}
        //cout << endl;
        // Yucky, raw way
        const size_t MAX_PRIMES = 64;
        long rgnPrimes[MAX_PRIMES];

        do {
            ULONG cFetched;
            hr = spEnum->Next(MAX_PRIMES, rgnPrimes, &cFetched);

            if (SUCCEEDED(hr)) {
                if (hr == S_OK) cFetched = MAX_PRIMES;

                for (long* pn = &rgnPrimes[0]; pn != &rgnPrimes[cFetched]; ++pn) {
                    cout << *pn << " ";
                }
            }
        } while (hr == S_OK);

        cout << endl;
        spPrimes.Release();
    }

    ::CoUninitialize();
}


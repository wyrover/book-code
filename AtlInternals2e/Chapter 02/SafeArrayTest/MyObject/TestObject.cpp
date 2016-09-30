// TestObject.cpp : Implementation of CTestObject

#include "stdafx.h"
#include "TestObject.h"
#include <atlsafe.h>
#import "C:\Program Files\IVI\Bin\IviDriverTypeLib.dll" no_namespace

// CTestObject


STDMETHODIMP CTestObject::GetArray(SAFEARRAY** ppsa)
{
    if (!ppsa) return E_POINTER;

    long rgIndices[] = { 4, 5 };
    {
        IIviDriver* pDriver = NULL;
        IUnknown* punk = NULL;
        CComPtr<IUnknown> spUnk;
        CComSafeArray<BSTR> sa;
    }
    /*
    {
        CComSafeArrayBound bound1(3);
        CComSafeArrayBound bound2(4);
        CComSafeArrayBound bound3(5);
        CComSafeArrayBound rgBound[] = { bound1, bound2, bound3 };
        CComSafeArray<int> sa(rgBound, 3);
    }

    {
        CComSafeArray<double> saSrc(500);
        CComSafeArray<char> saDest(saSrc);
    }

    {
        CComSafeArray<long> saOrig;

        CComSafeArray<long> sa1;
        sa1.Attach(saOrig);
    }

    {
        CComSafeArray<long> sa(5);
        sa.SetAt(3, 5);
    }
    */
    {
        CComSafeArray<int> sa(5);
        ATLTRACE(sa.IsSizable() ? "\n\nSizeable\n\n" : "\n\nNo\n\n");
    }
    {
        CComSafeArray<int> sa;
        sa.Add(2);
        int rgVal[] = { 4, 6 };
        sa.Add(2, rgVal);
        sa.Add(sa);
    }
    {
        CComSafeArray<int> sa(5);
        ATLASSERT(sa[2] == 0);
        long l = sa[3];
        sa[2] = 3;
        ATLASSERT(sa[2] == 3);
    }
    return S_OK;
}


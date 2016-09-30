// TestObject.h : Declaration of the CTestObject

#pragma once
#include "resource.h"       // main symbols


// ITestObject
[
    object,
    uuid("5D8C02D0-11EC-47C2-8468-BD1909A1627A"),
    oleautomation,  helpstring("ITestObject Interface"),
    pointer_default(unique)
]
__interface ITestObject :
IUnknown {
    [id(1), helpstring("method GetArray")] HRESULT GetArray([out, retval, satype(VARIANT_BOOL)] SAFEARRAY** myArray);
};



// CTestObject

[
    coclass,
    threading("apartment"),
    support_error_info("ITestObject"),
    vi_progid("MyObject.TestObject"),
    progid("MyObject.TestObject.1"),
    version(1.0),
    uuid("2B70187A-8E12-4DDC-BE8D-1A5A151D698F"),
    helpstring("TestObject Class")
]
class ATL_NO_VTABLE CTestObject :
    public ITestObject
{
public:
    CTestObject()
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

public:

    STDMETHOD(GetArray)(SAFEARRAY** myArray);
};


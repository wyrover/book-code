// Created by Microsoft (R) C/C++ Compiler Version 13.00.9466
//
// c:\kirk\atl internals 2e\src\chapter 05\licensedemo\licensedobject\coearpolitic.mrg.h
// compiler-generated file created 11/10/02 at 13:41:53
//
// This C++ source file is intended to be a representation of the
// source code injected by the compiler.  It may not compile or
// run exactly as the original source file.
//


//+++ Start Injected Code
[no_injected_text(true)];      // Suppress injected text, it has already been injected
#pragma warning(disable: 4543) // Suppress warnings about skipping injected text
#pragma warning(disable: 4199) // Suppress warnings from attribute providers

#pragma message("\n\nNOTE: This merged source file should be visually inspected for correctness.\n\n")
//--- End Injected Code

// CoEarPolitic.h : Declaration of the CEarPolitic

#pragma once
#include "resource.h"       // main symbols
#include "MyLicense.h"


// IEarPolitic
[
    object,
    uuid("525DE96A-ABF6-46DF-8EF5-7E677A3F128C"),
    oleautomation,  helpstring("IEarPolitic Interface"),
    pointer_default(unique)
]
__interface IEarPolitic :
IUnknown {
};



// CEarPolitic

[
    coclass,
    threading("apartment"),
    vi_progid("LicenseDemo.EarPolitic"),
    progid("LicenseDemo.EarPolitic.1"),
    version(1.0),
    uuid("90B1E18D-2976-41A3-A4CC-7DA344318C58"),
    helpstring("EarPolitic Class")
]
class ATL_NO_VTABLE CEarPolitic :
    public IEarPolitic
    ,
/*+++ Added Baseclass */ public CComCoClass<CEarPolitic, &__uuidof(CEarPolitic)>,
/*+++ Added Baseclass */ public CComObjectRootEx<CComSingleThreadModel>,
/*+++ Added Baseclass */ public IProvideClassInfoImpl<&__uuidof(CEarPolitic)>
{
public:
    CEarPolitic()
    {
    }


    DECLARE_CLASSFACTORY2(CMyLicense)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease()
    {
    }

public:


    //+++ Start Injected Code For Attribute 'coclass'
#injected_line 30 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\coearpolitic.h"
    static DWORD* GetOpCodes()
    {
        static DWORD rgOps[] = {
            0x70000000,
            0x30004000,
            0x80000002,
            0x50000000,
            0x3000c000,
            0x80000004,
            0x60000000,
            0x30014000,
            0x80000002,
            0x50000000,
            0x3000c000,
            0x80000004,
            0x30018000,
            0x80000001,
            0x60000000,
            0x1000c000,
            0x3000c000,
            0x50000000,
            0x20010000,
            0x30010000,
            0x80000002,
            0x50000000,
            0x3001c000,
            0x80000001,
            0x30020000,
            0x80000005,
            0x20024000,
            0x30024000,
            0x30028000,
            0x8000000b,
            0x50000000,
            0x8003000d,
            0x60000000,
            0x8003800f,
            0x30040000,
            0x80000011,
            0x60000000,
            0x60000000,
            0x60000000,
            0
        };
        return rgOps;
    }
#injected_line 30 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\coearpolitic.h"
    static RGSDWORD* GetOpcodeDWORDVals()
    {
        static RGSDWORD rgDWORDS[] = {
            {0, 0}
        };
        return rgDWORDS;
    }
#injected_line 30 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\coearpolitic.h"
    static RGSBinary* GetOpcodeBinaryVals()
    {
        static RGSBinary rgBinary[] = {
            {0, 0}
        };
        return rgBinary;
    }
#injected_line 30 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\coearpolitic.h"
    static RGSStrings* GetOpcodeStringVals()
    {
        static RGSStrings rgStrings[] = {
            {_T(""), 0 },
            {_T("LicenseDemo.EarPolitic.1"), 0 },
            {_T("%FriendlyName%"), 1 },
            {_T("CLSID"), 0 },
            {_T("{90B1E18D-2976-41A3-A4CC-7DA344318C58}"), 0 },
            {_T("LicenseDemo.EarPolitic"), 0 },
            {_T("CurVer"), 0 },
            {_T("ProgID"), 0 },
            {_T("VersionIndependentProgID"), 0 },
            {_T("Programmable"), 0 },
            {_T("%MODULETYPE%"), 1 },
            {_T("%MODULE%"), 1 },
            {_T("ThreadingModel"), 0 },
            {_T("apartment"), 0 },
            {_T("AppID"), 0 },
            {_T("%APPID%"), 1 },
            {_T("TypeLib"), 0 },
            {_T("%MODULEGUID%"), 1 },
            {NULL, 0}
        };
        return rgStrings;
    }
#injected_line 30 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\coearpolitic.h"
    static HRESULT WINAPI UpdateRegistry(BOOL bRegister)
    {
        CRegistryVirtualMachine rvm;
        HRESULT hr;

        if (FAILED(hr = rvm.AddStandardReplacements()))
            return hr;

        rvm.AddReplacement(_T("FriendlyName"), GetObjectFriendlyName());
        return rvm.VMUpdateRegistry(GetOpCodes(), GetOpcodeStringVals(), GetOpcodeDWORDVals(), GetOpcodeBinaryVals(), bRegister);
    }
#injected_line 30 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\coearpolitic.h"
    static const TCHAR* GetObjectFriendlyName()
    {
        return _T("CEarPolitic Object");
    }
#injected_line 30 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\coearpolitic.h"
    static const TCHAR* GetProgID()
    {
        return _T("LicenseDemo.EarPolitic.1");
    }
#injected_line 30 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\coearpolitic.h"
    static const TCHAR* GetVersionIndependentProgID()
    {
        return _T("LicenseDemo.EarPolitic");
    }
#injected_line 30 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\coearpolitic.h"
    BEGIN_COM_MAP(CEarPolitic)
    COM_INTERFACE_ENTRY(IEarPolitic)
    COM_INTERFACE_ENTRY(IProvideClassInfo)
    END_COM_MAP()

    //--- End Injected Code For Attribute 'coclass'
};

//+++ Start Injected Code For Attribute 'coclass'
#injected_line 30 "c:\\kirk\\atl internals 2e\\src\\chapter 05\\licensedemo\\licensedobject\\coearpolitic.h"

OBJECT_ENTRY_AUTO(__uuidof(CEarPolitic), CEarPolitic)

//--- End Injected Code For Attribute 'coclass'



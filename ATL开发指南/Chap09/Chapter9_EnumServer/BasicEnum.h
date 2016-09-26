// BasicEnum.h : Declaration of the CBasicEnum

#ifndef __BASICENUM_H_
#define __BASICENUM_H_

#include "resource.h"       // main symbols

#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CBasicEnum
class ATL_NO_VTABLE CBasicEnum :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CBasicEnum, &CLSID_BasicEnum>,
    public ISupportErrorInfo,
    public IBasicEnum
{
public:
    CBasicEnum()
    {
        // Put some items in the vector
        m_vecStrings.push_back(L"StringOne");
        m_vecStrings.push_back(L"StringTwo");
        m_vecStrings.push_back(L"StringThree");
        m_vecStrings.push_back(L"StringFour");
        m_vecStrings.push_back(L"StringFive");
    }

    DECLARE_REGISTRY_RESOURCEID(IDR_BASICENUM)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CBasicEnum)
    COM_INTERFACE_ENTRY(IBasicEnum)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    END_COM_MAP()

// ISupportsErrorInfo
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IBasicEnum
public:
    STDMETHOD(get_Enum)(/*[out, retval]*/ IEnumString  **ppVal);

private:
    std::vector<LPOLESTR> m_vecStrings;
};

#endif //__BASICENUM_H_

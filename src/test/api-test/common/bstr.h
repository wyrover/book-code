#ifndef bstr_h__
#define bstr_h__

#include <wbemidl.h>
#include <vector>

inline BSTR ConvertStringToBSTR(const char* pSrc)
{
    if (!pSrc) return NULL;

    DWORD cwch;
    BSTR wsOut(NULL);

    if (cwch = ::MultiByteToWideChar(CP_ACP, 0, pSrc,
                                     -1, NULL, 0)) { //get size minus NULL terminator
        cwch--;
        wsOut = ::SysAllocStringLen(NULL, cwch);

        if (wsOut) {
            if (!::MultiByteToWideChar(CP_ACP,
                                       0, pSrc, -1, wsOut, cwch)) {
                if (ERROR_INSUFFICIENT_BUFFER == ::GetLastError())
                    return wsOut;

                ::SysFreeString(wsOut);//must clean up
                wsOut = NULL;
            }
        }
    }

    return wsOut;
}


inline BSTR ConvertStringToBSTR(const wchar_t* pSrc)
{
    if (!pSrc) return NULL;

    DWORD cwch = wcslen(pSrc);
    BSTR wsOut(NULL);
    /* Allocate the BSTR (without the NULL terminator) */
    wsOut = ::SysAllocStringLen(NULL, cwch);

    if (wsOut) {
        wcsncpy(wsOut, pSrc, cwch);
    }

    return wsOut;
}


inline BSTR ConvertUTF8StringToBSTR(const char *pSrc)
{
    DWORD cwch;
    BSTR wsOut(NULL);

    if (!pSrc) return NULL;

    /* Compute the needed size with the NULL terminator */
    cwch = MultiByteToWideChar(CP_UTF8, 0, pSrc, -1, NULL, 0);

    if (cwch == 0) return NULL;

    /* Allocate the BSTR (without the NULL terminator) */
    wsOut = SysAllocStringLen(NULL, cwch - 1);

    if (!wsOut) {
        _com_issue_error(HRESULT_FROM_WIN32(ERROR_OUTOFMEMORY));
        return NULL;
    }

    /* Convert the string */
    if (MultiByteToWideChar(CP_UTF8, 0, pSrc, -1, wsOut, cwch) == 0) {
        /* We failed, clean everything up */
        cwch = GetLastError();
        SysFreeString(wsOut);
        wsOut = NULL;
        _com_issue_error(!IS_ERROR(cwch) ? HRESULT_FROM_WIN32(cwch) : cwch);
    }

    return wsOut;
}



inline char* ConvertBSTRToStdString(BSTR pSrc)
{
    if (!pSrc) return NULL;

    //convert even embeded NULL
    DWORD cb, cwch = ::SysStringLen(pSrc);
    char *szOut = NULL;

    if (cb = ::WideCharToMultiByte(CP_ACP, 0,
                                   pSrc, cwch + 1, NULL, 0, 0, 0)) {
        szOut = new char[cb];

        if (szOut) {
            szOut[cb - 1]  = '\0';

            if (!::WideCharToMultiByte(CP_ACP, 0,
                                       pSrc, cwch + 1, szOut, cb, 0, 0)) {
                delete []szOut;//clean up if failed;
                szOut = NULL;
            }
        }
    }

    return szOut;
}

inline std::wstring ConvertBSTRToStdWstring(BSTR pSrc)
{
    if (!pSrc)
        return L"";

    DWORD cb = ::SysStringLen(pSrc);
    std::vector<wchar_t> data(cb + 1, '\0');
    wcsncpy(&data[0], pSrc, cb);
    return std::wstring(&data[0]);
}



//std::string variant_to_str(const VARIANT& var)
//{
//  char buf[128] = { 0 };
//  switch (var.vt)
//  {
//  case VT_NULL:
//  case VT_EMPTY:
//      return "NULL";
//  case VT_I2:
//      StringCbPrintfA(buf, sizeof(buf), "0x%x", (int16_t)var.iVal);
//      return std::string(buf);
//  case VT_I4:
//      StringCbPrintfA(buf, sizeof(buf), "0x%x", (int32_t)var.lVal);
//      return std::string(buf);
//  case VT_UI1:
//      StringCbPrintfA(buf, sizeof(buf), "0x%x", (uint8_t)var.cVal);
//      return std::string(buf);
//  case VT_UI2:
//      StringCbPrintfA(buf, sizeof(buf), "0x%x", (uint16_t)var.uiVal);
//      return std::string(buf);
//  case VT_UI4:
//      StringCbPrintfA(buf, sizeof(buf), "0x%x", (uint32_t)var.ulVal);
//      return std::string(buf);
//  case VT_INT:
//      StringCbPrintfA(buf, sizeof(buf), "0x%x", (int32_t)var.intVal);
//      return std::string(buf);
//  case VT_UINT:
//      StringCbPrintfA(buf, sizeof(buf), "0x%x", (uint32_t)var.uintVal);
//      return std::string(buf);
//  case VT_BSTR:
//      return std::string(ConvertBSTRToStdString(var.bstrVal));
//  default:
//      StringCbPrintfA(buf, sizeof(buf), "unknwon type (vt=0x%x)", var.vt);
//      return std::string(buf);
//  };
//#pragma todo("complete this function...")
//
//  return "";
//}


inline INT GetVarInt(IWbemClassObject *pObj, std::wstring property)
{
    VARIANT var;
    INT iValue = 0;
    HRESULT hr = pObj->Get(property.c_str(), 0, &var, 0, 0);

    if (SUCCEEDED(hr) && var.vt == VT_I4) {
        iValue = var.lVal;
        VariantClear(&var);
    }

    return iValue;
}


inline std::wstring GetVarString(IWbemClassObject *pObj, std::wstring property)
{
    VARIANT var;
    std::wstring s = L"<undefined>";
    HRESULT hr = pObj->Get(property.c_str(), 0, &var, 0, 0);

    if (SUCCEEDED(hr) && var.vt == VT_BSTR) {
        s = ConvertBSTRToStdWstring(var.bstrVal);
        VariantClear(&var);
    }

    return s;
}

inline HRESULT SetVarString(IWbemClassObject *pObj,
                            std::wstring property, std::wstring value)
{
    VARIANT var;
    HRESULT hr;
    VariantInit(&var);
    V_VT(&var) = VT_BSTR;
    V_BSTR(&var) = SysAllocString(value.c_str());
    hr = pObj->Put(property.c_str(), 0, &var, CIM_EMPTY);
    VariantClear(&var);
    return hr;
}



#endif // bstr_h__

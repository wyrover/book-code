#include "wmi.h"
#include "bstr.h"
#include <atlstr.h>



namespace cactus
{
CWmiUtilities::CWmiUtilities()
{
    _hResult = -1;
    loc_ = NULL;
    svc_ = NULL;
    pEnum_ = NULL;
    pls_ = NULL;
}

CWmiUtilities::~CWmiUtilities()
{
    if (pls_ != NULL) {
        pls_->Release();
    }

    if (svc_ != NULL) {
        svc_->Release();
    }

    if (pEnum_ != NULL) {
        pEnum_->Release();
    }

    if (loc_ != NULL) {
        loc_->Release();
    }

    //Closes the COM library
    CoUninitialize();
}

BOOL CWmiUtilities::ConnectWmi(const wchar_t* csWmiNameSpace)
{
    if (!csWmiNameSpace)
        return FALSE;

    //用CoInitializeEx函数初始化COM接口
    _hResult = CoInitializeEx(0, COINIT_MULTITHREADED);

    if (FAILED(_hResult)) {
        return FALSE;
    }

    //用CoInitializeSecurity函数注册并设置进程的默认的安全值
    _hResult = CoInitializeSecurity(
                   NULL,
                   -1,
                   NULL,
                   NULL,
                   RPC_C_AUTHN_LEVEL_DEFAULT,
                   RPC_C_IMP_LEVEL_IMPERSONATE,
                   NULL,
                   EOAC_NONE,
                   NULL
               );

    if (FAILED(_hResult)) {
        return FALSE;
    }

    //用CoCreateInstance函数来初始化IWbemLocator接口
    _hResult = CoCreateInstance(
                   CLSID_WbemLocator,
                   0,
                   CLSCTX_INPROC_SERVER,
                   IID_IWbemLocator,
                   (LPVOID *)&loc_
               );

    if (FAILED(_hResult)) {
        return FALSE;
    }

    /*USES_CONVERSION*/;
    _hResult = loc_->ConnectServer(
                   bstr_t(csWmiNameSpace),
                   NULL,
                   NULL,
                   0,
                   NULL,
                   0,
                   0,
                   &svc_
               );

    if (FAILED(_hResult)) {
        return FALSE;
    }

    //用CoSetProxyBlanket函数设置IWbemServices代理的安全值
    _hResult = CoSetProxyBlanket(
                   svc_,
                   RPC_C_AUTHN_WINNT,
                   RPC_C_AUTHZ_NONE,
                   NULL,
                   RPC_C_AUTHN_LEVEL_CALL,
                   RPC_C_IMP_LEVEL_IMPERSONATE,
                   NULL,
                   EOAC_NONE
               );

    if (FAILED(_hResult)) {
        return FALSE;
    }

    return TRUE;
}

BOOL CWmiUtilities::ExecQuery(const wchar_t* csWQL)
{
    if (!csWQL)
        return FALSE;

    USES_CONVERSION;
    _hResult = svc_->ExecQuery(
                   bstr_t("WQL"),
                   bstr_t(csWQL),
                   WBEM_FLAG_RETURN_IMMEDIATELY,
                   NULL,
                   &pEnum_
               );

    if (FAILED(_hResult)) {
        return FALSE;
    }

    return TRUE;
}

BOOL CWmiUtilities::BeginEnumClassObject(const wchar_t* className)
{
    try {
        if ((!svc_))
            return FALSE;

        if (pEnum_)
            pEnum_->Release();

        pEnum_ = NULL;
        // Get the list of object instances.
        _hResult = svc_->CreateInstanceEnum(bstr_t(className),             // name of class
                                            0,
                                            NULL,
                                            &pEnum_);// pointer to enumerator

        if (FAILED(_hResult))
            return FALSE;

        return TRUE;
    } catch (...) {
        _hResult = WBEM_E_FAILED;
        return FALSE;
    }
}

BOOL CWmiUtilities::GetBOOLProperty(const wchar_t* csName, BOOL& bValue)
{
    VARIANT prop;
    VariantInit(&prop);
    //USES_CONVERSION;
    HRESULT hr = pls_->Get(
                     csName,
                     0,
                     &prop,
                     NULL,
                     NULL
                 );

    if (FAILED(hr)) {
        VariantClear(&prop);
        return FALSE;
    }

    if (prop.vt == VT_BOOL) {
        bValue = prop.boolVal;
    }

    VariantClear(&prop);
    return TRUE;
}


BOOL CWmiUtilities::GetUINT32Property(const wchar_t* csName, UINT32& uInt32Value)
{
    VARIANT prop;
    VariantInit(&prop);
    //USES_CONVERSION;
    HRESULT hr = pls_->Get(
                     csName,
                     0,
                     &prop,
                     NULL,
                     NULL
                 );

    if (FAILED(hr)) {
        VariantClear(&prop);
        return FALSE;
    }

    if (prop.vt == VT_I4) {
        uInt32Value = prop.lVal;
    }

    VariantClear(&prop);
    return TRUE;
}

BOOL CWmiUtilities::PutInstance()
{
    HRESULT hr = svc_->PutInstance(
                     pls_,
                     WBEM_FLAG_UPDATE_ONLY,
                     NULL,
                     NULL
                 );

    if (FAILED(hr)) {
        return FALSE;
    }

    return TRUE;
}

BOOL CWmiUtilities::PutStringProperty(const wchar_t* csName, const wchar_t* csValue)
{
    if (!csName)
        return FALSE;

    VARIANT var;
    VariantInit(&var);
    var.vt = VT_BSTR;
    var.bstrVal = bstr_t(csValue);
    HRESULT hr = pls_->Put(
                     csName,//A parameter that must point to a valid property name. cannot be NULL.
                     0,          //Reserved. This parameter must be 0 (zero).
                     &var,       //point to a valid VARIANT, which becomes the new property value
                     0           //一般是0
                 );
    VariantClear(&var);

    if (FAILED(hr)) {
        return FALSE;
    }

    return PutInstance();
}

BOOL CWmiUtilities::GetStringProperty(const wchar_t* csName, std::wstring& csValue)
{
    VARIANT prop;
    VariantInit(&prop);
    HRESULT hr = pls_->Get(
                     csName,
                     0,
                     &prop,
                     NULL,
                     NULL
                 );

    if (FAILED(hr)) {
        VariantClear(&prop);
        return FALSE;
    }

    if (prop.vt == VT_BSTR) {
        csValue = ConvertBSTRToStdWstring(prop.bstrVal);
    }

    VariantClear(&prop);
    return TRUE;
}


HRESULT CWmiUtilities::GetValue( const wchar_t* name, CComVariant* value )
{
	value->Clear();

	CComBSTR name_string;
	HRESULT hr = name_string.Append(name);
	if (FAILED(hr)) {
		return hr;
	}
	hr = pls_->Get(name_string, 0, value, 0, 0);
	if (FAILED(hr)) {
		return hr;
	}

	return S_OK;
}

HRESULT CWmiUtilities::GetValue( const wchar_t* name, CString* value )
{
	CComVariant var;
	HRESULT hr = GetValue(name, &var);
	if (FAILED(hr)) {
		return hr;
	}

	//ASSERT1(V_VT(&var) == VT_BSTR);
	value->SetString(var.bstrVal);
	return S_OK;
}

HRESULT CWmiUtilities::GetValue( const wchar_t* name, bool* value )
{
	CComVariant var;
	HRESULT hr = GetValue(name, &var);
	if (FAILED(hr)) {
		return hr;
	}

	//ASSERT1(V_VT(&var) == VT_BOOL);
	*value = var.boolVal != 0;
	return S_OK;
}

HRESULT CWmiUtilities::GetValue( const wchar_t* name, int* value )
{
	CComVariant var;
	HRESULT hr = GetValue(name, &var);
	if (FAILED(hr)) {
		return hr;
	}

	//ASSERT1(V_VT(&var) == VT_I4);
	*value = var.lVal;
	return S_OK;
}

HRESULT CWmiUtilities::GetValue( const wchar_t* name, UINT32* value )
{
	CComVariant var;
	HRESULT hr = GetValue(name, &var);
	if (FAILED(hr)) {
		return hr;
	}

	//ASSERT1(V_VT(&var) == VT_UI4);
	*value = var.ulVal;
	return S_OK;
}

std::wstring CWmiUtilities::GetString(std::wstring item)
{
    VARIANT prop;
    VariantInit(&prop);
    HRESULT hr = pls_->Get(
                     bstr_t(item.c_str()),
                     0,
                     &prop,
                     NULL,
                     NULL
                 );
    std::wstring value = L"";

    if (FAILED(hr)) {
        VariantClear(&prop);
        return value;
    }

    if (prop.vt == VT_BSTR) {
        value = ConvertBSTRToStdWstring(prop.bstrVal);
    }

    VariantClear(&prop);
    return value;
}



BOOL CWmiUtilities::Next()
{
    if (pEnum_ == NULL) {
        return FALSE;
    }

    ULONG uReturn = 0;
    _hResult = pEnum_->Next(
                   WBEM_INFINITE,
                   1,
                   &pls_,
                   &uReturn
               );

    if (uReturn == 0) {
        return FALSE;
    }

    return TRUE;
}

BOOL CWmiUtilities::PutBOOLProperty(const wchar_t* csName, BOOL bValue)
{
    if (!csName)
        return FALSE;

    VARIANT var;
    VariantInit(&var);
    var.vt = VT_BOOL;
    var.bVal = bValue;
    //USES_CONVERSION;
    HRESULT hr = pls_->Put(
                     csName,//A parameter that must point to a valid property name. cannot be NULL.
                     0,          //Reserved. This parameter must be 0 (zero).
                     &var,       //point to a valid VARIANT, which becomes the new property value
                     0           //一般是0
                 );
    VariantClear(&var);

    if (FAILED(hr)) {
        return FALSE;
    }

    return PutInstance();
}

BOOL CWmiUtilities::PutUINT32Property(const wchar_t* csName, UINT32 uInt32Value)
{
    if (!csName)
        return FALSE;

    VARIANT var;
    VariantInit(&var);
    var.vt = VT_I4;
    var.lVal = uInt32Value;
    //USES_CONVERSION;
    HRESULT hr = pls_->Put(
                     csName,//A parameter that must point to a valid property name. cannot be NULL.
                     0,          //Reserved. This parameter must be 0 (zero).
                     &var,       //point to a valid VARIANT, which becomes the new property value
                     0           //一般是0
                 );
    VariantClear(&var);

    if (FAILED(hr)) {
        return FALSE;
    }

    return PutInstance();
}

BOOL CWmiUtilities::IsEof()
{
    if (pEnum_ == NULL) {
        return TRUE;
    }

    HRESULT hr = pEnum_->Reset();

    if (FAILED(hr)) {
        return TRUE;
    }

    return !Next();
}

} // namespace cactus
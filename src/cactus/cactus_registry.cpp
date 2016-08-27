/*!
* \file cactus_registry.cpp
* \brief 注册表类
*
* 注册表类实现
*
* \author wangyang
* \date 2015/01/30
* \version 1.0
*/

#include <cactus/cactus_registry.h>
#include <cactus/cactus_types.h>
#include <cactus/cactus_string.h>
#include <cactus/cactus_exception.h>

#include <algorithm>
#include <atlstr.h>
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <ShellAPI.h>

namespace cactus
{
const static int nMaxValueNameSize = MAX_PATH;
const static int nMaxValueValueSize = 4096;
const static int nMaxClassSize = MAX_PATH;
const static int nMaxKeyNameSize = MAX_PATH;
} // namespace cactus

namespace cactus
{

tstring GetRegisteredExePath(LPCTSTR ptszExeName)
{
    CRegKey key;
    tstring tstr = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\");
    tstr += ptszExeName;

    if (ERROR_SUCCESS != key.Open(HKEY_LOCAL_MACHINE, tstr.c_str())) {
        if (ERROR_SUCCESS != key.Open(HKEY_CURRENT_USER, tstr.c_str()))
            return _T("");
    }

    TCHAR tsz [MAX_PATH] = _T("");
    DWORD dw = MAX_PATH;
    key.QueryValue(tsz, _T("Path"), &dw);
    return tsz;
}

} // namespace cactus


namespace cactus
{



LONG CRegKey::Open(HKEY hkey, const string16& key)
{
    return ATL::CRegKey::Open(hkey, key.c_str());
}

LONG CRegKey::Open(const string16& key, bool readOnly /*= false*/, REGSAM extraSam /*= 0*/)
{
    LONG lRes;
    HKEY hRootKey;
    string16 subkey;
    string16::size_type pos = key.find(L'\\');

    if (pos != string16::npos) {
        string16 rootKey = key.substr(0, pos);
        hRootKey = handleFor(rootKey);
        subkey   = key.substr(pos + 1);

        if (readOnly) {
            lRes = ATL::CRegKey::Open(hRootKey, subkey.c_str(), KEY_READ | extraSam);
        } else {
            lRes = ATL::CRegKey::Create(hRootKey, subkey.c_str(), REG_NONE, REG_OPTION_NON_VOLATILE, KEY_READ | KEY_WRITE | extraSam, NULL, NULL);
        }
    }

    return lRes;
}



LONG CRegKey::get_subkey_names(HKEY hkey, const string16& key, std::vector<string16>& list)
{
    ATL::CRegKey reg;
    LONG lRes;
    lRes = reg.Open(hkey, key.c_str());

    if (lRes == ERROR_SUCCESS) {
        DWORD subKeyCount = 0;
        DWORD valueCount = 0;

        if (RegQueryInfoKey(reg.m_hKey, NULL, NULL, NULL, &subKeyCount, NULL, NULL, &valueCount, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
            DWORD index = 0;

            while (index < subKeyCount) {
                WCHAR name[MAX_PATH];
                DWORD name_size = MAX_PATH;

                if (reg.EnumKey(index++, name, &name_size, NULL) == ERROR_SUCCESS) {
                    list.push_back(name);
                }
            }
        }
    }

    return lRes;
}

LONG CRegKey::get_subkey_names(const string16& fullkey, std::vector<string16>& list)
{
    string16 subkey;
    HKEY hkey;
    LONG lRes;

    if (_split_key(fullkey, hkey, subkey)) {
        lRes = CRegKey::get_subkey_names(hkey, subkey, list);
    }

    return lRes;
}

LONG CRegKey::get_value_names(HKEY hkey, const string16& key, std::vector<string16>& list)
{
    std::vector<string16> results;
    ATL::CRegKey reg;
    LONG lRes;
    lRes = reg.Open(hkey, key.c_str());

    if (lRes == ERROR_SUCCESS) {
        DWORD valueCount = 0;

        if (RegQueryInfoKey(reg.m_hKey, NULL, NULL, NULL, NULL, NULL, NULL, &valueCount, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) {
            DWORD index = 0;

            while (index < valueCount) {
                WCHAR name[MAX_PATH];
                DWORD name_size = MAX_PATH;

                if (::RegEnumValue(reg.m_hKey, index++, name, &name_size, 0, 0, 0, 0) == ERROR_SUCCESS) {
                    list.push_back(name);
                }
            }
        }
    }

    return lRes;
}

LONG CRegKey::get_value_names(const string16& fullkey, std::vector<string16>& list)
{
    string16 subkey;
    HKEY hkey;
    LONG lRes;

    if (_split_key(fullkey, hkey, subkey)) {
        lRes = CRegKey::get_value_names(hkey, subkey, list);
    }

    return lRes;
}

LONG CRegKey::DeleteKey(HKEY hkey, const string16& key)
{
    ATL::CRegKey reg;
    LONG lRes = reg.Open(hkey, StringUtil::sWEmpty.c_str());

    if (lRes == ERROR_SUCCESS) {
        lRes = reg.RecurseDeleteKey(key.c_str());
    }

    return lRes;
}

LONG CRegKey::ReadDWORDValue(HKEY hkey, const string16& key, const string16& name, DWORD& data)
{
    ATL::CRegKey reg;
    LONG lRes = reg.Open(hkey, key.c_str());

    if (lRes == ERROR_SUCCESS) {
        lRes = reg.QueryDWORDValue(name.c_str(), data);
    }

    return lRes;
}

LONG CRegKey::ReadStringValue(HKEY hkey, const string16& key, const string16& name, string16& data)
{
    ATL::CRegKey reg;
    LONG lRes = reg.Open(hkey, key.c_str());

    if (lRes == ERROR_SUCCESS) {
        std::vector<char16> buf(MAX_PATH);
        ULONG length = MAX_PATH - 1;
        lRes = reg.QueryStringValue(name.c_str(), &buf[0], &length);

        if (lRes == ERROR_SUCCESS) {
            data = string16(&buf[0]);
        } else {
            data = StringUtil::sWEmpty;
        }
    }

    return lRes;
}

LONG CRegKey::ReadStringValue(const string16& name, string16& data)
{
    std::vector<char16> buf(MAX_PATH);
    DWORD length = MAX_PATH;
    LONG ret = ATL::CRegKey::QueryStringValue(name.c_str(), &buf[0], &length);

    if (ret == ERROR_SUCCESS) {
        data = string16(&buf[0]);
    } else if (ret == ERROR_MORE_DATA) {
        int i = 1;

        while (ret == ERROR_MORE_DATA) {
            length = MAX_PATH * i++;
            std::vector<char16> buf2(length + 1);
            ret = ATL::CRegKey::QueryStringValue(name.c_str(), &buf2[0], &length);
            data = string16(&buf2[0]);
        }
    } else {
        data = StringUtil::sWEmpty;
    }

    return ret;
}

LONG CRegKey::ReadMultiStringValue(HKEY hkey, const string16& key, const string16& name, string16& data)
{
    ATL::CRegKey reg;
    LONG lRes = reg.Open(hkey, key.c_str());

    if (lRes == ERROR_SUCCESS) {
        ULONG length = 0;
        LONG lRet = reg.QueryMultiStringValue(name.c_str(), NULL, &length);

        if (ERROR_SUCCESS != lRet)
            return lRet;

        std::vector<char16> buf(MAX_PATH);
        lRes = reg.QueryMultiStringValue(name.c_str(), &buf[0], &length);

        if (lRes == ERROR_SUCCESS) {
            data = string16(&buf[0]);
        } else {
            data = StringUtil::sWEmpty;
        }
    }

    return lRes;
}

LONG CRegKey::WriteStringValue(HKEY hkey, const string16& key, const string16& name, const string16& data)
{
    ATL::CRegKey reg;
    LONG lRes = reg.Create(hkey, key.c_str());

    if (lRes == ERROR_SUCCESS) {
        lRes = reg.SetStringValue(name.c_str(), data.c_str(), REG_SZ);
    }

    return lRes;
}

LONG CRegKey::WriteStringValue(const string16& name, const string16& data)
{
    return ATL::CRegKey::SetStringValue(name.c_str(), data.c_str(), REG_SZ);
}

LONG CRegKey::WriteDWORDValue(HKEY hkey, const string16& key, const string16& name, const DWORD data)
{
    ATL::CRegKey reg;
    LONG lRes = reg.Create(hkey, key.c_str());

    if (lRes == ERROR_SUCCESS) {
        lRes = reg.SetDWORDValue(name.c_str(), data);
    }

    return lRes;
}

LONG CRegKey::WriteDWORDValue(const string16& name, const DWORD data)
{
    return ATL::CRegKey::SetDWORDValue(name.c_str(), data);
}

BOOL CRegKey::HasValues(HKEY hkey, const string16& key)
{
    cactus::CRegKey reg;
    LONG lRes = reg.Open(hkey, key);

    if (lRes == ERROR_SUCCESS) {
        return reg.HasValues();
    }

    return FALSE;
}

LONG CRegKey::ReadDefaultStringValue(string16& data)
{
    return ReadStringValue(StringUtil::sWEmpty, data);
}

LONG CRegKey::ReadDWORDValue(const string16& name, DWORD& data)
{
    return ATL::CRegKey::QueryDWORDValue(name.c_str(), data);
}

LONG CRegKey::ReadBinaryValue(const string16& name, BYTE* data, DWORD& length)
{
    return ATL::CRegKey::QueryBinaryValue(name.c_str(), data, &length);
}

LONG CRegKey::ReadBinaryValue(const string16& name, std::vector<BYTE>& buf)
{
    if (!m_hKey)
        return ERROR_INVALID_HANDLE;

    ULONG length = 0;
    LONG lRet = CRegKey::QueryBinaryValue(name.c_str(), NULL, &length);

    if (ERROR_SUCCESS != lRet)
        return lRet;

    buf.reserve(length);

    if (buf.size() < length)
        return ERROR_OUTOFMEMORY;

    lRet = CRegKey::QueryBinaryValue(name.c_str(), &buf[0], &length);

    if (ERROR_SUCCESS != lRet)
        return lRet;

    return ERROR_SUCCESS;
}

LONG CRegKey::ReadMultiStringValue(const string16& name, std::vector<string16>& valuelist)
{
    if (!m_hKey)
        return ERROR_INVALID_HANDLE;

    ULONG length = 0;
    LONG lRet = CRegKey::QueryMultiStringValue(name.c_str(), NULL, &length);

    if (ERROR_SUCCESS != lRet)
        return lRet;

    std::vector<char16> buf(length + 1);

    if (buf.size() < length)
        return ERROR_OUTOFMEMORY;

    lRet = CRegKey::QueryMultiStringValue(name.c_str(), &buf[0], &length);

    if (ERROR_SUCCESS != lRet) {
        return lRet;
    }

    LPCWSTR lpEnd  = &buf[buf.size() - 1];
    LPCWSTR lpNext = &buf[0];

    while (lpNext && lpNext < lpEnd && *lpNext) {
        valuelist.push_back(lpNext);
        lpNext = std::find(lpNext, lpEnd, L'\0');

        if (lpNext == lpEnd)
            break;

        ++lpNext;
    }

    return ERROR_SUCCESS;
}

LONG CRegKey::ReadMultiStringValue(HKEY hkey, const string16& key, const string16& name, std::vector<string16>& valuelist)
{
    cactus::CRegKey reg;
    LONG lRes = reg.Open(hkey, key.c_str());

    if (lRes == ERROR_SUCCESS) {
        lRes = reg.ReadMultiStringValue(name, valuelist);
    }

    return lRes;
}

LONG CRegKey::WriteBinaryValue(const string16& name, BYTE* data, DWORD length)
{
    return ATL::CRegKey::SetBinaryValue(name.c_str(), data, length);
}

LONG CRegKey::DeleteValue(const string16& name)
{
    return ATL::CRegKey::DeleteValue(name.c_str());
}

LONG CRegKey::DeleteSubKey(const string16& key)
{
    return ATL::CRegKey::DeleteSubKey(key.c_str());
}

BOOL CRegKey::HasValues() const
{
    TCHAR szName[512];
    DWORD nNameLen = sizeof(szName) / sizeof(TCHAR);
    return (::RegEnumValue(m_hKey, 0, szName, &nNameLen,
                           NULL, NULL, NULL, NULL) == ERROR_SUCCESS);
}

BOOL CRegKey::CopyRegistryKey(HKEY hkRootFrom, const string16& strFromPath, HKEY hkRootTo, const string16& strToPath)
{
    HKEY hkFrom;
    LONG res = ::RegOpenKeyEx(hkRootFrom, strFromPath.c_str(), 0, KEY_READ, &hkFrom);

    if (ERROR_SUCCESS != res) {
        return FALSE;
    }

    HKEY hkTo;
    res = ::RegCreateKeyEx(hkRootTo, strToPath.c_str(), 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &hkTo, 0);

    if (ERROR_SUCCESS != res) {
        ::RegCloseKey(hkFrom);
        return FALSE;
    }

    BOOL bRes = _CopyKeys(hkFrom, hkTo) && _CopyValues(hkFrom, hkTo);
    ::RegCloseKey(hkFrom);
    ::RegCloseKey(hkTo);
    return bRes;
}

BOOL CRegKey::_CopyKeys(HKEY hkFrom, HKEY hkTo)
{
    TCHAR lpstrName[nMaxKeyNameSize];
    TCHAR lpstrClass[nMaxClassSize];

    for (int i = 0;; i++) {
        DWORD nNameSize = nMaxKeyNameSize;
        DWORD nClassSize = nMaxClassSize;
        LONG res = ::RegEnumKeyEx(hkFrom, i, lpstrName, &nNameSize, 0, lpstrClass, &nClassSize, 0);

        if (ERROR_NO_MORE_ITEMS == res) {
            break;
        }

        HKEY hkNew;
        res = ::RegCreateKeyEx(hkTo, lpstrName, 0, lpstrClass, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &hkNew, 0); ;

        if (ERROR_SUCCESS != res) {
            return FALSE;
        }

        ::RegCloseKey(hkNew);
        BOOL bRes = CopyRegistryKey(hkFrom, lpstrName, hkTo, lpstrName);

        if (! bRes) {
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CRegKey::_CopyValues(HKEY hkFrom, HKEY hkTo)
{
    TCHAR lpstrName[nMaxValueNameSize];
    BYTE pValue[nMaxValueValueSize];

    for (int i = 0;; i++) {
        DWORD nType;
        DWORD nNameSize = nMaxValueNameSize;
        DWORD nValueSize = nMaxValueValueSize;
        LONG res = ::RegEnumValue(hkFrom, i, lpstrName, &nNameSize, 0, &nType, pValue, &nValueSize);

        if (ERROR_NO_MORE_ITEMS == res) {
            break;
        }

        res = ::RegSetValueEx(hkTo, lpstrName, 0, nType, pValue, nValueSize);

        if (ERROR_SUCCESS != res) {
            return FALSE;
        }
    }

    return TRUE;
}

void CRegKey::ExportRegistry(const string16& rootkey, const string16& key, const string16& filename)
{
    FILE *fp;
    HKEY hKeyRootName;
    string16 fullpath;

    if (rootkey == _T("HKEY_CLASSES_ROOT"))
        hKeyRootName = HKEY_CLASSES_ROOT;
    else if (rootkey == _T("HKEY_CURRENT_USER"))
        hKeyRootName = HKEY_CURRENT_USER;
    else if (rootkey == _T("HKEY_LOCAL_MACHINE"))
        hKeyRootName = HKEY_LOCAL_MACHINE;
    else if (rootkey == _T("HKEY_USERS"))
        hKeyRootName = HKEY_USERS;
    else if (rootkey == _T("HKEY_PERFORMANCE_DATA"))
        hKeyRootName = HKEY_PERFORMANCE_DATA;
    else if (rootkey == _T("HKEY_CURRENT_CONFIG"))
        hKeyRootName = HKEY_CURRENT_CONFIG;
    else if (rootkey == _T("HKEY_DYN_DATA"))
        hKeyRootName = HKEY_DYN_DATA;

    fp = _wfopen(filename.c_str(), L"w+");

    if (fp == NULL) {
        MessageBox(NULL, _T("Error while creating the file"), _T("Registry export"), MB_OK);
        return;
    }

    if (key.empty())
        fullpath = rootkey;
    else
        fullpath = rootkey + _T("\\") + key;

    //// First print the header ..... this may be different for some version of Windows... do manually change if required
    //Here need to add version check
    DWORD dwVersion = GetVersion();

    // Get build numbers for Windows NT or Win32s
    if (dwVersion < 0x80000000) {              // Windows NT
        _ftprintf(fp, _T("%s\n"), _T("Windows Registry Editor Version 5.00"));
    } else {    // Win32s
        _ftprintf(fp, _T("%s\n"), _T("REGEDIT4"));
    }

    _EnumerateValues(hKeyRootName, key, fp, fullpath);
    _EnumerateKey(hKeyRootName, key , fp , fullpath);
    fclose(fp);
}

void CRegKey::_EnumerateValues(HKEY hkey, const string16& key, FILE *fp, const string16& fullpath)
{
    static HKEY hLastKey = hkey;
    LONG lResult;
    DWORD dwIndex = 0;
    HKEY hCurKey = hkey;
    DWORD dwKeyType;
    DWORD dwKeyDataLength, dwKeyNameLen;
    LPBYTE pbbinKeyData = NULL;
    TCHAR *tcKeyName = NULL;
    TCHAR tcDataType[1024] = _T("");
    lResult = RegOpenKeyEx(hCurKey, key.c_str(), 0, KEY_QUERY_VALUE , &hkey);

    if (lResult != ERROR_SUCCESS)
        return;

    DWORD lNoOfValues = 0;
    DWORD lLongestKeyNameLen = 1;
    DWORD lLongestDataLen = 1;
    lResult = RegQueryInfoKey(hkey, NULL, NULL, NULL, NULL, NULL, NULL, &lNoOfValues, &lLongestKeyNameLen, &lLongestDataLen,
                              NULL, NULL);

    if (lResult != ERROR_SUCCESS)
        return;

    _ftprintf(fp, _T("\n[%s]\n"), fullpath.c_str());
    hLastKey = hkey;
    lLongestKeyNameLen++;
    lLongestDataLen++;
    tcKeyName = new TCHAR[lLongestKeyNameLen];
    pbbinKeyData = new BYTE[lLongestDataLen];
    CString  cstrTemp;
    string16 cstrFinalData;

    while (TRUE) {
        memset(pbbinKeyData, 0, lLongestDataLen);
        memset(tcKeyName, 0, lLongestKeyNameLen);
        dwKeyType = dwKeyDataLength = dwKeyNameLen = 0;
        dwKeyNameLen = lLongestKeyNameLen;
        dwKeyDataLength = lLongestDataLen;
        lResult = RegEnumValue(hkey, dwIndex, tcKeyName, &dwKeyNameLen, NULL, &dwKeyType, pbbinKeyData, &dwKeyDataLength);

        if (lResult == ERROR_NO_MORE_ITEMS)
            break;

        _FormatDataWithDataType(dwKeyType, pbbinKeyData, dwKeyDataLength, cstrFinalData);
        //// For (default) key names the tcKeyName is empty and dwKeyNameLen is zero ...in such case we need to
        //// have assignment like @ = "value"
        CString cstrTest;
        cstrTest = tcKeyName;

        if (cstrTest.IsEmpty()) {
            cstrTemp.Format(_T("@="));
        } else {
            cstrTemp.Format(_T("\"%s\"="), tcKeyName);
        }

        cstrTemp += cstrFinalData.c_str();
        _ftprintf(fp, _T("%s"), (LPCTSTR)cstrTemp);
        dwIndex++;
    }

    RegCloseKey(hkey);
    delete tcKeyName;
    delete[] pbbinKeyData;
}

void CRegKey::_EnumerateKey(HKEY hkey, const string16& key, FILE *fp, const string16& fullpath)
{
    TCHAR strCurString[1024];
    long lResult;
    DWORD dwCurIndex = 0;
    HKEY hCurKey;
    CString currentKey;
    string16 subKey = key;
    string16 fullKey = fullpath;
    //// first open the root key to get the handle...
    lResult = RegOpenKeyEx(hkey, key.c_str(), 0, KEY_ENUMERATE_SUB_KEYS, &hCurKey);

    if (lResult != ERROR_SUCCESS)
        return;

    do {
        lResult = RegEnumKey(hCurKey, dwCurIndex, strCurString , sizeof(strCurString));

        if ((lResult == ERROR_NO_MORE_ITEMS) || (lResult == ERROR_INVALID_HANDLE)) {
            break;
        } else {
            currentKey.Format(_T("%s"), strCurString);

            if (currentKey.IsEmpty())
                fullKey = fullpath;
            else {
                fullKey = fullpath + _T("\\") + (LPCTSTR)currentKey;
                subKey  = key + _T("\\") + strCurString;
            }

            _EnumerateValues(hkey, subKey , fp, fullKey);

            if (_IsSubKey(hkey , subKey)) {
                _EnumerateKey(hkey , subKey , fp, fullKey);
            }

            dwCurIndex++;
        }
    } while (TRUE);

    RegCloseKey(hCurKey);
}

BOOL CRegKey::_IsSubKey(HKEY hkey, const string16& key)
{
    if (_IsExist(hkey, key.c_str())) {
        HKEY phkResult;

        if (ERROR_SUCCESS == RegOpenKeyEx(hkey, key.c_str(), 0,
                                          KEY_READ, &phkResult)) {
            TCHAR sz[MAX_PATH];

            if (ERROR_SUCCESS == RegEnumKey(phkResult, 0, sz, MAX_PATH)) {
                RegCloseKey(phkResult);
                return TRUE;
            }

            RegCloseKey(phkResult);
        }
    }

    return FALSE;
}

BOOL CRegKey::_IsExist(HKEY hkey, const string16& key)
{
    HKEY phkResult;

    if (ERROR_SUCCESS == RegOpenKeyEx(hkey, key.c_str(), 0,
                                      KEY_EXECUTE, &phkResult)) {
        RegCloseKey(phkResult);
        return TRUE;
    }

    return FALSE;
}

void CRegKey::_FormatDataWithDataType(DWORD dwKeyType, LPBYTE pbbinKeyData, DWORD dwKeyDataLength, string16 &output)
{
    CString cstrTemp, cstrTemp1 , cstrTemp2, cstrOutput;
    int nIndex = 0;

    switch (dwKeyType) {
    case REG_SZ: {
        cstrTemp.Format(_T("\"%s\"\n"), pbbinKeyData);

        for (int i = 0; i < cstrTemp.GetLength(); i++) {
            cstrTemp1 = cstrTemp.Mid(i, 1);

            if (cstrTemp.Mid(i, 1) == "\\") {
                cstrTemp1 = cstrTemp.Left(i);
                cstrTemp2 = cstrTemp.Right(cstrTemp.GetLength() - i - 1);
                cstrTemp = cstrTemp1 + _T("\\\\") + cstrTemp2;
                i++;
            }
        }

        cstrOutput = cstrTemp;
        break;
    }

    case REG_DWORD: { /// same is for REG_DWORD_LITTLE_ENDIAN
        DWORD dwValue;
        memcpy(&dwValue, pbbinKeyData, sizeof DWORD);
        cstrTemp.Format(_T("dword:%08x\n"), dwValue);
        cstrOutput = cstrTemp;
        break;
    }

    case REG_BINARY:
    case REG_MULTI_SZ:
    case REG_EXPAND_SZ:
    case REG_FULL_RESOURCE_DESCRIPTOR:
    case REG_RESOURCE_LIST:
    case REG_RESOURCE_REQUIREMENTS_LIST: {
        if (dwKeyType != REG_BINARY)
            cstrOutput.Format(_T("hex(%d):"), dwKeyType);
        else
            cstrOutput.Format(_T("hex:"));

        for (DWORD dwIndex = 0; dwIndex < dwKeyDataLength; dwIndex++) {
            cstrTemp1.Format(_T("%02x"), pbbinKeyData[dwIndex]);

            if (dwIndex != 0 && (dwIndex % 0x15 == 0)) {
                cstrTemp += _T(",\\\n");
                cstrTemp += cstrTemp1;
            } else {
                if (cstrTemp.IsEmpty())
                    cstrTemp = cstrTemp1;
                else
                    cstrTemp += _T(",") + cstrTemp1;
            }
        }

        cstrTemp += _T("\n");
        cstrOutput += cstrTemp;
        break;
    }

    case REG_NONE:
    case REG_DWORD_BIG_ENDIAN:
    case REG_LINK:
        //// TODO : add code for these types...
        break;
    }

    output = (LPCTSTR)cstrOutput;
}

void CRegKey::ShellExportRegister(const string16& export_str, const string16& filename)
{
    string16 parameters;
    parameters = L"/e \"" + filename + L"\" \"" + export_str + L"\"";
    ShellExecute(0, L"open", L"regedit.exe",
                 parameters.c_str(), NULL, SW_SHOWNORMAL);
}

void CRegKey::ShellImportRegister(const string16& filename)
{
    string16 parameters;
    parameters = L"/s \"" + filename + L"\"";
    ShellExecute(NULL, L"open", L"regedit.exe", parameters.c_str(), NULL, SW_HIDE);
}

HKEY CRegKey::handleFor(const string16& rootKey)
{
    if (rootKey == L"HKEY_CLASSES_ROOT")
        return HKEY_CLASSES_ROOT;
    else if (rootKey == L"HKEY_CURRENT_CONFIG")
        return HKEY_CURRENT_CONFIG;
    else if (rootKey == L"HKEY_CURRENT_USER")
        return HKEY_CURRENT_USER;
    else if (rootKey == L"HKEY_LOCAL_MACHINE")
        return HKEY_LOCAL_MACHINE;
    else if (rootKey == L"HKEY_USERS")
        return HKEY_USERS;
    else if (rootKey == L"HKEY_PERFORMANCE_DATA")
        return HKEY_PERFORMANCE_DATA;
}



BOOL CRegKey::_split_key(const string16& fullkey, HKEY& hkey, string16& subkey)
{
    string16 rootkey;
    string16::size_type pos = fullkey.find(L'\\');

    if (pos != string16::npos) {
        rootkey = fullkey.substr(0, pos);
        hkey = handleFor(rootkey);
        subkey   = fullkey.substr(pos + 1);
        return TRUE;
    }

    return FALSE;
}

BOOL CRegKey::ReadRegMString(HKEY hkey, const string16& key, const string16& name, CSimpleArray<string16>& ms)
{
    DWORD dwBuffer = nMaxValueValueSize;
    DWORD dwType = REG_MULTI_SZ;
    TCHAR *pbuf = new TCHAR[nMaxValueValueSize];

    if (pbuf && ERROR_SUCCESS == SHGetValue(hkey, key.c_str(), name.c_str(), &dwType, pbuf, &dwBuffer)) {
        size_t nCur = 0, nPrev = 0, nShortLen = dwBuffer / sizeof(TCHAR);

        if (nShortLen > 2) {
            if (*(pbuf + nShortLen - 1) == '\0')
                nShortLen--;

            while ((nCur = (int)(_tcschr(pbuf + nPrev, '\0') - pbuf)) < nShortLen) {
                ms.Add(pbuf + nPrev);
                nPrev = nCur + 1;
            }
        }

        return TRUE;
    }

    return FALSE;
}

BOOL CRegKey::WriteRegMString(HKEY hkey, const string16& key, const string16& name, const CSimpleArray<string16> &ms)
{
    INT nLen = 0;

    for (int i = 0; i < ms.GetSize(); ++i) {
        nLen += ms[i].size() + 1;
    }

    nLen += (nLen == 0 ? 2 : 1);
    TCHAR *pbuf = new TCHAR[nLen];

    if (!pbuf)
        return FALSE;

    ZeroMemory(pbuf, sizeof(TCHAR)*nLen);
    TCHAR *p = pbuf;

    for (int i = 0; i < ms.GetSize(); ++i) {
        const string16 &str = ms[i];
        _tcscpy(p, str.c_str());
        p += str.size() + 1;
    }

    BOOL bRet = ERROR_SUCCESS == SHSetValue(hkey, key.c_str(), name.c_str(), REG_MULTI_SZ, pbuf, nLen * sizeof(TCHAR));
    delete pbuf;
    return bRet;
}









} // namespace cactus





#include <cactus/cactus_file_version.h>
#include <cactus/cactus_string.h>

#pragma comment(lib, "version.lib")

namespace cactus
{


BOOL GetPEFileVersion(const string16& filename, ULARGE_INTEGER& lgVersion)
{
    if (!PathFileExists(filename.c_str()))
        return FALSE;

    BOOL bResult = FALSE, bRet = FALSE;
    DWORD dwHandle          = 0;
    DWORD dwFileInfoSize    = 0;
    VS_FIXEDFILEINFO *pFixFileInfo = NULL;
    UINT uLen               = 0;
    lgVersion.QuadPart = 0;

    do {
        dwFileInfoSize = ::GetFileVersionInfoSize(filename.c_str(), &dwHandle);

        if (0 == dwFileInfoSize)
            break;

        std::vector<BYTE> buffer(dwFileInfoSize);
        bRet = ::GetFileVersionInfo(filename.c_str(), dwHandle, dwFileInfoSize, &buffer[0]);

        if (!bRet)
            break;

        bRet = ::VerQueryValue(&buffer[0], _T("\\"), (LPVOID *)&pFixFileInfo, &uLen);

        if (!bRet)
            break;

        if (uLen > 0) {
            lgVersion.HighPart = pFixFileInfo->dwFileVersionMS;
            lgVersion.LowPart = pFixFileInfo->dwFileVersionLS;
        }

        bResult = TRUE;
    } while (0);

    return bResult;
}

BOOL GetFileVersion(const string16& filename, ULARGE_INTEGER& lgVersion)
{
    return GetPEFileVersion(filename, lgVersion);
}

BOOL GetFileVersion(const string16& filename, string16& version)
{
    ULARGE_INTEGER llver;

    if (GetFileVersion(filename, llver)) {
        version = GetFileVersionString(llver);
        return TRUE;
    }

    return FALSE;
}

string16 GetFileVersionString(ULARGE_INTEGER ullVersion)
{
    string16 version = format_string(L"%u.%u.%u.%u",
                                     HIWORD(ullVersion.HighPart), LOWORD(ullVersion.HighPart),
                                     HIWORD(ullVersion.LowPart), LOWORD(ullVersion.LowPart)
                                    );
    /*

        strVersion.Format(
        _T("%d.%d.%d.%d"),
        HIWORD(ullVersion.HighPart), LOWORD(ullVersion.HighPart),
        HIWORD(ullVersion.LowPart), LOWORD(ullVersion.LowPart)
        );
    */
    return version;
}

ULARGE_INTEGER GetDigitalVersionByString(const string16& version)
{
    ULARGE_INTEGER ullRet = {0};
    CString strVersion = version.c_str();
    int nPos[4] = {0, 0, 0, 0};

    for (int i = 1; i < 4; i ++) {
        nPos[i] = strVersion.Find(_T('.'), nPos[i - 1]) + 1;
    }

    ullRet.HighPart = (DWORD)MAKELONG(::StrToInt(strVersion.Mid(nPos[1])), ::StrToInt(strVersion));
    ullRet.LowPart = (DWORD)MAKELONG(::StrToInt(strVersion.Mid(nPos[3])), ::StrToInt(strVersion.Mid(nPos[2])));
    return ullRet;
}

BOOL VersionLess(const string16& version1, const string16& version2)
{
    ULARGE_INTEGER ullVerLeft  = GetDigitalVersionByString(version1);
    ULARGE_INTEGER ullVerRight = GetDigitalVersionByString(version2);
    return (ullVerLeft.QuadPart < ullVerRight.QuadPart);
}



}


namespace cactus
{
//-------------------------------------------------------------------
// CFileVersionInfo
//-------------------------------------------------------------------

CFileVersionInfo::CFileVersionInfo()
{
    Reset();
}


CFileVersionInfo::~CFileVersionInfo()
{
}


BOOL CFileVersionInfo::GetTranslationId(LPVOID lpData, UINT unBlockSize, WORD wLangId, DWORD &dwId, BOOL bPrimaryEnough/*= FALSE*/)
{
    LPWORD lpwData;

    for (lpwData = (LPWORD)lpData; (LPBYTE)lpwData < ((LPBYTE)lpData) + unBlockSize; lpwData += 2) {
        if (*lpwData == wLangId) {
            dwId = *((DWORD*)lpwData);
            return TRUE;
        }
    }

    if (!bPrimaryEnough)
        return FALSE;

    for (lpwData = (LPWORD)lpData; (LPBYTE)lpwData < ((LPBYTE)lpData) + unBlockSize; lpwData += 2) {
        if (((*lpwData) & 0x00FF) == (wLangId & 0x00FF)) {
            dwId = *((DWORD*)lpwData);
            return TRUE;
        }
    }

    return FALSE;
}


BOOL CFileVersionInfo::Create(HMODULE hModule /*= NULL*/)
{
    CString strPath;
    GetModuleFileName(hModule, strPath.GetBuffer(_MAX_PATH + 1), _MAX_PATH);
    strPath.ReleaseBuffer();
    return Create(strPath);
}


BOOL CFileVersionInfo::Create(LPCTSTR lpszFileName)
{
    Reset();
    DWORD   dwHandle;
    DWORD   dwFileVersionInfoSize = GetFileVersionInfoSize((LPTSTR)lpszFileName, &dwHandle);

    if (!dwFileVersionInfoSize)
        return FALSE;

    LPVOID  lpData = (LPVOID)new BYTE[dwFileVersionInfoSize];

    if (!lpData)
        return FALSE;

    try {
        if (!GetFileVersionInfo((LPTSTR)lpszFileName, dwHandle, dwFileVersionInfoSize, lpData))
            throw FALSE;

        // catch default information
        LPVOID  lpInfo;
        UINT        unInfoLen;

        if (VerQueryValue(lpData, _T("\\"), &lpInfo, &unInfoLen)) {
            ASSERT(unInfoLen == sizeof(m_FileInfo));

            if (unInfoLen == sizeof(m_FileInfo))
                memcpy(&m_FileInfo, lpInfo, unInfoLen);
        }

        // find best matching language and codepage
        VerQueryValue(lpData, _T("\\VarFileInfo\\Translation"), &lpInfo, &unInfoLen);
        DWORD   dwLangCode = 0;

        if (!GetTranslationId(lpInfo, unInfoLen, GetUserDefaultLangID(), dwLangCode, FALSE)) {
            if (!GetTranslationId(lpInfo, unInfoLen, GetUserDefaultLangID(), dwLangCode, TRUE)) {
                if (!GetTranslationId(lpInfo, unInfoLen, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), dwLangCode, TRUE)) {
                    if (!GetTranslationId(lpInfo, unInfoLen, MAKELANGID(LANG_ENGLISH, SUBLANG_NEUTRAL), dwLangCode, TRUE))
                        // use the first one we can get
                        dwLangCode = *((DWORD*)lpInfo);
                }
            }
        }

        CString strSubBlock;
        strSubBlock.Format(_T("\\StringFileInfo\\%04X%04X\\"), dwLangCode & 0x0000FFFF, (dwLangCode & 0xFFFF0000) >> 16);

        // catch string table
        if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock + _T("CompanyName")), &lpInfo, &unInfoLen))
            m_strCompanyName = (LPCTSTR)lpInfo;

        if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock + _T("FileDescription")), &lpInfo, &unInfoLen))
            m_strFileDescription = (LPCTSTR)lpInfo;

        if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock + _T("FileVersion")), &lpInfo, &unInfoLen)) {
            m_strFileVersion = (LPCTSTR)lpInfo;
            m_strFileVersion.Replace(_T(" "), _T("")); // remove spaces
            m_strFileVersion.Replace(_T(","), _T(".")); // replace commas with periods
        }

        if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock + _T("InternalName")), &lpInfo, &unInfoLen))
            m_strInternalName = (LPCTSTR)lpInfo;

        if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock + _T("LegalCopyright")), &lpInfo, &unInfoLen))
            m_strLegalCopyright = (LPCTSTR)lpInfo;

        if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock + _T("OriginalFileName")), &lpInfo, &unInfoLen))
            m_strOriginalFileName = (LPCTSTR)lpInfo;

        if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock + _T("ProductName")), &lpInfo, &unInfoLen))
            m_strProductName = (LPCTSTR)lpInfo;

        if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock + _T("ProductVersion")), &lpInfo, &unInfoLen)) {
            m_strProductVersion = (LPCTSTR)lpInfo;
            m_strProductVersion.Replace(_T(" "), _T("")); // remove spaces
            m_strProductVersion.Replace(_T(","), _T(".")); // replace commas with periods
        }

        if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock + _T("Comments")), &lpInfo, &unInfoLen))
            m_strComments = (LPCTSTR)lpInfo;

        if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock + _T("LegalTrademarks")), &lpInfo, &unInfoLen))
            m_strLegalTrademarks = (LPCTSTR)lpInfo;

        if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock + _T("PrivateBuild")), &lpInfo, &unInfoLen))
            m_strPrivateBuild = (LPCTSTR)lpInfo;

        if (VerQueryValue(lpData, (LPTSTR)(LPCTSTR)(strSubBlock + _T("SpecialBuild")), &lpInfo, &unInfoLen))
            m_strSpecialBuild = (LPCTSTR)lpInfo;

        delete[] lpData;
    } catch (BOOL) {
        delete[] lpData;
        return FALSE;
    }

    return TRUE;
}


WORD CFileVersionInfo::GetFileVersion(int nIndex) const
{
    if (nIndex == 0)
        return (WORD)(m_FileInfo.dwFileVersionLS & 0x0000FFFF);
    else if (nIndex == 1)
        return (WORD)((m_FileInfo.dwFileVersionLS & 0xFFFF0000) >> 16);
    else if (nIndex == 2)
        return (WORD)(m_FileInfo.dwFileVersionMS & 0x0000FFFF);
    else if (nIndex == 3)
        return (WORD)((m_FileInfo.dwFileVersionMS & 0xFFFF0000) >> 16);
    else
        return 0;
}


WORD CFileVersionInfo::GetProductVersion(int nIndex) const
{
    if (nIndex == 0)
        return (WORD)(m_FileInfo.dwProductVersionLS & 0x0000FFFF);
    else if (nIndex == 1)
        return (WORD)((m_FileInfo.dwProductVersionLS & 0xFFFF0000) >> 16);
    else if (nIndex == 2)
        return (WORD)(m_FileInfo.dwProductVersionMS & 0x0000FFFF);
    else if (nIndex == 3)
        return (WORD)((m_FileInfo.dwProductVersionMS & 0xFFFF0000) >> 16);
    else
        return 0;
}


DWORD CFileVersionInfo::GetFileFlagsMask() const
{
    return m_FileInfo.dwFileFlagsMask;
}


DWORD CFileVersionInfo::GetFileFlags() const
{
    return m_FileInfo.dwFileFlags;
}


DWORD CFileVersionInfo::GetFileOs() const
{
    return m_FileInfo.dwFileOS;
}


DWORD CFileVersionInfo::GetFileType() const
{
    return m_FileInfo.dwFileType;
}


DWORD CFileVersionInfo::GetFileSubtype() const
{
    return m_FileInfo.dwFileSubtype;
}


CTime CFileVersionInfo::GetFileDate() const
{
    FILETIME    ft;
    ft.dwLowDateTime = m_FileInfo.dwFileDateLS;
    ft.dwHighDateTime = m_FileInfo.dwFileDateMS;
    return CTime(ft);
}


CString CFileVersionInfo::GetCompanyName() const
{
    return m_strCompanyName;
}


CString CFileVersionInfo::GetFileDescription() const
{
    return m_strFileDescription;
}


CString CFileVersionInfo::GetFileVersion() const
{
    return m_strFileVersion;
}


CString CFileVersionInfo::GetInternalName() const
{
    return m_strInternalName;
}


CString CFileVersionInfo::GetLegalCopyright() const
{
    return m_strLegalCopyright;
}


CString CFileVersionInfo::GetOriginalFileName() const
{
    return m_strOriginalFileName;
}


CString CFileVersionInfo::GetProductName() const
{
    return m_strProductName;
}


CString CFileVersionInfo::GetProductVersion() const
{
    return m_strProductVersion;
}


CString CFileVersionInfo::GetComments() const
{
    return m_strComments;
}


CString CFileVersionInfo::GetLegalTrademarks() const
{
    return m_strLegalTrademarks;
}


CString CFileVersionInfo::GetPrivateBuild() const
{
    return m_strPrivateBuild;
}


CString CFileVersionInfo::GetSpecialBuild() const
{
    return m_strSpecialBuild;
}


void CFileVersionInfo::Reset()
{
    ZeroMemory(&m_FileInfo, sizeof(m_FileInfo));
    m_strCompanyName.Empty();
    m_strFileDescription.Empty();
    m_strFileVersion.Empty();
    m_strInternalName.Empty();
    m_strLegalCopyright.Empty();
    m_strOriginalFileName.Empty();
    m_strProductName.Empty();
    m_strProductVersion.Empty();
    m_strComments.Empty();
    m_strLegalTrademarks.Empty();
    m_strPrivateBuild.Empty();
    m_strSpecialBuild.Empty();
}

}


namespace cactus
{
////////////////////////////////////////////////////////////////
// 1998 Microsoft Systems Journal
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// CModuleVersion provides an easy way to get version info
// for a module.(DLL or EXE).
//
CModuleVersion::CModuleVersion()
{
}

//////////////////
// Destroy: delete version info
//
CModuleVersion::~CModuleVersion()
{
}

//////////////////
// Get file version info for a given module
// Allocates storage for all info, fills "this" with
// VS_FIXEDFILEINFO, and sets codepage.
//
BOOL CModuleVersion::GetFileVersionInfo(LPCTSTR modulename)
{
    m_translation.charset = 1252;    // default = ANSI code page
    memset((VS_FIXEDFILEINFO*)this, 0, sizeof(VS_FIXEDFILEINFO));
    // get module handle
    TCHAR filename[_MAX_PATH];
    HMODULE hModule = ::GetModuleHandle(modulename);

    if (hModule == NULL && modulename != NULL)
        return FALSE;

    // get module file name
    DWORD len = GetModuleFileName(hModule, filename, sizeof(filename) / sizeof(filename[0]));

    if (len <= 0)
        return FALSE;

    // read file version info
    DWORD dwDummyHandle; // will always be set to zero
    len = GetFileVersionInfoSize(filename, &dwDummyHandle);

    if (len <= 0)
        return FALSE;

    BYTE* pbData = new BYTE[len]; // allocate version info

    if (!pbData)
        return FALSE;

    m_atpVersionInfo.Attach(pbData);

    if (!::GetFileVersionInfo(filename, 0, len, m_atpVersionInfo))
        return FALSE;

    LPVOID lpvi;
    UINT iLen;

    if (!VerQueryValue(m_atpVersionInfo, _T("\\"), &lpvi, &iLen))
        return FALSE;

    // copy fixed info to myself, which am derived from VS_FIXEDFILEINFO
    *(VS_FIXEDFILEINFO*)this = *(VS_FIXEDFILEINFO*)lpvi;

    // Get translation info
    if (VerQueryValue(m_atpVersionInfo,
                      _T("\\VarFileInfo\\Translation"), &lpvi, &iLen) && iLen >= 4) {
        m_translation = *(TRANSLATION*)lpvi;
    }

    return dwSignature == VS_FFI_SIGNATURE;
}

BOOL CModuleVersion::GetDLLVersionInfo(LPCTSTR filename)
{
    m_translation.charset = 1252;    // default = ANSI code page
    memset((VS_FIXEDFILEINFO*)this, 0, sizeof(VS_FIXEDFILEINFO));
    // get module handle
    //HMODULE hModule = LoadLibrary(filename);
    //if (hModule==NULL)
    //    return FALSE;
    // some times, LoadLibrary may fail , --- by bbcallen
    HMODULE hModule = LoadLibraryEx(filename, NULL, LOAD_LIBRARY_AS_DATAFILE);

    if (hModule == NULL)
        return FALSE;

    // read file version info
    DWORD dwDummyHandle; // will always be set to zero
    DWORD len = GetFileVersionInfoSize(filename, &dwDummyHandle);

    if (len <= 0) {
        FreeLibrary(hModule);
        return FALSE;
    }

    BYTE* pbData = new BYTE[len]; // allocate version info

    if (!pbData) {
        FreeLibrary(hModule);
        return FALSE;
    }

    m_atpVersionInfo.Attach(pbData);

    if (!::GetFileVersionInfo(filename, 0, len, m_atpVersionInfo)) {
        FreeLibrary(hModule);
        return FALSE;
    }

    LPVOID lpvi;
    UINT iLen;

    if (!VerQueryValue(m_atpVersionInfo, _T("\\"), &lpvi, &iLen)) {
        FreeLibrary(hModule);
        return FALSE;
    }

    // copy fixed info to myself, which am derived from VS_FIXEDFILEINFO
    *(VS_FIXEDFILEINFO*)this = *(VS_FIXEDFILEINFO*)lpvi;

    // Get translation info
    if (VerQueryValue(m_atpVersionInfo,
                      _T("\\VarFileInfo\\Translation"), &lpvi, &iLen) && iLen >= 4) {
        m_translation = *(TRANSLATION*)lpvi;
    }

    FreeLibrary(hModule);
    return dwSignature == VS_FFI_SIGNATURE;
}

//////////////////
// Get string file info.
// Key name is something like "CompanyName".
// returns the value as a CString.
//
CString CModuleVersion::GetValue(LPCTSTR lpKeyName)
{
    CString sVal;

    if (m_atpVersionInfo) {
        // To get a string value must pass query in the form
        //
        //    "\StringFileInfo\<langID><codepage>\keyname"
        //
        // where <langID><codepage> is the languageID concatenated with the
        // code page, in hex. Wow.
        //
        CString query;
        query.Format(_T("\\StringFileInfo\\%04x%04x\\%s"),
                     m_translation.langID,
                     m_translation.charset,
                     lpKeyName);
        LPCTSTR pVal;
        UINT iLenVal;

        if (VerQueryValue(m_atpVersionInfo, (LPTSTR)(LPCTSTR)query,
                          (LPVOID*)&pVal, &iLenVal)) {
            sVal = pVal;
        }
    }

    return sVal;
}

}
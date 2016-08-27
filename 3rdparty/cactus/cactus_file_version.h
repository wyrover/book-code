#ifndef __CACTUS_CACTUS_FILE_VERSION_H__
#define __CACTUS_CACTUS_FILE_VERSION_H__

#include <cactus/cactus.h>
#include <cactus/cactus_types.h>
#include <atltime.h>


namespace cactus
{
BOOL CACTUS_API GetPEFileVersion(const string16& filename, ULARGE_INTEGER& lgVersion);

BOOL CACTUS_API GetFileVersion(const string16& filename, ULARGE_INTEGER& lgVersion);

BOOL CACTUS_API GetFileVersion(const string16& filename, string16& version);

string16 CACTUS_API GetFileVersionString(ULARGE_INTEGER ullVersion);

ULARGE_INTEGER CACTUS_API GetDigitalVersionByString(const string16& version);

BOOL CACTUS_API VersionLess(const string16& version1, const string16& version2);
}


namespace cactus
{
class CACTUS_API CFileVersionInfo
{
    // construction/destruction
public:
    CFileVersionInfo();
    virtual ~CFileVersionInfo();

    // operations
public:
    BOOL Create(HMODULE hModule = NULL);
    BOOL Create(LPCTSTR lpszFileName);

    // attribute operations
public:
    WORD GetFileVersion(int nIndex) const;
    WORD GetProductVersion(int nIndex) const;
    DWORD GetFileFlagsMask() const;
    DWORD GetFileFlags() const;
    DWORD GetFileOs() const;
    DWORD GetFileType() const;
    DWORD GetFileSubtype() const;
    CTime GetFileDate() const;

    CString GetCompanyName() const;
    CString GetFileDescription() const;
    CString GetFileVersion() const;
    CString GetInternalName() const;
    CString GetLegalCopyright() const;
    CString GetOriginalFileName() const;
    CString GetProductName() const;
    CString GetProductVersion() const;
    CString GetComments() const;
    CString GetLegalTrademarks() const;
    CString GetPrivateBuild() const;
    CString GetSpecialBuild() const;

    // implementation helpers
protected:
    virtual void Reset();
    BOOL GetTranslationId(LPVOID lpData, UINT unBlockSize, WORD wLangId, DWORD &dwId, BOOL bPrimaryEnough = FALSE);

    // attributes
private:
    VS_FIXEDFILEINFO m_FileInfo;

    CString m_strCompanyName;
    CString m_strFileDescription;
    CString m_strFileVersion;
    CString m_strInternalName;
    CString m_strLegalCopyright;
    CString m_strOriginalFileName;
    CString m_strProductName;
    CString m_strProductVersion;
    CString m_strComments;
    CString m_strLegalTrademarks;
    CString m_strPrivateBuild;
    CString m_strSpecialBuild;
};
}


namespace cactus
{
////////////////////////////////////////////////////////////////
// 1998 Microsoft Systems Journal
//
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//

//////////////////
// CModuleVersion version info about a module.
// To use:
//
// CModuleVersion ver
// if (ver.GetFileVersionInfo("_T("mymodule))) {
//    // info is in ver, you can call GetValue to get variable info like
//    CString s = ver.GetValue(_T("CompanyName"));
// }
//
// You can also call the static fn DllGetVersion to get DLLVERSIONINFO.
//
class CModuleVersion : public VS_FIXEDFILEINFO
{
protected:
    CAutoVectorPtr<BYTE> m_atpVersionInfo;   // all version info

    struct TRANSLATION {
        WORD langID;         // language ID
        WORD charset;        // character set (code page)
    } m_translation;

public:
    CModuleVersion();
    virtual ~CModuleVersion();

    BOOL        GetFileVersionInfo(LPCTSTR modulename);
    BOOL        GetDLLVersionInfo(LPCTSTR filename);
    CString     GetValue(LPCTSTR lpKeyName);
};
}


#endif // __CACTUS_CACTUS_FILE_VERSION_H__

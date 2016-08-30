
#include <comdef.h>


class CEnumeration
{
protected:
    bool    m_IsDir;
    _bstr_t m_Path;

public:
    CEnumeration()              {}
    bool                            IsDir() const
    {
        return m_IsDir;
    }
    const _bstr_t               &   Path() const
    {
        return m_Path;
    }

    virtual                         ~CEnumeration()     {}
    virtual bool                    AllowRead(ACCESS_MASK mask) const = 0;
    virtual bool                    AllowWrite(ACCESS_MASK mask) const = 0;
    virtual CEnumeration        *   FindNext(DWORD * error) = 0;
    virtual PSECURITY_DESCRIPTOR    GetSecurity() const = 0;
};

class CFileEnumeration : public CEnumeration
{
    HANDLE                  m_hFind;
    const static    DWORD   READ_MASK;
    const static    DWORD   WRITE_MASK;
public:
    CFileEnumeration(const _bstr_t & path, bool IsDir)
    {
        m_Path = path;
        m_IsDir = IsDir;
        m_hFind = INVALID_HANDLE_VALUE;
    }
    ~CFileEnumeration()
    {
        if (m_hFind != INVALID_HANDLE_VALUE)
            ::FindClose(m_hFind);
    }

    CEnumeration * FindNext(DWORD * error)
    {
        WIN32_FIND_DATA info;
        *error = ERROR_SUCCESS;

        if (m_hFind == INVALID_HANDLE_VALUE)  {
            _bstr_t path = m_Path + _T("\\*");
            m_hFind = ::FindFirstFile(path, &info);

            if (m_hFind == INVALID_HANDLE_VALUE)  {
                if (GetLastError() == ERROR_NO_MORE_ITEMS)
                    *error = ERROR_SUCCESS;
                else
                    *error = GetLastError();

                return NULL;
            }
        } else {
            if (! ::FindNextFile(m_hFind, &info))  {
                return NULL;
            }
        }

        if (_tcscmp(info.cFileName, _T(".")) == 0  ||
            _tcscmp(info.cFileName, _T("..")) == 0) {
            return FindNext(error);
        }

        _bstr_t path = _tcschr((const TCHAR *)m_Path, 0)[-1] == '\\'
                       ? m_Path + info.cFileName
                       : m_Path + _T("\\") + info.cFileName;
        bool isDir = (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        return new CFileEnumeration(path, isDir);
    }


    PSECURITY_DESCRIPTOR GetSecurity() const
    {
        // Get security descriptor
        DWORD                   nb = 1000;
        PSECURITY_DESCRIPTOR    sd = (PSECURITY_DESCRIPTOR) new BYTE[ nb ];
        BOOL    ok = ::GetFileSecurity(m_Path,
                                       DACL_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION,
                                       sd, nb, &nb);

        if (!ok  &&  GetLastError() == ERROR_INSUFFICIENT_BUFFER)  {
            delete sd;
            sd = (PSECURITY_DESCRIPTOR) new BYTE[ nb ];
            ok = ::GetFileSecurity(m_Path, DACL_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION, sd, nb, &nb);
        }

        if (!ok)  {
            delete sd;
            sd = NULL;
        }

        return sd;
    }

    bool AllowRead(ACCESS_MASK mask) const
    {
        return (mask & READ_MASK) != 0;
    }

    bool AllowWrite(ACCESS_MASK mask) const
    {
        return (mask & WRITE_MASK) != 0;
    }
};


class CRegEnumeration : public CEnumeration
{
    const static    DWORD   READ_MASK;
    const static    DWORD   WRITE_MASK;
    HKEY                    m_hKey;
    int                     m_FindIndex;
public:
    static HKEY OpenPath(const TCHAR * path, const TCHAR ** fullname)
    {
        static const struct {
            const TCHAR *   Name;
            const TCHAR *   FullName;
            HKEY            Key;
        } RegKeys[] = {
            _T("HKLM"),     _T("HKEY_LOCAL_MACHINE"),   HKEY_LOCAL_MACHINE,
            _T("HKU"),      _T("HKEY_USERS"),           HKEY_USERS,
            _T("HKCU"),     _T("HKEY_CURRENT_USER"),    HKEY_CURRENT_USER,
            _T("HKCC"),     _T("HKEY_CURRENT_CONFIG"),  HKEY_CURRENT_CONFIG,
            _T("HKCR"),     _T("HKEY_CLASSES_ROOT"),    HKEY_CLASSES_ROOT,
        };
        const TCHAR * root;
        int i;

        for (i = 0; i < sizeof RegKeys / sizeof RegKeys[0]; ++i)  {
            root = RegKeys[i].Name;

            if (_tcsnicmp(path, root, _tcslen(root)) == 0)
                break;

            root = RegKeys[i].FullName;

            if (_tcsnicmp(path, root, _tcslen(root)) == 0)
                break;
        }

        HKEY hKey = NULL;

        if (i < sizeof RegKeys / sizeof RegKeys[0])  {
            int len = _tcslen(root);

            if (path[len] == '\\')
                ++len;

            ::RegOpenKeyEx(RegKeys[i].Key, path + len, 0, MAXIMUM_ALLOWED, &hKey);

            if (fullname)
                *fullname = RegKeys[i].FullName;
        }

        return hKey;
    }

    CRegEnumeration(const _bstr_t & path, bool IsDir)
    {
        m_hKey = OpenPath(path, NULL);
        m_Path = path;
        m_IsDir = true;
        m_FindIndex = 0;
    }
    ~CRegEnumeration()
    {
        FindClose();
    }

    CEnumeration * FindNext(DWORD * error)
    {
        TCHAR       Name[ MAX_PATH ];
        DWORD       cName = MAX_PATH;
        FILETIME    time;
        LONG status = ::RegEnumKeyEx(m_hKey, m_FindIndex++, Name, &cName, NULL, NULL, NULL, &time);

        if (status)  {
            if (status == ERROR_NO_MORE_ITEMS)
                *error = ERROR_SUCCESS;
            else
                *error = status;

            return NULL;
        }

        _bstr_t path = m_Path + _T("\\") + Name;
        *error = ERROR_SUCCESS;
        return new CRegEnumeration(path, true);
    }

    void FindClose()
    {
        RegCloseKey(m_hKey);
    }

    PSECURITY_DESCRIPTOR GetSecurity() const
    {
        // Get security descriptor
        DWORD                   nb = 1000;
        PSECURITY_DESCRIPTOR    sd = (PSECURITY_DESCRIPTOR) new BYTE[ nb ];
        LONG                    status = ::RegGetKeySecurity(m_hKey,
                                         DACL_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION,
                                         sd, &nb);

        if (status == ERROR_INSUFFICIENT_BUFFER)  {
            delete sd;
            sd = (PSECURITY_DESCRIPTOR) new BYTE[ nb ];
            status = ::RegGetKeySecurity(m_hKey, DACL_SECURITY_INFORMATION | OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION, sd, &nb);
        }

        if (status)  {
            delete sd;
            sd = NULL;
        }

        return sd;
    }

    bool AllowRead(ACCESS_MASK mask) const
    {
        return (mask & READ_MASK) != 0;
    }

    bool AllowWrite(ACCESS_MASK mask) const
    {
        return (mask & WRITE_MASK) != 0;
    }
};

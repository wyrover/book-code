#if !defined (_REGISTRYKEYS_H_)
#define _REGISTRYKEYS_H_

/* Function prototypes */
HKEY    getAppGlobalRegKey () ;
HKEY    getAppUserRegKey () ;
BOOL    registerFileExtension (LPCTSTR pszExt,
                               LPCTSTR pszProgID, int nMajorVersion) ;
BOOL    registerProgID (REFCLSID clsid, LPCTSTR pszProgID,
                        UINT uShortLongNameID, UINT uLongAppName,
                        int nIconIndex, int nMajorVersion) ;

#endif  /* _REGISTRYKEYS_H_ */

#if !defined (_VERSIONINFO_H_)
#define _VERSIONINFO_H_

/* Function prototypes */

LPVOID  getVersionInfoBlock () ;
LPCTSTR getVersionInfoValue (LPVOID pVerInfo, LPCTSTR pszName) ;

#endif      /* _VERSIONINFO_H_ */

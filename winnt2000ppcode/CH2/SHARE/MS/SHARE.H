/*--------------------------------------------------
      SHARE.H
  --------------------------------------------------*/

#ifdef __DLL__
# define DLLFUNC __declspec(dllexport)
#else
# define DLLFUNC __declspec(dllimport)
#endif

/* exported functions */
DLLFUNC BOOL AddString(PSTR pString);
DLLFUNC void ShowAllStrings(void);

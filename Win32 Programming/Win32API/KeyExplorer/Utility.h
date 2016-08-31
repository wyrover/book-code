#if !defined (_UTILITY_H_)
#define _UTILITY_H_


/////////////////////////////////////////////////////////////////////////////
// Utility functions

/* Function prototypes */

LPCTSTR getAppName () ;
LPCTSTR getHelpFileName () ;
BOOL    centerWindow (HWND hwndCentered, HWND hwndPreferredOwner) ;

#define DIM(a)  (sizeof (a) / sizeof (a[0])) 

/////////////////////////////////////////////////////////////////////////////
// Diagnostic functions

#ifdef _DEBUG

/* Function prototypes */

BOOL assertFailedOnLine (LPCSTR lpszFileName, int nLine);

#define THIS_FILE          __FILE__
#define ASSERT(f) \
	do { \
	  if (!(f) && assertFailedOnLine (THIS_FILE, __LINE__)) \
        FatalExit (0); \
	} while (0) \

#define VERIFY(f)          ASSERT(f)

#else   // _DEBUG

#define ASSERT(f)          ((void)0)
#define VERIFY(f)          ((void)(f))

#endif // !_DEBUG

#endif      /* _UTILITY_H_ */

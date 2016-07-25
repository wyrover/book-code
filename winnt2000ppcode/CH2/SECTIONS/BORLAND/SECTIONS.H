/*--------------------------------------------------
      SECTIONS.H
      Prototypes for functions exported from
      the Sections DLL.

      written by Brian G. Myers
  --------------------------------------------------*/

/* compiler defines __DLL__ when -WDE option is set */
/* for building a DLL                               */

#ifdef __DLL__
# define DLLFUNC __declspec(dllexport)
#else
# define DLLFUNC __declspec(dllimport)
#endif

DLLFUNC UINT GetNumClients( void );
DLLFUNC UINT GetNumThreadsLocal( void );
DLLFUNC UINT GetNumThreadsGlobal( void );

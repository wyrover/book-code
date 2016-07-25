/*--------------------------------------------------
      SECTIONS.H
      Prototypes for functions exported from
      the Sections DLL.

      written by Brian G. Myers      
  --------------------------------------------------*/

/* Compiler sets dllexport when building the DLL */
/* and dllimport when building the client EXE    */

#ifdef __DLL__
# define DLLFUNC __declspec(dllexport)
#else
# define DLLFUNC __declspec(dllimport)
#endif

DLLFUNC UINT GetNumClients( void );
DLLFUNC UINT GetNumThreadsLocal( void );
DLLFUNC UINT GetNumThreadsGlobal( void );

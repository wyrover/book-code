#include <windows.h>

#define IDS_ERRBADFILENAME    1000


typedef struct SECTIONINFO   *LPSECTIONINFO;
typedef struct tagDbgSection
    {
    char	    szSection[IMAGE_SIZEOF_SHORT_NAME];
    ULONG	    uVirtualAddress;
    ULONG	    uSize;
    LPSECTIONINFO   Next;
    }SECTIONINFO;


typedef struct tagImportDirectory
    {
    DWORD    dwRVAFunctionList;
    DWORD    dwDummy1;
    DWORD    dwDummy2;
    DWORD    dwRVAModule;
    DWORD    dwRVAFirstFunction;
    }IMAGE_IMPORT_DIRECTORY, * PIMAGE_IMPORT_DIRECTORY;


/* global prototypes for functions in pefile.c */
PIMAGE_FILE_HEADER	WINAPI FileHeaderOffset (LPVOID);
PIMAGE_OPTIONAL_HEADER	WINAPI OptionalHeaderOffset (LPVOID);
PIMAGE_SECTION_HEADER	WINAPI SectionHeaderOffset (LPVOID);
PIMAGE_IMPORT_DIRECTORY WINAPI ImportDirectoryOffset (LPVOID);
int			WINAPI NumOfSections (LPVOID);
void			WINAPI RetrieveModuleName (char *, HANDLE);
void			WINAPI RetrieveSectionNames (HANDLE, HANDLE, SECTIONINFO **);
LPVOID			WINAPI GetModuleEntryPoint (LPVOID);
LPVOID			WINAPI GetImageBase (LPVOID);
int			WINAPI GetImportModuleNames (LPVOID, HANDLE, char  **);
int			WINAPI GetImportFunctionNamesByModule (LPVOID, HANDLE, char *, char  **);
int			WINAPI GetExportFunctionNames (LPVOID, HANDLE, char **);

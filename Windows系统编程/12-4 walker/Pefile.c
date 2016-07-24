
#include "pefile.h"



PIMAGE_SECTION_HEADER	WINAPI IDSectionHeaderOffset (LPVOID);


BOOL WINAPI DllMain (
    HANDLE    hModule,
    DWORD     dwFunction,
    LPVOID    lpNot)
{
    return TRUE;
}



/* return offset to file header */
PIMAGE_FILE_HEADER   WINAPI FileHeaderOffset (
    LPVOID    lpFile)
{
    int 		     ImageHdrOffset = 0;

    /* if DOS based file, skip DOS header and file signature */
    if (*((USHORT *)lpFile) == IMAGE_DOS_SIGNATURE)
	{
	/* file image header offset exists after DOS header and nt signature */
	ImageHdrOffset = (int)((ULONG *)lpFile)[15] + sizeof (ULONG);
	if (*((ULONG *)((char *)lpFile + ImageHdrOffset - sizeof (ULONG))) != IMAGE_NT_SIGNATURE)
	    return NULL;
	}

    /* optional header exists immediately after file header and image header */
    return (PIMAGE_FILE_HEADER)((int)lpFile + ImageHdrOffset);
}




/* return optional header data */
PIMAGE_OPTIONAL_HEADER	WINAPI OptionalHeaderOffset (
    LPVOID    lpFile)
{
    int 		     ImageHdrOffset = 0;

    /* if DOS based file, skip DOS header and file signature */
    if (*((USHORT *)lpFile) == IMAGE_DOS_SIGNATURE)
	{
	/* file image header offset exists after DOS header and nt signature */
	ImageHdrOffset = (int)((ULONG *)lpFile)[15] + sizeof (ULONG);
	if (*((ULONG *)((char *)lpFile + ImageHdrOffset - sizeof (ULONG))) != IMAGE_NT_SIGNATURE)
	    return NULL;
	}

    /* optional header exists immediately after file header and image header */
    return (PIMAGE_OPTIONAL_HEADER)((char *)lpFile + ImageHdrOffset + sizeof (IMAGE_FILE_HEADER));
}




/* return pointer to first section header */
PIMAGE_SECTION_HEADER	WINAPI SectionHeaderOffset (
    LPVOID    lpFile)
{
    int 		     ImageHdrOffset = 0;

    /* if DOS based file, skip DOS header and file signature */
    if (*((USHORT *)lpFile) == IMAGE_DOS_SIGNATURE)
	{
	/* file image header offset exists after DOS header and nt signature */
	ImageHdrOffset = (int)((ULONG *)lpFile)[15] + sizeof (ULONG);
	if (*((ULONG *)((char *)lpFile + ImageHdrOffset - sizeof (ULONG))) != IMAGE_NT_SIGNATURE)
	    return NULL;
	}

    /* optional header exists immediately after file header and image header */
    return (PIMAGE_SECTION_HEADER)((int)OptionalHeaderOffset (lpFile) +
	(int)((PIMAGE_FILE_HEADER)((int)lpFile + ImageHdrOffset))->SizeOfOptionalHeader);
}





/* return offset to first IMAGE_IMPORT_DIRECTORY entry */
PIMAGE_IMPORT_DIRECTORY  WINAPI ImportDirectoryOffset (
	LPVOID	  lpFile)
{
    PIMAGE_OPTIONAL_HEADER   poh = OptionalHeaderOffset (lpFile);
    PIMAGE_SECTION_HEADER    psh = SectionHeaderOffset (lpFile);
    int 		     nSections = NumOfSections (lpFile);
    int 		     i = 0;
    LPVOID		     VAImportDir;

    VAImportDir = (LPVOID)poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

    /* locate section containing import directory */
    while (i++<nSections)
	{
	if (psh->VirtualAddress <= (DWORD)VAImportDir &&
	    psh->VirtualAddress + psh->SizeOfRawData > (DWORD)VAImportDir)
	    break;
	psh++;
	}

    if (i > nSections)
	return 0;

    /* return image import directory offset */
    return (PIMAGE_IMPORT_DIRECTORY)(((int)lpFile + (int)VAImportDir - psh->VirtualAddress) +
				   (int)psh->PointerToRawData);
}




/* return pointer to image directory section header */
PIMAGE_SECTION_HEADER	WINAPI IDSectionHeaderOffset (
    LPVOID    lpFile)
{
    int 		     ImageHdrOffset = 0;
    PIMAGE_OPTIONAL_HEADER   poh = OptionalHeaderOffset (lpFile);
    PIMAGE_SECTION_HEADER    psh = SectionHeaderOffset (lpFile);
    int 		     nSections = NumOfSections (lpFile);
    int 		     i = 0;
    LPVOID		     VAImportDir;

    VAImportDir = (LPVOID)poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

    /* locate section containing import directory */
    while (i++<nSections)
	{
	if (psh->VirtualAddress <= (DWORD)VAImportDir &&
	    psh->VirtualAddress + psh->SizeOfRawData > (DWORD)VAImportDir)
	    break;
	psh++;
	}

    if (i > nSections)
	return 0;
    else
	return psh;
}





/* return the total number of sections in the module */
int   WINAPI NumOfSections (
    LPVOID    lpFile)
{
    int 		     ImageHdrOffset = 0;

    /* if DOS based file, skip DOS header and file signature */
    if (*((USHORT *)lpFile) == IMAGE_DOS_SIGNATURE)
	{
	/* file image header offset exists after DOS header and nt signature */
	ImageHdrOffset = (int)((ULONG *)lpFile)[15] + sizeof (ULONG);
	if (*((ULONG *)((char *)lpFile + ImageHdrOffset - sizeof (ULONG))) != IMAGE_NT_SIGNATURE)
	    return 0;
	}

    /* section total is found in the file header */
    return (int)((PIMAGE_FILE_HEADER)((int)lpFile + ImageHdrOffset))->NumberOfSections;
}



/* retrieve name of module from module's open file handle */
void WINAPI RetrieveModuleName (
    char      *lpszModule,
    HANDLE    hFile)
{
    HANDLE		     hMapFile;
    LPVOID		     lpFile;
    char		     *lpszName;
    int 		     nSections;
    ULONG		     VAExportDir;
    int 		     i=0;
    int 		     ImageHdrOffset;
    PIMAGE_SECTION_HEADER    psh;
    PIMAGE_FILE_HEADER	     pfh;
    PIMAGE_OPTIONAL_HEADER   poh;
    PIMAGE_EXPORT_DIRECTORY  ped;


    /* memory map handle to DLL for easy access */
    hMapFile = CreateFileMapping (hFile,
				  (LPSECURITY_ATTRIBUTES)NULL,
				  PAGE_READONLY,
				  0,
				  0,
				  NULL);

    /* map view of entire file */
    lpFile = MapViewOfFile (hMapFile, FILE_MAP_READ, 0, 0, 0);

    /* if DOS based file */
    if (*((USHORT *)lpFile) == IMAGE_DOS_SIGNATURE)
	{
	/* file image header offset exists after DOS header and nt signature */
	ImageHdrOffset = (int)((ULONG *)lpFile)[15] + sizeof (ULONG);
	if (*((ULONG *)((char *)lpFile + ImageHdrOffset - sizeof (ULONG))) !=
	    IMAGE_NT_SIGNATURE)
	    {
	    strcpy (lpszModule, "Error, no IMAGE_NT_SIGNATURE");
	    goto EXIT;
	    }
	}

    pfh = (PIMAGE_FILE_HEADER)((char *)lpFile + ImageHdrOffset);

    /* if optional header exists and exports directory exists proceed */
    if (pfh->SizeOfOptionalHeader)
	{
	/* locate virtual address for Export Image Directory in OptionalHeader */
	poh = (PIMAGE_OPTIONAL_HEADER)((char *)pfh + sizeof (IMAGE_FILE_HEADER));
	VAExportDir = poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

	/* locate section where export virtual address is located */
	psh = (PIMAGE_SECTION_HEADER)((char *)poh + pfh->SizeOfOptionalHeader);
	nSections = pfh->NumberOfSections;
	while (i++<nSections)
	    {
	    if (psh->VirtualAddress <= VAExportDir &&
		psh->VirtualAddress + psh->SizeOfRawData > VAExportDir)
		break;
	    psh++;
	    }

	/* locate export image directory */
	if (i < nSections)
	    ped = (PIMAGE_EXPORT_DIRECTORY)((char *)lpFile +
		(VAExportDir - psh->VirtualAddress) + psh->PointerToRawData);
	else
	    {
	    strcpy (lpszModule, "IMAGE_EXPORT_DIRECTORY not found");
	    goto EXIT;
	    }

	/* read name from export directory */
	lpszName = (char *)lpFile + ped->Name + (psh->PointerToRawData - psh->VirtualAddress);
	strcpy (lpszModule, lpszName);
	}

    else
	strcpy (lpszModule, "Error, no IMAGE_OPTIONAL_HEADER");

EXIT:
    /* clean up before exiting */
    UnmapViewOfFile (lpFile);
    CloseHandle (hMapFile);
}




/* retieve section names from module file handle */
void WINAPI RetrieveSectionNames (
    HANDLE	  hHeap,
    HANDLE	  hFile,
    SECTIONINFO   **pSection)
{
    HANDLE		     hMapFile;
    LPVOID		     lpFile;
    int 		     nSections;
    int 		     i=0;
    int 		     ImageHdrOffset;
    PIMAGE_SECTION_HEADER    psh;
    PIMAGE_FILE_HEADER	     pfh;
    SECTIONINFO 	     *ps;


    /* memory map handle to DLL for easy access */
    hMapFile = CreateFileMapping (hFile,
				  (LPSECURITY_ATTRIBUTES)NULL,
				  PAGE_READONLY,
				  0,
				  0,
				  NULL);

    /* map view of entire file */
    lpFile = MapViewOfFile (hMapFile, FILE_MAP_READ, 0, 0, 0);

    /* if DOS based file */
    if (*((USHORT *)lpFile) == IMAGE_DOS_SIGNATURE)
	{
	/* file image header offset exists after DOS header and nt signature */
	ImageHdrOffset = (int)((ULONG *)lpFile)[15] + sizeof (ULONG);
	if (*((ULONG *)((char *)lpFile + ImageHdrOffset - sizeof (ULONG))) !=
	    IMAGE_NT_SIGNATURE)
	    goto EXIT;
	}

    pfh = (PIMAGE_FILE_HEADER)((char *)lpFile + ImageHdrOffset);

    /* if optional header exists, offset first section header */
    psh = (PIMAGE_SECTION_HEADER)((char *)pfh +
	      sizeof (IMAGE_FILE_HEADER) + pfh->SizeOfOptionalHeader);

    /* allocate one section header for each section */
    ps = *pSection = (SECTIONINFO *)HeapAlloc (hHeap,
					       HEAP_ZERO_MEMORY,
					       sizeof (SECTIONINFO));
    nSections = pfh->NumberOfSections;
    while (TRUE)
	{
	strcpy (ps->szSection, psh[i].Name);
	ps->uVirtualAddress = psh[i].VirtualAddress;
	ps->uSize = psh[i].SizeOfRawData;

	if (++i >= nSections)
	    break;

	/* allocate heap memory for sections */
	ps->Next = (LPSECTIONINFO)HeapAlloc (hHeap,
					     HEAP_ZERO_MEMORY,
					     sizeof (SECTIONINFO));
	ps = (SECTIONINFO *)ps->Next;
	}

EXIT:
    /* clean up before exiting */
    UnmapViewOfFile (lpFile);
    CloseHandle (hMapFile);
}




/* function returns the entry point for an exe module lpFile must
   be a memory mapped file pointer to the beginning of the image file */
LPVOID	WINAPI GetModuleEntryPoint (
    LPVOID    lpFile)
{
    PIMAGE_OPTIONAL_HEADER   poh = OptionalHeaderOffset (lpFile);

    if (poh != NULL)
	return (LPVOID)(poh->AddressOfEntryPoint);
    else
	return NULL;
}




/* retrieve entry point */
LPVOID	WINAPI GetImageBase (
    LPVOID    lpFile)
{
    PIMAGE_OPTIONAL_HEADER   poh = OptionalHeaderOffset (lpFile);

    if (poh != NULL)
	return (LPVOID)(poh->ImageBase);
    else
	return NULL;
}




/* get import modules names separated by null terminators, return module count */
int  WINAPI GetImportModuleNames (
    LPVOID    lpFile,
    HANDLE    hHeap,
    char      **pszModules)
{
    PIMAGE_IMPORT_DIRECTORY  pid = ImportDirectoryOffset (lpFile);
    PIMAGE_SECTION_HEADER    pidsh = IDSectionHeaderOffset (lpFile);
    BYTE		     *pData = (BYTE *)pid;
    int 		     nCnt = 0, nSize = 0, i;
    char		     *pModule[1024];  /* hardcoded maximum number of modules?? */
    char		     *psz;

    /* extract all import modules */
    while (pid->dwRVAModule)
	{
	/* allocate temporary buffer for absolute string offsets */
	pModule[nCnt] = (char *)(pData + (pid->dwRVAModule-pidsh->VirtualAddress));
	nSize += strlen (pModule[nCnt]) + 1;

	/* increment to the next import directory entry */
	pid++;
	nCnt++;
	}

    /* copy all strings to one chunk of heap memory */
    *pszModules = HeapAlloc (hHeap, HEAP_ZERO_MEMORY, nSize);
    psz = *pszModules;
    for (i=0; i<nCnt; i++)
	{
	strcpy (psz, pModule[i]);
	psz += strlen (psz) + 1;
	}

    return nCnt;
}




/* get import module function names separated by null terminators, return function count */
int  WINAPI GetImportFunctionNamesByModule (
    LPVOID    lpFile,
    HANDLE    hHeap,
    char      *pszModule,
    char      **pszFunctions)
{
    PIMAGE_IMPORT_DIRECTORY  pid = ImportDirectoryOffset (lpFile);
    PIMAGE_SECTION_HEADER    pidsh = IDSectionHeaderOffset (lpFile);
    DWORD		     dwBase = ((DWORD)pid - pidsh->VirtualAddress);
    int 		     nCnt = 0, nSize = 0;
    DWORD		     dwFunction;
    char		     *psz;

    /* find module's pid */
    while (pid->dwRVAModule &&
	   strcmp (pszModule, (char *)(pid->dwRVAModule+dwBase)))
	pid++;

    /* count functions and total space required for them */
    nSize += strlen ((char *)((*(DWORD *)(pid->dwRVAFirstFunction + dwBase)) + dwBase+2)) + 1;

    /* last image directory does not have a separate function list, so improvise */
    if (!(dwFunction = pid->dwRVAFunctionList))
	dwFunction = pid->dwRVAFirstFunction + 4;

    while (dwFunction &&
	   *(char *)((*(DWORD *)(dwFunction + dwBase)) + dwBase+2))
	{
	nSize += strlen ((char *)((*(DWORD *)(dwFunction + dwBase)) + dwBase+2)) + 1;
	dwFunction += 4;
	nCnt++;
	}

    /* allocate memory off heap for function names */
    *pszFunctions = HeapAlloc (hHeap, HEAP_ZERO_MEMORY, nSize);
    psz = *pszFunctions;
    strcpy (psz, (char *)((*(DWORD *)(pid->dwRVAFirstFunction + dwBase)) + dwBase+2));
    psz += strlen ((char *)((*(DWORD *)(pid->dwRVAFirstFunction + dwBase)) + dwBase+2)) + 1;

    /* last image directory does not have a separate function list, so improvise */
    if (!(dwFunction = pid->dwRVAFunctionList))
	dwFunction = pid->dwRVAFirstFunction + 4;

    while (dwFunction &&
	   *((char *)((*(DWORD *)(dwFunction + dwBase)) + dwBase+2)))
	{
	strcpy (psz, (char *)((*(DWORD *)(dwFunction + dwBase)) + dwBase+2));
	psz += strlen ((char *)((*(DWORD *)(dwFunction + dwBase)) + dwBase+2)) + 1;
	dwFunction += 4;
	}

    return nCnt;
}




/* get exported function names separated by null terminators, return count of functions */
int  WINAPI GetExportFunctionNames (
    LPVOID    lpFile,
    HANDLE    hHeap,
    char      **pszFunctions)
{

    return 0;
}

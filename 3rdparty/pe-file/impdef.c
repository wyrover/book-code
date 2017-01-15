//==================================
// This is a modification of:
//   PEDUMP - Matt Pietrek 1995
// done by Ismael Jurado 1997
//
// This program only outputs to stdout a DEF file from a DLL given in the
// command line.
// I don´t know how to contact Matt, so if this program is not legal, please
// let me know.
//    ismaelj@hotmail.com
//==================================

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

// This code was taken from WINNT.H, which is not included in the current
// Cygnus port.
#ifndef _WINNT_
#define FIELD_OFFSET(type, field)    ((LONG)&(((type *)0)->field))
#define IMAGE_FIRST_SECTION( ntheader ) ((PIMAGE_SECTION_HEADER)
\
((DWORD)ntheader +
 \
 FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader) +
 \
 ((PIMAGE_NT_HEADERS)(ntheader))->FileHeader.SizeOfOptionalHeader
 \
))

#define IMAGE_SIZEOF_SHORT_NAME              8
#define IMAGE_DIRECTORY_ENTRY_EXPORT         0   // Export Directory
#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16
#define IMAGE_NT_SIGNATURE                  0x00004550  // PE00
#define IMAGE_DOS_SIGNATURE                 0x5A4D      // MZ

typedef struct _IMAGE_DATA_DIRECTORY {
    DWORD   VirtualAddress;
    DWORD   Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_FILE_HEADER {
    WORD    Machine;
    WORD    NumberOfSections;
    DWORD   TimeDateStamp;
    DWORD   PointerToSymbolTable;
    DWORD   NumberOfSymbols;
    WORD    SizeOfOptionalHeader;
    WORD    Characteristics;
} IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

typedef struct _IMAGE_OPTIONAL_HEADER {
    //
    // Standard fields.
    //

    WORD    Magic;
    BYTE    MajorLinkerVersion;
    BYTE    MinorLinkerVersion;
    DWORD   SizeOfCode;
    DWORD   SizeOfInitializedData;
    DWORD   SizeOfUninitializedData;
    DWORD   AddressOfEntryPoint;
    DWORD   BaseOfCode;
    DWORD   BaseOfData;

    //
    // NT additional fields.
    //

    DWORD   ImageBase;
    DWORD   SectionAlignment;
    DWORD   FileAlignment;
    WORD    MajorOperatingSystemVersion;
    WORD    MinorOperatingSystemVersion;
    WORD    MajorImageVersion;
    WORD    MinorImageVersion;
    WORD    MajorSubsystemVersion;
    WORD    MinorSubsystemVersion;
    DWORD   Reserved1;
    DWORD   SizeOfImage;
    DWORD   SizeOfHeaders;
    DWORD   CheckSum;
    WORD    Subsystem;
    WORD    DllCharacteristics;
    DWORD   SizeOfStackReserve;
    DWORD   SizeOfStackCommit;
    DWORD   SizeOfHeapReserve;
    DWORD   SizeOfHeapCommit;
    DWORD   LoaderFlags;
    DWORD   NumberOfRvaAndSizes;
    IMAGE_DATA_DIRECTORY
    DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER, *PIMAGE_OPTIONAL_HEADER;


typedef struct _IMAGE_NT_HEADERS {
    DWORD Signature;
    IMAGE_FILE_HEADER FileHeader;
    IMAGE_OPTIONAL_HEADER OptionalHeader;
} IMAGE_NT_HEADERS, *PIMAGE_NT_HEADERS;

typedef struct _IMAGE_SECTION_HEADER {
    BYTE    Name[IMAGE_SIZEOF_SHORT_NAME];
    union {
        DWORD   PhysicalAddress;
        DWORD   VirtualSize;
    } Misc;
    DWORD   VirtualAddress;
    DWORD   SizeOfRawData;
    DWORD   PointerToRawData;
    DWORD   PointerToRelocations;
    DWORD   PointerToLinenumbers;
    WORD    NumberOfRelocations;
    WORD    NumberOfLinenumbers;
    DWORD   Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

typedef struct _IMAGE_EXPORT_DIRECTORY {
    DWORD   Characteristics;
    DWORD   TimeDateStamp;
    WORD    MajorVersion;
    WORD    MinorVersion;
    DWORD   Name;
    DWORD   Base;
    DWORD   NumberOfFunctions;
    DWORD   NumberOfNames;
    PDWORD  *AddressOfFunctions;
    PDWORD  *AddressOfNames;
    PWORD   *AddressOfNameOrdinals;
} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;

typedef struct _IMAGE_DOS_HEADER {      // DOS .EXE header
    WORD   e_magic;                     // Magic number
    WORD   e_cblp;                      // Bytes on last page of file
    WORD   e_cp;                        // Pages in file
    WORD   e_crlc;                      // Relocations
    WORD   e_cparhdr;                   // Size of header in
    paragraphs
    WORD   e_minalloc;                  // Minimum extra paragraphs
    needed
    WORD   e_maxalloc;                  // Maximum extra paragraphs
    needed
    WORD   e_ss;                        // Initial (relative) SS value
    WORD   e_sp;                        // Initial SP value
    WORD   e_csum;                      // Checksum
    WORD   e_ip;                        // Initial IP value
    WORD   e_cs;                        // Initial (relative) CS value
    WORD   e_lfarlc;                    // File address of relocation
    table
    WORD   e_ovno;                      // Overlay number
    WORD   e_res[4];                    // Reserved words
    WORD   e_oemid;                     // OEM identifier (for
    e_oeminfo)
    WORD   e_oeminfo;                   // OEM information; e_oemid
    specific
    WORD   e_res2[10];                  // Reserved words
    LONG   e_lfanew;                    // File address of new exe
    header
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;
#endif // __WINNT__

// Here starts Matt's code
//
//
// MakePtr is a macro that allows you to easily add to values (including
// pointers) together without dealing with C's pointer arithmetic.  It
// essentially treats the last two parameters as DWORDs.  The first
// parameter is used to typecast the result to the appropriate pointer type.
#define MakePtr( cast, ptr, addValue ) (cast)( (DWORD)(ptr) + (DWORD)(addValue))

PIMAGE_SECTION_HEADER GetEnclosingSectionHeader(DWORD rva,
                      PIMAGE_NT_HEADERS pNTHeader)
{
    PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(pNTHeader);
    unsigned i;

    for (i = 0; i < pNTHeader->FileHeader.NumberOfSections; i++, section++) {
        // Is the RVA within this section?
        if ((rva >= section->VirtualAddress) &&
            (rva < (section->VirtualAddress + section->Misc.VirtualSize)))
            return section;
    }

    return 0;
}

void DumpExportsSection(DWORD base, PIMAGE_NT_HEADERS pNTHeader)
{
    PIMAGE_EXPORT_DIRECTORY exportDir;
    PIMAGE_SECTION_HEADER header;
    INT delta;
    DWORD i;
    PDWORD functions;
    PWORD ordinals;
    PSTR *name;
    DWORD exportsStartRVA, exportsEndRVA;
    exportsStartRVA =
        pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    exportsEndRVA = exportsStartRVA +
                    pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
    // Get the IMAGE_SECTION_HEADER that contains the exports.  This is
    // usually the .edata section, but doesn't have to be.
    header =
        (PIMAGE_SECTION_HEADER)GetEnclosingSectionHeader(exportsStartRVA, pNTHeader);

    if (!header) return;

    delta = (INT)(header->VirtualAddress - header->PointerToRawData);
    exportDir = MakePtr(PIMAGE_EXPORT_DIRECTORY, base, exportsStartRVA - delta);
    printf("EXPORTS\n");
    functions = (PDWORD)((DWORD)exportDir->AddressOfFunctions - delta + base);
    ordinals = (PWORD)((DWORD)exportDir->AddressOfNameOrdinals - delta + base);
    name = (PSTR *)((DWORD)exportDir->AddressOfNames - delta + base);

    for (i = 0; i < exportDir->NumberOfFunctions; i++) {
        DWORD entryPointRVA = functions[i];
        DWORD j;
        int flag = 0;

        if (entryPointRVA == 0)     // Skip over gaps in exported function
            continue;               // ordinals (the entrypoint is 0 for

        // these functions).
        // See if this function has an associated name exported for it.
        for (j = 0; j < exportDir->NumberOfNames; j++)
            if (ordinals[j] == i) {
                printf("%s\t@%u", name[j] - delta + base, i + exportDir->Base);
                flag = 1;
            }

        // Is it a forwarder?  If so, the entry point RVA is inside the
        // .edata section, and is an RVA to the DllName.EntryPointName
        if ((entryPointRVA >= exportsStartRVA) && (entryPointRVA <= exportsEndRVA)) {
            printf("%s\t@%u", entryPointRVA - delta + base, i + exportDir->Base);
            flag = 1;
        }

        if (flag) printf("\n");
    }
}

void DumpExeFile(PIMAGE_DOS_HEADER dosHeader)
{
    PIMAGE_NT_HEADERS pNTHeader;
    DWORD base = (DWORD)dosHeader;
    pNTHeader = MakePtr(PIMAGE_NT_HEADERS, dosHeader,
                        dosHeader->e_lfanew);

    // First, verify that the e_lfanew field gave us a reasonable
    // pointer, then verify the PE signature.
    // IJ: gcc doesn't support C exeptions??
//    __try
//    {
    if (pNTHeader->Signature != IMAGE_NT_SIGNATURE) {
        printf("Not a Portable Executable (PE) EXE\n");
        return;
    }

//    }
    /*
        __except( TRUE )    // Should only get here if pNTHeader (above) is bogus
        {
            printf( "invalid .EXE\n");
            return;
        }
    */
    DumpExportsSection(base, pNTHeader);
    printf("\n");
}

void DumpFile(LPCWSTR filename)
{
    HANDLE hFile;
    HANDLE hFileMapping;
    LPVOID lpFileBase;
    PIMAGE_DOS_HEADER dosHeader;
    hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL,
                       OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Couldn't open file with CreateFile()\n");
        return;
    }

    hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);

    if (hFileMapping == 0) {
        CloseHandle(hFile);
        printf("Couldn't open file mapping with CreateFileMapping()\n");
        return;
    }

    lpFileBase = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);

    if (lpFileBase == 0) {
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        printf("Couldn't map view of file with MapViewOfFile()\n");
        return;
    }

    wprintf(L";Exports of file %s\n\n", filename);
    dosHeader = (PIMAGE_DOS_HEADER)lpFileBase;

    if (dosHeader->e_magic == IMAGE_DOS_SIGNATURE) {
        DumpExeFile(dosHeader);
    } else printf("Format Unknown!\n");
}

char HelpText[] =
    "IMPDEF\n"
    "Syntax: IMPDEF DLLname\n\n"
    "Modification of:\n"
    "PEDUMP - Win32/COFF EXE/OBJ/LIB file dumper - 1995 Matt Pietrek\n"
    "by Ismael Jurado 1997. Only produces a DEF file from a DLL\n"
    "Let me know if this modification is not legal: ismaelj@hotmail.com\n";

int _tmain(int argc, _TCHAR* argv [])
{
    if (argc == 1) {
        printf(HelpText);
        return 1;
    }

    DumpFile(argv[1]);
    return 0;
}

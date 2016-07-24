
#include <windows.h>
#include "memory.h"


/**************************************************************************
 * HANDLE CreateMapFile(char *MapFileName)
 *
 * Purpose: Create a Map file to map named share memory
 *
 * Inputs:  none
 *
 * Returns: MapFileHandle - a handle to the file
 *                        or NULL if failure
 *
 * Calls:   CreateFile, ErrorOut
 *
\**************************************************************************/
HANDLE CreateMapFile(char *MapFileName)
{
   HANDLE MapFileHandle;

   MapFileHandle= CreateFile(MapFileName,
                             GENERIC_READ | GENERIC_WRITE,
                             FILE_SHARE_READ | FILE_SHARE_WRITE,
                             NULL,
                             CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL,
                             NULL);

   if (MapFileHandle == INVALID_HANDLE_VALUE)
   {
      ErrorOut("CreateFile");
      return(NULL);
   }
   else
      return(MapFileHandle);

}

/**************************************************************************
 * HANDLE CreateMap(HANDLE FileToBeMapped, char *MapName )
 *
 * Purpose: Create File Mapping object using the open file handle
 *
 * Inputs:  FileToBeMapped - handle to the file
 *
 * Returns: MapHandle - handle to the file mapping object
 *                    or NULL if failure
 *
 * Calls:   CreateFileMapping, ErrorOut
\**************************************************************************/

HANDLE CreateMap(HANDLE FileToBeMapped, LPSTR MapName)
{
   HANDLE MapHandle;

   MapHandle= CreateFileMapping(FileToBeMapped,
                                NULL,
                                PAGE_READWRITE,
                                0,
                                4096,
                                MapName);

   if (MapHandle == NULL)
   {
      ErrorOut("CreateFileMapping");
      return(NULL);
   }
   else
      return(MapHandle);
}


/**************************************************************************
 * LPVOID MapView(HANDLE hMap)
 *
 * Purpose: Map the file mapping object into address space
 *
 * Inputs:  hMap - handle to the mapping object
 *
 * Returns: MappedPointer - pointer to the address space that the
 *                        object is mapped into
 *                        or NULL if failure
 *
 * Calls:   MapViewOfFile, ErrorOut
 *
\**************************************************************************/

LPVOID MapView(HANDLE hMap)
{
   LPVOID MappedPointer;

   MappedPointer= MapViewOfFile(hMap,
                                FILE_MAP_WRITE | FILE_MAP_READ,
                                0, 0, 4096);
   if (MappedPointer == NULL)
   {
      ErrorOut("MapViewOfFile");
      return(NULL);
   }
   else
      return(MappedPointer);
}

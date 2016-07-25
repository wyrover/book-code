// ****************************************************************************
//
// Module:  winshmem.C
// Author:  Dick Lam
//
// Purpose: C++ class source file for osSharedMemory
//
// Notes:  This is a base class.  It contains general implementation methods
//         for memory blocks shared between processes and threads.
//
// ****************************************************************************

#define STRICT
#include <windows.h>

#include <string.h>
#include "osshmem.h"

// ****************************************************************************

// osSharedMemory - constructor

osSharedMemory::osSharedMemory(ipcSharedMemory *interface, const char *name,
                               long blocksize)

{
   // init instance variables
   myInterface = interface;
   myID = 0L;
   myName = 0;
   isOwner = 1;
   myBlock = 0;

   // check that input name is valid
   if (!name || (strlen(name) == 0)) {
      myInterface->myState = ipcSharedMemory::badname;
      return;
   }

   // allocate space for the name
   myName = new char[strlen(name) + 1];
   if (!myName) {
      myInterface->myState = ipcSharedMemory::bad;
      return;
   }

   // and save the complete shared memory pathname
   strcpy(myName, name);

   // set the object state and create the block
   myInterface->myState = ipcSharedMemory::good;
   CreateBlock(blocksize);
}

// ----------------------------------------------------------------------------

// osSharedMemory - constructor

osSharedMemory::osSharedMemory(ipcSharedMemory *interface, const char *name)

{
   // init instance variables
   myInterface = interface;
   myID = 0L;
   myName = 0;
   isOwner = 0;
   myBlock = 0;

   // check that input name is valid
   if (!name || (strlen(name) == 0)) {
      myInterface->myState = ipcSharedMemory::badname;
      return;
   }

   // allocate space for the name
   myName = new char[strlen(name) + 1];
   if (!myName) {
      myInterface->myState = ipcSharedMemory::bad;
      return;
   }

   // and save the complete shared memory pathname
   strcpy(myName, name);

   // set the object state and create the block
   myInterface->myState = ipcSharedMemory::good;
   OpenBlock();
}

// ----------------------------------------------------------------------------

// ~osSharedMemory - destructor

osSharedMemory::~osSharedMemory()

{
   // see if shared memory block was opened
   if (myInterface->myState == ipcSharedMemory::good) {
      // close the block
      CloseBlock();
   }

   // and delete the block name
   delete myName;
}

// ----------------------------------------------------------------------------

// Name - returns the name of the memory block

char *osSharedMemory::Name() const

{
   return myName;
}

// ----------------------------------------------------------------------------

// Pointer - returns a pointer to the memory block

void *osSharedMemory::Pointer() const

{
   return myBlock;
}

// ----------------------------------------------------------------------------

// Owner - returns 1 if this is the owner (creator), and 0 otherwise

int osSharedMemory::Owner() const

{
   return isOwner;
}

// ----------------------------------------------------------------------------

// CreateBlock - creates a new shared memory block

void osSharedMemory::CreateBlock(long blocksize)

{
   // create the shared memory block
   HANDLE hMappedFile = CreateFileMapping( (HANDLE)0xFFFFFFFF, 0,
                                           PAGE_READWRITE, 0L, (DWORD)blocksize,
                                           (LPTSTR)myName );
   if (!hMappedFile) {
      myInterface->myState = ipcSharedMemory::bad;
      return;
   }

   if (GetLastError() == ERROR_ALREADY_EXISTS) {
      myInterface->myState = ipcSharedMemory::bad;
      CloseHandle(hMappedFile);
      return;
   }

   // create a view of the object
   myBlock = (void *)MapViewOfFile(hMappedFile, FILE_MAP_WRITE, 0L, 0L, 0L);
   if (!myBlock) {
      myInterface->myState = ipcSharedMemory::bad;
      CloseHandle(hMappedFile);
      return;
   }

   myID = (unsigned long)hMappedFile;
}

// ----------------------------------------------------------------------------

// OpenBlock - opens an existing shared memory block

void osSharedMemory::OpenBlock()

{
   // open the shared memory block
   HANDLE hMappedFile = OpenFileMapping(FILE_MAP_WRITE, FALSE, (LPTSTR)myName);
   if (!hMappedFile) {
      myInterface->myState = ipcSharedMemory::notfound;
      return;
   }

   // create a view of the object
   myBlock = (void *)MapViewOfFile(hMappedFile, FILE_MAP_WRITE, 0L, 0L, 0L);
   if (!myBlock) {
      myInterface->myState = ipcSharedMemory::notfound;
      CloseHandle(hMappedFile);
      return;
   }

   myID = (unsigned long)hMappedFile;
}

// ----------------------------------------------------------------------------

// CloseBlock - closes a memory block

void osSharedMemory::CloseBlock()

{
   // make sure the block was opened ok
   if ( myInterface->myState == ipcSharedMemory::good) {
      UnmapViewOfFile( (LPVOID)myBlock );
      CloseHandle( (HANDLE)myID );
   }
}

// ****************************************************************************

// end of winshmem.C

// ****************************************************************************
//
// Module:  os2shmem.C
// Author:  Dick Lam
//
// Purpose: C++ class source file for osSharedMemory
//
// Notes:  This is a base class.  It contains general implementation methods
//         for memory blocks shared between processes and threads.
//
// ****************************************************************************

#define INCL_DOS
#include <os2.h>

#include <string.h>
#include "osshmem.h"

// ****************************************************************************

// init operating system-specific variables

const char *memPath = "\\SHAREMEM\\";

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
   myName = new char[strlen(memPath) + strlen(name) + 1];
   if (!myName) {
      myInterface->myState = ipcSharedMemory::bad;
      return;
   }

   // and form the complete shared memory pathname
   strcpy(myName, memPath);
   strcat(myName, name);

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
   myName = new char[strlen(memPath) + strlen(name) + 1];
   if (!myName) {
      myInterface->myState = ipcSharedMemory::bad;
      return;
   }

   // and form the complete shared memory pathname
   strcpy(myName, memPath);
   strcat(myName, name);

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
   if (DosAllocSharedMem(&myBlock, (PSZ)myName, blocksize, fALLOC) != 0)
      myInterface->myState = ipcSharedMemory::bad;
}

// ----------------------------------------------------------------------------

// OpenBlock - opens an existing shared memory block

void osSharedMemory::OpenBlock()

{
   // access an already existing shared memory block
   if (DosGetNamedSharedMem(&myBlock, (PSZ)myName, fGETNMSHR) != 0)
      myInterface->myState = ipcSharedMemory::notfound;
}

// ----------------------------------------------------------------------------

// CloseBlock - closes a memory block

void osSharedMemory::CloseBlock()

{
   // make sure the block was opened ok
   if ( myInterface->myState == ipcSharedMemory::good)
      DosFreeMem(myBlock);
}

// ****************************************************************************

// end of os2shmem.C

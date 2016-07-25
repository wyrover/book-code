// ****************************************************************************
//
// Module:  aixshmem.C
// Author:  Dick Lam
//
// Purpose: C++ class source file for osSharedMemory
//
// Notes:  This is a base class.  It contains general implementation methods
//         for memory blocks shared between processes and threads.
//
// ****************************************************************************

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/mode.h>
#include "osshmem.h"

// ****************************************************************************

// init operating system-specific variables

const char *memPath = "/tmp/";

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

   // create a file for later call to ftok
   if (::creat(myName, O_RDWR | O_CREAT | O_EXCL) == -1) {
      myInterface->myState = ipcSharedMemory::badname;
      return;
   }

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

   // set the object state and open the block
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

      // if the process is the shared memory owner, delete the file
      if (isOwner)
         ::unlink(myName);
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
   // get key
   key_t memkey = ::ftok(myName, 'V');
   if (memkey == (key_t)-1) {
      myInterface->myState = ipcSharedMemory::bad;
      return;
   }

   // create the shared memory block
   int newmem = shmget(memkey, (size_t)blocksize, IPC_CREAT |
                                                  S_IRUSR | S_IWUSR |
                                                  S_IRGRP | S_IWGRP |
                                                  S_IROTH | S_IWOTH);
   if (newmem == -1) {
      myInterface->myState = ipcSharedMemory::bad;
      unlink(myName);
      return;
   }

   // attach the shared memory to this process
   myBlock = (void *)shmat(newmem, 0, 0);
   if (myBlock == (void *)-1) {
      myInterface->myState = ipcSharedMemory::bad;
      myBlock = 0;

      struct shmid_ds buffer;
      shmctl(newmem, IPC_RMID, &buffer);        // remove shared memory

      unlink(myName);
      return;
   }

   myID = (unsigned long)newmem;
}

// ----------------------------------------------------------------------------

// OpenBlock - opens an existing shared memory block

void osSharedMemory::OpenBlock()

{
   // get key
   key_t memkey = ::ftok(myName, 'V');
   if (memkey == (key_t)-1) {
      myInterface->myState = ipcSharedMemory::bad;
      return;
   }

   // access an already existing shared memory block
   size_t blocksize = 0;
   int newmem = shmget(memkey, blocksize, S_IRUSR | S_IWUSR |
                                          S_IRGRP | S_IWGRP |
                                          S_IROTH | S_IWOTH);
   if (newmem == -1) {
      myInterface->myState = ipcSharedMemory::notfound;
      return;
   }

   // attach the shared memory to this process
   myBlock = (void *)shmat(newmem, 0, 0);
   if (myBlock == (void *)-1) {
      myInterface->myState = ipcSharedMemory::bad;
      myBlock = 0;

      struct shmid_ds buffer;
      shmctl(newmem, IPC_RMID, &buffer);        // remove shared memory

      return;
   }

   myID = (unsigned long)newmem;
}

// ----------------------------------------------------------------------------

// CloseBlock - closes a memory block

void osSharedMemory::CloseBlock()

{
   // make sure the block was opened ok
   if ( myInterface->myState == ipcSharedMemory::good) {
      shmdt( (char *)myBlock );         // detach the shared memory

      // if this is the block owner, delete the shared memory block
      if (isOwner) {
         struct shmid_ds buffer;
         shmctl( (int)myID, IPC_RMID, &buffer );
      }
   }
}

// ****************************************************************************

// end of aixshmem.C

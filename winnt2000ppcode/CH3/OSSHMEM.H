

// ****************************************************************************
//
// Module:  osshmem.h
// Author:  Dick Lam
//
// Purpose: C++ class header file for osSharedMemory
//
// Notes:  This is a base class.  It contains general implementation methods
//         for memory blocks shared between processes and threads.
//
// ****************************************************************************

#ifndef MODULE_osSharedMemoryh
#define MODULE_osSharedMemoryh

#include "ipcshmem.h"

// class declaration
class osSharedMemory {

public:

   // constructor and destructor
   osSharedMemory(ipcSharedMemory *interface, const char *name, long blocksize);
   osSharedMemory(ipcSharedMemory *interface, const char *name);

   virtual ~osSharedMemory();

   // methods for getting memory block parameters [name, pointer to the block,
   // and whether this is the owner (creator) of the block]
   char *Name() const;
   void *Pointer() const;
   int Owner() const;

protected:

   ipcSharedMemory *myInterface;        // pointer to the interface instance
   unsigned long myID;                  // id of memory block
   char *myName;                        // shared memory block name
   int isOwner;                         // flag indicating owner

   void *myBlock;                       // pointer to the memory block

   // methods for handling the memory block
   void CreateBlock(long blocksize);
   void OpenBlock();
   void CloseBlock();

private:

   // private copy constructor and operator= (define these and make them
   // public to enable copy and assignment of the class)
   osSharedMemory(const osSharedMemory&);
   osSharedMemory& operator=(const osSharedMemory&);

};

#endif

// ****************************************************************************

// end of osshmem.h

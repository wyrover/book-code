// ****************************************************************************
//
// Module:  ossem.h
// Author:  Dick Lam
//
// Purpose: C++ class header file for osSemaphore
//
// Notes:  This is a base class.  It contains general implementation methods
//         for semaphores used in signalling between processes and threads.
//
// ****************************************************************************

#ifndef MODULE_osSemaphoreh
#define MODULE_osSemaphoreh

#include "ipcsem.h"

// class declaration
class osSemaphore {

public:

   // constructor and destructor
   osSemaphore(ipcSemaphore *interface, const char *name, ipcSemaphoreType type,
               ipcSemaphoreOp operation);
   virtual ~osSemaphore();

   // methods for getting semaphore parameters [name, semaphore id, type
   // of semaphore (mutex or event) and whether this is the owner (creator)
   // of the semaphore]
   char *Name() const;
   unsigned long ID() const;
   ipcSemaphoreType Type() const;
   int Owner() const;

   // mutex semaphore methods
   void CreateMutex();
   void OpenMutex();
   void RequestMutex();
   void ReleaseMutex();
   unsigned long QueryMutex();
   void CloseMutex();

   // event semaphore methods
   void CreateEvent();
   void OpenEvent();
   void PostEvent();
   void ResetEvent();
   void WaitEvent();
   unsigned long QueryEvent();
   void CloseEvent();

protected:

   ipcSemaphore *myInterface;    // pointer to the interface instance
   char *myName;                 // semaphore name, id and type
   unsigned long myID;
   ipcSemaphoreType myType;
   int isOwner;                  // flag indicating whether this is owner

private:

   // private copy constructor and operator= (define these and make them
   // public to enable copy and assignment of the class)
   osSemaphore(const osSemaphore&);
   osSemaphore& operator=(const osSemaphore&);

};

#endif

// ****************************************************************************

// end of ossem.h

// ****************************************************************************
//
// Module:  ipcmutex.C
// Author:  Dick Lam
//
// Purpose: C++ class source file for ipcMutexSemaphore
//
// Notes:  This class is derived from ipcSemaphore.  It is an interface class
//         for mutex semaphores that can be used to control access to a shared
//         resource across processes or threads.
//
// ****************************************************************************

#include "ipcmutex.h"
#include "ossem.h"

// ****************************************************************************

// ipcMutexSemaphore - constructor

ipcMutexSemaphore::ipcMutexSemaphore(const char *name, ipcSemaphoreOp operation)
   : ipcSemaphore(name, mutex, operation)

{
   // check the state of the object
   if (myState != good)
      return;

   // create or open the semaphore
   if (operation == semcreate)
      myImpl->CreateMutex();
   else if (operation == semaccess)
      myImpl->OpenMutex();
}

// ----------------------------------------------------------------------------

// ~ipcMutexSemaphore - destructor

ipcMutexSemaphore::~ipcMutexSemaphore()

{
   // close the semaphore
   if (myState == good)
      myImpl->CloseMutex();
}

// ----------------------------------------------------------------------------

// Query - returns the number of requests made of the semaphore

unsigned long ipcMutexSemaphore::Query()

{
   if (myState == good)
      return myImpl->QueryMutex();

   return 0L;
}

// ----------------------------------------------------------------------------

// Request - requests the semaphore

void ipcMutexSemaphore::Request()

{
   if (myState == good)
      myImpl->RequestMutex();
}

// ----------------------------------------------------------------------------

// Release - releases the semaphore

void ipcMutexSemaphore::Release()

{
   if (myState == good)
      myImpl->ReleaseMutex();
}

// ****************************************************************************

// end of ipcmutex.C

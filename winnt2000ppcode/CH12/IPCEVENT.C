// ****************************************************************************
//
// Module:  ipcevent.C
// Author:  Dick Lam
//
// Purpose: C++ class source file for ipcEventSemaphore
//
// Notes:  This class is derived from ipcSemaphore.  It is an interface class
//         for event semaphores that can be used to signal events across
//         processes or threads.
//
// ****************************************************************************

#include "ipcevent.h"
#include "ossem.h"

// ****************************************************************************

// ipcEventSemaphore - constructor

ipcEventSemaphore::ipcEventSemaphore(const char *name, ipcSemaphoreOp operation)
   : ipcSemaphore(name, event, operation)

{
   // check the state of the object
   if (myState != good)
      return;

   // create or open the semaphore
   if (operation == semcreate)
      myImpl->CreateEvent();
   else if (operation == semaccess)
      myImpl->OpenEvent();
}

// ----------------------------------------------------------------------------

// ~ipcEventSemaphore - destructor

ipcEventSemaphore::~ipcEventSemaphore()

{
   // close the semaphore
   if (myState == good)
      myImpl->CloseEvent();
}

// ----------------------------------------------------------------------------

// Query - returns the number of requests made of the semaphore

unsigned long ipcEventSemaphore::Query()

{
   if (myState == good)
      return myImpl->QueryEvent();

   return 0L;
}

// ----------------------------------------------------------------------------

// Post - posts the semaphore

void ipcEventSemaphore::Post()

{
   if (myState == good)
      myImpl->PostEvent();
}

// ----------------------------------------------------------------------------

// Reset - resets the semaphore

void ipcEventSemaphore::Reset()

{
   if (myState == good)
      myImpl->ResetEvent();
}

// ----------------------------------------------------------------------------

// Wait - waits for a semaphore event to be posted

void ipcEventSemaphore::Wait()

{
   if (myState == good)
      myImpl->WaitEvent();
}

// ****************************************************************************

// end of ipcevent.C

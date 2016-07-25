// ****************************************************************************
//
// Module:  winsem.C
// Author:  Dick Lam
//
// Purpose: C++ class source file for osSemaphore
//
// Notes:  This is a base class.  It contains general implementation methods
//         for semaphores used in signalling between processes and threads.
//
// ****************************************************************************

#define STRICT
#include <windows.h>

// to avoid conflicts, the following 4 macros (defined in winbase.h) must
// be undefined
#ifdef CreateMutex
#undef CreateMutex
#endif

#ifdef OpenMutex
#undef OpenMutex
#endif

#ifdef CreateEvent
#undef CreateEvent
#endif

#ifdef OpenEvent
#undef OpenEvent
#endif

#include <string.h>
#include "ossem.h"


// ****************************************************************************

// osSemaphore - constructor

osSemaphore::osSemaphore(ipcSemaphore *interface, const char *name,
                         ipcSemaphoreType type, ipcSemaphoreOp operation)

{
   // init instance variables
   myInterface = interface;
   myName = 0;
   myID = 0L;
   myType = type;
   if (operation == semcreate)
      isOwner = 1;
   else
      isOwner = 0;

   // check that input name is valid
   if (!name || (strlen(name) == 0)) {
      myInterface->myState = ipcSemaphore::badname;
      return;
   }

   // allocate space for the name
   myName = new char[strlen(name) + 1];
   if (!myName) {
      myInterface->myState = ipcSemaphore::bad;
      return;
   }

   // and form the complete semaphore pathname
   strcpy(myName, name);

   // set the object state and return
   myInterface->myState = ipcSemaphore::good;
}

// ----------------------------------------------------------------------------

// ~osSemaphore - destructor

osSemaphore::~osSemaphore()

{
   delete myName;
}

// ----------------------------------------------------------------------------

// Name - returns the name of the semaphore

char *osSemaphore::Name() const

{
   return myName;
}

// ----------------------------------------------------------------------------

// ID - returns the semaphore id

unsigned long osSemaphore::ID() const

{
   return myID;
}

// ----------------------------------------------------------------------------

// Type - returns the type of semaphore

ipcSemaphoreType osSemaphore::Type() const

{
   return myType;
}

// ----------------------------------------------------------------------------

// Owner - returns 1 if this is the owner (creator), and 0 otherwise

int osSemaphore::Owner() const

{
   return isOwner;
}

// ----------------------------------------------------------------------------

// CreateMutex - creates a mutex semaphore

void osSemaphore::CreateMutex()

{
   HANDLE hsem = ::CreateMutexA(0, FALSE, (LPCTSTR)myName);
   if (!hsem)
      myInterface->myState = ipcSemaphore::bad;
   else
      myID = (unsigned long)hsem;
}

// ----------------------------------------------------------------------------

// OpenMutex - opens a mutex semaphore

void osSemaphore::OpenMutex()

{
   HANDLE hsem = ::OpenMutexA(MUTEX_ALL_ACCESS, FALSE, (LPCTSTR)myName);
   if (!hsem)
      myInterface->myState = ipcSemaphore::notfound;
   else
      myID = (unsigned long)hsem;
}

// ----------------------------------------------------------------------------

// RequestMutex - requests a mutex semaphore

void osSemaphore::RequestMutex()

{
   ::WaitForSingleObject( (HANDLE)myID, INFINITE );
}

// ----------------------------------------------------------------------------

// ReleaseMutex - releases a mutex semaphore

void osSemaphore::ReleaseMutex()

{
   ::ReleaseMutex( (HANDLE)myID );
}

// ----------------------------------------------------------------------------

// QueryMutex - queries a mutex semaphore

unsigned long osSemaphore::QueryMutex()

{
   switch (::WaitForSingleObject( (HANDLE)myID, 0 )) {
      case WAIT_OBJECT_0:
         return 1L;

      case WAIT_ABANDONED:
         return 1L;

      case WAIT_TIMEOUT:
         return 0L;

      default:
         return 0L;
   }
}

// ----------------------------------------------------------------------------

// CloseMutex - closes a mutex semaphore

void osSemaphore::CloseMutex()

{
   ::CloseHandle( (HANDLE)myID );
}

// ----------------------------------------------------------------------------

// CreateEvent - creates an event semaphore

void osSemaphore::CreateEvent()

{
   HANDLE hsem = ::CreateEventA(0, FALSE, FALSE, (LPCTSTR)myName);
   if (!hsem)
      myInterface->myState = ipcSemaphore::bad;
   else
      myID = (unsigned long)hsem;
}

// ----------------------------------------------------------------------------

// OpenEvent - opens an event semaphore

void osSemaphore::OpenEvent()

{
   HANDLE hsem = ::OpenEventA(EVENT_ALL_ACCESS, FALSE, (LPCTSTR)myName);
   if (!hsem)
      myInterface->myState = ipcSemaphore::notfound;
   else
      myID = (unsigned long)hsem;
}

// ----------------------------------------------------------------------------

// PostEvent - posts an event semaphore

void osSemaphore::PostEvent()

{
   ::SetEvent( (HANDLE)myID );
}

// ----------------------------------------------------------------------------

// ResetEvent - resets an event semaphore

void osSemaphore::ResetEvent()

{
   ::ResetEvent( (HANDLE)myID );
}

// ----------------------------------------------------------------------------

// WaitEvent - waits on an event semaphore

void osSemaphore::WaitEvent()

{
   ::WaitForSingleObject( (HANDLE)myID, INFINITE );
}

// ----------------------------------------------------------------------------

// QueryEvent - queries an event semaphore

unsigned long osSemaphore::QueryEvent()

{
   switch (::WaitForSingleObject( (HANDLE)myID, 0 )) {
      case WAIT_OBJECT_0:
         return 1L;

      case WAIT_ABANDONED:
         return 1L;

      case WAIT_TIMEOUT:
         return 0L;

      default:
         return 0L;
   }
}

// ----------------------------------------------------------------------------

// CloseEvent - closes an event semaphore

void osSemaphore::CloseEvent()

{
   ::CloseHandle( (HANDLE)myID );
}

// ****************************************************************************

// end of os2sem.C

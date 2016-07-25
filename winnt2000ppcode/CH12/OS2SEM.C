// ****************************************************************************
//
// Module:  os2sem.C
// Author:  Dick Lam
//
// Purpose: C++ class source file for osSemaphore
//
// Notes:  This is a base class.  It contains general implementation methods
//         for semaphores used in signalling between processes and threads.
//
// ****************************************************************************

#define INCL_DOS
#include <os2.h>

#include <string.h>
#include "ossem.h"

// ****************************************************************************

// init operating system-specific variables

const char *semPath = "\\SEM32\\";

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
   myName = new char[strlen(semPath) + strlen(name) + 1];
   if (!myName) {
      myInterface->myState = ipcSemaphore::bad;
      return;
   }

   // and form the complete semaphore pathname
   strcpy(myName, semPath);
   strcat(myName, name);

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
   if (::DosCreateMutexSem( (PSZ)myName, (PHMTX)&myID, 0L, 0 ) != 0)
      myInterface->myState = ipcSemaphore::bad;
}

// ----------------------------------------------------------------------------

// OpenMutex - opens a mutex semaphore

void osSemaphore::OpenMutex()

{
   myID = 0L;
   if (::DosOpenMutexSem( (PSZ)myName, (PHMTX)&myID) != 0)
      myInterface->myState = ipcSemaphore::notfound;
}

// ----------------------------------------------------------------------------

// RequestMutex - requests a mutex semaphore

void osSemaphore::RequestMutex()

{
   ::DosRequestMutexSem( (HMTX)myID, SEM_INDEFINITE_WAIT );
}

// ----------------------------------------------------------------------------

// ReleaseMutex - releases a mutex semaphore

void osSemaphore::ReleaseMutex()

{
   ::DosReleaseMutexSem( (HMTX)myID );
}

// ----------------------------------------------------------------------------

// QueryMutex - queries a mutex semaphore

unsigned long osSemaphore::QueryMutex()

{
   PID pOwner;
   TID tOwner;
   ULONG count;

   ::DosQueryMutexSem( (HMTX)myID, &pOwner, &tOwner, &count );
   return count;
}

// ----------------------------------------------------------------------------

// CloseMutex - closes a mutex semaphore

void osSemaphore::CloseMutex()

{
   ::DosCloseMutexSem( (HMTX)myID );
}

// ----------------------------------------------------------------------------

// CreateEvent - creates an event semaphore

void osSemaphore::CreateEvent()

{
   if (::DosCreateEventSem( (PSZ)myName, (PHEV)&myID, 0L, 0 ) != 0)
      myInterface->myState = ipcSemaphore::bad;
}

// ----------------------------------------------------------------------------

// OpenEvent - opens an event semaphore

void osSemaphore::OpenEvent()

{
   myID = 0L;
   if (::DosOpenEventSem( (PSZ)myName, (PHEV)&myID ) != 0)
      myInterface->myState = ipcSemaphore::notfound;
}

// ----------------------------------------------------------------------------

// PostEvent - posts an event semaphore

void osSemaphore::PostEvent()

{
   ::DosPostEventSem( (HEV)myID );
}

// ----------------------------------------------------------------------------

// ResetEvent - resets an event semaphore

void osSemaphore::ResetEvent()

{
   ULONG count;

   ::DosResetEventSem( (HEV)myID, &count );
}

// ----------------------------------------------------------------------------

// WaitEvent - waits on an event semaphore

void osSemaphore::WaitEvent()

{
   ::DosWaitEventSem( (HEV)myID, SEM_INDEFINITE_WAIT );
}

// ----------------------------------------------------------------------------

// QueryEvent - queries an event semaphore

unsigned long osSemaphore::QueryEvent()

{
   ULONG count;

   ::DosQueryEventSem( (HEV)myID, &count );
   return count;
}

// ----------------------------------------------------------------------------

// CloseEvent - closes an event semaphore

void osSemaphore::CloseEvent()

{
   ::DosCloseEventSem( (HEV)myID );
}

// ****************************************************************************

// end of os2sem.C

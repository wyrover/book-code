// ****************************************************************************
//
// Module:  aixsem.C
// Author:  Dick Lam
//
// Purpose: C++ class source file for osSemaphore
//
// Notes:  This is a base class.  It contains general implementation methods
//         for semaphores used in signalling between processes and threads.
//
// ****************************************************************************

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/mode.h>
#include "ossem.h"

// ****************************************************************************

// init operating system-specific variables

const char *semPath = "/tmp/";

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

   // create a file for later call to ftok
   if (isOwner) {
      if (::creat(myName, O_RDWR | O_CREAT | O_EXCL) == -1) {
         myInterface->myState = ipcSemaphore::badname;
         return;
      }
   }

   // set the object state and return
   myInterface->myState = ipcSemaphore::good;
}

// ----------------------------------------------------------------------------

// ~osSemaphore - destructor

osSemaphore::~osSemaphore()

{
   // if the process is the semaphore owner, delete the file and remove
   // the semaphore from the system
   if (isOwner && (myInterface->myState == ipcSemaphore::good)) {
      ::semctl( (int)myID, 0, IPC_RMID, 0);
      ::unlink(myName);
   }

   // delete the name
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
   // get key
   key_t semkey = ::ftok(myName, 'V');
   if (semkey == (key_t)-1) {
      myInterface->myState = ipcSemaphore::bad;
      return;
   }

   // and create the semaphore
   int newsem = ::semget(semkey, 1, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR |
                                    S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
   if (newsem == -1) {
      myInterface->myState = ipcSemaphore::bad;
      return;
   }

   myID = (unsigned long)newsem;
}

// ----------------------------------------------------------------------------

// OpenMutex - opens a mutex semaphore

void osSemaphore::OpenMutex()

{
   // get key
   key_t semkey = ::ftok(myName, 'V');
   if (semkey == (key_t)-1) {
      myInterface->myState = ipcSemaphore::bad;
      return;
   }

   // access an already existing semaphore
   int newsem = ::semget(semkey, 0, S_IRUSR | S_IWUSR | S_IRGRP |
                                    S_IWGRP | S_IROTH | S_IWOTH);
   if (newsem == -1) {
      myInterface->myState = ipcSemaphore::notfound;
      return;
   }

   myID = (unsigned long)newsem;
}

// ----------------------------------------------------------------------------

// RequestMutex - requests a mutex semaphore

void osSemaphore::RequestMutex()

{
   static struct sembuf request_op[2] = {{0, 0, 0},
                                         {0, 1, SEM_UNDO}};

   ::semop( (int)myID, request_op, 2 );
}

// ----------------------------------------------------------------------------

// ReleaseMutex - releases a mutex semaphore

void osSemaphore::ReleaseMutex()

{
   static struct sembuf release_op = {0, -1, IPC_NOWAIT | SEM_UNDO};

   ::semop( (int)myID, &release_op, 1 );
}

// ----------------------------------------------------------------------------

// QueryMutex - queries a mutex semaphore

unsigned long osSemaphore::QueryMutex()

{
   unsigned short values[1];

   ::semctl( (int)myID, 0, GETALL, values );
   return (unsigned long)values[0];
}

// ----------------------------------------------------------------------------

// CloseMutex - closes a mutex semaphore

void osSemaphore::CloseMutex()

{
}

// ----------------------------------------------------------------------------

// CreateEvent - creates an event semaphore

void osSemaphore::CreateEvent()

{
   // get key
   key_t semkey = ::ftok(myName, 'V');
   if (semkey == (key_t)-1) {
      myInterface->myState = ipcSemaphore::bad;
      return;
   }

   // and create the semaphore
   int newsem = ::semget(semkey, 1, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR |
                                    S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
   if (newsem == -1) {
      myInterface->myState = ipcSemaphore::bad;
      return;
   }

   myID = (unsigned long)newsem;
}

// ----------------------------------------------------------------------------

// OpenEvent - opens an event semaphore

void osSemaphore::OpenEvent()

{
   // get key
   key_t semkey = ::ftok(myName, 'V');
   if (semkey == (key_t)-1) {
      myInterface->myState = ipcSemaphore::bad;
      return;
   }

   // access an already existing semaphore
   int newsem = ::semget(semkey, 0, S_IRUSR | S_IWUSR | S_IRGRP |
                                    S_IWGRP | S_IROTH | S_IWOTH);
   if (newsem == -1) {
      myInterface->myState = ipcSemaphore::notfound;
      return;
   }

   myID = (unsigned long)newsem;
}

// ----------------------------------------------------------------------------

// PostEvent - posts an event semaphore

void osSemaphore::PostEvent()

{
   static struct sembuf post_op = {0, -1, IPC_NOWAIT | SEM_UNDO};

   ::semop( (int)myID, &post_op, 1 );
}

// ----------------------------------------------------------------------------

// ResetEvent - resets an event semaphore

void osSemaphore::ResetEvent()

{
   static struct sembuf reset_op = {0, 1, SEM_UNDO};

   ::semop( (int)myID, &reset_op, 1 );
}

// ----------------------------------------------------------------------------

// WaitEvent - waits on an event semaphore

void osSemaphore::WaitEvent()

{
   static struct sembuf wait_op = {0, 0, 0};

   ::semop( (int)myID, &wait_op, 1 );
}

// ----------------------------------------------------------------------------

// QueryEvent - queries an event semaphore

unsigned long osSemaphore::QueryEvent()

{
   unsigned short values[1];

   ::semctl( (int)myID, 0, GETALL, values );
   return (unsigned long)values[0];
}

// ----------------------------------------------------------------------------

// CloseEvent - closes an event semaphore

void osSemaphore::CloseEvent()

{
}

// ****************************************************************************

// end of aixsem.C

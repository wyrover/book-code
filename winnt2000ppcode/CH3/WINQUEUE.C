// ****************************************************************************
//
// Module:  winqueue.C
// Author:  Dick Lam
//
// Purpose: C++ class source file for osMessageQueue
//
// Notes:  This is a base class.  It contains general implementation methods
//         for message queues for sending messages between processes.
//
// ****************************************************************************

#define STRICT
#include <windows.h>

#include <string.h>
#include "ipcevent.h"
#include "osqueue.h"

// ****************************************************************************

// init operating system-specific variables

const char *queuePath = "\\\\.\\mailslot\\";    // which equals "\\.\mailslot\"

// ****************************************************************************

// osMessageQueue - constructor

osMessageQueue::osMessageQueue(ipcMessageQueue *interface, const char *name)

{
   // init instance variables
   myInterface = interface;
   myPid = (unsigned long)GetCurrentProcessId();
   myID = 0L;
   myName = 0;
   isOwner = 1;
   mySem = 0;

   // check that input name is valid
   if (!name || (strlen(name) == 0)) {
      myInterface->myState = ipcMessageQueue::badname;
      return;
   }

   // allocate space for the name
   myName = new char[strlen(queuePath) + strlen(name) + 1];
   if (!myName) {
      myInterface->myState = ipcMessageQueue::bad;
      return;
   }

   // and form the complete message queue pathname
   strcpy(myName, queuePath);
   strcat(myName, name);

   // set the object state and create the queue
   myInterface->myState = ipcMessageQueue::good;
   CreateQueue();
}

// ----------------------------------------------------------------------------

// osMessageQueue - constructor

osMessageQueue::osMessageQueue(ipcMessageQueue *interface, const char *name,
                               unsigned long powner)

{
   // init instance variables
   myInterface = interface;
   myPid = powner;
   myID = 0L;
   myName = 0;
   isOwner = 0;
   mySem = 0;

   // check that input name is valid
   if (!name || (strlen(name) == 0)) {
      myInterface->myState = ipcMessageQueue::badname;
      return;
   }

   // allocate space for the name
   myName = new char[strlen(queuePath) + strlen(name) + 1];
   if (!myName) {
      myInterface->myState = ipcMessageQueue::bad;
      return;
   }

   // and form the complete message queue pathname
   strcpy(myName, queuePath);
   strcat(myName, name);

   // set the object state and open the queue
   myInterface->myState = ipcMessageQueue::good;
   OpenQueue();
}

// ----------------------------------------------------------------------------

// ~osMessageQueue - destructor

osMessageQueue::~osMessageQueue()

{
   // see if message queue was opened
   if (myInterface->myState == ipcMessageQueue::good) {
      // close the queue
      CloseQueue();
   }

   // and delete the queue name and semaphore
   delete myName;
   delete mySem;
}

// ----------------------------------------------------------------------------

// Name - returns the name of the queue

char *osMessageQueue::Name() const

{
   return myName;
}

// ----------------------------------------------------------------------------

// ID - returns the ID of the queue

unsigned long osMessageQueue::ID() const

{
   return myID;
}

// ----------------------------------------------------------------------------

// Pid - returns the process id of the queue's owner

unsigned long osMessageQueue::Pid() const

{
   return myPid;
}

// ----------------------------------------------------------------------------

// Owner - returns 1 if this is the owner (creator), and 0 otherwise

int osMessageQueue::Owner() const

{
   return isOwner;
}

// ----------------------------------------------------------------------------

// Read - reads data from the queue (owner only)

int osMessageQueue::Read(void *data, long datasize, int wait)

{
   // make sure this is the queue owner and not a client
   if (!isOwner)
      return ipcMessageQueue::notowner;

   // make sure arguments are valid
   if (!data || (datasize == 0L))
      return ipcMessageQueue::badargument;

   // and read the queue message
   DWORD bytes;
   BOOL rc;

   if (wait) {
      do {
         rc = ReadFile( (HANDLE)myID, (LPVOID)data, (DWORD)datasize,
                        &bytes, 0 );
      } while (rc == FALSE);
   } else {
      rc = ReadFile( (HANDLE)myID, (LPVOID)data, (DWORD)datasize, &bytes, 0 );
   }

   if (rc)
      return ipcMessageQueue::readerror;

   return ipcMessageQueue::good;
}

// ----------------------------------------------------------------------------

// Write - writes data to the queue (clients only)

int osMessageQueue::Write(void *data, long datasize)

{
   // make sure this is a client and not the queue owner
   if (isOwner)
      return ipcMessageQueue::notclient;

   // make sure arguments are valid
   if (!data || (datasize == 0L))
      return ipcMessageQueue::badargument;

   // write data into the queue
   DWORD cbwritten;

   BOOL rc = WriteFile( (HANDLE)myID, (CONST VOID *)data, (DWORD)datasize,
                        &cbwritten, (LPOVERLAPPED)0 );
   if (!rc || (cbwritten != (DWORD)datasize))
      return ipcMessageQueue::writeerror;

   return ipcMessageQueue::good;
}

// ----------------------------------------------------------------------------

// Peek - returns the number of waiting messages in the queue

unsigned long osMessageQueue::Peek()

{
   // make sure this is the queue owner and not a client
   if (!isOwner)
      return 0L;

   DWORD maxsize, nextsize, timeout, nmsgs = 0L;
   BOOL rc = GetMailslotInfo( (HANDLE)myID, &maxsize, &nextsize,
                              &nmsgs, &timeout );
   if (!rc)
      return 0L;

   return (unsigned long)nmsgs;
}

// ----------------------------------------------------------------------------

// Purge - purges the queue of all messages

int osMessageQueue::Purge()

{
   // make sure this is the queue owner and not a client
   if (!isOwner)
      return ipcMessageQueue::notowner;


   // set up variables for querying and reading the queue
   DWORD maxsize, nextsize, timeout, nmsgs, bytes;
   void *data;
   BOOL rc;

   // loop to query number of messages left and size of next message
   do {
      nmsgs = 0L;
      rc = GetMailslotInfo( (HANDLE)myID, &maxsize, &nextsize,
                            &nmsgs, &timeout );
      if (!rc)
         return ipcMessageQueue::good;

      if (nmsgs > 0L) {
         // there is a message so allocate memory for it
         data = (void *)new char[nextsize];
         if (!data)
            return ipcMessageQueue::readerror;

         // read the message
         ReadFile( (HANDLE)myID, (LPVOID)data, nextsize, &bytes, 0 );

         // delete memory allocated for message
         delete data;
      }
   } while (nmsgs > 0L);

   return ipcMessageQueue::good;
}

// ----------------------------------------------------------------------------

// CreateQueue - creates a new message queue

void osMessageQueue::CreateQueue()

{
   // create the queue
   HANDLE hq = CreateMailslot( (LPTSTR)myName, 0L, 0L, 0 );
   if (hq == INVALID_HANDLE_VALUE) {
      myInterface->myState = ipcMessageQueue::bad;
      return;
   }

   myID = (unsigned long)hq;
}

// ----------------------------------------------------------------------------

// OpenQueue - opens an existing message queue

void osMessageQueue::OpenQueue()

{
   // access the queue by opening a mailslot file
   HANDLE hq = CreateFile( (LPTSTR)myName, GENERIC_WRITE,
                           FILE_SHARE_READ, 0, OPEN_EXISTING,
                           FILE_ATTRIBUTE_NORMAL, (HANDLE)0 );
   if (hq == INVALID_HANDLE_VALUE) {
      myInterface->myState = ipcMessageQueue::bad;
      return;
   }

   myID = (unsigned long)hq;
}

// ----------------------------------------------------------------------------

// CloseQueue - closes a message queue

void osMessageQueue::CloseQueue()

{
   // make sure the queue was opened ok
   if ( myInterface->myState == ipcMessageQueue::good) {
      // if this is the queue owner, purge it - then delete the queue
      if (isOwner)
         Purge();
      CloseHandle( (HANDLE)myID );
   }
}

// ****************************************************************************

// end of winqueue.C

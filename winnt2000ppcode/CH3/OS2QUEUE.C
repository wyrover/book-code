// ****************************************************************************
//
// Module:  os2queue.C
// Author:  Dick Lam
//
// Purpose: C++ class source file for osMessageQueue
//
// Notes:  This is a base class.  It contains general implementation methods
//         for message queues for sending messages between processes.
//
// ****************************************************************************

#define INCL_DOS
#include <os2.h>

#include <string.h>
#include "ipcevent.h"
#include "osqueue.h"

// ****************************************************************************

// init operating system-specific variables

const char *queuePath = "\\QUEUES\\";

// ****************************************************************************

// osMessageQueue - constructor

osMessageQueue::osMessageQueue(ipcMessageQueue *interface, const char *name)

{
   // init instance variables
   myInterface = interface;
   myPid = 0L;
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

   // determine the process id
   PTIB ptib;
   PPIB ppib;

   DosGetInfoBlocks(&ptib, &ppib);
   myPid = (unsigned long)ppib->pib_ulpid;

   // create an event semaphore
   mySem = new ipcEventSemaphore(name, semcreate);
   if (!mySem) {
      myInterface->myState = ipcMessageQueue::bad;
      return;
   }

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

   // and form the complete shared memory pathname
   strcpy(myName, queuePath);
   strcat(myName, name);

   // access the queue server's event semaphore
   mySem = new ipcEventSemaphore(name, semaccess);
   if (!mySem) {
      myInterface->myState = ipcMessageQueue::bad;
      return;
   }

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

   // initialize variables for read
   REQUESTDATA requestdata;
   BYTE elempriority;
   ULONG rc, elemcode = 0L, datalen = sizeof(PVOID);
   HEV hsem = (HEV)mySem->ID();
   PVOID mem;

   requestdata.pid = myPid;

   // reset the event semaphore before trying a read
   mySem->Reset();

   // and read the queue message
   if (wait)
      rc = DosReadQueue( (HQUEUE)myID, &requestdata, &datalen, &mem,
                         elemcode, (BOOL32)DCWW_WAIT, &elempriority, hsem );
   else
      rc = DosReadQueue( (HQUEUE)myID, &requestdata, &datalen, &mem,
                         elemcode, (BOOL32)DCWW_NOWAIT, &elempriority, hsem );
   if (rc != 0)
      return ipcMessageQueue::readerror;

   // copy data from shared memory to return buffer
   memcpy(data, mem, requestdata.ulData);

   // clean up and return
   DosFreeMem(mem);
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

   // set up shared memory block for the data
   PVOID mem;

   ULONG rc = DosAllocSharedMem(&mem, 0, datasize, fALLOC | OBJ_GIVEABLE);
   if (rc != 0)
      return ipcMessageQueue::writeerror;

   // grant access to shared memory to the owner of the queue
   rc = DosGiveSharedMem(mem, (PID)myPid, fPERM);
   if (rc != 0) {
      DosFreeMem(mem);
      return ipcMessageQueue::writeerror;
   }
   ::memcpy(mem, data, datasize);   // copy data into shared memory

   // write pointer to shared memory into the queue, and pass the size of
   // the shared memory block as the request data
   rc = DosWriteQueue( (HQUEUE)myID, datasize, sizeof(PVOID), mem, 0L );
   if (rc != 0) {
      DosFreeMem(mem);
      return ipcMessageQueue::writeerror;
   }

   // clean up and return
   DosFreeMem(mem);             // free the shared memory block
   return ipcMessageQueue::good;
}

// ----------------------------------------------------------------------------

// Peek - returns the number of waiting messages in the queue

unsigned long osMessageQueue::Peek()

{
   // make sure this is the queue owner and not a client
   if (!isOwner)
      return 0L;

   ULONG nmsgs = 0L;

   DosQueryQueue( (HQUEUE)myID, &nmsgs );
   return nmsgs;
}

// ----------------------------------------------------------------------------

// Purge - purges the queue of all messages

int osMessageQueue::Purge()

{
   // make sure this is the queue owner and not a client
   if (!isOwner)
      return ipcMessageQueue::notowner;

   DosPurgeQueue( (HQUEUE)myID );
   return ipcMessageQueue::good;
}

// ----------------------------------------------------------------------------

// CreateQueue - creates a new message queue

void osMessageQueue::CreateQueue()

{
   // create the queue
   HQUEUE hq;
   ULONG rc = DosCreateQueue(&hq, QUE_FIFO, (PSZ)myName);
   if (rc != 0) {
      myInterface->myState = ipcMessageQueue::bad;
      return;
   }

   myID = (unsigned long)hq;
}

// ----------------------------------------------------------------------------

// OpenQueue - opens an existing shared memory block

void osMessageQueue::OpenQueue()

{
   // access the queue
   ULONG rc = DosOpenQueue( (PPID)&myPid, (PHQUEUE)&myID, (PSZ)myName );
   if (rc != 0) {
      myID = 0L;
      myInterface->myState = ipcMessageQueue::bad;
      return;
   }
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
      DosCloseQueue( (HQUEUE)myID );
   }
}

// ****************************************************************************

// end of os2queue.C

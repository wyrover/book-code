// ****************************************************************************
//
// Module:  aixqueue.C
// Author:  Dick Lam
//
// Purpose: C++ class source file for osMessageQueue
//
// Notes:  This is a base class.  It contains general implementation methods
//         for message queues for sending messages between processes.
//
// ****************************************************************************

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/mode.h>
#include "ipcevent.h"
#include "osqueue.h"

// ****************************************************************************

// init operating system-specific variables

const char *queuePath = "/tmp/";

// ****************************************************************************

// osMessageQueue - constructor

osMessageQueue::osMessageQueue(ipcMessageQueue *interface, const char *name)

{
   // init instance variables
   myInterface = interface;
   myPid = (unsigned long)::getpid();
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

   // create a file for later call to ftok
   if (::creat(myName, O_RDWR | O_CREAT | O_EXCL) == -1) {
      myInterface->myState = ipcMessageQueue::badname;
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

      // if the process is the queue owner, delete the file
      if (isOwner)
         ::unlink(myName);
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

   // allocate memory for msgbuf
   struct msgbuf *buffer = (struct msgbuf *)new char[sizeof(struct msgbuf)
                                                     + datasize];
   if (!buffer)
      return ipcMessageQueue::readerror;

   // read a message from the queue
   int rc;

   if (wait)
      rc = msgrcv( (int)myID, (void *)buffer, (int)datasize, 0, MSG_NOERROR );
   else
      rc = msgrcv( (int)myID, (void *)buffer, (int)datasize, 0,
                   IPC_NOWAIT | MSG_NOERROR );
   if (rc == -1) {
      delete buffer;
      return ipcMessageQueue::readerror;
   }

   // copy data from buffer into return data structure, clean up and return
   ::memcpy(data, (void *)buffer->mtext, (size_t)datasize);
   delete buffer;
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

   // allocate memory for msgbuf
   struct msgbuf *buffer = (struct msgbuf *)new char[sizeof(struct msgbuf)
                                                     + datasize];
   if (!buffer)
      return ipcMessageQueue::writeerror;

   // copy data into msgbuf
   buffer->mtype = 1L;
   ::memcpy( (void *)buffer->mtext, data, (size_t)datasize );

   // write buffer to the queue
   int rc = msgsnd( (int)myID, (void *)buffer, (size_t)datasize, IPC_NOWAIT );
   if (rc == -1) {
      delete buffer;
      return ipcMessageQueue::writeerror;
   }

   // clean up and return
   delete buffer;
   return ipcMessageQueue::good;
}

// ----------------------------------------------------------------------------

// Peek - returns the number of waiting messages in the queue

unsigned long osMessageQueue::Peek()

{
   // make sure this is the queue owner and not a client
   if (!isOwner)
      return 0L;

   unsigned long nmsgs = 0L;
   struct msqid_ds status;

   msgctl( (int)myID, IPC_STAT, &status );
   nmsgs = (unsigned long)status.msg_qnum;

   return nmsgs;
}

// ----------------------------------------------------------------------------

// Purge - purges the queue of all messages

int osMessageQueue::Purge()

{
   // make sure this is the queue owner and not a client
   if (!isOwner)
      return ipcMessageQueue::notowner;

   char buffer[1];

   // read all queue messages until none are left
   while (Peek() > 0L) {
      if (Read( (void *)buffer, 1L, 0) != ipcMessageQueue::good)
         return ipcMessageQueue::readerror;
   }

   return ipcMessageQueue::good;
}

// ----------------------------------------------------------------------------

// CreateQueue - creates a new message queue

void osMessageQueue::CreateQueue()

{
   // get key
   key_t qkey = ::ftok(myName, 'V');
   if (qkey == (key_t)-1) {
      myInterface->myState = ipcMessageQueue::bad;
      return;
   }

   // create the message queue
   int newqueue = msgget(qkey, IPC_CREAT | S_IRUSR | S_IWUSR |
                                           S_IRGRP | S_IWGRP |
                                           S_IROTH | S_IWOTH);
   if (newqueue == -1) {
      myInterface->myState = ipcMessageQueue::bad;
      unlink(myName);
      return;
   }

   myID = (unsigned long)newqueue;
}

// ----------------------------------------------------------------------------

// OpenQueue - opens an existing message queue block

void osMessageQueue::OpenQueue()

{
   // get key
   key_t qkey = ::ftok(myName, 'V');
   if (qkey == (key_t)-1) {
      myInterface->myState = ipcMessageQueue::bad;
      return;
   }

   // access an already existing message queue
   int newqueue = msgget(qkey, S_IRUSR | S_IWUSR |
                               S_IRGRP | S_IWGRP |
                               S_IROTH | S_IWOTH);
   if (newqueue == -1) {
      myInterface->myState = ipcMessageQueue::notfound;
      return;
   }

   myID = (unsigned long)newqueue;
}

// ----------------------------------------------------------------------------

// CloseQueue - closes a message queue

void osMessageQueue::CloseQueue()

{
   // make sure the queue was opened ok
   if ( myInterface->myState == ipcMessageQueue::good) {
      // if this is the queue owner, delete the queue
      if (isOwner)
         msgctl( (int)myID, IPC_RMID, 0 );
   }
}

// ****************************************************************************

// end of aixqueue.C

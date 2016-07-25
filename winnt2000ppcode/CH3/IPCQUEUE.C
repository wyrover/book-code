// ****************************************************************************
//
// Module:  ipcqueue.C
// Author:  Dick Lam
//
// Purpose: C++ class source file for ipcMessageQueue
//
// Notes:  This is a base class.  It is the interface class for
//         creating and accessing a message queue that handles messages
//         between processes.
//
// ****************************************************************************

#include "ipcqueue.h"
#include "osqueue.h"

// ****************************************************************************

// ipcMessageQueue - constructor for server

ipcMessageQueue::ipcMessageQueue(const char *name)

{
   // init instance variables
   myState = good;
   myImpl = new osMessageQueue(this, name);
   if (!myImpl)
      myState = bad;
}

// ----------------------------------------------------------------------------

// ipcMessageQueue - constructor for clients

ipcMessageQueue::ipcMessageQueue(const char *name, unsigned long powner)

{
   // init instance variables
   myState = good;
   myImpl = new osMessageQueue(this, name, powner);
   if (!myImpl)
      myState = bad;
}

// ----------------------------------------------------------------------------

// ~ipcMessageQueue - destructor

ipcMessageQueue::~ipcMessageQueue()

{
   delete myImpl;
}

// ----------------------------------------------------------------------------

// Name - returns the name of the queue

char *ipcMessageQueue::Name() const

{
   if (!myImpl)
      return 0;

   return myImpl->Name();
}

// ----------------------------------------------------------------------------

// ID - returns the queue id

unsigned long ipcMessageQueue::ID() const

{
   if (!myImpl)
      return 0L;

   return myImpl->ID();
}

// ----------------------------------------------------------------------------

// Pid - returns the process id of the Queue owner

unsigned long ipcMessageQueue::Pid() const

{
   if (!myImpl)
      return 0L;

   return myImpl->Pid();
}

// ----------------------------------------------------------------------------

// Owner - returns 1 if this is the owner (creator), and 0 otherwise

int ipcMessageQueue::Owner() const

{
   if (!myImpl)
      return 0;

   return myImpl->Owner();
}

// ----------------------------------------------------------------------------

// Read - reads a message from the queue (queue owner only)

int ipcMessageQueue::Read(void *data, long datasize, int wait)

{
   if (!myImpl)
      return bad;

   return myImpl->Read(data, datasize, wait);
}

// ----------------------------------------------------------------------------

// Write - writes a message to the queue (queue clients only)

int ipcMessageQueue::Write(void *data, long datasize)

{
   if (!myImpl)
      return bad;

   return myImpl->Write(data, datasize);
}

// ----------------------------------------------------------------------------

// Peek - returns the number of entries in the queue

unsigned long ipcMessageQueue::Peek()

{
   if (!myImpl)
      return 0L;

   return myImpl->Peek();
}

// ----------------------------------------------------------------------------

// Purge - removes all entries from the queue

int ipcMessageQueue::Purge()

{
   if (!myImpl)
      return bad;

   return myImpl->Purge();
}

// ****************************************************************************

// end of ipcqueue.C

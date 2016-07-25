// ****************************************************************************
//
// Module:  ipcmutex.h
// Author:  Dick Lam
//
// Purpose: C++ class header file for ipcMutexSemaphore
//
// Notes:  This class is derived from ipcSemaphore.  It is an interface class
//         for mutex semaphores that can be used to control access to a shared
//         resource across processes or threads.
//
// ****************************************************************************

#ifndef MODULE_ipcMutexSemaphoreh
#define MODULE_ipcMutexSemaphoreh

#include "ipcsem.h"

// class declaration
class EXPORT ipcMutexSemaphore : public ipcSemaphore {

public:

   // constructor and destructor
   ipcMutexSemaphore(const char *name, ipcSemaphoreOp operation = semcreate);
   virtual ~ipcMutexSemaphore();

   // query method for number of requests made
   virtual unsigned long Query();

   // request and release methods (to lock and unlock resources)
   virtual void Request();
   virtual void Release();

private:

   // private copy constructor and operator= (define these and make them
   // public to enable copy and assignment of the class)
   ipcMutexSemaphore(const ipcMutexSemaphore&);
   ipcMutexSemaphore& operator=(const ipcMutexSemaphore&);

};

#endif

// ****************************************************************************

// end of ipcmutex.h

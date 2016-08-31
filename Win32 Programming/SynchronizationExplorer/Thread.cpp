#include "stdafx.h"
#include "SynchronizationExplorer.h"
#include "afxmt.h" // multithreading capabilities
#include "thread.h"
#include "uwm.h"
#include "IntEdit.h"
#include "worker.h"
#include "stdlib.h"

CCriticalSection CThread::lockLock;
BOOL CThread:: locking = TRUE;

int CThread::n = 0;
CCriticalSection CThread::nLock;

int CThread::delay = 100;  // shared speed is 100ms delay by default
BOOL CThread::random = FALSE; // use (TRUE) or don't use (FALSE) random times
CCriticalSection CThread::delayLock;

int CThread::messaging = MESSAGING_ASYNCHRONOUS;
CCriticalSection CThread::messagingLock;


/****************************************************************************
*                              CThread::CThread
* Effect: 
*       Constructor for our local thread worker class
****************************************************************************/

CThread::CThread()
    {
     state = STATE_LOCK;  // make sure we are in correct initial state!
     live = TRUE;
     shuttingdown = FALSE;
     mode = MODE_SIMULATED;
     threadID = NULL;
     hthread = NULL;
     hwnd = NULL;
     suspending = FALSE;
    }

/****************************************************************************
*                              CThread::~CThread
* Effect: 
*       Destructor
****************************************************************************/

CThread::~CThread()
    {
     if(hthread != NULL)
        CloseHandle(hthread);
    }

/****************************************************************************
*                                CThread::work
* Result: void
*       
* Effect: 
*       This is the computation loop for the example.
****************************************************************************/

void CThread::work()
    {
     int sum;
     BOOL dolock;
     int mydelay;
     BOOL israndom;
     int param;
     int next_state; 
     int thismode;
     int thismessaging;
     BOOL dosleep;  // add sleep for this step?

     while(TRUE)
        { /* while alive */
         BOOL alive;
         BOOL quit;

         liveLock.Lock();
         alive = live;
         liveLock.Unlock();
         if(!alive)
            break;  // exit loop.  Note we don't guarantee unlock in this case!

         dosleep = TRUE;  // assume we'll sleep

         switch(state)
            { /* next state */
             case STATE_LOCK:
                     /* This is a clean point to shut down */
                     shutdownLock.Lock();
                     quit = shuttingdown;
                     shutdownLock.Unlock();
                     if(quit)
                        { /* terminate execution */
                         liveLock.Lock();
                         alive = FALSE;
                         liveLock.Unlock();
                         continue;  // exit as we go around again
                        } /* terminate execution */
                     /*                        
                        We want the value of the lock to be the same on entry
                        and exit from our simulated loop.  To do this, we must
                        make a local copy once, at the start of the iteration.
                        Otherwise, it could be changed by the control panel
                        while we're "running" and we would get inconsistent
                        results.  Since we must always pass through the state
                        STATE_LOCK on each iteration, this is guaranteed to be
                        correct.  In particular, STATE_LOCK is always the
                        initial state
                     */

                     lockLock.Lock();
                     dolock = locking;
                     lockLock.Unlock();

                     next_state = STATE_FETCH;
                     param = dolock;  
                     dosleep = dolock;
                     break;
             case STATE_FETCH:
                     sum = n;
                     next_state = STATE_INCREMENT;
                     param = sum;
                     break;
             case STATE_INCREMENT:
                     sum++;
                     next_state = STATE_STORE;
                     param = sum;
                     break;
             case STATE_STORE:
                     n = sum;
                     next_state = STATE_UNLOCK;
                     param = 0;
                     break;
             case STATE_UNLOCK:
                     next_state = STATE_LOCK;
                     param = dolock;
                     dosleep = dolock; // sleep if this is a real step
            } /* next state */

         // Report current state
         modeLock.Lock();
         thismode = mode;
         modeLock.Unlock();

         delayLock.Lock();
         mydelay = delay;
         israndom = random;
         delayLock.Unlock();

         if(mydelay == 0)
            { /* free running */
             thismode = MODE_FREE;
            } /* free running */

         switch(thismode)
            { /* mode */
             case MODE_FREE:
                     // Avoid queue cram: massive queue length with not
                     // work in high-speed mode
                     ::SendMessage(hwnd, UWM_STATE, state, param);
                     break;
             case MODE_SIMULATED:
             case MODE_SINGLE:
                     messagingLock.Lock();
                     thismessaging = messaging;
                     messagingLock.Unlock();

                     switch(thismessaging)
                        { /* messaging style */
                         case MESSAGING_ASYNCHRONOUS:
                                 ::PostMessage(hwnd, UWM_STATE, state, param);
                                 break;
                         case MESSAGING_SYNCHRONOUS:
                                 ::SendMessage(hwnd, UWM_STATE, state, param);
                                 break;
                        } /* messaging style */
                     break;
            } /* mode */

          // Handle locking based on the state

          switch(state)
             { /* state */
              case STATE_LOCK:
                      if(!dolock)
                         break;
                      nLock.Lock();  // lock the variable n
                      break;
              case STATE_FETCH:
                      break;
              case STATE_INCREMENT:
                      break;
              case STATE_STORE:
                      break;
              case STATE_UNLOCK:
                     if(!dolock)
                        break;
                     nLock.Unlock(); // unlock the variable n
                     break;
            } /* state */

         // We next determine what mode we are in.  The possible modes are:
         //     - free-running: Execute as quickly as possible
         //     - simulated time: Delay by an amount based on the control panel
         //     - single step: wait for user to click Step button

         suspendLock.Lock();
         BOOL dosuspend = suspending;
         suspendLock.Unlock();

         // If we are in a simulated suspend, change the local copy of the
         // mode to 'single'

         if(dosuspend)
            { /* suspending */
             thismode = MODE_SINGLE;
             suspendLock.Lock();
             suspending = FALSE;
             suspendLock.Unlock();
            } /* suspending */

         switch(thismode)
            { /* mode */
             case MODE_FREE:
                     Sleep(1); // force possible yield for versimillitude
                     break;
             case MODE_SIMULATED:
                     if(!dosleep)
                        { /* no sleep */
                         Sleep(1);  // force possible yield
                         break;
                        } /* no sleep */

                     if(israndom)
                        { /* random sleep */
                         int lower = mydelay / 2;
                         int upper = 3 * mydelay / 2;
                         int range = upper - lower;
                         int delta;
                         if(range == 0)
                            delta = 0;
                         else
                            delta = rand() % range;
                         Sleep(lower + delta);
                        } /* random sleep */
                     else
                        { /* uniform sleep */
                         Sleep(mydelay);
                        } /* uniform sleep */
                     break;
             case MODE_SINGLE:
                     ::PostMessage(hwnd, UWM_WAITING, 0, 0);
                     ::SuspendThread(hthread);
                     ::PostMessage(hwnd, UWM_RUNNING, 0, 0);
                     break;
            } /* mode */

         state = next_state;
        } /* while alive */

    }

/****************************************************************************
*                           CThread::workerFunction
* Inputs:
*       LPVOID lParam: Arbitrary 32-bit pointer, (LPVOID)(CThread *)
* Result: long
*       Value which can be checked by caller on thread termination
*       0, always (we don't care)
* Effect: 
*       The worker thread.  
* Notes:
*       This is a 'static' function and takes a CThread parameter as its
*       optional parameter.  It is then able to call a real member function
*       so that access is available to all of the appropriate member values.
****************************************************************************/

long WINAPI CThread::workerFunction(LPVOID lParam)
    {
     CThread * thisThread = (CThread *)lParam;
     ::DuplicateHandle(
                        ::GetCurrentProcess(),  // source process
                        ::GetCurrentThread(),   // (pseudo) handle to duplicate
                        ::GetCurrentProcess(),  // target process
                        &thisThread->hthread,   // place to put result
                        0,                      // access: ignored
                        FALSE,                  // no need to inherit
                        DUPLICATE_SAME_ACCESS); // no change in access
     thisThread->threadID = ::GetCurrentThreadId();

     // Note that we CANNOT use the CSingleLock class because it is deeply
     // broken (study the effect of the m_bAcquired flag in the case where
     // we get to this event before the parent thread has a chance to lock
     // it!)
     ::SetEvent(CWorker::event.m_hObject);
     ::PostMessage(thisThread->hwnd, UWM_RUNNING, 0, 0);

#ifdef _DEBUG
     {
      CString msg;
      msg.Format(_T("0x%02x: Starting thread 0x%x 0x%08x\r\n"), 
                                GetCurrentThreadId(),
                                thisThread->threadID,
                                thisThread->hthread);
      OutputDebugString(msg);
     }
#endif

     // Now go do the actual work

     thisThread->work();


     // Close our duplicate handle so the thread will go away

     CloseHandle(thisThread->hthread);

#ifdef _DEBUG
     {
      CString msg;
      msg.Format(_T("0x%02x: Thread 0x%x completed [0x%08x]\r\n"), 
                        GetCurrentThreadId(),
                        thisThread->threadID,
                        thisThread->hthread);
      OutputDebugString(msg);
     }
#endif

     return 0;
    }

/****************************************************************************
*                                CThread::kill
* Result: void
*       
* Effect: 
*       Kills the thread "instantly" by simulating the effects of 
*       ::TerminateThread
****************************************************************************/

void CThread::kill()
    {
#ifdef _DEBUG
     {
      CString msg;
      msg.Format(_T("0x%02x: => CThread::kill\r\n"), GetCurrentThreadId());
      OutputDebugString(msg);
     }
#endif
     liveLock.Lock();
     live = FALSE;
     liveLock.Unlock();
#ifdef _DEBUG
     {
      CString msg;
      msg.Format(_T("0x%02x: <= CThread::kill\r\n"), GetCurrentThreadId());
      OutputDebugString(msg);
     }
#endif
    }

/****************************************************************************
*                              CThread::suspend
* Result: void
*       
* Effect: 
*       Simulates the effect of suspending a thread
****************************************************************************/

void CThread::suspend()
    {
     suspendLock.Lock();
     suspending = TRUE;
     suspendLock.Unlock();
    }

/****************************************************************************
*                              CThread::setLock
* Inputs:
*       BOOL newlock: New lock mode to set
* Result: BOOL
*       Previous lock value
* Effect: 
*       Changes the lock state
****************************************************************************/

BOOL CThread::setLock(BOOL newlock)
    {
     BOOL oldlock;

     lockLock.Lock();
        oldlock = locking;
        locking = newlock;
     lockLock.Unlock();

     return oldlock;
    }

/****************************************************************************
*                             CThread::setRandom
* Inputs:
*       BOOL newrand: New randomize boolean value: TRUE to randomize,
                      FALSE to usen uniform timing
* Result: BOOL
*       Previous random setting
* Effect: 
*       Updates the random-timing flag
****************************************************************************/

BOOL CThread::setRandom(BOOL newrand)
    {
     delayLock.Lock();
        BOOL oldrand = random;
        random = newrand;
     delayLock.Unlock();
     return oldrand;
    }

/****************************************************************************
*                              CThread::shutdown
* Result: void
*       
* Effect: 
*       Sets the shutdown flag for the current object
****************************************************************************/

void CThread::shutdown()
    {
     shutdownLock.Lock();
     shuttingdown = TRUE;
     shutdownLock.Unlock();
    }

/****************************************************************************
*                              CThread::setDelay
* Inputs:
*       int newdelay: New delay amount
* Result: int
*       Old delay amount
* Effect: 
*       Modifies the delay time value
****************************************************************************/

int CThread::setDelay(int newdelay)
    {
     delayLock.Lock();
        BOOL olddelay = delay;
        delay = newdelay;
     delayLock.Unlock();
     return olddelay;
    }

/****************************************************************************
*                              CThread::setMode
* Inputs:
*       int newMode: New mode to set
* Result: int
*       Previous mode
* Effect: 
*       Sets the new mode: free, single-step, or simulated time
****************************************************************************/

int CThread::setMode(int newMode)
    {
     int oldmode;

     modeLock.Lock();
     oldmode = mode;
     mode = newMode;
     modeLock.Unlock();
     return oldmode;
    }

/****************************************************************************
*                               CThread::resume
* Result: void
*       
* Effect: 
*       Resumes this thread from a simulated suspend.  It must be suspended
*       in a real SuspendThread for this to work right.  The ::SuspendThread
*       caller takes responsibility for parent notification
****************************************************************************/

void CThread::resume()
    {
     ::ResumeThread(hthread);
    }

/****************************************************************************
*                              CThread::isAlive
* Result: BOOL
*       TRUE if thread is still alive
*       FALSE if thread is dead
****************************************************************************/

BOOL CThread::isAlive() const
    {
     if(hthread == NULL)
        return FALSE;

     DWORD code;
     ::GetExitCodeThread(hthread, &code);
     return (code == STILL_ACTIVE);
    }

/****************************************************************************
*                            CThread::setMessaging
* Inputs:
*       int newmode: MESSAGING_SYNCHRONOUS or MESSAGING_ASYNCHRONOUS
* Result: int
*       Previous setting
* Effect: 
*       Change the messaging flag
****************************************************************************/

int CThread::setMessaging(int newmode)
    {
     int oldmode;

     messagingLock.Lock();
     oldmode = messaging;
     messaging = newmode;
     messagingLock.Unlock();
     return oldmode;
    }

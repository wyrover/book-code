class CThread {
    public:
        CThread::CThread();
        CThread::~CThread();

        DWORD getThreadID() { return threadID; }
        BOOL isAlive() const;
        static BOOL setLock(BOOL newlock);
        static BOOL setRandom(BOOL newrand);
        static int  setDelay(int newdelay);
        static int  setMessaging(int newmode);
        int setMode(int newMode); // non-static...per-worker option

        //----------------------------------------------------------------
        // The single, global, shared counter and its lock
        static CCriticalSection nLock;
        static int n;
        //----------------------------------------------------------------

        // This is the delay value.  It is common to all worker threads
        static CCriticalSection delayLock;
        static int delay;   // simulated delay in ms. (nominal)
        static BOOL random; // delay is randomly varied according to negative
                            // exponential distribution

        // The window handle of its controller window
        HWND hwnd;

        //----------------------------------------------------------------
        void kill();
        void suspend();
        void resume();
        void shutdown();
        void setWindow(HWND newhwnd) { hwnd = newhwnd; }
        //----------------------------------------------------------------
        static long WINAPI workerFunction(LPVOID lParam); // the thread root function

    protected:
        // The global state flag that says whether locking mode is in effect
        // or not (for all threads)

        static CCriticalSection lockLock;
        static BOOL locking;

        static CCriticalSection messagingLock;
        static int messaging;
#define MESSAGING_ASYNCHRONOUS 0
#define MESSAGING_SYNCHRONOUS  1
        //////////////////////////////////////////////////////////////////////


        ///////////////////////////////////////////////////////////////////
        //
        // The thread ID and handle

public:
        DWORD threadID;
        HANDLE hthread;

protected:
        // These flags are used by the worker thread and set by
        // the GUI thread.  

        CCriticalSection liveLock;
        BOOL live;  // set to TRUE when started, GUI sets to FALSE to kill instantly

        CCriticalSection shutdownLock;
        BOOL shuttingdown;  // set to FALSE initially, GUI sets to TRUE to get clean shutdown
        
        CCriticalSection suspendLock;
        BOOL suspending;
        
        CCriticalSection modeLock;
        int mode;
#define MODE_FREE       0
#define MODE_SIMULATED  1
#define MODE_SINGLE     2


        // This is the private state flag used by the worker thread.  
        int state;
#define STATE_LOCK      0
#define STATE_FETCH     1
#define STATE_INCREMENT 2
#define STATE_STORE     3
#define STATE_UNLOCK    4
        

        void work();
        //
        //////////////////////////////////////////////////////////////////
        
              };

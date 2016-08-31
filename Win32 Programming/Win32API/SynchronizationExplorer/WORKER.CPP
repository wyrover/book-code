// Worker.cpp : implementation file
//

#include "stdafx.h"
#include "afxmt.h" // multithreading capabilities
#include "SynchronizationExplorer.h"
#include "thread.h"
#include "IntEdit.h"
#include "Worker.h"
#include "evntname.h"
#include "uwm.h"
#include "childfrm.h"
#include "delta.h"
#include "panel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEvent CWorker::event(FALSE, FALSE, StartupEventName, NULL);

/////////////////////////////////////////////////////////////////////////////
// CWorker

IMPLEMENT_DYNCREATE(CWorker, CFormView)

CWorker::CWorker()
        : CFormView(CWorker::IDD)
{
        loops = 0;
        initialized = FALSE;
        //{{AFX_DATA_INIT(CWorker)
        //}}AFX_DATA_INIT
}

CWorker::~CWorker()
{
}

void CWorker::DoDataExchange(CDataExchange* pDX)
{
        CFormView::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CWorker)
        DDX_Control(pDX, IDC_PRIORITY, c_Priority);
        DDX_Control(pDX, IDC_SPIN_PRIORITY, c_SpinPriority);
        DDX_Control(pDX, IDC_RUNSTOP, c_RunStop);
        DDX_Control(pDX, IDC_STEP, c_Step);
        DDX_Control(pDX, IDC_UNLOCK, c_Unlock);
        DDX_Control(pDX, IDC_STORE, c_Store);
        DDX_Control(pDX, IDC_LOCK, c_Lock);
        DDX_Control(pDX, IDC_INCREMENT, c_Increment);
        DDX_Control(pDX, IDC_FETCH, c_Fetch);
        DDX_Control(pDX, IDC_N, c_N);
        DDX_Control(pDX, IDC_INTERATIONS, c_Iterations);
        //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWorker, CFormView)
        ON_MESSAGE(UWM_STATE, OnState)
        ON_MESSAGE(UWM_WAITING, OnWaiting)
        ON_MESSAGE(UWM_RUNNING, OnRunning)
        //{{AFX_MSG_MAP(CWorker)
        ON_WM_CLOSE()
        ON_BN_CLICKED(IDC_STEP, OnStep)
        ON_WM_GETMINMAXINFO()
        ON_WM_DESTROY()
        ON_BN_CLICKED(IDC_RUNSTOP, OnRunstop)
        ON_EN_CHANGE(IDC_PRIORITY, OnChangePriority)
        ON_WM_WINDOWPOSCHANGING()
        ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorker diagnostics

#ifdef _DEBUG
void CWorker::AssertValid() const
{
        CFormView::AssertValid();
}

void CWorker::Dump(CDumpContext& dc) const
{
        CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWorker message handlers

void CWorker::OnInitialUpdate() 
{
        CFormView::OnInitialUpdate();
        
        // This should be -2,0 but -2,0 disables the control 
#define SPIN_OFFSET 0
        c_SpinPriority.SetRange(-2 + SPIN_OFFSET, 0 + SPIN_OFFSET);
        c_SpinPriority.SetPos(-1);

        GetParentFrame()->RecalcLayout();
        ResizeParentToFit(TRUE);
        ResizeParentToFit(FALSE);
        ((CChildFrame *)GetParentFrame())->setSize();

        threadstuff.setWindow(m_hWnd);

        c_Step.EnableWindow(FALSE);

        CRect r;
        c_N.GetClientRect(&r);
        if(nfont.CreateFont(r.Height(), 
                         0, // width
                         0, // escapement
                         0, // orientation
                         FW_BOLD, // weight
                         TRUE,    // italic
                         FALSE,   // underline
                         FALSE,   // strikeout
                         ANSI_CHARSET, // character set
                         OUT_DEFAULT_PRECIS,
                         CLIP_DEFAULT_PRECIS, 
                         DEFAULT_QUALITY,
                         DEFAULT_PITCH,
                         _T("Arial")))
           { /* use large font */
            c_N.SetFont(&nfont);
           } /* use large font */

        thread = AfxBeginThread((AFX_THREADPROC)CThread::workerFunction, 
                                   (LPVOID)&threadstuff,
                                   c_Priority.GetVal() - SPIN_OFFSET, //GUI gets preference
                                   0, // stack size
                                   CREATE_SUSPENDED, 
                                   NULL // security: nothing special
                                  );

      
        // TODO: Add your specialized code here and/or call the base class
        
        thread->ResumeThread();

#ifdef _DEBUG
        {
         CString msg;
         msg.Format(_T("0x%02x: Thread 0x%x created, waiting for startup complete\r\n"), 
                                GetCurrentThreadId(),
                                thread->m_nThreadID); // threadstuff.threadID not yet set!
         OutputDebugString(msg);
        }
#endif
        ::WaitForSingleObject(event.m_hObject, INFINITE);

#ifdef _DEBUG
        {
         CString msg;
         msg.Format(_T("0x%02x: Thread 0x%x startup complete\r\n"), 
                                GetCurrentThreadId(),
                                threadstuff.threadID);
         OutputDebugString(msg);
        }
#endif

     initialized = TRUE;
}

/****************************************************************************
*                              CWorker::OnState
* Inputs:
*       WPARAM state: New state value for the worker thread
*       LPARAM lParam: 
*               STATE_LOCK: (BOOL)locking flag
*               STATE_FETCH: old value of n
*               STATE_INCREMENT: new value of n
*               STATE_STORE: unused, 0
*               STATE_UNLOCK: (BOOL)locking flag
* Result: LRESULT
*       logically void, 0, always
* Effect: 
*       Posts the thread's current state to the GUI window
****************************************************************************/

LRESULT CWorker::OnState(WPARAM state, LPARAM lParam)
    {
     int id = 0;
     CString s;
     switch(state)
        { /* which state */
         case STATE_LOCK:
                 c_Lock.EnableWindow(lParam);
                 c_Unlock.EnableWindow(lParam);
                 if(lParam)
                    id = IDC_LOCK;
                 break;
         case STATE_FETCH:
                 id = IDC_FETCH;
                 s.Format(_T("%d"), lParam);
                 c_N.SetWindowText(s);
                 break;
         case STATE_INCREMENT:
                 id = IDC_INCREMENT;
                 s.Format(_T("%d"), lParam);
                 c_N.SetWindowText(s);
                 ::PostMessage(CPanel::panel, UWM_N, lParam, 0);
                 break;
         case STATE_STORE:
                 id = IDC_STORE;
                 break;
         case STATE_UNLOCK:
                 loops++;
                 ::PostMessage(CPanel::panel, UWM_LOOP, 0, 0);
                 if(lParam)
                    id = IDC_UNLOCK;
                 break;
        } /* which state */

     if(id != 0)
        CheckRadioButton(IDC_LOCK, IDC_UNLOCK, id);
     
     s.Format(_T("%d"), loops);
     c_Iterations.SetWindowText(s);

     return 0; // unused return value
    }

void CWorker::OnClose() 
{
#ifdef _DEBUG
        OutputDebugString(_T("=> CWorker::OnClose\r\n"));
#endif
        threadstuff.shutdown();
        
#ifdef _DEBUG
        {
         CString msg;
         msg.Format(_T("0x%02x: Waiting for thread 0x%x shutdown\r\n"), 
                                        GetCurrentThreadId(),
                                        threadstuff.threadID);
         OutputDebugString(msg);
        }
#endif

        endThread();

#ifdef _DEBUG
        {
         CString msg;
         msg.Format(_T("0x%02x: Thread 0x%x shutdown complete\r\n"), 
                                        GetCurrentThreadId(),
                                        threadstuff.threadID);
         OutputDebugString(msg);
        }
#endif  
        CFormView::OnClose();

#ifdef _DEBUG
        OutputDebugString(_T("<= CWorker::OnClose\r\n"));
#endif

}

/****************************************************************************
*                               CWorker::OnStep
* Result: void
*       
* Effect: 
*       Single-steps the thread
****************************************************************************/

void CWorker::OnStep() 
{
 threadstuff.setMode(MODE_SINGLE);
 threadstuff.resume();
}

/****************************************************************************
*                             CWorkder::OnWaiting
* Inputs:
*       WPARAM: ignored
*       LPARAM: ignored
* Result: LRESULT
*       logically void, 0, always
* Effect: 
*       Marks the thread as waiting.  Enables the button and labels it "Step"
****************************************************************************/

LRESULT CWorker::OnWaiting(WPARAM, LPARAM)
    {
     CString s;
     s.LoadString(IDS_RUN);
     c_RunStop.SetWindowText(s);
     c_Step.EnableWindow(TRUE);
     mode = MODE_WAITING;
     return 0;
    }

/****************************************************************************
*                             CWorkder::OnRunning
* Inputs:
*       WPARAM: ignored
*       LPARAM: ignored
* Result: LRESULT
*       logically void, 0, always
* Effect: 
*       Marks the thread as Running.  Enables the button and labels it "Step"
****************************************************************************/

LRESULT CWorker::OnRunning(WPARAM, LPARAM)
    {
     CString s;
     s.LoadString(IDS_STOP);
     c_RunStop.SetWindowText(s);
     c_Step.EnableWindow(FALSE);
     mode = MODE_RUNNING;
     return 0;
    }

void CWorker::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
        CFormView::OnGetMinMaxInfo(lpMMI);
}

/****************************************************************************
*                             CWorker::endThread
* Result: void
*       
* Effect: 
*       Kills the thread.  Ideally, waits for it to shut down gracefully,
*       but because of kills, the thread may be blocked on a lock and we
*       must blow it away the hard way.
****************************************************************************/

#define WAIT_FOR_SHUTDOWN 3000   // should be enough time

void CWorker::endThread()
    {
     switch(::WaitForSingleObject(threadstuff.hthread, WAIT_FOR_SHUTDOWN))
        { /* wait */
         case WAIT_FAILED:
                 return;  // should not have failed.  Probably already dead.
         case WAIT_OBJECT_0:
                 return;  // successful.  Thread is now dead.
         case WAIT_TIMEOUT:
                 break;   // Whoops.  Thread is probably blocked.
         default:
                 return;  // should not happen.
        } /* wait */
     
     // If we get here, the kill timed out.  This means the thread is
     // internally locked.  Ugly as it is, we have to actually call
     // TerminateThread.  

#ifdef _DEBUG
     {
      CString msg;
      msg.Format(_T("0x%02x: Kill thread 0x%x timed out.  Calling TerminateThread\r\n"),
                        GetCurrentThreadId(),
                        threadstuff.threadID);
      OutputDebugString(msg);
     }
#endif     

     // This is really evil.  It can mess up any DLLs we have attached
     // (including MFC and CRT) because they never get the detach events.
     // The only reason we survive this is because we are quitting.
     // Otherwise, the OnClose handler would have effected a cleaner
     // thread termination.  We get away with this ONLY because in this
     // demo program we know that the only lock we have is a Critical Section
     // lock and losing it is harmless.
     TerminateThread(threadstuff.hthread, 0);

     switch(::WaitForSingleObject(threadstuff.hthread, WAIT_FOR_SHUTDOWN))
        { /* wait */
         case WAIT_FAILED:
                 return;  // should not have failed.  
         case WAIT_OBJECT_0:
                 return;  // successful.  Thread is now, finally, dead.
         case WAIT_TIMEOUT:
                 break;   // Whoops.  Disaster.  TODO: issue warning
         default:
                 return;  // should not happen.
        } /* wait */

    }

void CWorker::OnDestroy() 
{
        CFormView::OnDestroy();
#ifdef _DEBUG
        OutputDebugString(_T("=> CWorker::OnDestroy\r\n"));
#endif
        
        // This is actually a VERY DANGEROUS thing to do in general: you
        // should never TERMINATE a thread because you don't know what
        // it might have locked!  However, in this case, the only way we
        // can get here is if the application is being shut down and
        // the window is being destroyed.  In this case, we MUST terminate
        // the thread in some fashion.  We could wait for a "clean" termination
        // point, but that SHOULD have been handled by the OnClose handler.
        // So we know that something serious is happening, and we just 
        // terminate the worker thread

        // Note that we don't use TerminateThread, which is very risky and
        // can even leave the kernel in an inconsistent state; but our
        // kill function, although it is a "clean" kill from the viewpoint
        // of Win32, is a "dirty" kill (akin to TerminateThread) with respect
        // to our simulation.
        
        if(threadstuff.isAlive())
           { /* kill it off */
            threadstuff.kill();

#ifdef _DEBUG
            {
             CString msg;
             msg.Format(_T("0x%02x: Waiting for thread 0x%x destroy\r\n"), 
                                        GetCurrentThreadId(),
                                        threadstuff.threadID);
             OutputDebugString(msg);
            }
#endif
        
            endThread();

#ifdef _DEBUG
            {
             CString msg;
             msg.Format(_T("0x%02x: Thread 0x%x destroyed\r\n"), 
             GetCurrentThreadId(),
                                        threadstuff.threadID);
             OutputDebugString(msg);
            }
#endif
           } /* kill it off */

#ifdef _DEBUG
        OutputDebugString(_T("<= CWorker::OnDestroy\r\n"));
#endif
}

/****************************************************************************
*                             CWorker::OnRunstop
* Result: void
*       
* Effect: 
*       Handles the pushbutton that toggles between running and stopped state.
*       Notifications to change button captions are sent by the controlled
*       thread.
****************************************************************************/

void CWorker::OnRunstop() 
{
 switch(mode)
    { /* which mode */
     case MODE_WAITING:
             threadstuff.setMode(MODE_SIMULATED);
             threadstuff.resume();
             break;
     case MODE_RUNNING:
             threadstuff.suspend();
             break;
    } /* which mode */
}

void CWorker::OnChangePriority() 
{
 if(!initialized)
    return;
 ::SetThreadPriority(threadstuff.hthread, c_Priority.GetVal() - SPIN_OFFSET);
}

void CWorker::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
        CFormView::OnWindowPosChanging(lpwndpos);
        
}

BOOL CWorker::PreCreateWindow(CREATESTRUCT& cs) 
{
 
 	cs.style &= ~(WS_MAXIMIZE);
	
	return CFormView::PreCreateWindow(cs);
}


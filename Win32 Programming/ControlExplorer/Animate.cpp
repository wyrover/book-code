#include "stdafx.h"
#include "resource.h"
#include "animate.h"

// The timer interval we use, in milliseconds
#define UNDER_CONSTRUCTION_VAL 500
#define TimerID 1

/****************************************************************************
*                               beginAnimation
* Inputs:
*       CWnd * wnd: Window that is the animation candidate
* Result: void
*       
* Effect: 
*       - Hides the window.  
*	- Moves the icons IDC_WORKING1, IDC_WORKING2 and IDC_WORKING3 to 
*	  the edges of the window.
*       - Makes them visible
*	- Sets up a timer message
****************************************************************************/

void beginAnimation(CWnd * wnd)
    {
     CRect rect;
     wnd->GetWindowRect(&rect);
     wnd->GetParent()->ScreenToClient(&rect);
     
     CWnd * c_Working1 = wnd->GetParent()->GetDlgItem(IDC_WORKING1);
     CWnd * c_Working2 = wnd->GetParent()->GetDlgItem(IDC_WORKING2);
     CWnd * c_Working3 = wnd->GetParent()->GetDlgItem(IDC_WORKING3);

     c_Working1->SetWindowPos(NULL, rect.left, rect.top, 0, 0, 
     				   SWP_NOSIZE | SWP_NOZORDER);
     c_Working2->SetWindowPos(NULL, rect.right - ::GetSystemMetrics(SM_CXICON),
     				   rect.top, 0, 0, 
				   SWP_NOSIZE | SWP_NOZORDER);
     c_Working3->SetWindowPos(NULL, rect.left + rect.Width() / 2 - 
     					 ::GetSystemMetrics(SM_CXICON) / 2,
				   rect.top, 0, 0, 
				   SWP_NOSIZE | SWP_NOZORDER);


     wnd->GetParent()->SetTimer(TimerID, UNDER_CONSTRUCTION_VAL, NULL);
     c_Working1->ShowWindow(SW_SHOW);
     c_Working2->ShowWindow(SW_SHOW);
     c_Working3->ShowWindow(SW_SHOW);
}


/****************************************************************************
*                                endAnimation
* Inputs:
*       CWnd * wnd: Window to display
* Result: void
*       
* Effect: 
*       - Hides the animation icons
*	- Makes the window visible
*	- Kills the timer
****************************************************************************/

void endAnimation(CWnd * wnd)
    {
     wnd->GetParent()->KillTimer(TimerID);
     CWnd * c_Working1 = wnd->GetParent()->GetDlgItem(IDC_WORKING1);
     CWnd * c_Working2 = wnd->GetParent()->GetDlgItem(IDC_WORKING2);
     CWnd * c_Working3 = wnd->GetParent()->GetDlgItem(IDC_WORKING3);

     c_Working1->ShowWindow(SW_HIDE);
     c_Working2->ShowWindow(SW_HIDE);
     c_Working3->ShowWindow(SW_HIDE);
     wnd->ShowWindow(SW_SHOW);

    }


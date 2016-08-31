#include "stdafx.h"
#include "ctlmap.h"


/****************************************************************************
*                               controlsToFlags
* Inputs:
*       CWND * wnd: Window which contains the buttons to be tested
*	controlmap * map: Pointer to control map table
*	BOOL useall (FALSE): set TRUE to include disabled controls, otherwise
*			     disabled controls are ignored
* Result: DWORD
*       Flags representing the buttons actually checked
****************************************************************************/
DWORD controlsToFlags(CWnd * wnd, controlmap * map, BOOL useall)
{
 DWORD flags = 0;

 for(int i = 0; map[i].ctlid != 0; i++)
    { /* scan controls */
     CButton * ctl = (CButton *)wnd->GetDlgItem(map[i].ctlid);
     if(ctl == NULL)
	continue;  // just in case, we ignore missing controls
     if(ctl->GetCheck() == BST_CHECKED && (useall || ctl->IsWindowEnabled()))
	flags |= map[i].style;
    } /* scan controls */
 return flags;
}

#include "stdafx.h"
#include "afxtempl.h"
#include "winmgr.h"

CList<HWND, HWND> CWindowManager::windowlist(10);


/****************************************************************************
*                             CWindowManager::Add
* Inputs:
*       HWND hwnd: Window handle to add
* Result: void
*       
* Effect: 
*       Adds the window to the list
****************************************************************************/

void CWindowManager::Add(HWND hwnd)
    {
     if(windowlist.Find(hwnd))
	return;  // already in list, avoid redundancy

     windowlist.AddTail(hwnd);
    }

/****************************************************************************
*                           CWindowManager::Remove
* Inputs:
*       HWND hwnd: Window to remove
* Result: void
*       
* Effect: 
*       Removes the window from the list
****************************************************************************/

void CWindowManager::Remove(HWND hwnd)
    {
     POSITION p = windowlist.Find(hwnd);
     if(p == NULL)
	return;
     windowlist.RemoveAt(p);
    }

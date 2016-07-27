#include <windows.h>
#include "stdafx.h"
#include "CObject.h"

#ifndef _CView_
#define _CView_

class CView;


// message map ---------------------------------------------------
typedef LRESULT (CView::*CViewFunPointer)(WPARAM,LPARAM);

typedef struct tagMessageMap
 {
    UINT iMsg;
    CViewFunPointer fp;
} MessageMap;

static CViewFunPointer fpCViewGlobal;//pointer to a member function


//{{seojt
#define NUM    1000
#define TWOPI  (2 * 3.14159)
//}}seojt



// class CView ---------------------------------------------------
class CView : public CObject 
{
public:
    //{{seojt
    PAINTSTRUCT ps;
    POINT pt[NUM];
    HDC hdc;
    int cyClient;
    int cxClient;
    //}}seojt

    //{{AFX_MESSAGE
    LRESULT OnCreate(WPARAM,LPARAM);
    LRESULT OnDraw(WPARAM,LPARAM);
    LRESULT OnDestroy(WPARAM,LPARAM);
    //{{seojt
    LRESULT OnSize(WPARAM wParam,LPARAM lParam);
    LRESULT OnLButtonDown(WPARAM wParam,LPARAM lParam);
    //}}seojt
    //}}AFX_MESSAGE

    DECLARE_MESSAGE_MAP()
};//class CView

#endif
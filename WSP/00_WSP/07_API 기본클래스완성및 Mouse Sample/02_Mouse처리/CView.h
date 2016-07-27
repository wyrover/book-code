//=================================================================
//   05단계 ) 완성 
//      WinMain과 윈도우 프로시저는 은 숨겨진다.
//		처리해야 할 메시지에만 집중하여 클래스를 설계한다.
//		제공되는 파일 : stdafx.h,  stdafx.cpp
//						CObject.h, CObject.cpp
//		구현해야 할 파일 : CView.h, CView.cpp
//=================================================================

// [ 메시지 맵 정리 ]
// 1. MFC는 메시지 루프를 메시지 펌프(message pump)라고 부른다.
//    MFC의 메시지 펌프는 GetMessage()를 사용하지 않고 
//    PeekMessage()를 사용하며, 노는 시간(idle time)을 처리하기 위한
//    특별한 가상 함수 OnIdle()을 가진다.
// 2. 메시지 맵이란 윈도우 메시지를 받아야 할 클래스가 가지는 정적
//    구조체 배열이다.
//    이 정적 구조체 배열은 처리할 메시지와 메시지에 대응하는 맴버
//    함수에 대한 포인터를 핵심적으로 가진다.
// 3. MFC는 매시지 맵의 코드 자동화를 위해 DECLARE_MESSAGE_MAP등의
//    매크로를 가진다.

//=================================================================

#include <windows.h>
#include "stdafx.h"
#include "CObject.h"

#ifndef _CView_
#define _CView_

// message map structure -----------------------------------------
class CView;


typedef LRESULT (CView::*CViewFunPointer)(HWND,WPARAM,LPARAM);

typedef struct tagMessageMap 
{
    UINT iMsg;
    CViewFunPointer fp;
} MessageMap;

static CViewFunPointer fpCViewGlobal;//pointer to a member function


// class CView ---------------------------------------------------
class CView : public CObject 
{
	// 맴버 변수 ---------------------------------
	BOOL bFull;	// 전체 보기인가??
	//--------------------------------------------
public:
	CView();
	void ModifyStyle( HWND hwnd, UINT Remove, UINT Add, BOOL bRedraw = TRUE);
	BOOL WINAPI PreTranslateMessage(HWND hwnd, MSG *pMsg);

	CDocument * GetDocument();

public:
	LRESULT OnPaint(HWND hwnd, WPARAM wParam, LPARAM lParam);

    //{{AFX_MESSAGE
	LRESULT OnSetFocus(HWND, WPARAM, LPARAM);
    LRESULT OnCreate(HWND, WPARAM,LPARAM);
	LRESULT OnSize(HWND, WPARAM, LPARAM);
	LRESULT OnCommand(HWND, WPARAM, LPARAM);
	LRESULT OnKeyDown(HWND, WPARAM, LPARAM);
    LRESULT OnDestroy(HWND, WPARAM,LPARAM);
	LRESULT OnLButtonDown(HWND hwnd, WPARAM wParam, LPARAM lParam);
    //}}AFX_MESSAGE

    DECLARE_MESSAGE_MAP()
};

#endif



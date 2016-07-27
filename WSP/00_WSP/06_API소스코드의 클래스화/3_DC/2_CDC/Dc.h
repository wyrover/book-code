// DC.h: interface for the CDC class.
//
//////////////////////////////////////////////////////////////////////

#include "CView.h"

#if !defined(_CDC_DEFINED_)
#define _CDC_DEFINED_


class CDC  
{
private:
    CView* pView;		// CDC 객체를 생성한 윈도우 핸들을 얻기 위함
    PAINTSTRUCT ps;
    HDC hdc;			// 실제 GDI 함수를 호출할 때 전달됨 

public:
    CDC(CView* pView);
    virtual ~CDC();


    BOOL MoveToEx(int, int, LPPOINT);
    BOOL LineTo(int, int);
    BOOL Polyline (CONST POINT*, int);
};//class CDC

#endif // !defined(_CDC_DEFINED_)

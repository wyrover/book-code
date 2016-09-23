// C3600Splash.cpp : 实现文件
//

#include "stdafx.h"
#include "A-Protect.h"
#include "C3600Splash.h"


// C3600Splash

IMPLEMENT_DYNAMIC(C3600Splash, CWnd)

C3600Splash::C3600Splash()
{

}

C3600Splash::~C3600Splash()
{
}


// C3600Splash 消息处理程序

BEGIN_MESSAGE_MAP(C3600Splash, CWnd)  
    ON_WM_PAINT()  
    ON_WM_TIMER()  
END_MESSAGE_MAP() 

// CWzdSplash 消息处理程序  
void C3600Splash::Create(UINT nBitmapID)  
{  
    m_bitmap.LoadBitmap(nBitmapID);  
    BITMAP bitmap;  
    m_bitmap.GetBitmap(&bitmap);  
    //CreateEx(0,AfxRegisterWndClass(0),"",WS_POPUP|WS_VISIBLE|WS_BORDER,0,0,bitmap.bmWidth,bitmap.bmHeight,NULL,0);  
    CreateEx(0,  
            AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),  
            NULL, WS_POPUP | WS_VISIBLE, 0, 0, bitmap.bmWidth, bitmap.bmHeight, NULL , NULL);  
}  
void C3600Splash::OnPaint()  
{  
    // TODO: 在此处添加消息处理程序代码  
    // 不为绘图消息调用 CWnd::OnPaint()  
    CPaintDC dc(this); // device context forpainting  
    BITMAP bitmap;  
    m_bitmap.GetBitmap(&bitmap);  
    CDC dcComp;  
    dcComp.CreateCompatibleDC(&dc);  
    dcComp.SelectObject(&m_bitmap);  
    // draw bitmap  
    dc.BitBlt(0,0,bitmap.bmWidth,bitmap.bmHeight,&dcComp,0,0,SRCCOPY);  
}  
void C3600Splash::OnTimer(UINT_PTR nIDEvent)  
{  
    // TODO: 在此添加消息处理程序代码和/或调用默认值  
    //CWnd::OnTimer(nIDEvent);  
    DestroyWindow(); //销毁初始画面窗口  
}  
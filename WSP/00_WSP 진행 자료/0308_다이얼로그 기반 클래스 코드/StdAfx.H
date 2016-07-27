#include <windows.h>

#ifndef _stdafx_
#define _stdafx_

#define DECLARE_MESSAGE_MAP()           static MessageMap messageMap[];
#define BEGIN_MESSAGE_MAP(class_name)   MessageMap class_name::messageMap[]={
#define END_MESSAGE_MAP()               {0,NULL}};

//Forward declaration------------------------------------------------
BOOL  CALLBACK DlgProc(HWND hwnd,UINT iMsg,WPARAM wParam,LPARAM lParam);

#endif _stdafx_
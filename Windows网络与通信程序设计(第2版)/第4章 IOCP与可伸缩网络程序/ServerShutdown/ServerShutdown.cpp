////////////////////////////////////
// ServerShutdown.cpp文件


#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{	
	HANDLE hEvent = ::OpenEvent(EVENT_ALL_ACCESS, FALSE, "ShutdownEvent");
	if(hEvent != NULL)
	{
		::SetEvent(hEvent);
		::CloseHandle(hEvent);
		::MessageBox(NULL, " 服务器关闭成功！\n", "ServerShutdown", 0);
	}
	else
	{
		::MessageBox(NULL, " 服务器还没有启动！\n", "ServerShutdown", 0);
	}
	return 0;
}
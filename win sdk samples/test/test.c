#include <windows.h>

int WINAPI
WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR pszCmdLine, int iCmdShow)
{
	MessageBox(NULL, "Narf!", "Pinky says...", MB_OK | MB_ICONEXCLAMATION);
	return 0;
}
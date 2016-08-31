#ifndef find_window_h__
#define find_window_h__

#include <Windows.h>

namespace cactus
{

class FindWindow
{
public:
    HWND static FindWindowFromProcessId(DWORD dwProcessId);
    HWND static FindWindowFromProcess(HANDLE hProcess);
};

}

#endif // find_window_h__

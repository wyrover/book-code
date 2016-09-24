#include <windows.h>
#include <iostream>

using namespace std;

int main()
{
    DebugBreak();
    GetProcAddress(GetModuleHandleA("kernel32.DLL"),"GetProcAddress");
    cout << "Hello World!" << endl;
    return 0;
}


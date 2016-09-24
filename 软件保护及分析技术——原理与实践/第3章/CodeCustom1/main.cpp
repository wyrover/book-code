#include <Windows.h>
#include <iostream>

#include "Q:/ToolKit/WinLicense/WinlicenseSDK/Include/C/winlicensesdk.h"
using namespace std;

//用于存放序列号值
DWORD gSerialNo = 0;
//判断授权状态
BOOL checkLicense()
{
    VM_START
    if (gSerialNo == 'r')
        return TRUE;
    VM_END
    return FALSE;
}
BOOL Register(DWORD serialNo)
{
    VM_START
    gSerialNo = serialNo;
    return checkLicense();
    VM_END
}
int main()
{
    VM_START
    Register(getchar());
    if (!checkLicense())
        return -1;
    cout << "Hello World!" << endl;
    getchar();
    VM_END
    return 0;
}


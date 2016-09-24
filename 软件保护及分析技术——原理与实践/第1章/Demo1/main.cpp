#include <windows.h>
#include <iostream>

using namespace std;

int main()
{
    cout << "请按任意键继续!" << endl;
    DebugBreak();
    if (getchar())
        cout << "功能1!" << endl;
    else
        cout << "功能2!" << endl;
    //暂停程序方便查看结果
    getchar();
    return 0;
}


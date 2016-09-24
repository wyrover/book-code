#include <windows.h>
#include <iostream>

using namespace std;

void* gSystemEnter;
void __declspec(naked) ourSystemCall()
{
    __asm{

    }
}


int main()
{
    cout << "Hello World!" << endl;
    return 0;
}


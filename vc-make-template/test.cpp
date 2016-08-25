#include <stdio.h>
#include <windows.h>
#include <iostream>

int main(int argc, char *argv[])
{
    printf("Hello Runtime Library.\n");

    #ifdef _MT
        std::cout << "/MT:_MT" << std::endl;
    #endif

    #ifdef _DLL
        std::cout << "MD:_MT,_DLL" << std::endl;
    #endif

    MessageBox(0, "Hello Windows.", "test", 0);
    
    return 0;
}

/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/
class CThisCall
{
public:
    void Func7(int iOne);
    void Func8(int iOne, int iTwo);
} g_obj;

extern "C" void __cdecl CDeclFunc1(int iOne);
extern "C" void __cdecl CDeclFunc2(int iOne, int iTwo);
extern "C" void __stdcall StdcallFunc3(int iOne);
extern "C" void __stdcall StdcallFunc4(int iOne, int iTwo);
extern "C" void __fastcall FastcallFunc5(int iOne);
extern "C" void __fastcall FastcallFunc6(int iOne, int iTwo);

void __cdecl wmain()
{
    CDeclFunc1(1);
    CDeclFunc2(1,2);
    StdcallFunc3(1);
    StdcallFunc4(1,2);
    FastcallFunc5(1);
    FastcallFunc6(1,2);    
    g_obj.Func7(1);
    g_obj.Func8(1,2);    
}

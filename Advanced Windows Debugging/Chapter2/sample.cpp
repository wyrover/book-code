/*++
Copyright (c) Advanced Windows Debugging (ISBN 0321374460) from Addison-Wesley Professional.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

--*/

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <process.h>
#include <malloc.h>
#include "menu.h"
#include "debug.h"
#pragma auto_inline(off)
int CalledFunction1(int a){return 1;}
int CalledFunction2(int a,int b){return 2;}
int CalledFunction3(int a,int b,int c){return 3;}
int CalledFunction4(int a,int b,int c,int d){return 4;}
int CalledFunction5(int a,int b,int c,int d,int e){return 5;}
void try_except();


class Global
{
public:
    int m_ref;
    Global():m_ref(1){};
    ~Global()
        {
            m_ref = 0;
        };    
} gGlobal;

void RaiseBP()
{
    DebugBreak();
}

void RaiseCPP()
{
    throw 0;
}

void RaiseAV()
{
    WCHAR* invalidAddress = 0;
    *invalidAddress = 0;
}

void RaiseStackOverFlow(__int64 limit)
{
    if (limit > 10 ) return; 
    RaiseStackOverFlow(limit);
}

void __cdecl RaiseStackOverFlowThread(void * threadParam)
{
    Sleep(1000);
    __int64 limit = (__int64)threadParam;
    RaiseStackOverFlow(limit);
}

class KBTest
{
    int m_lastN;
public:
        KBTest::KBTest()
        {
        m_lastN = -1;
        }
		virtual ~KBTest()
		{}

#pragma optimize("y",off)

static unsigned int __cdecl Fibonacci_fpo (unsigned int n)
    {
        switch(n)
        {
            case 0: STOP_ON_DEBUGGER;return 0;
            case 1: return 1;
            default: return Fibonacci_fpo(n-1)+Fibonacci_fpo(n-2);
        }
    }
#pragma optimize("y",on)

    static unsigned int __cdecl Fibonacci_cdecl (unsigned int n)
    {
        switch(n)
        {
            case 0: STOP_ON_DEBUGGER;return 0;
            case 1: return 1;
            default: return Fibonacci_cdecl(n-1)+Fibonacci_cdecl(n-2);
        }
    }
    static unsigned int __fastcall Fibonacci_fastcall(unsigned int n)
    {
        switch(n)
        {
            case 0: STOP_ON_DEBUGGER;return 0;
            case 1: return 1;
            default: return Fibonacci_fastcall(n-1)+Fibonacci_fastcall(n-2);
        }
    }
    static unsigned int __stdcall Fibonacci_stdcall(unsigned int n)
    {
        switch(n)
        {
            case 0: STOP_ON_DEBUGGER;return 0;
            case 1: return 1;
            default: return Fibonacci_stdcall(n-1)+Fibonacci_stdcall(n-2);
        }
    }

    unsigned int Fibonacci_thiscall(unsigned int n)
    {
        m_lastN = n;
        int localN = n + gGlobal.m_ref;
        switch(n)
        {
            case 0: STOP_ON_DEBUGGER;return 0;
            case 1: return 1;
            default: 
                {
                return Fibonacci_thiscall(localN-2)+Fibonacci_thiscall(localN-3);
                }
        }
    }
};


extern int Function5(int a, int b, int c, int d, int e);

void Stack()
{
        wprintf (L"F (32) = %d\n",KBTest::Fibonacci_stdcall(32));
}


void Stack64()
{
        wprintf (L"%d\n", Function5(1,2,3,4,5));
}

void StackObj()
{
        KBTest kbTest;
        wprintf (L"F (32) = %d\n",kbTest.Fibonacci_thiscall(32));
}

void StackFPO()
{
        wprintf (L"F (32) = %d\n",KBTest::Fibonacci_fpo(32));
}

void StackFast()
{
        wprintf (L"F (32) = %d\n",KBTest::Fibonacci_fastcall(32));
} 
void StackCdecl()
{
        wprintf (L"F (32) = %d\n",KBTest::Fibonacci_cdecl(32));
} 
void StackOverflow()
{
        _beginthread(&RaiseStackOverFlowThread, 0x1, NULL);
        _beginthread(&RaiseStackOverFlowThread, 0x1, NULL);
        _beginthread(&RaiseStackOverFlowThread, 0x1, NULL);
        _getch();
} 
void HandledAV()
{       
    __try
    {
        RaiseAV();
    }
   __except(TRUE)
    {
    }
}
void HandledBP()
{    
    __try
    {
        RaiseBP();
    }
   __except(TRUE)
    {
    }
}
void OutputDebug()
{
        OutputDebugStringA("Application ");
        OutputDebugStringW(L" running\n");
}


OPTIONS options[]={
{L'1',L"To generate access violation exception",RaiseAV},
{L'2',L"To generate breakpoint exception",RaiseBP},
{L'3',L"To generate C++ exception",RaiseCPP},
{L'4',L"To generate a standard stack",Stack},
{L'5',L"To generate a standard call stack(x64)",Stack64},
{L'6',L"To generate a C++ call stack",StackObj},
{L'7',L"To generate a Fast call stack",StackFast},
{L'8',L"To generate a cdecl call stack",StackCdecl},
{L'9',L"To generate a FPO call stack",StackFPO},
{L'a',L"To generate a stack overflow",StackOverflow},
{L'b',L"To generate a handled access violation exception",HandledAV},
{L'c',L"To generate a handled breakpoint exception",HandledBP},
{L'd',L"To check the exception chain",try_except},
{L'e',L"To call OutputDebugString",OutputDebug},
{L'x',L"To exit",NULL},
}; 

int _cdecl wmain( ULONG argc, WCHAR* argv[] )
{
    AppInfo appInfo=AppInfo(options);
    appInfo.Loop();
    return  0;
}


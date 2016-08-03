/******************************************************************************
Module:  BadLock.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>


DWORD WINAPI LockProcessHandler(PVOID pvParam) {

   HANDLE hProcess = (HANDLE) pvParam;
   WaitForSingleObject(hProcess, INFINITE);
   CloseHandle(hProcess);
   return(TRUE);
}


void LockProcess() {

   STARTUPINFO si = { sizeof(si) };
   PROCESS_INFORMATION pi;
   TCHAR sz[] = TEXT("notepad");
   CreateProcess(NULL, sz, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
   CloseHandle(pi.hThread);

   DWORD threadID;
   CloseHandle(
      CreateThread(NULL, 0, LockProcessHandler, pi.hProcess, 0, &threadID));
   _tprintf(TEXT("LockProcess = %u\n"), threadID);
   _tprintf(TEXT("   Notepad = %u\n"), pi.dwProcessId);
}


DWORD WINAPI SimpleThreadHandler(PVOID pvParam) {
  
   WaitForSingleObject((HANDLE) pvParam, INFINITE);   

   return(0);
}


DWORD WINAPI BlockedThreadHandler(PVOID pvParam) {
  
   HANDLE hParentThread;
   DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), 
      GetCurrentProcess(), &hParentThread, 0, FALSE, 
      DUPLICATE_SAME_ACCESS);
   HANDLE handles[3];
   DWORD threadID1;
   handles[0] = CreateThread(NULL, 0, SimpleThreadHandler, 
      hParentThread, 0, &threadID1);
   DWORD threadID2;
   handles[1] = CreateThread(NULL, 0, SimpleThreadHandler, 
      hParentThread, 0, &threadID2);
   DWORD threadID3;
   handles[2] = CreateThread(NULL, 0, SimpleThreadHandler, 
      hParentThread, 0, &threadID3);
   
   _tprintf(TEXT("LockThreads = %u\n"), GetCurrentThreadId());
   _tprintf(TEXT("   thread1 = %u (0x%x)\n"), threadID1, PtrToUlong(handles[0]));
   _tprintf(TEXT("   thread2 = %u (0x%x)\n"), threadID2, PtrToUlong(handles[1]));
   _tprintf(TEXT("   thread3 = %u (0x%x)\n"), threadID3, PtrToUlong(handles[2]));

   Sleep(100);

   // Note: WaitForMultipleObjects is not handled by WCT
   WaitForMultipleObjects(3, handles, TRUE, INFINITE);   
   //WaitForSingleObject(handles[0], INFINITE);   

   _tprintf(TEXT("LockThreads is over\n"));
   
   return(0);
}

void LockThreads() {

   DWORD threadID;
   CloseHandle(
      CreateThread(NULL, 0, BlockedThreadHandler, NULL, 0, &threadID));
}



CRITICAL_SECTION csAbandonned;

DWORD WINAPI AbandonnedCriticalSectionHandler(PVOID pvParam) {

   if (pvParam == NULL)
   {
      EnterCriticalSection(&csAbandonned);
      _tprintf(TEXT("   %u has abandonned a critical section\n"), GetCurrentThreadId());  

      // never leave the critical section
      return(0);
   }
   else
   {
      _tprintf(TEXT("   %u is entering an abandonned critical section\n"), GetCurrentThreadId());  
      EnterCriticalSection(&csAbandonned);
      _tprintf(TEXT("   %u is leaving an abandonned critical section\n"), GetCurrentThreadId());  
      LeaveCriticalSection(&csAbandonned);
      _tprintf(TEXT("   %u has left an abandonned critical section\n"), GetCurrentThreadId());  
      return(0);
   }
}


void AbandonnedCriticalSection() {

   InitializeCriticalSection(&csAbandonned);
   
   DWORD threadID;
   HANDLE hThread = 
      CreateThread(NULL, 0, AbandonnedCriticalSectionHandler, NULL, 0, &threadID);

   _tprintf(TEXT("AbandonnedCriticalSection:\n"));
   _tprintf(TEXT("   thread = %u\n"), threadID);
   
   Sleep(1000);
   
   hThread = 
      CreateThread(NULL, 0, AbandonnedCriticalSectionHandler, (PVOID)1, 0, &threadID);
}



HANDLE hMutex1;
HANDLE hMutex2;
HANDLE hMutex3;

DWORD WINAPI LockMutexHandler(PVOID pvParam) {

   DWORD dwAction = PtrToUlong(pvParam);
   if (dwAction == 1) {
      WaitForSingleObject(hMutex1, INFINITE);
      Sleep(1000);
      WaitForSingleObject(hMutex2, INFINITE);
   } else 
   if (dwAction == 2)
   {
      WaitForSingleObject(hMutex2, INFINITE);
      Sleep(1000);
      WaitForSingleObject(hMutex3, INFINITE);
   } else {
      WaitForSingleObject(hMutex3, INFINITE);
      Sleep(1000);
      WaitForSingleObject(hMutex1, INFINITE);
   }

   return(0);
}

void LockMutex() {
   hMutex1 = CreateMutex(NULL, FALSE, TEXT("FirstMutex"));
   hMutex2 = CreateMutex(NULL, FALSE, TEXT("SecondMutex"));
   hMutex3 = CreateMutex(NULL, FALSE, TEXT("ThirdMutex"));

   DWORD threadID1;
   CloseHandle(
      CreateThread(NULL, 0, LockMutexHandler, (PVOID)1, 0, &threadID1));
   DWORD threadID2;
   CloseHandle(
      CreateThread(NULL, 0, LockMutexHandler, (PVOID)2, 0, &threadID2));
   DWORD threadID3;
   CloseHandle(
      CreateThread(NULL, 0, LockMutexHandler, (PVOID)3, 0, &threadID3));
   
   _tprintf(TEXT("LockMutex:\n"));
   _tprintf(TEXT("   thread1 = %u\n"), threadID1);
   _tprintf(TEXT("   thread2 = %u\n"), threadID2);
   _tprintf(TEXT("   thread3 = %u\n"), threadID3);
   _tprintf(TEXT("   hMutex1 = 0x%x\n"), PtrToUlong(hMutex1));
   _tprintf(TEXT("   hMutex2 = 0x%x\n"), PtrToUlong(hMutex2));
   _tprintf(TEXT("   hMutex3 = 0x%x\n"), PtrToUlong(hMutex3));
}



CRITICAL_SECTION cs1;
CRITICAL_SECTION cs2;

DWORD WINAPI LockCriticalSectionHandler(PVOID pvParam) {
   
   DWORD dwAction = PtrToUlong(pvParam);
   if (dwAction == 1) {
      EnterCriticalSection(&cs1);
      Sleep(1000);
      EnterCriticalSection(&cs2);
   } else {
      EnterCriticalSection(&cs2);
      Sleep(1000);
      EnterCriticalSection(&cs1);
   }

   return(0);
}

void LockCriticalSections() {

   InitializeCriticalSection(&cs1);
   InitializeCriticalSection(&cs2);

   DWORD threadID1;
   CreateThread(NULL, 0, LockCriticalSectionHandler, (PVOID)1, 0, &threadID1);
   DWORD threadID2;
   CreateThread(NULL, 0, LockCriticalSectionHandler, (PVOID)2, 0, &threadID2);
   
   _tprintf(TEXT("LockCriticalSections:\n"));
   _tprintf(TEXT("   thread1 = %u\n"), threadID1);
   _tprintf(TEXT("   thread2 = %u\n"), threadID2);
   _tprintf(TEXT("   &cs1 = 0x%x\n"), PtrToUlong(&cs1));
   _tprintf(TEXT("   &cs2 = 0x%x\n"), PtrToUlong(&cs2));
}


DWORD WINAPI LockInfiniteMutexHandler(PVOID pvParam) {

   HANDLE hMutex = (HANDLE)pvParam;
   WaitForSingleObject(hMutex, INFINITE);

   _tprintf(TEXT("Thread %u never goes up to here.\n"), GetCurrentThreadId());
   return(0);
}

void LockInfinite() {

   HANDLE hMutex = CreateMutex(NULL, TRUE, TEXT("InfiniteMutex"));
   
   DWORD threadID;
   CloseHandle(
      CreateThread(NULL, 0, LockInfiniteMutexHandler, (PVOID)hMutex, 0, 
         &threadID));

   _tprintf(TEXT("Infinite wait on 0x%x by %u:\n"), PtrToUlong(hMutex), threadID);
}



DWORD WINAPI RunningThreadHandler(PVOID pvParam) {

   SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
   
   // Infinite loop
   for (;;) ;

   return(0);
}

void RunningThread() {

   DWORD threadID;
   CloseHandle(CreateThread(NULL, 0, RunningThreadHandler, (PVOID)1, 0, &threadID));

   _tprintf(TEXT("Running thread:\n"));
   _tprintf(TEXT("   thread = %u\n"), threadID);
}


void TestDeadlock() {

   _tprintf(TEXT("TestDeadlock process ID: %u\n"), GetCurrentProcessId());
   _tprintf(TEXT("--------------------------------\n"));

   RunningThread();
   LockInfinite();
   LockCriticalSections();
   LockMutex();
   AbandonnedCriticalSection();
   LockProcess();
   LockThreads();
}


int _tmain(int argc, _TCHAR* argv[]) {
   
   TestDeadlock();
   
   MessageBox(NULL, NULL, TEXT("Click OK to end the application"), MB_OK);
   return 0;
}


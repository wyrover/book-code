/******************************************************************************
Module:  UserSyncCompare.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/

#include "..\CommonFiles\CmnHdr.h"     /* See Appendix A. */
#include <windows.h>
#include <stdio.h>
#include <tchar.h>


// Stop watch class from Chapter 7
class CStopwatch {
public:
   CStopwatch() { QueryPerformanceFrequency(&m_liPerfFreq); Start(); }

   void Start() { QueryPerformanceCounter(&m_liPerfStart); }

   // Returns # of milliseconds since Start was called
   __int64 Now() const {   
      LARGE_INTEGER liPerfNow;
      QueryPerformanceCounter(&liPerfNow);
      return(((liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000) 
         / m_liPerfFreq.QuadPart);
      }

private:
   LARGE_INTEGER m_liPerfFreq;   // Counts per second
   LARGE_INTEGER m_liPerfStart;  // Starting count
};


DWORD g_nIterations = 1000000;
typedef void (CALLBACK* OPERATIONFUNC)();

DWORD WINAPI ThreadIterationFunction(PVOID operationFunc) {
	OPERATIONFUNC op = (OPERATIONFUNC) operationFunc;
   for (DWORD iteration = 0; iteration < g_nIterations; iteration++) {
      op();
   }  
   return 0;
}

void MeasureConcurrentOperation(
   TCHAR* operationName, DWORD nThreads, OPERATIONFUNC operationFunc) {
   HANDLE* phThreads = new HANDLE[nThreads];

   SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
   for (DWORD currentThread = 0; currentThread < nThreads; currentThread++) {
      phThreads[currentThread] = 
         CreateThread(NULL, 0, ThreadIterationFunction, operationFunc, 0, NULL);
   }
   SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);

   CStopwatch watch;
   WaitForMultipleObjects(nThreads, phThreads, TRUE, INFINITE);
   __int64 elapsedTime = watch.Now();
   _tprintf(
	   TEXT("Threads=%u, Milliseconds=%u, Test=%s\n"), 
      nThreads, (DWORD)elapsedTime, operationName);

   // Don't forget to clean up the thread handles
   for (DWORD currentThread = 0; currentThread < nThreads; currentThread++) {
      CloseHandle(phThreads[currentThread]);
   }
   delete phThreads;
}



// -----------------------------------------------------------
// List of tests to run:
// -----------------------------------------------------------
// Reading from a volatile int with NO synchronization at all
// Writing to an int using InterlockedIncrement
// Reading from a volatile int using critical sections
// Read from a volatile int using SRWLock
// Read from a volatile int using Mutex
// -----------------------------------------------------------

volatile LONG gv_value = 0;

// 'lValue': local variable is initialized but not referenced
#pragma warning(disable:4189)
void WINAPI VolatileReadCallback()
{
   LONG lValue = gv_value; 
}
#pragma warning(default:4189)

void WINAPI VolatileWriteCallback()
{
   gv_value = 0; 
}

void WINAPI InterlockedIncrementCallback()
{
	InterlockedIncrement(&gv_value);
}

CRITICAL_SECTION  g_cs;
void WINAPI CriticalSectionCallback()
{
	EnterCriticalSection(&g_cs);
	gv_value = 0;
	LeaveCriticalSection(&g_cs);
}


HANDLE g_hMutex;
void WINAPI MutexCallback()
{
	WaitForSingleObject(g_hMutex, INFINITE);
	gv_value = 0;
	ReleaseMutex(g_hMutex);
}


// Slim Reader/Writer Lock global variable
SRWLOCK g_srwLock;

void WINAPI SRWLockReadCallback() {
	AcquireSRWLockShared(&g_srwLock);
	gv_value = 0;
	ReleaseSRWLockShared(&g_srwLock);
}

void WINAPI SRWLockWriteCallback() {
	AcquireSRWLockExclusive(&g_srwLock);
	gv_value = 0;
	ReleaseSRWLockExclusive(&g_srwLock);
}


int _tmain(int argc, _TCHAR* argv[]) {
	
   for (int nThreads = 1; nThreads <= 4; nThreads *= 2) {
      MeasureConcurrentOperation(TEXT("Volatile Read"), nThreads, VolatileReadCallback);
		MeasureConcurrentOperation(TEXT("Volatile Write"), nThreads, VolatileWriteCallback);
		MeasureConcurrentOperation(TEXT("Interlocked Increment"), nThreads, InterlockedIncrementCallback);

		// Prepare the critical section
		InitializeCriticalSection(&g_cs);
		MeasureConcurrentOperation(TEXT("Critical Section"), nThreads, CriticalSectionCallback);
		// Don't forget to cleanup
		DeleteCriticalSection(&g_cs);

      // Prepare the Slim Reader/Writer lock
		InitializeSRWLock(&g_srwLock);
		MeasureConcurrentOperation(TEXT("SRWLock Read"), nThreads, SRWLockReadCallback);
		MeasureConcurrentOperation(TEXT("SRWLock Write"), nThreads, SRWLockWriteCallback);
		// NOTE: You can't cleanup a Slim Reader/Writer lock

		// Prepare the mutex
		g_hMutex = CreateMutex(NULL, false, NULL);
		MeasureConcurrentOperation(TEXT("Mutex"), nThreads, MutexCallback);
		CloseHandle(g_hMutex);
		_tprintf(TEXT("\n"));
	}   

	return(0);
}


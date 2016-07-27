#include <iostream.h>
#include <windows.h>

/*
void CALLBACK foo( DWORD error,
				   DWORD byte,
				   OVERLAPPED* p)
{
	cout << "비동기 작업이 완료 되었습니다." << endl;
	cout << "총 작업한 byte : " << byte << endl;
}
*/
	// OVERLAPPED 구조체 확장 - C++일때
/*
	struct OVERLAPPED_PLUS : public OVERLAPPED
	{
		int jobID;
		int clientID;
	};
*/	
	struct OVERLAPPED_PLUS
	{
		OVERLAPPED ov;
		int jobID;
	};


DWORD WINAPI foo(void* p)
{
	HANDLE hPort = (HANDLE)p;

	DWORD byte;
	DWORD err;
	OVERLAPPED* pov;
	DWORD key;

	while ( GetQueuedCompletionStatus( hPort, &byte, &key, &pov, INFINITE) )
	{
		cout << "비동기 작업이 완료 되었습니다." << endl;
		cout << "Byte : " << byte << endl;
		cout << "완료키 : " << key << endl;
		OVERLAPPED_PLUS* p = (OVERLAPPED_PLUS*)pov;
		cout << "JobID : " << p->jobID << endl;
	}
	return 0;
}







void main()
{
	HANDLE hFile = CreateFile( "COM1", // Serial port
							   GENERIC_WRITE,
							   FILE_SHARE_READ | FILE_SHARE_WRITE,
							   0, // 보안
							   OPEN_EXISTING, //
							   FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
							   0);

	// Baud rate setting...

	if ( hFile == INVALID_HANDLE_VALUE ) // -1
	{
		cout << "COM1 을 열수가 없습니다" << endl;
		return;
	}
	//-------------------------------------------------------------
	// IOCP를 생성한다.
	HANDLE hPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE,// 장치 핸들.
										   0, // 이미 만들어진 port#
										   0,
										   2); // 동시에 작업할수 있는 스레드의 수.

	// 완성된 IOCP에 비동기 작업을할 장치를 연결한다.
	CreateIoCompletionPort( hFile, hPort, 1, // 완료 키..
							0);

	// 스레드를 생성해서 비동기 작업을 대기한다.
	CreateThread( 0, 0, foo, (void*)hPort, 0, 0);
	CreateThread( 0, 0, foo, (void*)hPort, 0, 0);

	//-----------------------------------------------------------------
	//------------------------------------------------
	const int size = 2048; // 2k
	char buf[size];

	DWORD len;
	//---------------------------------
	// 비동기 IO 요청하기.
	/*
	OVERLAPPED ov = { 0};
	ov.hEvent = CreateEvent( 0, 0, 0, "e");
	ov.Internal		= 0;
	ov.InternalHigh = 0; // os 가 사용 사용자는 사용하지 않는다.
	ov.Offset		= 0; // 화일작업을 시작하고 싶은 화일의 offset
	ov.OffsetHigh	= 0;
	*/
	OVERLAPPED_PLUS ov = { 0 };
	ov.jobID = 1;

	BOOL b = WriteFile( hFile, buf, size, &len, (OVERLAPPED*)&ov);



	int n;
	cin >> n;

//	BOOL b = WriteFileEx( hFile, buf, size, &ov, foo);

	// 다른작업을 하고... 마지막으로.
	// SleepEx()혹은 WaitForSingleObjectEx()로 대기해야 한다.
	
//	SleepEx( 3 0000, TRUE ); // 대기하다 비동기작업 종료시.. foo로..


	//----------------------------------------------

	if ( b == TRUE )
	{
		cout << "동기적으로 쓰기를 완료 했습니다." << endl;
	}
	else if ( b == FALSE && GetLastError() == ERROR_IO_PENDING)
	{
		cout << "비동기로 작업중입니다." << endl;

	//	WaitForSingleObject( ov.hEvent , INFINITE);

		cout << "비동기 작업 완료" << endl;
	}
	else
		cout <<"COM1에 쓸수가 없습니다" << endl;




	CloseHandle( hFile );
}












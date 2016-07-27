#include "std.h"

char	gbuf[65536]="메모리 통계 파일\r\n";

void myservice_init()
{
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };
	si.cb = sizeof( si );

	BOOL b = CreateProcess( 0,				// app name
							"freecell.exe", // app name + commandline args
							0, 0,			// pko, tko 보안
							FALSE,			// KOHT 의 상속여부
							NORMAL_PRIORITY_CLASS, // 우선순위 | 플래그
							0, 0,			// 환경변수, 현재 디렉토리
							&si,			// startup info 정보
							&pi);	// 생성된 프로세스, 스레드의 핸들과 ID
									// 를 담을 변수.
	if ( b )
	{
		// 자식의 핸들을 닫는 이유를 반드시 알아야 한다.
		// 자식의 핸들을 닫지 않으면 자식이 종료 되어도.. 
		// 자식을 관리하던
		// PKO, TKO 는 파괴 되지 않고 있다가 (참조개수 1) 
		// 부모가 자식의
		// 핸들을 닫거나 부모가종료(자동으로 핸들 닫힘)되야만 
		// PKO, TKO 파괴된다.
//		CloseHandle( pi.hProcess); //핸들이 필요한 경우는 닫지 않아야 한다.
//		CloseHandle( pi.hThread);
	}

	// 새로운 로그 파일을 작성한다.
	HANDLE hFile=CreateFile("c:\\MemStat.txt", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	CloseHandle(hFile);
}

void myservice_run()
{
	MEMORYSTATUS ms;
	DWORD dwWritten;
	char str[256];
	SYSTEMTIME st;
	HANDLE hFile;

	// 현재 시간과 메모리 양을 조사하여 문자열로 조립한다.
	GetLocalTime(&st);
	GlobalMemoryStatus(&ms);
	wsprintf(str, "%d월 %d일 %02d시 %02d분 %02d초 => "
		"사용가능 물리 메모리=%dMbytes(%d%%), 사용가능 가상메모리=%dMbytes,"
		" 사용가능 페이징 파일=%dMbytes\r\n",
		st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,
		ms.dwAvailPhys/1048576,100-ms.dwMemoryLoad,ms.dwAvailVirtual/1048576,
		ms.dwAvailPageFile/1048576);

	// 파일로 통계를 출력한다. 버퍼가 가득찬 경우 파일을 다시 만든다.
	if (strlen(gbuf) > 60000) 
	{
		hFile=CreateFile("c:\\MemStat.txt",GENERIC_WRITE,0,NULL,
			CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		strcpy(gbuf,"메모리 통계 파일\r\n");
	} 
	else 
	{
		hFile=CreateFile("c:\\MemStat.txt",GENERIC_WRITE,0,NULL,
			OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	}
	strcat(gbuf, str);
	WriteFile(hFile, gbuf, strlen(gbuf), &dwWritten, NULL);
	CloseHandle(hFile);
}

void myservice_OnNewFile()
{
	HANDLE hFile=CreateFile("c:\\MemStat.txt",GENERIC_WRITE,0,NULL,
			CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	strcpy(gbuf,"메모리 통계 파일\r\n");
	CloseHandle(hFile);
}
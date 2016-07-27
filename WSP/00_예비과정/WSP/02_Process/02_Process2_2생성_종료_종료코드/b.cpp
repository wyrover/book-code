#include <windows.h>
#include <stdio.h>

void main()
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
	int cmd;
	while ( 1 )
	{
		scanf("%d", &cmd );

		if ( cmd == 2 )
		{
			// 프로세스 강제로 종료 하기
			TerminateProcess( pi.hProcess, 100); // 100은 종료 코드

			// TerminateProcess는 비동기 함수 이다. 즉.. 이 순간 
			// 정말로 죽었다고 생각할수는 없다.

			// 모든 프로세스는 죽을때 PKO 가 Signal 된다.

			// 특정 KO 가 시그널 될때까지 현재 실행흐름(스레드)를 대기 시킨다.

			WaitForSingleObject( pi.hProcess, INFINITE );


			// 이제 정말로 자식 프로세스는 죽었다.

			cmd = 1; // ?
		}




		//============================================================
		if ( cmd == 1 )
		{
			DWORD code;
			GetExitCodeProcess( pi.hProcess, &code ); // 프로세스의 종료 코드얻기
			
			if ( code == STILL_ACTIVE ) // 0x103 으로 정의된 상수.
			{
				printf("아직 자식이 살아 있습니다.\n");
			}
			else
			{
				printf("자식 종료 : %d\n", code );
				CloseHandle( pi.hProcess );  // 자식이 종료 되었으므로
									// 핸들을 닫아서 PKO 가 파괴 되도록 한다.
			}
		}
	}
}












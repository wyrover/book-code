/****************************************************************************

  4) 동기 I/O와 비동기 I/O를 서꺼어서 사용할 수 없다.

     ex) 비동기 I/O를 사용해서 읽기를 완료할때까지 1초 간격으로
		 메시지를 출력하는 예 

         정상적인 실행결과를 확인하려면 대용량의 파일이나, 플로피디스크
		 같은 느린 장치로 시험 필요 
****************************************************************************/

// 비동기 I/O를 이용한 파일 읽기 

#include <windows.h>
#include <stdio.h>

#define BUFSIZE		(10000000)

char buf[BUFSIZE];

int main()
{
	// 수동 재설정 타입의 이름없는 이벤트
	// 객체 생성
	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if( hEvent == 0 )
	{
		printf("이벤트 객체 생성 실패");
		return -1;
	}

	// FILE_FLAG_OVERLAPPED를 지정해 파일 열기 
	HANDLE hFile = CreateFile("TESTFILE.AVI", GENERIC_READ,
		0, NULL, OPEN_ALWAYS, 
		FILE_FLAG_OVERLAPPED, NULL);	// OVERLAPPED 플래그 지정 
										// 즉, 이 파일은 동기 I/O를 
										// 수행할 수 없다. 
										// 반드시 비동기 I/O로 수행..
	if( hFile != INVALID_HANDLE_VALUE)
	{
		memset(buf, 0, sizeof(buf));
		DWORD xferCnt = 0;

		OVERLAPPED oi;				// ReadFile/WriteFile이 종료될때까지
									// 유효한 데이터로 존재해야 함 
		oi.hEvent		= hEvent;	// 이벤트 핸들러
		oi.Offset		= 0;		// [입력] 전송시작 위치의 하위 32bit
		oi.OffsetHigh	= 0;		// [입력] 전송시작 위치의 상위 32bit
		oi.Internal		= 0;		// [출력] 상태:비설정값, 호환성을 위해 
		oi.InternalHigh	= 0;		// [출력] 전송된 바이트 수:비설정값, 호환성을 위해  

		if( ReadFile(hFile, buf, sizeof(buf), &xferCnt, &oi))
		{
			printf("읽기 처리 완료");
		}
		else
		{
			DWORD rc = GetLastError();

			// 읽고 있는 상태 확인 
			// 즉 처리가 정상적으로 시작되었으나 완료되지 않은 상태 
			if( rc == ERROR_IO_PENDING)
			{
				bool error = false;
				int cnt = 0;
												  //반환 : 성공이면 TRUE
				while(!GetOverlappedResult(hFile, // 대상 파일 핸들
					&oi,						// OVERLAPPED구조체의 주소
					&xferCnt,					// 실제로 전송된 바이트 수 
					FALSE))						// TRUE면 무한 대기, FALSE면 즉시 복귀 
				{
					rc = GetLastError();
					if( rc != ERROR_IO_INCOMPLETE)
					{
						printf("비동기 I/O 처리 중 에러가"
							"발생(RC=%ld)\n", rc);
						error = true;
						break;
					}

					printf("비동기 I/O 완료를 대기중"
						"(%d번째)\n", ++cnt);
					Sleep(1000);
				}

				if( !error)
				{
					printf("비동기 I/O완료\n");
					printf("읽은 바이트수: %0ld\n", 
						xferCnt);
				}
			}
			else
			{
				printf("비동기 I/O 시작"
					"실패(RC=%ld)\n", rc);
			}
		}
		CloseHandle(hFile);
	}
	else
	{
		fprintf(stderr, "파일 열기 실패\n");
	}
	CloseHandle(hEvent);
	return 0;
}
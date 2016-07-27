/****************************************************************************

  3) GetOverlappedResult API로 처리 결과를 얻어온다.

     대기용 API를 사용하면 처리 완료는 알수있으나 중요한 처리 결과를 
	 알수는 없다.

	 이를 위해 GetOverlappedResult API가 존재한다.

	 BOOL GetOverlappedResult{	// 반환값 : 성공이면 TRUE
	  HANDLE hFile,				// 대상 파일 핸들
	  LPOVERLAPPED lpOver..		// OVERLAPPED 구조체의 주소
	  LPDWORD lpNumber...		// 실제로 전송된 바이트 수
	  BOOL bWait				// TRUE면 무한대기 FALSE면 즉시 복귀
	  );			


     BOOL GetOverlappedResult(HANDLE hFile, OVERLAPPED *lpOverlapped,
	  DWORD *lpNumberOfBytesTransfered, BOOL bWait)
	  {
		// 비동기 I/O가 완료되었는가 확인
		BOOL rc = HasOverlappedIoCompleted(lpOverlapped);
		if(rc)
		{
			*lpNumberOfBytesTransfered = 
				lpOverlapped->InternalHigh;
		}
		else
		{
			if(bWait)
			{
				HANDLE hSync = lpOverlapped->hEvent;
				if( hSync == 0) hSync = hFile;
				if(WaitForSingleObject(hSync, INFINITE) == 
						WAIT_OBJECT_0)
				{
					rc = TRUE;
					*lpNumberOfBytesTransfered = 
						lpOverlapped->InternalHigh;
				}
			}
		}
		return rc;
	  }

    *) 단지 비동기 I/O 처리가 종료되었다는 것만 확인하려면 
	   HasOverlappedIoCompleted API를 사용할 수도 있다.

****************************************************************************/














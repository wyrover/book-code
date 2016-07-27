//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
// [ 일반적 코딩 예 ]
//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓

BOOL Funcarama1()
{
	HANDLE	hFile			= INVALID_HANDLE_VALUE;
	PVOID	pvBuf			= NULL;
	DOWRD	dwNumBytesRead;	
	BOOL	fOK;
	
	hFile = CreateFile(..., ..., ..., ..., ..., ..., ...);
	if(hFile == INVALID_HANDLE_VALUE)	{
		return FALSE;
	}
	pvBuf = VirtualAlloc(..., ..., ..., ...);
	if(pvBuf == NULL)	{
		CloseHandle(hFile);
		return FALSE;
	}
	fOK = ReadFile(..., ..., ..., ..., ...);
	if(!OK || (dwNumBytesRead == 0)	{
		VirtualFree(..., ...);
		CloseHandle(hFile);
		return FALSE;
	}
	// 작업....
	// 마무리
	VirtualFree(.......);
	CloseHandle(hFile);
	return FALSE;
}
// 변경 1
BOOL Funcarama1()
{
	HANDLE	hFile			= INVALID_HANDLE_VALUE;
	PVOID	pvBuf			= NULL;
	DOWRD	dwNumBytesRead;	
	BOOL	fOK, fSuccess	= FALSE;
	
	hFile = CreateFile(..., ..., ..., ..., ..., ..., ...);
	if(hFile != INVALID_HANDLE_VALUE)	
	{
		pvBuf = VirtualAlloc(..., ..., ..., ...);
		if(pvBuf != NULL)	
		{
			fOK = ReadFile(..., ..., ..., ..., ...);
			if(fOK && (dwNumBytesRead != 0))	
			{
				// 작업....
				// 마무리
				fSuccess = TRUE;
			}
		}
		VirtualFree(.......);
	}
	CloseHandle(hFile);
	return FALSE;
}
// 코드 이해는 쉽지만 조건문이 많이 추가됨으로 들여쓰기가 많아져 화면의 오른쪽으로
// 치우쳐 있디.
// 증단 처리자를 이용 변경 
BOOL Funcarama1()
{
	HANDLE	hFile			= INVALID_HANDLE_VALUE;
	PVOID	pvBuf			= NULL;
	__try	
	{
		DOWRD	dwNumBytesRead;	
		BOOL	fOK;
		hFile = CreateFile(..., ..., ..., ..., ..., ..., ...);
		if(hFile == INVALID_HANDLE_VALUE)	{
			return FALSE;
		}
		pvBuf = VirtualAlloc(..., ..., ..., ...);
		if(pvBuf == NULL)	{
			return FALSE;
		}
		fOK = ReadFile(..., ..., ..., ..., ...);
		if(!fOK || (dwNumBytesRead != 1024))	{
			return FALSE;
		}
		// 데이터 처리할 작업 
	}
	__finally {
		// 함수의 실행여부를 나타내는 모든 요소를 마무리
		if(pvBuf != NULL)
			VirtualFree(pvBuf,...);
		if(hFile != INVALID_HANDLE_VALUE)
			CloseHandle(hFile);
	}
	// 작업 계속 수행 
	return FALSE;
}


// 모든 마무리 코드를 finally블록 안에 모여있다는 것
// return문과 같은 실행 흐름 변경코드 문제 
BOOL Funcarama1()
{
	HANDLE	hFile			= INVALID_HANDLE_VALUE;
	PVOID	pvBuf			= NULL;
	BOOL	fFunctinoOK		= NULL;
	__try	
	{
		DOWRD	dwNumBytesRead;	
		BOOL	fOK;
		hFile = CreateFile(..., ..., ..., ..., ..., ..., ...);
		if(hFile == INVALID_HANDLE_VALUE)	{
			__leave;
		}
		pvBuf = VirtualAlloc(..., ..., ..., ...);
		if(pvBuf == NULL)	{
			__leave;
		}
		fOK = ReadFile(..., ..., ..., ..., ...);
		if(!fOK || (dwNumBytesRead != 1024))	{
			__leave;
		}
		// 데이터 처리할 작업 
		// 함수가 올바르게 실행되었음을 알린다. 
		fFunctionOK	 = TRUE;
	}
	__finally {
		// 함수의 실행여부를 나타내는 모든 요소를 마무리
		if(pvBuf != NULL)
			VirtualFree(pvBuf,...);
		if(hFile != INVALID_HANDLE_VALUE)
			CloseHandle(hFile);
	}
	// 작업 계속 수행 
	return fFunctionOK;
}
// __leave ==> try블록이 끝나는 것으로 점프하게 된다. 

// AbnormalTerminateion()
// __finally 에서만 사용 가능한 함수
// try 에서 정상적으로 __finally로 들어왔다면 false를 리턴
BOOL Funcarama1()
{
	DWORD	dwTemp;
	// 1. 작업 처리
	__try	{
		WaitForSingleObject(g_hSem, INFINITE);
		DwTemp = Fincinator(g_dwProctedData);
	}
	__finally	{
		RelaseSemaphore(g_hSem, 1, NULL);
		if(!AbnormalTerminateion()){
			// 정상적으로 들어옴
		}
		else
		{
			// 비정상적으로 들어옴 
		}
	}
	return(dwTemp);
}
// 1.오류를 마무리하는 부분이 한 곳에 모여있고 그것이 실행되게 되어있기 때문에
//   오류 처리를 간단히 할 수 있다.
// 2. 프로그램의 가독성을 높일 수 있다.
// 3. 코드 유지가 쉽다.
// 4. 적당히 사용된다면 빠른 시간에 수행된다.
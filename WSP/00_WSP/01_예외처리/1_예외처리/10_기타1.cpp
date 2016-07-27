//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
// [ 예외 처리 2 ]
//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓


//─────────────────────────────────────
// 중단 처리자 
//─────────────────────────────────────


// Funcenstein1 /////////////////////////////////////////////////////////////
DWORD Funcenstenin1()
{
	DWORD dwTemp;
		// 1. 작업할 과정 기술
	__try 
	{
		// 2. protected 데이터를 접근할 수 있도록 요청하고 그것을 사용
		WaitForSingleObject(g_hMutex, INFINITE);
		g_dwProtectedData = 5;
		dwTemp = g_dwProtectedData;
		// 새로운 값을 돌려준다.
		 return(dwTemp);
	}
	__finally 
	{
		// 3. 다른 곳에도 protected 데이터를 허용한다.
		ReleaseMutex(g_hMutex);
	}
		// 4. 처리할 작업을 계속한다.
	dwTemp = 9;
	return(dwTemp);
}
// 1. 기본 1 --> 2 --> 3 --> 4 : 리턴값 9
// 2. return문 추가시 
//    스레드는 세마포어를 놓아주지 않는다.--> 문제 발생 
//    중단처리자를 사용함으로써 return 문 실행을 막아준다.
//     return 문이 try 문을 빠져나가려고 할 때, finally블록을 먼저 실행시킨다.
//     finally블록이 실행된 후에 함수는 복귀한다.    
//     따라서 리턴값 : 5  

// Funcenstein2 /////////////////////////////////////////////////////////////
DWORD Funcenstenin1()
{
	DWORD dwTemp;
		// 1. 작업할 과정 기술

	__try {

		// 2. protected 데이터를 접근할 수 있도록 요청하고 그것을 사용
		WaitForSingleObject(g_hSem, INFINITE);
		g_dwProtectedData = 5;
		dwTemp = g_dwProtectedData;
		// finally블록으로 점프한다.
		goto ReturnValue;
	}
	__finally {

		// 3. 다른 곳에도 protected 데이터를 허용한다.
		ReleaseSemaphore(g_hSem, 1, NULL);
	}

		// 4. 처리할 작업을 계속한다.
	dwTemp = 9;
ReturnValue:
	return(dwTemp);
}
// goto문을 만나면 finally블록을 실행한 후에 ReturnValue 레이블 아래의 코드를 
// 실행하게 된다.
// 따라서 return : 5
// 좋은 구문은 아니다 인위적으로 실행 흐름 변경....--> 성능 저하 초래 

// 사용 예  /////////////////////////////////////////////////////////////
DWORD Funcenstenin1()
{
	DWORD dwTemp;
		// 1. 작업할 과정 기술

	__try {

		// 2. protected 데이터를 접근할 수 있도록 요청하고 그것을 사용
		WaitForSingleObject(g_hSem, INFINITE);

		// 만약 부적절한 메모리 접근을 시도하는 함수라 하면..
		// 오류 박스가 보여 지고 프로세스 중단됨....
		// 다른 프로세스의 세마포어는 계속 기다리게 된다.
		dwTemp = Funcinator(g_dwProctedData);
	}
	__finally {

		// 3. 다른 곳에도 protected 데이터를 허용한다.
		// 하지만 __finally구문의 실행으로 인해 점유를 포기하게 만들어준다.
		ReleaseSemaphore(g_hSem, 1, NULL);
	}
		// 4. 처리할 작업을 계속한다.
	dwTemp = 9;
	return(dwTemp);
}

// 사용 예??  /////////////////////////////////////////////////////////////
DWORD Funcenstenin1()
{
	DWORD dwTemp = 0;
	while(dwTemp < 10) 
	{
		__try 
		{
			if (dwTemp == 2)
				continue;
			if (dwTemp == 3)
				break;
		}
		__finally 
		{
			dwTemp++;
		}
	}
	dwTemp += 10;
	return(dwTemp);
}

// 사용 예??  /////////////////////////////////////////////////////////////
DWORD Funcenstenin1()
{
	DWORD dwTemp;
		// 1. 작업할 과정 기술
	__try {

		// 2. protected 데이터를 접근할 수 있도록 요청하고 그것을 사용
		WaitForSingleObject(g_hSem, INFINITE);
		g_dwProctededData = 5;
		dwTemp = g_dwProctedData;
		// 새로운 값을 돌려준다.
		return(dwTemp);
	}
	__finally {
		// 3. 다른 곳에도 protected 데이터를 허용한다.
		ReleaseSemaphore(g_hSem, 1, NULL);
		return(103);
	}
	dwTemp = 9;
	return(dwTemp);
}
/////////////////////////////////////////////////////////////////////////////////////////
// 중단 처리자 사용시 try블록이나 finally 블록안에 return, continue, break, goto
// 문을 제거하고 이 명령들은 중단처리자 바깥부분에 넣는 것이 좋다. 
// --> 컴파일러가 보다 적은 코드와 빠른 코드를 생성하게 도와준다.
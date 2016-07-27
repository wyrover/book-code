//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓
// [ 예외 처리 1 ]
//〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓〓



// 예외처리 사용 : 실행 파일 크기 증가, 실행 속도 감소
// 따라서 전통적인 에러를 처리하기 힘든 경우에만 예외처리 
// 메커니즘 사용 
//Funcmeister1

DWORD Funcmeister1()
{
	DOWRD dwTemp;
		// 1. 데이터 처리 작업
	__try 
	{
		// 2. 연산을 수행 
		dwTemp = 0;
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {
		// 3. 예외상황 처리 한다. 이 부분은 실행되지 않는다.
	}
	// 4. 처리할 작업을 계속한다.
	return(dwTemp);
}
// try-finally와이 차이점
// 1. 실행 흐름 : 1 -> 2 -> 4
//    예외 상황이 발생될 경우에만 __except가 실행된다.
// 2. return, goto, continue, break문은 중단처리자 에서는 사용되는것이 좋지 않으나
//    예외 핸들러에서는 크게 상관이 없다.  


//Funcmeister2
#include <windows.h>
int main()
{
	DWORD dwTemp = 0;
		// 1. 데이터 처리 작업
	__try {
		// 2. 연산을 수행 
		MessageBox(NULL, "1", NULL, MB_OK);
		dwTemp = 5/dwTemp;	// 예외 상황을 발생시킨다. 
		MessageBox(NULL, "2", NULL, MB_OK);
		dwTemp += 10;		// 실행되지 않는다. 

	}
	__except(EXCEPTION_EXECUTE_HANDLER) {  // 0, 1, -1 ??
		// 3. 예외상황 처리 한다. 
		MessageBox(NULL, "3", NULL, MB_OK);
		MessageBeep(0);
	}
	// 4. 처리할 작업을 계속한다.
	MessageBox(NULL, "4", NULL, MB_OK);
	return(dwTemp);
}

// 5를 0으로 나누려고 시도하면 예외 상황 발생됨 --> except문의 맨 앞으로 이동...
// 예외 필터 : 
// 정의 된 값  1 --> EXCEPTION_EXECUTE_HANDLER (일반적으로 사용)
// 정의 된 값  0 --> EXCEPTION_CONTINUE_SEARCH
//                   예외 상황 인식 못함   
// 정의 된 값 -1 --> EXCEPTION_CONTINUE_EXECUTION --> 무한루프 
// 예외가 발생한 지점으로 가서 계속 실행 

// ??? ////////////////////////////////////////////////////////////////////////////

#include <windows.h>
char g_szBuffer[100];

LONG OliFilter1(char **ppchBuffer)
{
	if(*ppchBuffer == NULL)
	{
		*ppchBuffer = g_szBuffer;
		return(EXCEPTION_CONTINUE_EXECUTION);	//-1
	}
	return (EXCEPTION_EXECUTE_HANDLER);	// 1
}

void main()
{
	int x = 0;
	char *pchBuffer = NULL;
	__try {
		*pchBuffer = 'j';	// 오류 발생 
		x = 5 / x;
	}
	__except (OliFilter1(&pchBuffer)) {
	}
}



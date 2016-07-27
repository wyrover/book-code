//------------------------------------------------------------
// 대화상자 기반 애플리케이션의 구조와 동작원리 이해
//------------------------------------------------------------


/*

  대화상자는 다양한 컨트롤을 포함하는 일종의 윈도우 
  사용자의 입력을 처리하거나 출력하는 목적으로 사용 
   ex) 메모장의 찾기

  대화상자 기반 애플리캐이션
   ex) 계산기 


//---------------------------------------------
// 1. 구조와 동작 원리 
//---------------------------------------------

  - 대화상자 기반 애플리케이션을 작성하기 위해서는 코드와 더불어 대화상자
    템플릿을 작성해야 함 

    1) 비주얼 C++의 리소스 편집기를 이용하여 대화상자를 시각적으로 디자인

	2) 디스크에 *.rc 파일로 저장
	   이를 리소스 스크립트라 부르는데, 
	   대화상자와 대화상자에 포함된 컨트롤에 대한 정보가 택스트 형태로 저장된다.

	3) 프로젝트 컴파일 과정에서 리소스 컴파일러가 실행되어, *.rc 파일을 이진 
	   형태인 *.res 로 변환하고, 최종 링크 과정에서 실행 파일에 리소스로 
	   포함된다. 

    4) 프로그램 실행 중 대화상자를 생성하는 API 함수를 호출하면 운영체제는 실행
	   파일에 포함된 대화상자 템플릿을 토대로 대화상자를 생성한다. 


#include <windows.h>

// 1) 대화상자와 대화상자에 포함된 컨트롤은 각각 서로 다른 ID를 사용한다.
      리소스 스크립트 (*.rc)를 저장하면 resource.h 파일이 같이 생성되는데
	  코드에서 ID를 직접 참조할 수 있도록 상수 정의되어 있다. 반드시 포함 
#include "resource.h"


// 2) 대화상자 메시지 처리 함수
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 3) 대화 상자 생성 하는 코드 
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

	return 0;
}

// 4) 대화상자 메시지 처리
BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, 
	WPARAM wParam, LPARAM lParam)
{
	static char buf[BUFSIZE+1];

	switch(uMsg){

	// 5) 대화상자의 초기화  작업 수행 
	case WM_INITDIALOG:
		return TRUE;

   // 6) 대화상자에 포함된 컨트롤이 발생시키는 메시지 
         
	case WM_COMMAND:
		switch(LOWORD(wParam)){		// 7) 컨트롤의 ID가 전달됨 
		case IDOK:

		    // 8) 대화상자 종료 함수 
			EndDialog(hDlg, 0);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

  [ 실   습 ] 대화상자 기반 애플리케이션 작성 



*/
/*****************************************************************************
[ 콘솔을 이용한 문서 편집기 ]
  - 저수준 API를 사용하여 마우스 입력 처리  

  콘솔로부터 들어오는 사용자 입력은 일단 입력 버퍼에 저장된다.
  입력 버퍼는 사용자가 키보드의 키를 누르거나 뗄때 혹은 마우스 포인터를 옮기는 등의 
  이벤트 정보를 보관 유지하는 INPUT_RECORD 구조체를 포함하는 큐다.
  콘솔 입력용 API를 사용해서 입력 버퍼에서 이벤트를 꺼내 사용자 입력을 처리한다.

  콘솔 입력용 API는 고수준과 저수준으로 나뉜다.
  ReadFile(고수준API) : C/C++라이브러리 함수와 동일 수준
                        즉 마우스 입력은 무시됨
  
  ReadConsoleInput(저수준 API) : 입력 버퍼로부터 이벤트를 INPUT_RECORD 구조체 
                                형태로 꺼낼 수 있다.
   문자열 입력 처리시에는 귀찮치만, 마우스 조작을 이용할 수 있는 장점을 가진다.
   기본값이 마우스 입력을 받지 않는 설정이므로 SetConsoleMode API를 사용해 입력 
   보드 변경이 필요하다.


실행코드)
   유사 문서 편집기 
   커서 키와 마우스 커서의 위치를 옮겨서 그곳에 문자를 입력할 수 있슴
   ESC 키를 누르면 프로그램이 종료되며 그때까지 입력한 내용을 'edit.txt' 파일에 저장하게 됨
   2byte 문자 입력은 가능하나, 커서 이동시는 화살표키를 두번 눌러야 함
********************************************************************************/

#pragma comment(linker, "/subsystem:console")

#include <windows.h>
#include <stdio.h>

int main()
{
	HANDLE				hIn, hOut, hOrig;
	INPUT_RECORD		ir;
	DWORD				mode;
	BOOL				bCont = TRUE;

	CONSOLE_SCREEN_BUFFER_INFO	csbi;
	CONSOLE_CURSOR_INFO			cci;
	COORD						csr;

	unsigned char		ch;
	DWORD				sz;
	FILE				*fp;

	// 입력 버퍼 핸들 얻기
	hIn = CreateFile("CONIN$", GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);

	if( hIn == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "입력 버퍼 핸들 얻기 실패\n");
		return -1;
	}

	// 스크린 버퍼 핸들 얻기
	hOrig = CreateFile("CONOUT$",  GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0); 

	if( hOrig == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "스크린 버퍼 핸들 얻기 실패\n");
		return -1;
	}

	// 새로운 스크린 버퍼를 만들어서 활성화
	hOut = CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE, 0, NULL,
		CONSOLE_TEXTMODE_BUFFER, NULL);
	if( hOut == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "스크린 버퍼 생성 실패\n");
		CloseHandle(hOrig);
		CloseHandle(hIn);
		return -1;
	}
	// 활성 스크린 버퍼 변경 
	SetConsoleActiveScreenBuffer(hOut);


	// 윈도우 제목 변경
	SetConsoleTitle("문서 편집기(ESC로 종료)");


	// 마우스 입력을 받기 위해 입력 모드 변경
	GetConsoleMode(hIn, &mode);
	SetConsoleMode(hIn, mode | ENABLE_MOUSE_INPUT);

	// 커서의 크기를 문자 셀과 같도록
	cci.dwSize		= 100;
	cci.bVisible	= TRUE;

	SetConsoleCursorInfo(hOut, &cci);


	// 입력 처리 루프
	while( bCont )
	{
		// 입력 처리 이벤트 하나 읽기
		if( ReadConsoleInput(hIn, &ir, 1, &sz))
		{
			// 현재 커서의 위치 얻기
			GetConsoleScreenBufferInfo(hOut, &csbi);
			csr.X	= csbi.dwCursorPosition.X;
			csr.Y	= csbi.dwCursorPosition.Y;

			// 키보드 입력
			if( ir.EventType == KEY_EVENT)
			{
				// 눌려진 이벤트만 처리
				if( ir.Event.KeyEvent.bKeyDown )
				{
					switch(ir.Event.KeyEvent.wVirtualKeyCode)
					{
						// ESC 이면 종료
					case VK_ESCAPE:	bCont	= FALSE;	break;

						// 화살표키면 커서 이동
					case VK_RIGHT:
						if( csr.X < csbi.dwSize.X -1 )
						{
							csr.X++;
							SetConsoleCursorPosition(hOut, csr);
						}
						break;
					case VK_LEFT:
						if( csr.X < csbi.dwSize.X -1 )
						{
							csr.X--;
							SetConsoleCursorPosition(hOut, csr);
						}
						break;
					case VK_UP:
						if( csr.Y < csbi.dwSize.Y -1 )
						{
							csr.Y--;
							SetConsoleCursorPosition(hOut, csr);
						}
						break;
					case VK_DOWN:
						if( csr.Y < csbi.dwSize.Y -1 )
						{
							csr.Y--;
							SetConsoleCursorPosition(hOut, csr);
						}
						break;
					case VK_RETURN:
						if( csr.Y	< csbi.dwSize.Y-1)
						{
							csr.X	= 0;
							csr.Y++;
							SetConsoleCursorPosition(hOut, csr);
						}
						break;

					default:
						// 무효한 문자라면 커서 위치에 출력
						ch = (unsigned char)ir.Event.KeyEvent.uChar.AsciiChar;

						if( ch >= 0x20 )
						{
							WriteFile(hOut, &ir.Event.KeyEvent.uChar.AsciiChar, 1, &sz, NULL);
						}


					}
				}
			}
			else if(ir.EventType	== MOUSE_EVENT)
			{
				// 버튼이 눌려졌으면 커서 이동
				if( ir.Event.MouseEvent.dwButtonState	& 0x01)
				{
					SetConsoleCursorPosition(hOut, ir.Event.MouseEvent.dwMousePosition);
				}
			}
		}
	}

	SetConsoleActiveScreenBuffer(hOrig);
	CloseHandle(hOrig);
	CloseHandle(hIn);

	// 스크린 버퍼의 내용을 파일에 저장
	//....

	CloseHandle(hOut);

	return 0;


}



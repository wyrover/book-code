#pragma comment(linker, "/subsystem:windows")

#define _WIN32_WINNT 0x0500 // win2000 용 함수와 메시지를 사용하기 위해
#define WINVER	0x0500 // win98 용 함수와 메시지를 사용하기 위해

#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"

//전체 그리라고 했는데, 일부만 그려지는가 과연.
//그렇다면 그걸 어떻게 알고 일부만 그리는가.
#define EMPTY 0
#define ONE	1
#define TWO 2
#define THREE 3
#define FOUR 4
#define FIVE 5
#define SIX 6
#define SEVEN 7
#define EIGHT 8
#define BOMB 99

#define CLOSE 0
#define OPEN 1
#define QUESTION 2
#define FLAG 3
#define NONFOUNDMINE 4
#define ERRORCHECKMINE 5

#define START 4
#define NORMAL 5
#define WIN 6
#define LOSE 7
#define MINE_WIDTH 16
#define MINE_HEIGHT 16
#define BOARD_SIZE 9
#define KILL 100
#define MINE_NUM 10

#define PICFLAG	16
#define PICQUESTION 16*2
#define PICREDMINE	16*3
#define PICXMINE	16*4
#define PICBLACKMINE	16*5

typedef struct _BLOCK
{
	int state;
	char isOpen;
} BLOCK;

BLOCK g_board[BOARD_SIZE][BOARD_SIZE];
int GAMESTATE = NORMAL;
int chooseMineNum;
int foundMineNum;
int blockMove;
int RButtonDown;
int LButtonDown;

void InitMine(HWND hwnd)
{
	// 현재 상태 초기화
	GAMESTATE = START;
	// 사용자가 선택한 지뢰 갯수 초기화
	chooseMineNum = 0;
	// 실제 찾아진 지뢰 갯수 초기화
	foundMineNum = 0;
	blockMove = 0;
	RButtonDown = 0;
	LButtonDown = 0;

	int i, j, x, y;
		
	// 보드 초기화.
	for(i=0; i < BOARD_SIZE; i++)
	{
		for(j=0; j < BOARD_SIZE; j++)
		{
			g_board[i][j].isOpen = CLOSE;
			g_board[i][j].state = EMPTY;
		}
	}

	// 폭탄을 랜덤하게 배치하고 state 항목을 초기화한다.
	srand(time(NULL));
	
	for(int count = 0; count < MINE_NUM;)
	{
		i = rand() % 9;
		j = rand() % 9;
	
		if(g_board[i][j].state != BOMB)
		{
			g_board[i][j].state = BOMB;
			++count;

			// 지뢰 주변의 갯수 설정
			for(x = max(0, i - 1); x <= min(BOARD_SIZE-1, i + 1); x++)
			{
				for(y = max(0, j - 1); y <= min(BOARD_SIZE-1, j + 1); y++)
				{
					if(g_board[x][y].state != BOMB)
					{
						++g_board[x][y].state;
					}
				}
			}
		}
	}
}

void LButtonClick(HWND hwnd, int x, int y)
{
	if((x < 0 || y < 0 || x >= BOARD_SIZE || y >= BOARD_SIZE) && (x+y) != KILL)	return;

	// 이미 열리거나 선택된 위치라면 그냥 리턴
	if(g_board[x][y].isOpen != CLOSE)	return;

	g_board[x][y].isOpen = OPEN;

	// 지뢰를 밟은 거라면
	if(g_board[x][y].state == BOMB || (x+y == KILL))
	{
		GAMESTATE = LOSE;// 게임에 졌음을 설정

		//모든 영역 보여주기.
		InvalidateRect(hwnd, 0, FALSE);
		UpdateWindow(hwnd);
		

		if(MessageBox(hwnd, "지뢰를 밟았습니다.\n\n다시 게임을 시작하겠습니까?", "지뢰찾기", MB_YESNO) == IDYES)
		{
			InitMine(hwnd);
			InvalidateRect(hwnd, 0, FALSE);
			UpdateWindow(hwnd);
		}

		return;
	}

	// EMPTY 블럭을 클릭한 경우 이어진 EMPTY 블럭 모두 열어주기.
	if(g_board[x][y].state == EMPTY)
	{
		for(int i = max(0, x-1); i <= min(BOARD_SIZE-1, x+1); i++)
		{
			for(int j = max(0, y-1); j <= min(BOARD_SIZE-1, y+1); j++)
			{
				LButtonClick(hwnd, i, j);
			}
		}
	}

	// 현재 클릭 위치 다시 그리기
	RECT rect;
	rect.left	= 12 + MINE_WIDTH * x;
	rect.top	= 55 + MINE_HEIGHT * y;
	rect.right	= rect.left + MINE_WIDTH;
	rect.bottom	= rect.top + MINE_HEIGHT;

	InvalidateRect(hwnd, &rect, FALSE);
	
}

void RButtonClick(HWND hwnd, int x, int y)
{
	int i, j;

	if(x < 0 || y < 0 || x >= BOARD_SIZE || y >= BOARD_SIZE)	return;

	if(g_board[x][y].isOpen == OPEN)	return;		// 이미 열린 위치라면 그냥 리턴
	else if(g_board[x][y].isOpen == FLAG) 	// 이미 지뢰로 선택된 위치라면
	{
		// 물음표 기호로 바꾼다.
		g_board[x][y].isOpen = QUESTION; 
		// 선택 갯수를 줄인다
		--chooseMineNum;
		// 찾은 갯수를 줄인다
		if(g_board[x][y].state == BOMB)	--foundMineNum;
	}
	else if(g_board[x][y].isOpen == QUESTION)	// 물음표 표시 제거
		g_board[x][y].isOpen = CLOSE;
	else	// 닫힌 상태면, 지뢰플래그 설정
	{
		g_board[x][y].isOpen = FLAG;
		// 지뢰 설정한 갯수 추가
		++chooseMineNum;
		// 지뢰 있는 곳을 선택했으면 찾은 지뢰갯수 추가
		if(g_board[x][y].state == BOMB)	++foundMineNum;
	}

	// 현재 클릭 위치 다시 그리기
	RECT rect;
	rect.left	= 12 + MINE_WIDTH * x;
	rect.top	= 55 + MINE_HEIGHT * y;
	rect.right	= rect.left + MINE_WIDTH;
	rect.bottom	= rect.top + MINE_HEIGHT;

	InvalidateRect(hwnd, &rect, FALSE);

	// 만약 찾은 지뢰 갯수가 10개라면
	if(foundMineNum == MINE_NUM)
	{
		for(i=0; i<BOARD_SIZE; i++)
		{
			for(j=0; j<BOARD_SIZE; j++)
				if(g_board[i][j].isOpen == CLOSE) return;
		}
		// 게임에서 이겼음을 설정한다
		GAMESTATE = WIN;
		InvalidateRect(hwnd, NULL, TRUE);

		if(MessageBox(hwnd, "지뢰찾기 승리!\n\n다시 게임을 시작하겠습니까?",
			"지뢰찾기", MB_YESNO) == IDYES)
		{
			InitMine(hwnd);
			InvalidateRect(hwnd, NULL, FALSE);
		}
	}
}

void CheckGameEnd(HWND hwnd)
{
	// 지뢰만 남았는지 확인. 모든 남은 블럭이 지뢰가 아니면 그냥 리턴.
	for(int i = 0; i < BOARD_SIZE; i++)
	{
		for(int j = 0; j < BOARD_SIZE; j++)
		{
			if(g_board[i][j].isOpen == CLOSE && g_board[i][j].state != BOMB)	return;
		}
	}

	// 열리지 않은 지뢰들을 지뢰로 설정한 것처럼 바꿈
	for(i = 0; i < BOARD_SIZE; i++)
	{
		for(int j = 0; j < BOARD_SIZE; j++)
			if(g_board[i][j].isOpen == CLOSE)	g_board[i][j].isOpen = FLAG;
	}

	// 게임에서 이겼음을 설정한다
	GAMESTATE = WIN;
	InvalidateRect(hwnd, 0, TRUE);

	if(MessageBox(hwnd, "지뢰찾기 승리!\n\n다시 게임을 시작하겠습니까?",
		"지뢰찾기", MB_YESNO) == IDYES)
	{
		InitMine(hwnd);
		InvalidateRect(hwnd, 0, TRUE);
	}
}

void Draw3dRect(HDC hdc, int x, int y, int xx, int yy, BOOL down, int width)
{
	COLORREF clrMain = RGB(192, 192, 192),
			 clrLight = RGB(255, 255, 255),
			 clrDark = RGB(128, 128, 128);
	
	HPEN hPen1, hPen2, hOldPen;
	HBRUSH hBrush, hOldBrush;

	if(down)
	{
		hPen2 = CreatePen(PS_SOLID, 1, clrLight);
		hPen1 = CreatePen(PS_SOLID, 1, clrDark);
	}
	else
	{
		hPen1 = CreatePen(PS_SOLID, 1, clrLight);
		hPen2 = CreatePen(PS_SOLID, 1, clrDark);
	}

	hBrush = CreateSolidBrush(clrMain);
	hOldPen = (HPEN)SelectObject(hdc, hPen1); // DC의 기존 펜을 백업한다.
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush); // DC의 기존 브러쉬를 백업한다.

	Rectangle(hdc, x, y, xx+1, yy+1);
	
	for(int i=0; i < width; i++)
	{
		SelectObject(hdc, hPen1);

		MoveToEx(hdc, xx - 1, y, 0);
		LineTo(hdc, x, y);
		LineTo(hdc, x, yy-1);

		SelectObject(hdc, hPen2);
		
		MoveToEx(hdc, x, yy, 0);
		LineTo(hdc, xx, yy);
		LineTo(hdc, xx, y);

		x++; ;y++; xx--; yy--;
	}

	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);

	DeleteObject(hPen1);
	DeleteObject(hPen2);
	DeleteObject(hBrush);
}


LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE hInstance;
	static HBITMAP mybit;
	static POINTS pt = {0, 0};
	HDC    hdc, MemDC;
	PAINTSTRUCT  ps;
	static RECT rect;
		
	switch( msg )
	{	
	
	case WM_CREATE:
		{
			hInstance = ((LPCREATESTRUCT) lParam)->hInstance;
			mybit = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
			InitMine(hwnd);
		}
		return 0;

	case WM_PAINT:
		{			
			hdc = BeginPaint(hwnd, &ps);
			MemDC= CreateCompatibleDC(hdc);
			SelectObject(MemDC, mybit);

			int x =  (pt.x-12) / MINE_WIDTH;
			int y =  (pt.y-55) / MINE_HEIGHT;

			Draw3dRect(hdc, 0, 0, 168, 211, FALSE, 3);
			Draw3dRect(hdc, 9, 9, 156, 46, TRUE, 2);
			Draw3dRect(hdc, 15, 14, 57, 40, TRUE, 1);
			Draw3dRect(hdc, 109, 14, 150, 40, TRUE, 1);
			Draw3dRect(hdc, 9, 52, 159, 202, TRUE, 3);

			for(int i = 0; i < BOARD_SIZE; i++)
			{
				for(int j = 0; j < BOARD_SIZE; j++)
				{
					if(GAMESTATE == WIN || GAMESTATE == LOSE) // 게임이 끝났다면
					{
						// 마인이 있는 위치라면
						if(g_board[i][j].state == BOMB)
						{
							switch(g_board[i][j].isOpen)
							{
							case CLOSE:
								// 검정 지뢰
								BitBlt(hdc, i*MINE_WIDTH + 12, j*MINE_HEIGHT +55, MINE_WIDTH, MINE_HEIGHT, MemDC, 0, PICBLACKMINE, SRCCOPY);
								break;
							case OPEN:
								// 빨강 지뢰
								BitBlt(hdc, i*MINE_WIDTH + 12, j*MINE_HEIGHT +55, MINE_WIDTH, MINE_HEIGHT, MemDC, 0, PICREDMINE, SRCCOPY);
								break;
							case FLAG:
								// 깃발 그림
								BitBlt(hdc, i*MINE_WIDTH + 12, j*MINE_HEIGHT +55, MINE_WIDTH, MINE_HEIGHT, MemDC, 0, PICFLAG, SRCCOPY);
								break;
							case QUESTION:
								// 물음표 그림
								BitBlt(hdc, i*MINE_WIDTH + 12, j*MINE_HEIGHT +55, MINE_WIDTH, MINE_HEIGHT, MemDC, 0, PICQUESTION, SRCCOPY);
								break;
							case NONFOUNDMINE:
								// 숨겨진 지뢰
								BitBlt(hdc, i*MINE_WIDTH + 12, j*MINE_HEIGHT +55, MINE_WIDTH, MINE_HEIGHT, MemDC, 0, PICREDMINE, SRCCOPY);
								break;
							}
						}
						else
						{
							switch(g_board[i][j].isOpen)
							{
							case CLOSE:
								BitBlt(hdc, i*MINE_WIDTH + 12, j*MINE_HEIGHT +55, MINE_WIDTH, MINE_HEIGHT, MemDC, 0, 0, SRCCOPY);
								break;
							case OPEN :
								BitBlt(hdc, i*MINE_WIDTH + 12, j*MINE_HEIGHT +55, MINE_WIDTH, MINE_HEIGHT, MemDC, 0, (16*15)-g_board[i][j].state*16, SRCCOPY);
								break;
							case FLAG:
								// X 표시
								BitBlt(hdc, i*MINE_WIDTH + 12, j*MINE_HEIGHT +55, MINE_WIDTH, MINE_HEIGHT, MemDC, 0, PICXMINE, SRCCOPY);
								break;
							case QUESTION:
								// 물음표 그림
								BitBlt(hdc, i*MINE_WIDTH + 12, j*MINE_HEIGHT +55, MINE_WIDTH, MINE_HEIGHT, MemDC, 0, PICQUESTION, SRCCOPY);
								break;
							case ERRORCHECKMINE:
								// 지뢰로 잘못 설정한 블럭
								BitBlt(hdc, i*MINE_WIDTH + 12, j*MINE_HEIGHT +55, MINE_WIDTH, MINE_HEIGHT, MemDC, 0, PICXMINE, SRCCOPY);
								break;
							}
							
						}	
					}
					else
					{
						switch(g_board[i][j].isOpen)
						{
							case CLOSE: 								
								// 기본 블럭
								if(LButtonDown && !blockMove)	BitBlt(hdc, i*MINE_WIDTH + 12, j*MINE_HEIGHT +55, MINE_WIDTH, MINE_HEIGHT, MemDC, 0, 16*15, SRCCOPY);
								else BitBlt(hdc, i*MINE_WIDTH + 12, j*MINE_HEIGHT +55, MINE_WIDTH, MINE_HEIGHT, MemDC, 0, 0, SRCCOPY);
								break;
							case OPEN:
								// 숫자
								BitBlt(hdc, i*MINE_WIDTH + 12, j*MINE_HEIGHT +55, MINE_WIDTH, MINE_HEIGHT, MemDC, 0, (16*15)-g_board[i][j].state*16, SRCCOPY);
								break;
							case FLAG:
								// 깃발 그림
								BitBlt(hdc, i*MINE_WIDTH + 12, j*MINE_HEIGHT +55, MINE_WIDTH, MINE_HEIGHT, MemDC, 0, PICFLAG, SRCCOPY);
								break;
							case QUESTION:
								// 물음표 그림
								BitBlt(hdc, i*MINE_WIDTH + 12, j*MINE_HEIGHT +55, MINE_WIDTH, MINE_HEIGHT, MemDC, 0, PICQUESTION, SRCCOPY);
								break;
						}
					}

					
					
				}
			}

			DeleteDC(MemDC);
			EndPaint(hwnd, &ps);
					
		}
		return 0;
	case WM_MOUSEMOVE:
		{
			if(LButtonDown)
			{
				int x =  (pt.x-12) / MINE_WIDTH;
				int y =  (pt.y-55) / MINE_HEIGHT;
				int tempx = x;
				int tempy = y;
		
				pt = MAKEPOINTS(lParam);
				x =  (pt.x-12) / MINE_WIDTH;
				y =  (pt.y-55) / MINE_HEIGHT;
					
				if(g_board[x][y].isOpen == CLOSE)
				{	
					if(!(tempx==x && tempy==y))
					{
						rect.left	= 12 + MINE_WIDTH * x;
						rect.top	= 55 + MINE_HEIGHT * y;
						rect.right	= rect.left + MINE_WIDTH;
						rect.bottom	= rect.top + MINE_HEIGHT;
						InvalidateRect(hwnd, &rect, FALSE);
						UpdateWindow(hwnd);
						
						blockMove = 1;
						rect.left	= 12 + MINE_WIDTH * tempx;
						rect.top	= 55 + MINE_HEIGHT * tempy;
						rect.right	= rect.left + MINE_WIDTH;
						rect.bottom	= rect.top + MINE_HEIGHT;
						InvalidateRect(hwnd, &rect, FALSE);
						UpdateWindow(hwnd);
						
						blockMove = 0;
						}
				}
				else
				{
						blockMove = 1;
						rect.left	= 12 + MINE_WIDTH * tempx;
						rect.top	= 55 + MINE_HEIGHT * tempy;
						rect.right	= rect.left + MINE_WIDTH;
						rect.bottom	= rect.top + MINE_HEIGHT;
						InvalidateRect(hwnd, &rect, FALSE);
						UpdateWindow(hwnd);

						blockMove = 0;
				}
				return 0;
		
			}
			return 0;
		}  

	case WM_LBUTTONDOWN: 
		{
			if(GAMESTATE == WIN || GAMESTATE == LOSE)		return 0;	// 게임이 끝난 상태에서는 버튼입력을 받지 않음.
				
			pt = MAKEPOINTS(lParam);
			// NORMAL, WIN, LOSE의 세가지 상태 존재.
			int x =  (pt.x-12) / MINE_WIDTH;
			int y =  (pt.y-55) / MINE_HEIGHT;
			
			if(g_board[x][y].isOpen == CLOSE)
			{
				rect.left	= 12 + MINE_WIDTH * x;
				rect.top	= 55 + MINE_HEIGHT * y;
				rect.right	= rect.left + MINE_WIDTH;
				rect.bottom	= rect.top + MINE_HEIGHT;
				InvalidateRect(hwnd, &rect, FALSE);
			}
		
			LButtonDown = 1;
			// 클릭했으므로 위의 그림 다시 그려줌.
			
			// 왼쪽버튼이 눌린 상태에서 오른쪽버튼도 눌린 경우를 처리
			if(RButtonDown) 
			{
				int aroundflagNum = g_board[x][y].state;
				int realFlagCount=0;
				int checkFlagCount=0;
							
				// 보드 내의 블럭을 클릭했는지 검사.
				if(x < 0 || y < 0 || x >= BOARD_SIZE || y >= BOARD_SIZE)	return 0;

				// 클릭한 블럭이 열려 있는 상태인지 검사.
				if(g_board[x][y].isOpen != OPEN)		return 0;

				// 이제 이 부분까지 왔으면, for문 돌면서 열릴 조건인지 체크.
				for(int i=max(0, x-1); i<=min(x+1, BOARD_SIZE-1); i++)
				{
					for(int j=max(0, y-1); j<=min(y+1, BOARD_SIZE-1); j++)
					{
						if(!(i==x && j==y))
						{// 지뢰라고 체크한 곳이 실제 지뢰가 있는 위치인 경우 ++realFlagCount
							if(g_board[i][j].isOpen == FLAG)
							{
								++checkFlagCount;
								if(g_board[i][j].state == BOMB)
									++realFlagCount;
								else
									g_board[i][j].isOpen = ERRORCHECKMINE;
							}
							else
							{
								if(g_board[i][j].state == BOMB)
									g_board[i][j].isOpen = NONFOUNDMINE;
							}

						}
					}
				}

				// 실제 주변지뢰 갯수와 사용자가 체크한 지뢰 갯수가 같은데, 잘못 체크했을 경우
				if(aroundflagNum == checkFlagCount && realFlagCount != checkFlagCount)
				{
					LButtonClick(hwnd, KILL, 0);
					return 0;
				}

				// 모든 조건을 통과했으면 지뢰라고 체크한 곳을 제외한 클릭 위치 주변을 모두 연다.
				if(aroundflagNum == checkFlagCount)
				{
					for(int i=max(0, x-1); i<=min(x+1, BOARD_SIZE-1); i++)
					{
						for(int j=max(0, y-1); j<=min(y+1, BOARD_SIZE-1); j++)
						{
							if(!(i==x && j==y))
							{
								if(g_board[i][j].state != BOMB)		LButtonClick(hwnd, i, j);
							}
						}
					}
				}
				// 동시클릭에 대한 처리를 완료 했으므로 RButtonDown 을 초기화.
				LButtonDown = 0;
				RButtonDown = 0;
				// 현재 지뢰를 제외하고 모든 곳이 열려있는지 체크해서 '참' 값이면 게임 종료.				
				CheckGameEnd(hwnd);
			}
		}
		return 0;

	case WM_LBUTTONUP:
		{
			if(GAMESTATE == WIN || GAMESTATE == LOSE)		return 0;	// 게임이 끝난 상태에서는 버튼입력을 받지 않음.

			LButtonDown = 0;

			pt = MAKEPOINTS(lParam);
			int x = (pt.x - 12) / MINE_WIDTH;
			int y = (pt.y - 55) / MINE_HEIGHT;

			if(GAMESTATE == START)
			{
				int mineCount=0;
				int i, j;
				
				// 처음에 클릭한 위치에 지뢰가 있다면,
				if(g_board[x][y].state == BOMB)
				{
					// 지뢰를 옮길것이므로, 지뢰주변의 숫자를 -1 씩 한다.
					for(i=max(0, x - 1); i<=min(BOARD_SIZE-1, x + 1); i++)
					{	
						for(j=max(0, y - 1); j<=min(BOARD_SIZE-1, y + 1); j++)
						{
							if(!(i==x && j==y))
							{
								if(g_board[i][j].state != BOMB)
									--g_board[i][j].state;
							
								else	
									++mineCount;
							}
						}
					}

					// 그리고 지뢰가 있던 자리에도 적절한 숫자를 넣어줘야 하므로, 위의 mineCount-1 값을 넣어준다. (주변의 지뢰갯수)
					g_board[x][y].state = mineCount;
					
					// 그리고 지뢰를 다른 곳에 넣어준다.			
					srand(time(NULL));
					
					// 새로 할당한 위치에 이미 지뢰가 존재한다면, 다시 랜덤값을 할당받는다.
					do{
						i = rand() % BOARD_SIZE;	
						j = rand() % BOARD_SIZE;
					}while(g_board[i][j].state == BOMB);
					
					// 새로 할당한 위치에 지뢰가 존재하지 않는다면, 그 위치에 지뢰를 넣는다.
					g_board[i][j].state = BOMB;

					// 새로 할당한 위치 주변의 숫자를 전부 1씩 증가시킨다.
					for(int k = max(0, i - 1); k <= min(BOARD_SIZE-1, i + 1); k++)
					{	
						for(int l = max(0, j - 1); l <= min(BOARD_SIZE-1, j + 1); l++)
						{
							if(g_board[k][l].state != BOMB)
							{
								if(!(k==i && l==j))
									++g_board[k][l].state;
							}
						}
					}
				}
				GAMESTATE = NORMAL;
			}

			LButtonClick(hwnd, x, y);
			CheckGameEnd(hwnd);


		}
		return 0;
	
	case WM_RBUTTONDOWN:
		{
			if(GAMESTATE == WIN || GAMESTATE == LOSE)		return 0;

			pt = MAKEPOINTS(lParam);

			RButtonDown = 1;

			if(LButtonDown)
			{
				int x = (pt.x - 12) / MINE_WIDTH;
				int y = (pt.y - 55) / MINE_HEIGHT;
		
				int aroundflagNum = g_board[x][y].state;
				int realFlagCount=0;
				int checkFlagCount=0;
				
				// 보드 내의 블럭을 클릭했는지 검사.
				if(x < 0 || y < 0 || x >= BOARD_SIZE || y >= BOARD_SIZE)	return 0;

				// 클릭한 블럭이 열려 있는 상태인지 검사.
				if(g_board[x][y].isOpen != OPEN)		return 0;

				// 이제 이 부분까지 왔으면, for문 돌면서 열릴 조건인지 체크.
				for(int i=max(0, x-1); i<=min(x+1, BOARD_SIZE-1); i++)
				{
					for(int j=max(0, y-1); j<=min(y+1, BOARD_SIZE-1); j++)
					{
						if(!(i==x && j==y))
						{// 지뢰라고 체크한 곳이 실제 지뢰가 있는 위치인 경우 ++realFlagCount
							if(g_board[i][j].isOpen == FLAG)
							{
								++checkFlagCount;
								if(g_board[i][j].state == BOMB)
									++realFlagCount;
								else
									g_board[i][j].isOpen = ERRORCHECKMINE;
							}
							else
							{
								if(g_board[i][j].state == BOMB)
									g_board[i][j].isOpen = NONFOUNDMINE;
							}

						}
					}
				}
		
				// 실제 주변지뢰 갯수와 사용자가 체크한 지뢰 갯수가 같은데, 잘못 체크했을 경우
				if(aroundflagNum == checkFlagCount && realFlagCount != checkFlagCount)
				{
					LButtonClick(hwnd, KILL, 0);
					return 0;
				}

				// 모든 조건을 통과했으면 지뢰라고 체크한 곳을 제외한 클릭 위치 주변을 모두 연다.
				if(aroundflagNum == checkFlagCount)
				{
					for(int i=max(0, x-1); i<=min(x+1, BOARD_SIZE-1); i++)
					{
						for(int j=max(0, y-1); j<=min(y+1, BOARD_SIZE-1); j++)
						{
							if(!(i==x && j==y))
							{
								if(g_board[i][j].state != BOMB)		LButtonClick(hwnd, i, j);
							}
						}
					}
				}
				// 동시클릭에 대한 처리를 완료 했으므로 RButtonDown 을 초기화.
				RButtonDown = 0;
				LButtonDown = 0;
				// 현재 지뢰를 제외하고 모든 곳이 열려있는지 체크해서 '참' 값이면 게임 종료.				
				CheckGameEnd(hwnd);
			}
		}
		return 0;
	
	case WM_RBUTTONUP:
		{
			pt = MAKEPOINTS(lParam);
			int x = (pt.x - 12) / MINE_WIDTH;
			int y = (pt.y - 55) / MINE_HEIGHT;

			if(GAMESTATE == WIN || GAMESTATE == LOSE)		return 0;

			RButtonDown = 0;

			if(chooseMineNum == 10 && g_board[x][y].isOpen != FLAG)
			{
				MessageBox(hwnd, "더이상 선택할 수 없습니다. ", "지뢰찾기", MB_OK);
				return 0;
			}

			RButtonClick(hwnd, x, y);
		}
		return 0;

	case WM_DESTROY:
		DeleteObject(mybit);
		PostQuitMessage(0); 
		return 0;
	}
	return DefWindowProc( hwnd, msg, wParam, lParam);
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPSTR	  lpCmdLine, int nShowCmd )
{
	ATOM atom;
	WNDCLASS wc;
	HWND hwnd;
	MSG msg;
	
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hbrBackground= (HBRUSH)GetStockObject( WHITE_BRUSH ); 
	wc.hCursor		= LoadCursor( 0, IDC_ARROW );
	wc.hIcon		= LoadIcon( 0, IDI_APPLICATION);
	wc.hInstance	= hInstance;
	wc.lpfnWndProc  = WndProc; 
	wc.lpszClassName= "MYMINE";
	wc.lpszMenuName = 0;
	wc.style		= 0;

	atom = RegisterClass( &wc);
	
	if ( atom == 0 ) 
	{
		MessageBox( 0, "Fail To RegisterClass", "Error", MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx( 0, "MYMINE", "지뢰찾기", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~ WS_MAXIMIZEBOX,
								0, 0, 175, 240, 0, 0, hInstance, 0);
	ShowWindow( hwnd, nShowCmd); 
	UpdateWindow( hwnd );

	while ( GetMessage( &msg, 0, 0, 0) ) 
	{							
		TranslateMessage(&msg);
		DispatchMessage( &msg);
	}

	return 0;
}


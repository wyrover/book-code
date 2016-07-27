#pragma comment(linker, "/subsystem:windows")

#include <windows.h>
#include "resource.h"


#define CLOSE 0
#define OPEN  1
#define FLAG  2
#define random(n) (rand()%n)
#define BLOCK_X			10		// x축 블록 갯수
#define BLOCK_Y			10		// y축 블록 갯수
#define BOMBCNT     	10		// 폭탄 수

int STBG;
HINSTANCE g_hInst;
char ttt[100];
HWND g_hwnd;

enum STATE { EMPTY =0, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, BOMB = 99};

typedef struct _BLOCK
{
	STATE state;
	char  isOpen;
} BLOCK;

// 지뢰 판 배열 사이즈 
BLOCK g_board[9][9];	

int remain_bomb;		// 종료 조건을 위해
int remain_flag;		// 왼쪽   DIGITAL 디스플레이를 위해
int spent_time;			// 오른쪽 DIGITAL 디스플레이를 위해
BLOCK block[BLOCK_X+2][BLOCK_Y+2];	
int dFLAG_X00, dFLAG_0X0, dFLAG_00X;		// 왼쪽   DIGITAL 각각 100의자리 , 10의자리, 1의 자리
int dTIME_00X =12, dTIME_0X0=12, dTIME_X00=12;		// 오른쪽 DIGITAL 각각 100의자리 , 10의자리, 1의 자리
HANDLE hTimer;

BOOL State_Flag = FALSE;

// 버튼 영역 생성 
void Draw3dRect(HDC hdc, int , int , int , int ,BOOL , int );

// Init() 지최및 영역 초기화
void Init();

// 지뢰판에 Bitmap 뿌려줌 
void DrawBitmap(HDC ,int , int ,int ,HBITMAP ,int );

// 지뢰판 버튼 영역 체크 
BOOL DrawField(int ,int );

// 지뢰판 마우스 업 실행시 STATE 체크후 OPEN 
void search(int x, int y);

void Timer_Bitmap(HDC);

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
//	PAINTSTRUCT ps;
//	HDC hdc;
	static POINTS pt;
	static int bx;
	static int by;
	static BOOL flag = FALSE;
	static BOOL LBflag = FALSE;
	static BOOL RBflag = FALSE;
	static BOOL StartFlag = FALSE;

	static int bomb_level=0;

	static HBITMAP MyBitMap,MyItem,MyTime;
	
//	RECT rc = { 0, 0, 168,211};
//	UINT style;

	switch( msg )
	{	
	case WM_CREATE :
		{
			StartFlag = TRUE;
			Init();

			MyBitMap = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP5));
			MyItem   = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP1));
			MyTime   = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP3));
	
			dTIME_00X =12, dTIME_0X0=12, dTIME_X00=12;	

//			CreateWindow(TEXT("button"), "", WS_CHILD | WS_VISIBLE |
//						 BS_PUSHBUTTON,68,14,30,28, hwnd,(HMENU)0,g_hInst,NULL);
		}
		return 0;

	case WM_MOUSEMOVE :
	{
			RECT Pre_Rect,Now_Rect;
			POINTS now_pt;
			int now_bx, now_by;
	
			now_pt = MAKEPOINTS(lParam);
			now_bx = (now_pt.x-12)/16;
			now_by = (now_pt.y-55)/16;

			Now_Rect.left   = now_bx*16+12;
			Now_Rect.right  = now_bx*16+12+16;
			Now_Rect.top	= now_by*16+55; 
			Now_Rect.bottom = now_by*16+55+16;
			
			Pre_Rect.left   = bx*16+12;
			Pre_Rect.right  = bx*16+12+16;
			Pre_Rect.top	= by*16+55; 
			Pre_Rect.bottom = by*16+55+16;	

			// 클리핑 영역의 x.y 좌표 버튼 영역  		
			if ( State_Flag == TRUE) return 0;

			// LBUTTONDOWN 이벤트인지 체크 
			if ( wParam & MK_LBUTTON != MK_LBUTTON) {
				wParam = wParam & ~MK_LBUTTON;
				return 0;
			}
			else LBflag = FALSE;		

			// 클리핑 영역의 x.y 좌표 버튼 영역  		
			if ( DrawField( pt.x, pt.y) == FALSE ) return 0;
	
			// 클리핑 영역의 x.y 좌표 비교   		
			if ( (Now_Rect.left == Pre_Rect.left ) &&
				 (Now_Rect.top  == Pre_Rect.top  )) return 0;

			if ( g_board[bx][by].isOpen == CLOSE) { // 폭탄 
				 g_board[bx][by].isOpen = CLOSE; 
			}
			
			// 다시그리기 위해 해당 블럭을 무효화 한다.
		InvalidateRect( hwnd,&Pre_Rect,TRUE);
		UpdateWindow( hwnd );

		pt = MAKEPOINTS(lParam);
		bx = (pt.x-12)/16;
		by = (pt.y-55)/16;	
		LBflag = TRUE;

	//	Sleep(300);
		InvalidateRect( hwnd,&Now_Rect,TRUE);
		UpdateWindow( hwnd );

	}	
	return 0;
	case WM_LBUTTONDOWN :
		{
			RECT Rect;
			flag = FALSE;
			LBflag = TRUE;
			RBflag = FALSE;

			if (StartFlag == TRUE) {
				spent_time =0;
				hTimer = (HANDLE)SetTimer(hwnd,1,1000,NULL);
			}

			StartFlag = FALSE;

			pt = MAKEPOINTS(lParam);
			bx = (pt.x-12)/16;
			by = (pt.y-55)/16;	
			
			Rect.left   = bx*16+12;
			Rect.right  = bx*16+12+16;
			Rect.top	= by*16+55; 
			Rect.bottom = by*16+55+16;

			// 클리핑 영역의 x.y 좌표 버튼 영역  		
			if ( State_Flag == TRUE && 
				 DrawField( pt.x, pt.y) == TRUE ) return 0;

			// 클리핑 영역의 x.y 좌표 버튼 영역  		
			if ( DrawField( pt.x, pt.y) == FALSE ||
				 g_board[bx][by].isOpen == OPEN) return 0;
			
			// 다시그리기 위해 해당 블럭을 무효화 한다.
		InvalidateRect( hwnd,&Rect,TRUE);
		}
		return 0;
	case WM_RBUTTONUP :
		{
			RECT Rect;
			RECT F_Rect;
			LBflag = FALSE;
			RBflag = TRUE;

			flag = TRUE;
			pt = MAKEPOINTS(lParam);
			bx = (pt.x-12)/16;
			by = (pt.y-55)/16;	

			Rect.left   = bx*16+12;
			Rect.right  = bx*16+12+16;
			Rect.top	= by*16+55; 
			Rect.bottom = by*16+55+16;
	
			F_Rect.left   = 15;
			F_Rect.right  = 54;
			F_Rect.top	  = 14; 
			F_Rect.bottom = 48;


			// 클리핑 영역의 x.y 좌표 버튼 영역  		
			if ( DrawField( pt.x, pt.y) == FALSE ) return 0;			
	
			// x,y 좌표로 부터 bx, by 를 구한다.
			if ( g_board[bx][by].isOpen == FLAG ) {
				++remain_bomb;
				remain_flag=remain_bomb;
				g_board[bx][by].isOpen = CLOSE;
				InvalidateRect( hwnd,&F_Rect,TRUE);
				UpdateWindow( hwnd);
			}
			else if ( g_board[bx][by].isOpen == CLOSE) { // 폭탄 
				 g_board[bx][by].isOpen = FLAG; 
				 --remain_bomb;
				 remain_flag=remain_bomb;
				InvalidateRect( hwnd,&F_Rect,TRUE);
				UpdateWindow( hwnd);
			}

			InvalidateRect( hwnd,&Rect,TRUE);
		}
		return 0;
	case WM_LBUTTONUP :	
		{
			RECT Rect;
			LBflag = FALSE;
			flag = TRUE;
			pt = MAKEPOINTS(lParam);
			bx = (pt.x-12)/16;
			by = (pt.y-55)/16;	

			Rect.left   = bx*16+12;
			Rect.right  = bx*16+12+16;
			Rect.top	= by*16+55; 
			Rect.bottom = by*16+55+16;
	
			wParam = wParam & ~MK_LBUTTON;	

			// 클리핑 영역의 x.y 좌표 버튼 영역  		
			if ( State_Flag == TRUE) return 0;

			// x,y 좌표로 부터 bx, by 를 구한다.
			if ( g_board[bx][by].isOpen == OPEN ||
				 g_board[bx][by].isOpen == FLAG ) return 0;

			// 클리핑 영역의 x.y 좌표 버튼 영역  		
			if ( DrawField( pt.x, pt.y) == FALSE ) return 0;
			
			if ( g_board[bx][by].state == (STATE)(BOMB)) { // 폭탄 
				 g_board[bx][by].isOpen = OPEN; 
 	 			KillTimer(hwnd,1);
				State_Flag = TRUE;

				for (int i=0; i< 9; i++) {
					for (int j=0;j<9;j++) {
						RECT t_rt;
						t_rt.left   = i*16+12;
						t_rt.right  = i*16+12+16;
						t_rt.top	= j*16+55; 
						t_rt.bottom = j*16+55+16;

						if (g_board[i][j].state == (STATE)(BOMB) &&
							g_board[i][j].isOpen == (STATE)(EMPTY)) {
							
							bomb_level = 1;
                            g_board[i][j].isOpen = OPEN; 
							InvalidateRect( hwnd, &t_rt, TRUE);
							UpdateWindow( hwnd);
						}
						else if (g_board[i][j].state == (STATE)(BOMB) &&
								 g_board[i][j].isOpen == (STATE)(FLAG)) {
							bomb_level = 2;
							InvalidateRect( hwnd, &t_rt, TRUE);
							UpdateWindow( hwnd);
						}
					}
				}
						    
			}
			
			bomb_level =0;

			if ( g_board[bx][by].state != (STATE)(BOMB) ||
				g_board[bx][by].isOpen != FLAG ) { // 주변을 모두 열어준다.
				g_board[bx][by].isOpen = OPEN; 
				search(bx,by);
			}
			else if ( g_board[bx][by].isOpen == FLAG ) {
				g_board[bx][by].isOpen = CLOSE;
			}

			
			if(remain_flag < 0)					// 사용할 수 있는 깃발 수가 음수라면..
			{
				dFLAG_00X = (remain_flag*-1)%10;
				dFLAG_0X0 = int((remain_flag*-1)/10)%10;
				dFLAG_X00 = 15;
			}
			else								// 사용할 수 있는 깃발 수가 0 또는 양수라면
			{
				dFLAG_00X = remain_flag%10;
				dFLAG_0X0 = int(remain_flag/10)%10;
				dFLAG_X00 = int(remain_flag/100)%10;
			}

			{
				RECT rc = { 15,  14, 57,  40 };
				InvalidateRect( hwnd, &Rect, TRUE);
			}


			if(remain_bomb == 0 && remain_flag == 0)
			{
				KillTimer(hwnd,1);
				MessageBox( hwnd, "성공", "", MB_OK );
				State_Flag = TRUE;
			}


			InvalidateRect( hwnd,&Rect,TRUE);

		}
		return 0;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			short tbx;
		    short tby;
			int Hbomb,Tbomb,Obomb;

			hdc = BeginPaint( hwnd, &ps );

			Draw3dRect( hdc, 15,  14, 57,  40,  TRUE,  1 );


	//		Timer_Bitmap(hdc);
		
			Draw3dRect( hdc, 0,   0,  168, 211, FALSE, 3 );
			Draw3dRect( hdc, 9,   9,  159, 46,  TRUE,  2 );
			Draw3dRect( hdc, 15,  14, 57,  40,  TRUE,  1 );
			Draw3dRect( hdc, 109, 14, 150, 40,  TRUE,  1 );
			Draw3dRect( hdc, 9,   52, 159, 202, TRUE,  3 );

			//Draw3dRect( hdc, 65,  14, 100,  40,  FALSE,  2 );
			Draw3dRect( hdc, 68,  13, 98,  40,  FALSE,  2 );
			
			Hbomb = 12-(remain_bomb/100)%10;
			Tbomb = 12-(remain_bomb/10)%10;
			Obomb = 12-(remain_bomb%10);

			// 지뢰 갯수  
			/*******************************/
			DrawBitmap(hdc,15,14,3,MyTime,Hbomb);	// 100 
			DrawBitmap(hdc,28,14,3,MyTime,Tbomb);	// 10
			DrawBitmap(hdc,41,14,3,MyTime,Obomb);	// 1
			
			// 시간 
			/*******************************/
			DrawBitmap(hdc,109,14,3,MyTime,dTIME_00X);	// 100 
			DrawBitmap(hdc,122,14,3,MyTime,dTIME_0X0);	// 10
			DrawBitmap(hdc,135,14,3,MyTime,dTIME_X00);	// 1

			if (LBflag == TRUE)
				DrawBitmap(hdc,70,15,5,MyBitMap,3);
			else DrawBitmap(hdc,70,15,5,MyBitMap,1);

			for (int x = 12; x <= 140; x += 16 ) {
				for (int y = 55; y <= 183 ; y += 16) {
					tbx = (x-12)/16;
					tby = (y-55)/16;

					if (g_board[tbx][tby].isOpen == CLOSE  ) {
						if (LBflag == TRUE)
							Draw3dRect( hdc, x, y, x + 15, y + 15, TRUE, 2 ); 	
						else 
					 		Draw3dRect( hdc, x, y, x + 15, y + 15, FALSE, 2 );
					}
					else if (g_board[tbx][tby].isOpen == OPEN ) {

						switch((STATE)(g_board[tbx][tby].state)) {
						case  BOMB :	// 폭탄일시 
							Draw3dRect( hdc, x, y, x + 15, y + 15, TRUE, 2 );
							if (flag == TRUE)
								if (bomb_level == 0)
									DrawBitmap(hdc,x,y,1,MyItem,4);
								else 
									DrawBitmap(hdc,x,y,1,MyItem,5);
							break;
						default :
							Draw3dRect( hdc, x, y, x + 15, y + 15, TRUE, 2 );
							DrawBitmap(hdc,x,y,1,MyItem,16-g_board[tbx][tby].state);
						}
					}
					else if ( g_board[tbx][tby].isOpen == FLAG) {
						DrawBitmap(hdc,x,y,1,MyItem,2 );
						if (g_board[tbx][tby].state == (STATE)(BOMB) &&
							bomb_level == 2)
							DrawBitmap(hdc,x,y,1,MyItem,6);

					}
				}
			}

			EndPaint( hwnd, &ps );
		}
		return 0;
	case WM_TIMER:
		{
			RECT Rect;
			
			Rect.left   = 109;//109;
			Rect.right  = 148;//148;//148
			Rect.top	= 14; 
			Rect.bottom = 48;

			spent_time++;

			dTIME_00X = 12-(spent_time/100)%10;
			dTIME_0X0 = 12-(spent_time/10)%10;
			dTIME_X00 = 12-(spent_time%10);
			
			InvalidateRect( hwnd, &Rect, TRUE);

		}
		return 0;

	case WM_DESTROY:
		KillTimer(hwnd,1);
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
	g_hInst = hInstance;

	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hbrBackground= (HBRUSH)GetStockObject( WHITE_BRUSH ); 
	wc.hCursor		= LoadCursor( 0, IDC_ARROW );
	wc.hIcon		= LoadIcon( 0, IDI_APPLICATION);
	wc.hInstance	= hInstance;
	wc.lpfnWndProc  = WndProc; 
	wc.lpszClassName= "First";
	wc.lpszMenuName = 0;
	wc.style		= 0;

	atom = RegisterClass( &wc);
	
	if ( atom == 0 ) 
	{
		MessageBox( 0, "Fail To RegisterClass", "Error", MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx( 0, "first", "Hello", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME 
								& ~WS_MAXIMIZEBOX ,
								0, 0, 175,240, 0, 0, 
								hInstance, 0); 
	ShowWindow( hwnd, nShowCmd); 
	UpdateWindow( hwnd );
    g_hwnd	= hwnd;
	while ( GetMessage( &msg, 0, 0, 0) ) 
	{							
		TranslateMessage(&msg);
		DispatchMessage( &msg);
	}

	return 0;
}

// 버튼 영역 생성 
void Draw3dRect(HDC hdc, int x, int y, int xx, int yy, 
					BOOL down, int width )
{
	COLORREF	clrMain  = RGB(192,192,192),
				clrLight = RGB(255,255,255), 
				clrDark  = RGB(128,128,128);

	HPEN hPen1, hPen2, hOldPen;
	HBRUSH hBrush, hOldBrush;     

	if(down)    
	{
		hPen2 = CreatePen(PS_SOLID,1,clrLight);
		hPen1 = CreatePen(PS_SOLID,1,clrDark);
	} 
	else 
	{
		hPen1 = CreatePen(PS_SOLID,1,clrLight);
		hPen2 = CreatePen(PS_SOLID,1,clrDark);
	}  
	hBrush =  CreateSolidBrush( clrMain );
	hOldPen = (HPEN)SelectObject(hdc, hPen1);
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);  

	Rectangle(hdc, x , y, xx+1, yy+1);  

	for(int i=0; i < width; i++)
	{
		SelectObject(hdc, hPen1);

		MoveToEx(hdc, xx - 1, y, 0 );
		LineTo(hdc, x, y);
		LineTo(hdc, x, yy - 1 );  

		SelectObject(hdc, hPen2);

		MoveToEx(hdc, x, yy,0);
		LineTo(hdc, xx, yy);
		LineTo(hdc, xx, y);  

		x++; y++; xx--; yy--;
	} 
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);

	DeleteObject(hPen1); 
	DeleteObject(hPen2); 
	DeleteObject(hBrush);
} 

// Init() 지최및 영역 초기화 
void Init()
{
	int cnt =0;

	for ( int i = 0; i < 9; i++ ) 
		for ( int j = 0; j < 9; j++ ) 
			g_board[i][j].isOpen = CLOSE;

	// 폭탄 랜덤하게 배치하고... state 항목을 초기화 한다.
		
	for(i=0;i<10;)
	{
	srand(GetTickCount());
		int x = rand()%9;
		int y = rand()%9;
		if(g_board[x][y].state!=BOMB)
		{
			g_board[x][y].state=BOMB;
			
			for(int k=-1;k<2;)              //폭탄 주변에 숫자 초기화..
			{
				if((x+k)==-1)
				{
					++k;
					continue;
				}
				
				for(int j=-1;j<2;)
				{
					if (k==0 && j==0)
					{++j;} // i=0, j=0 즉 폭탄이 설치된 자리는 건너뛰고..
					else
					{
						if((y+j)==-1)
						{++j;}
						else
						{
							if((y+j)==9)
							{++j;}
							else
							{
								switch(g_board[x+k][y+j].state)
								{
								case EMPTY:
									g_board[x+k][y+j].state=ONE;break;
								case ONE:
									g_board[x+k][y+j].state=TWO;break;
								case TWO:
									g_board[x+k][y+j].state=THREE;break;
								case THREE:
									g_board[x+k][y+j].state=FOUR;break;
								case FOUR:
									g_board[x+k][y+j].state=FIVE;break;
								case FIVE:
									g_board[x+k][y+j].state=SIX;break;
								case SEVEN:
									g_board[x+k][y+j].state=EIGHT;break;
								}
								++j;
							}
						}
					}
				}
				++k;
				if((x+k)==9)
				{break;}
			}
			++i;
		}
	}  
	
	remain_flag = BOMBCNT;						// remain_flag는 마우스 우클릭으로 플레그 찍으면 무조건 1감소하고
	remain_bomb = BOMBCNT;						// remain_bomb는 플레그찍힌 곳에 폭탄이 있으면 1감소한다.


   	dFLAG_00X = remain_flag%10;
	dFLAG_0X0 = int(remain_flag/10)%10;
	dFLAG_X00 = int(remain_flag/100)%10;

	spent_time = 0;										// 오른쪽 디지털 디스플레이 : 게임시간을 표시한다.

	dTIME_00X = spent_time%10;
	dTIME_0X0 = int(spent_time/10)%10;
	dTIME_X00 = int(spent_time/100)%10; 
}


void DrawBitmap(HDC hdc,int x, int y,int selection,HBITMAP hBit,int Num)
{
	HDC MemDC;
	HBITMAP OldBitMap;
	int bx,by,ysize,Bsize;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitMap = (HBITMAP)SelectObject(MemDC,hBit);

	GetObject(hBit,sizeof(BITMAP),&bit);

	bx = bit.bmWidth;//70;//bit.bmWidth;
	ysize = bit.bmHeight;

	if (selection == 5) {
		Bsize= 24;
		ysize = ysize/Bsize;
		by = Bsize*Num;
	}
	else if (selection == 1)
	{
		Bsize= 16;
		ysize = ysize/Bsize;
		by = Bsize*Num;
	}
	else if (selection == 3)
	{
		Bsize= 23;
		ysize = ysize/Bsize;
		by = Bsize*Num;
	}
	
	BitBlt(hdc,x,y,bx,Bsize,MemDC,0,by-Bsize,SRCCOPY);

	SelectObject(MemDC,OldBitMap);
	DeleteDC(MemDC);

}

// 지뢰판 버튼 영역 체크 
BOOL DrawField(int x,int y)
{
	if (( x< 12 || x > 157 )|| 
		( y < 55 || y > 190 ) ) 
		return FALSE;
	else
		return TRUE;
}

void search(int x, int y)
{
	int i,j;
	RECT Rect;

	for(i=-1;i<2;)
	{
		if(((x+i)==-1)||((x+i)==9))
		{++i;continue;}
		
		for(j=-1;j<2;)
		{
			if ((i==0 && j==0) || ((y+j)==-1) || ((y+j)==9))
			{++j;} 
			else
			{
				if(g_board[x+i][y+j].isOpen==CLOSE)
				{
					Rect.left   = (x+i)*16+12;
					Rect.right  = (x+i)*16+12+16;
					Rect.top	= (y+j)*16+55; 
					Rect.bottom = (y+j)*16+55+16;

					if( (g_board[x+i][y+j].state==EMPTY) )
					{

						g_board[x+i][y+j].isOpen=OPEN;

						InvalidateRect( g_hwnd,&Rect,TRUE);
						UpdateWindow( g_hwnd );
						search(x+i,y+j);
						++j;
					}
					else if ( (g_board[x+i][y+j].state!=BOMB) &&
						      (g_board[x+i][y+j].isOpen!=FLAG) )
					{
						g_board[x+i][y+j].isOpen=OPEN;
						InvalidateRect( g_hwnd,&Rect,TRUE);
						UpdateWindow( g_hwnd );
						++j;
					}
					else
						++j;
				}
				else
				{		
				++j;
				}
			}
		}
		++i;
	
	}
	return ;
}


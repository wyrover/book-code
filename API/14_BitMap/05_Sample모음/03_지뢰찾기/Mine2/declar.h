

#define INTERVAL 16			//
typedef struct _POS{
		int x;	int y;	int xx;	int yy;
}POS;

typedef struct _FRAME_POS{
	POS first;				// 처음 프레임의 왼쪽 상위 좌표, 오른쪽 하위좌표
	POS second;				// 두번째 프레임의            "
	POS third;				// 세번째 프레임의            "
	POS four;				// 네번째 프레임의            "
	POS five;				// 다섯번째 프레임의          "
	POS smile;				// 웃는 버튼의 위치 - 버튼 자체 사이즈 26,26 
	POS miniPos;			// 작은 버튼의 시작 x,y 부터 끝 x,y까지
}FRAME_POS;

FRAME_POS Client;


void Init_Position(int width,int height){
	//===== 초급 중급 고급 과정 만들때 사용할 좌표값들
	
	Client.first.x = 0;		Client.first.y = 0;
	//------------------유동값--------------------------
	// 나중에 사용자 모드나 모드별 구현시 이부분 가로세로 갯수별 
	// 사이즈 조절만해주면 나머지는 자동으로 설정된다.

	Client.first.xx = (INTERVAL * width) + 24;	
	Client.first.yy = (INTERVAL * height) + 67;		// 55은 위에 높이 28 아래
	//--------------------------------------------------

	//--------------------------------------------------
	Client.second.x =9;		Client.second.y =9;
	Client.second.xx = Client.first.xx-9;	
	Client.second.yy =46;
	//--------------------------------------------------

	//--------------------------------------------------
	Client.third.x =15;		Client.third.y =14;
	Client.third.xx =57;		Client.third.yy =40;
	//--------------------------------------------------
	
	//--------------------------------------------------
	Client.four.x = Client.second.xx -50;
	Client.four.y = 14;
	Client.four.xx = Client.second.xx -9;
	Client.four.yy = 40;
	//--------------------------------------------------

	//--------------------------------------------------
	Client.five.x = 9;		Client.five.y = 52;
	Client.five.xx = Client.first.xx -9;
	Client.five.yy = Client.first.yy -9;
	//--------------------------------------------------

	//--------------------------------------------------
	Client.miniPos.x = 12;	Client.miniPos.y = 55;
	Client.miniPos.xx = Client.first.xx -28;
	Client.miniPos.yy = Client.first.yy -28;
	//--------------------------------------------------
	
	//------------------스마일 좌표----------------------
	Client.smile.x = (Client.second.x + Client.second.xx)/2 -13;
	Client.smile.y = 14;
	Client.smile.xx = Client.smile.x + 26;
	Client.smile.yy = 40;
	//--------------------------------------------------
}


// 지뢰찾기 처음에 지뢰를 매설할때 이 플래그가 FALSE인 상태에서 MOUSEUP 할경우
// FLAG를 TRUE로 바꾸고 지뢰를 마우스 클릭 좌표가 아닌 나머지 공간에 랜덤하게
// 매설한다.
static BOOL mineSettingFLAG = FALSE;

// 버튼을 푸시 상태에서 마우스 업을 하지 않으면 그림 바뀌어 보이는..
// LBUTTONUP할때 FALSE로 되고
// LBUTTONDOWN할때 TRUE가 되고
// MOUSEMOVE할때 TRUE이면 움직인 좌표에 눌린 그림을 보여줌
// 속성이 CLOSE 되어있는것만 해준다. 이미열려있는건 건들지 않는다
static BOOL ButtonPushMoveFLAG = FALSE;	

// WinMain 에서 hInstance값 엔트리 포인트...
HINSTANCE g_hInst;	


// 초기 값들..초보자의 경우 9,9
static int Xp;
static int Yp;

static int INITMINE;						//	전체 지뢰갯수를 설정할값 초기값 10개

#define GetClntX(K) ( (12+((K)*16)) )
#define GetClntY(P) ( (55+((P)*16)) )

#define ArrayX(x) =  ((int)((x-12) / 16))		//	ClntPos.x
#define ArrayY(y) =  ((int)((y-55) / 16))		//  ClntPos.y

//#define G_BOARD(i,j)	(*(*(g_board+i)+j))
#define G_BOARD(i,j) (*(g_board+(((i)*Xp)+(j))))






void Draw3dRect(HDC hdc, int x, int y, int xx, int yy, BOOL down, int width )
{
	COLORREF clrMain  = RGB(192,192,192),
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





//enum STATE { EMPTY = 0, ONE , TWO , THREE , FOUR , FIVE ,SIX ,SEVEN , EIGHT, BOMB=99};
#define EMPTY	0
#define ONE		1
#define TWO		2
#define THREE	3
#define FOUR	4
#define FIVE	5
#define SIX		6
#define SEVEN	7
#define EIGHT	8
#define BOMB	9

#define CLICKBOMB 10
#define CROSSBOMB 11
#define GENERAL   12


	//---------------1,   2   3   4   5   6   7  8  9  0
//	int timePos[10]={253,230,207,184,161,138,115,92,69,46};
	
	
	//EMPTY,1,2,3,4,5,6,7,8, 일반폭탄,클릭폭탄,X폭탄,CLOSE=12,FLAG=13,QUESTION=14
//	int picPos[16]={240,224,208,192,176,160,144,128,112,80,48,64,0,16,32};


#define CLOSE 0				// 닫힌그림
#define OPEN 1				// 열린그림
#define FLAG 13				// 깃발꼽는그림
#define QUESTION 14			// 물음표 그림




///// 스마일 정의					
// 속성을.. 클릭된 스마일,	 일반 스마일, 썬글라스 스마일 , 놀란 스마일, 찌프린 스마일
//			NORMAL_ON_SMILE NORMAL_SMILE, GLASS_SMILE		SUPRISE_SMILE	DISAPEAR_SMILE
//	int smilePos[5] = {0,24,48,72,96};				// x-24,y-24
#define NORMAL_ON_SMILE	0		//	일반 스마일        - 스마일 버튼 클릭시 -
#define GLASS_SMILE		1		//  선글라쓰 쓴 스마일 - 성공적으로 종료했을경우
#define DISAPEAR_SMILE	2		//	찌프린 스마일	   - LBUTTON_UP시. 폭탄일경우.
#define SUPRSISE_SMILE	3		//  놀란 스마일		   - 마우스 - LBUTTON_DOWN 시 

#define NORMAL_SMILE	4		//	일반 스마일		   - LBUTTON_UP시. 폭탄이 아닐경우







typedef struct _BLOCK{
	int state;
	char isOpen;			// OPEN FLAG QUESTION
	
}BLOCK;

//BLOCK g_board[Xp][Yp];
// 동적 할당..
BLOCK * g_board;


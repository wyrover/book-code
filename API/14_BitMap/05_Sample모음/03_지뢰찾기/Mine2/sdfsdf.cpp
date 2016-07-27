#include <stdio.h>							//	printf 문 되게 하는법.. 
#include <windows.h>
#include <time.h>
#include "resource.h"
#include "declar.h"




//------------------------------------------------------------------------------//
//								Init 초기화 함수
//------------------------------------------------------------------------------//
//	매겨변수 : Iwidth  = 가로사이즈를 결정한다
//			   iHeight = 세로 사이즈를 결정한다
//------------------------------------------------------------------------------//
//	기능 :-------------------------------------------------------------------
//------------------------------------------------------------------------------//
// 지뢰찾기의 가로 세로 사이즈를 결정하고 
// 그에따라 사용될 메모리를 동적 할당해주고
// 지뢰찾기 배경에 대한 모든 좌표치를 설정해주고
// 속성을 CLOSE와 EMPTY로 초기화한다
//------------------------------------------------------------------------------//
void Init(int Iwidth , int Iheight , int Imind)
{
	
	int i,j;
	Xp = Iwidth;
	Yp = Iheight;
	INITMINE = Imind;
	g_board = (BLOCK*)malloc(sizeof(BLOCK)*Xp*Yp);


	// 구조체에 대한초기화
	Init_Position(Iwidth,Iheight);			

		
		for(j = 0 ; j <Iheight ; ++j)
			for(i = 0; i < Iwidth ; ++i){			// X축 = I = Iwidth
						
			//g_board[i][j].isOpen = CLOSE;
		
			//(*(g_board+((i*Xp)+j))).isOpen = CLOSE;
			G_BOARD(i,j).isOpen = CLOSE;
			G_BOARD(i,j).state = EMPTY;
			
		
		}

}


//------------------------------------------------------------------------------//
//								Binding 바인딩 함수
//------------------------------------------------------------------------------//
//	매겨변수 : Bmind   = 심을 지뢰의 갯수를 결정해줄 변수
//			   Bwidth  = 가로 갯수	- 랜드함수를 사용하여 지뢰심을 위치를 결정한다
//			   Bheight = 세로 갯수	- 랜드함수를 사용하여 지뢰심을 위치를 결정한다
//			   first   = 처음 클릭시 x좌표(상대적인 좌표이다-배열의 인덱스에 해당)
//			   second  = 처음 클릭시 y좌표(x,y에 해당하는 좌표에 지뢰를 심지않게 한다)
//
//------------------------------------------------------------------------------//
//	기능 :		폭탄 바인딩 & 카운팅
//------------------------------------------------------------------------------//
//	1. 폭탄을 바인딩
// srand를 사용하여 배열의 가로 새로 위치를 랜덤하게 잡은뒤 폭탄을 넣는다.
//			-첫번째 클릭좌표에 심으면 안되고
//			-두번째	Bmind개수 만큼만 심는다
//						- 중복시 심지 않고 다른 좌표를 찾는다
//	2. 폭탄 COUNT
//	배열의 i,j의 상태가 폭탄이 아니라면 그배열을 기준으로 8방을 다 조사한다
//	8방향을 조사할때 i ,j의 범위를 초가하지 않게 결정한다
//	---다시 짤일이 생긴다면 꼭 벽을 주고 할것.-_-....
//	
//------------------------------------------------------------------------------//


void Binding(int Bmind , int Bwidth , int Bheight,	// Bmind는 심을 지뢰 갯수 Bwidth 는 x갯수 Bheight는 y갯수
			 int first,int second)					// first는 처음클릭시 x좌표 second는 처음 y좌표
{
	
	
	int Bcount = 0;
	srand(time(NULL));

	//rand() % mind;
	int Bi;
	int Bj;

	int i,j;

	
	// 지뢰심기...
	while(Bcount < Bmind){// 지뢰 갯수만큼 지뢰를 심었을때. 끝내라
		Bi = rand()% Bwidth;
		Bj = rand()% Bheight;

		// 마우스 처음클릭시 좌표가 아니고 폭탄도 아니라면
		//if ( (G_BOARD(Bi,Bj).state != BOMB) && (Bi !=first) && (Bj !=second)){
		//	 G_BOARD(Bi,Bj).state = BOMB ;					// 지뢰를 심어라
		//	Bcount++;										// 지뢰갯수를 증가시켜라
		//}
		if( (  G_BOARD(Bi,Bj).state != BOMB) && (Bi !=first) && (Bj !=second)){
			G_BOARD(Bi,Bj).state = BOMB;
			Bcount++;
			
		}
	}

	printf("\n\n\n----------------------------------------------\n");
	printf("------------------초기화모드 -----------------\n");
	printf("------------------심은 지뢰-----------------\n");
	printf("----------------------------------------------\n");
	
		
	for(j = 0 ; j < Bheight ; ++j){							// X축 = i = Bwidth
		for ( i = 0 ; i < Bwidth ; ++i){
			printf("%2d ",G_BOARD(i,j).state);
		}
		printf("\n");
	}



	//Bcount = 0;
	// 지뢰 카운트하는 부분...................
	
	for(j = 0 ; j < Bheight ; ++j){							// X축 = i = Bwidth
		for ( i = 0 ; i < Bwidth ; ++i){
			Bcount = 0;			
			
			// i,j좌표의 상태가 폭탄이 아니라면 state값을 하나 증가시킨다...
			if(G_BOARD(i,j).state != BOMB)   {

				if(i-1 >=0)						if (G_BOARD(i-1,j).state == BOMB) 		Bcount++;
				if((i-1 >=0) &&(j-1 >=0))		if (G_BOARD(i-1,j-1).state==BOMB)		Bcount++;
				if(i+1 < Bwidth)				if(G_BOARD(i+1,j).state==BOMB)			Bcount++;
				if((i+1<Bwidth&&(j+1)<Bheight))	if (G_BOARD(i+1,j+1).state==BOMB)		Bcount++;
				if(j-1 >=0)						if(G_BOARD(i,j-1).state==BOMB)			Bcount++;
				if((i+1<Bwidth)&&(j-1>=0))		if (G_BOARD(i+1,j-1).state==BOMB)		Bcount++;
				if(j+1 <Bheight)				if(G_BOARD(i,j+1).state==BOMB)			Bcount++;
				if((i-1 >=0)&&(j+1<Bheight))	if (G_BOARD(i-1,j+1).state==BOMB)		Bcount++;
				
			
				switch(Bcount){
				//printf("Bcount is %d \n\n" , Bcount);
				case 0 : 				G_BOARD(i,j).state = EMPTY;				break;
				case 1 : 				G_BOARD(i,j).state = ONE;				break;
				case 2 : 				G_BOARD(i,j).state = TWO;				break;
				case 3 : 				G_BOARD(i,j).state = THREE;				break;
				case 4 : 				G_BOARD(i,j).state = FOUR;				break;
				case 5 : 				G_BOARD(i,j).state = FIVE;				break;
				case 6 : 				G_BOARD(i,j).state = SIX;				break;
				case 7 : 				G_BOARD(i,j).state = SEVEN;				break;
				case 8 : 				G_BOARD(i,j).state = EIGHT;				break;					
				}


			}	// 폭탄비교 if문끝

		}		// 첫번째 for문

			// 두번째 for문
	}

	
//--------------------------------Test Mode--------------------------------
	printf("\n\n\n----------------------------------------------\n");
	printf("--------------초기화모드---------------------\n");
	printf("-------------------STATE-------------------\n");
	printf("----------------------------------------------\n");
	for( j = 0 ; j < Bheight ; ++j){						// x축  i = Bwidth
		for (  i = 0 ; i < Bwidth ; ++i){
		
			printf("%2d ",G_BOARD(i,j).state);
		}
		printf("\n");

	}
//--------------------------------Test Mode--------------------------------
		
}		// 프로그램 종료











//------------------------------------------------------------------------------//
//									EmptySearching 공백나오면 주루루룩
//------------------------------------------------------------------------------//
//
//	특징....-_- 진짜 무식하게 만든 탐색 재귀 알고리즘
//				: 일단 이안에들어오는 좌표는 검증된 범위 안의 좌표이다
//				: 그러나 경계에서 탐색을 물고늘어질때 어떤 상황일때 오픈해야하는데 경계에는 잘못된
//				: 값이 들어갈수도 있다. 그래서 else구문을 못쓰고 모든상황에 맞는 경우를 직접 선택하여
//				: 여는 모양을 만든다
//
//	다음에 싹다시고치게 된다면...벽을 꼭 만들고 8방향을 검색하기 위한 방향에 대한 배열을 만들어서 
//	그배열 사이즈는 8개로잡고 모든 방향을 결정할수 있게 하여 그 배열을 탐색할것의 인덱스로 사용하게 한다
//		G_BOARD(방향[count],방향[count])		이런식으로 count를 8개 다돌려서 모든 방향을 다검색해주면 
//		될것이다...아아아-_- 진짜 노가다 플그램...속도면에서는 다풀어서 짯으니 이게 빠를지도....-_-;;
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

void EmptySearching(int ArrX, int ArrY)		// 빈곳을 찾으면 숫자가 나올때까지 OPEN시킨다.
{

	
			//ArrX-1 , ArrY-1 방향에 대한 처리
			// 1번 if	상태가 EMPTY이면서 닫혀있으면 OPEN으로 해서 열게 보이게 하고
			//			또 자기자신 함수를 호출해서 그방향을 또찾는다...
			// 2번 if   만일 그좌표의 상태가 FLAG이거나 QUESTION 이면 아무처리도 안하고
			// 3번 if	1,2,3,4,5,6,7,8 일경우에는 열어준다
			// 이렇게 바보 노가다를 하는이유는 경계를 넘어선 값이 무엇일지몰라 else구문으로 처리하지못한다
			// 벽만 있어도...-_-
			// 아래 구문들도 방향에 대한 처리만 틀리고 똑같다

			if( (G_BOARD(ArrX-1,ArrY-1).state == EMPTY) && (G_BOARD(ArrX-1,ArrY-1).isOpen == CLOSE) )
			{
				G_BOARD(ArrX-1,ArrY-1).isOpen = OPEN;
				EmptySearching(ArrX-1,ArrY-1);
			}

			else if ((G_BOARD(ArrX-1,ArrY-1).state	== FLAG)	|| (G_BOARD(ArrX-1,ArrY-1).state == QUESTION) );
			else if ((G_BOARD(ArrX-1,ArrY-1).state	== ONE)		|| (G_BOARD(ArrX-1,ArrY-1).state == TWO)	 ||
					(G_BOARD(ArrX-1,ArrY-1).state	== THREE)	|| (G_BOARD(ArrX-1,ArrY-1).state == FOUR)	 ||
					(G_BOARD(ArrX-1,ArrY-1).state	== FIVE)	|| (G_BOARD(ArrX-1,ArrY-1).state == SIX)	 ||
					(G_BOARD(ArrX-1,ArrY-1).state   == SEVEN)	|| (G_BOARD(ArrX-1,ArrY-1).state == EIGHT))
			{
				G_BOARD(ArrX-1,ArrY-1).isOpen = OPEN;
			}



			//x, y-1

			if( (G_BOARD(ArrX,ArrY-1).state == EMPTY) && (G_BOARD(ArrX,ArrY-1).isOpen == CLOSE) )
			{
				G_BOARD(ArrX,ArrY-1).isOpen = OPEN;
				EmptySearching(ArrX,ArrY-1);
			}

			else if ((G_BOARD(ArrX,ArrY-1).state	== FLAG)	|| (G_BOARD(ArrX,ArrY-1).state == QUESTION) );
			else if ((G_BOARD(ArrX,ArrY-1).state	== ONE)		|| (G_BOARD(ArrX,ArrY-1).state == TWO)	 ||
					(G_BOARD(ArrX,ArrY-1).state	== THREE)	|| (G_BOARD(ArrX,ArrY-1).state == FOUR)	 ||
					(G_BOARD(ArrX,ArrY-1).state	== FIVE)	|| (G_BOARD(ArrX,ArrY-1).state == SIX)	 ||
					(G_BOARD(ArrX,ArrY-1).state   == SEVEN)	|| (G_BOARD(ArrX,ArrY-1).state == EIGHT))
			{
				G_BOARD(ArrX,ArrY-1).isOpen = OPEN;
			}


		//x+1 y-1
			if( (G_BOARD(ArrX+1,ArrY-1).state == EMPTY) && (G_BOARD(ArrX+1,ArrY-1).isOpen == CLOSE) )
			{
				G_BOARD(ArrX+1,ArrY-1).isOpen = OPEN;
				EmptySearching(ArrX+1,ArrY-1);
			}

			else if ((G_BOARD(ArrX+1,ArrY-1).state	== FLAG)	|| (G_BOARD(ArrX+1,ArrY-1).state == QUESTION) );
			else if ((G_BOARD(ArrX+1,ArrY-1).state	== ONE)		|| (G_BOARD(ArrX+1,ArrY-1).state == TWO)	 ||
					(G_BOARD(ArrX+1,ArrY-1).state	== THREE)	|| (G_BOARD(ArrX+1,ArrY-1).state == FOUR)	 ||
					(G_BOARD(ArrX+1,ArrY-1).state	== FIVE)	|| (G_BOARD(ArrX+1,ArrY-1).state == SIX)	 ||
					(G_BOARD(ArrX+1,ArrY-1).state   == SEVEN)	|| (G_BOARD(ArrX+1,ArrY-1).state == EIGHT))
			{
				G_BOARD(ArrX+1,ArrY-1).isOpen = OPEN;
			}

		//x+1 , y
			if( (G_BOARD(ArrX+1,ArrY).state == EMPTY) && (G_BOARD(ArrX+1,ArrY).isOpen == CLOSE) )
			{
				G_BOARD(ArrX+1,ArrY).isOpen = OPEN;
				EmptySearching(ArrX+1,ArrY);
			}

			else if ((G_BOARD(ArrX+1,ArrY).state	== FLAG)	|| (G_BOARD(ArrX+1,ArrY).state == QUESTION) );
			else if ((G_BOARD(ArrX+1,ArrY).state	== ONE)		|| (G_BOARD(ArrX+1,ArrY).state == TWO)	 ||
					(G_BOARD(ArrX+1,ArrY).state	== THREE)	|| (G_BOARD(ArrX+1,ArrY).state == FOUR)	 ||
					(G_BOARD(ArrX+1,ArrY).state	== FIVE)	|| (G_BOARD(ArrX+1,ArrY).state == SIX)	 ||
					(G_BOARD(ArrX+1,ArrY).state   == SEVEN)	|| (G_BOARD(ArrX+1,ArrY).state == EIGHT))
			{
				G_BOARD(ArrX+1,ArrY).isOpen = OPEN;	
			}

		//x+1 , y+1
			if( (G_BOARD(ArrX+1,ArrY+1).state == EMPTY) && (G_BOARD(ArrX+1,ArrY+1).isOpen == CLOSE) )
			{
				G_BOARD(ArrX+1,ArrY+1).isOpen = OPEN;
				EmptySearching(ArrX+1,ArrY+1);
			}

			else if ((G_BOARD(ArrX+1,ArrY+1).state	== FLAG)	|| (G_BOARD(ArrX+1,ArrY+1).state == QUESTION) );
			else if ((G_BOARD(ArrX+1,ArrY+1).state	== ONE)		|| (G_BOARD(ArrX+1,ArrY+1).state == TWO)	 ||
					(G_BOARD(ArrX+1,ArrY+1).state	== THREE)	|| (G_BOARD(ArrX+1,ArrY+1).state == FOUR)	 ||
					(G_BOARD(ArrX+1,ArrY+1).state	== FIVE)	|| (G_BOARD(ArrX+1,ArrY+1).state == SIX)	 ||
					(G_BOARD(ArrX+1,ArrY+1).state   == SEVEN)	|| (G_BOARD(ArrX+1,ArrY+1).state == EIGHT))
			{
				G_BOARD(ArrX+1,ArrY+1).isOpen = OPEN;
			}

		//x , y-1
			if( (G_BOARD(ArrX,ArrY-1).state == EMPTY) && (G_BOARD(ArrX,ArrY-1).isOpen == CLOSE) )
			{
				G_BOARD(ArrX,ArrY-1).isOpen = OPEN;
				EmptySearching(ArrX,ArrY-1);
			}

			else if ((G_BOARD(ArrX,ArrY-1).state	== FLAG)	|| (G_BOARD(ArrX,ArrY-1).state == QUESTION) );
			else if ((G_BOARD(ArrX,ArrY-1).state	== ONE)		|| (G_BOARD(ArrX,ArrY-1).state == TWO)	 ||
					(G_BOARD(ArrX,ArrY-1).state	== THREE)	|| (G_BOARD(ArrX,ArrY-1).state == FOUR)	 ||
					(G_BOARD(ArrX,ArrY-1).state	== FIVE)	|| (G_BOARD(ArrX,ArrY-1).state == SIX)	 ||
					(G_BOARD(ArrX,ArrY-1).state   == SEVEN)	|| (G_BOARD(ArrX,ArrY-1).state == EIGHT))
			{
				G_BOARD(ArrX,ArrY-1).isOpen = OPEN;		
			}

		// x-1 ,y+1
			if( (G_BOARD(ArrX-1,ArrY+1).state == EMPTY) && (G_BOARD(ArrX-1,ArrY+1).isOpen == CLOSE) )
			{
				G_BOARD(ArrX-1,ArrY+1).isOpen = OPEN;
				EmptySearching(ArrX-1,ArrY+1);
			}

			else if ((G_BOARD(ArrX-1,ArrY+1).state	== FLAG)	|| (G_BOARD(ArrX-1,ArrY+1).state == QUESTION) );
			else if ((G_BOARD(ArrX-1,ArrY+1).state	== ONE)		|| (G_BOARD(ArrX-1,ArrY+1).state == TWO)	 ||
					(G_BOARD(ArrX-1,ArrY+1).state	== THREE)	|| (G_BOARD(ArrX-1,ArrY+1).state == FOUR)	 ||
					(G_BOARD(ArrX-1,ArrY+1).state	== FIVE)	|| (G_BOARD(ArrX-1,ArrY+1).state == SIX)	 ||
					(G_BOARD(ArrX-1,ArrY+1).state   == SEVEN)	|| (G_BOARD(ArrX-1,ArrY+1).state == EIGHT))
			{
				G_BOARD(ArrX-1,ArrY+1).isOpen = OPEN	;	
			}


		// x-1 ,y
			if( (G_BOARD(ArrX-1,ArrY).state == EMPTY) && (G_BOARD(ArrX-1,ArrY).isOpen == CLOSE) )
			{
				G_BOARD(ArrX-1,ArrY).isOpen = OPEN;
				EmptySearching(ArrX-1,ArrY);
			}
			
			else if ((G_BOARD(ArrX-1,ArrY).state	== FLAG)	|| (G_BOARD(ArrX-1,ArrY).state == QUESTION) );
			else if ((G_BOARD(ArrX-1,ArrY).state	== ONE)		|| (G_BOARD(ArrX-1,ArrY).state == TWO)	 ||
					(G_BOARD(ArrX-1,ArrY).state	== THREE)	|| (G_BOARD(ArrX-1,ArrY).state == FOUR)	 ||
					(G_BOARD(ArrX-1,ArrY).state	== FIVE)	|| (G_BOARD(ArrX-1,ArrY).state == SIX)	 ||
					(G_BOARD(ArrX-1,ArrY).state   == SEVEN)	|| (G_BOARD(ArrX-1,ArrY).state == EIGHT))
			{
				G_BOARD(ArrX-1,ArrY).isOpen = OPEN;	
			}
	
}










//------------------------------------------------------------------------------//
//									WndProc
//------------------------------------------------------------------------------//
//
//	추가기능 :		printf설정 , 시간 카운팅, 
//
//------------------------------------------------------------------------------//
//	전역변수 :		
//					static int c	:	printf를 쓸때 사용하기위한 변수
//					UINT style		:	style을 바꾸어서 적용시켜주기 위한변수
//					static POINTS ClntPos	:	클라이언트 윈도우 좌표
//					static POINTS ArrayPos	:	상대적 배열 좌표
//					static TCHAR str[256]	:	테스트용 변수
//
//
//					static int WMFlagCount	:	왼쪽 상단에 표시할 플래그 갯수카운트용
//
//					static int timeCount	:	1씩 증가하는 시간변수 - 타이머와함께 작동
//					아래 3개의 변수를 timePos의 인덱스로 사용하여 비트맵상의 Y축 좌표값을 얻어온다
//					int timeF		:	화면의 시간 카운팅중 100단위를 위한 카운트 변수
//					int timeS		:	10단위
//				 	int timeT		:	1단위	
//
//					timePos의 배열의 인자 각각은 비트맵(IDB_BITMAP2)의 좌표를 가르킨다 
//								      1   2   3   4   5   6   7  8  9  0
//					int timePos[10]={253,230,207,184,161,138,115,92,69,46}
//
//					picPos의 배열의 인자 각각은 비트맵(IDB_BITMAP1)의 좌표를 가르킨다 
//					EMPTY,1,2,3,4,5,6,7,8, 일반폭탄,클릭폭탄,X폭탄,일반그림,깃발,물음표
//					int picPos[16]={240,224,208,192,176,160,144,128,112,80,48,64,0,16,32};
//
//------------------------------------------------------------------------------//


// printf되게하는법..
char* s_msg[1000] = {"WM_NULL", "WM_CREATE"};		// 메세지를 다넣어서 디버깅을 한다



LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	// printf되게하는법..
	//static int c = 0;
	//printf("%04d : %d\n" , c++ ,msg);
//////////////////////////////////////////////////////////////////////////////////////////////
	UINT style;
	

	static POINTS ClntPos;							// 클라이언트 윈도우 좌표
	static POINTS ArrayPos;							// 상대적 배열 좌표
	
	static TCHAR str[256];							// 테스트용 변수

	static int timeCount;							// 1씩 증가하는 시간변수
	int timeF;	int timeS; 	int timeT;	
		
	//---------------1,   2   3   4   5   6   7  8  9  0  -
	int timePos[11]={253,230,207,184,161,138,115,92,69,46,0};
	
	
	//EMPTY,1,2,3,4,5,6,7,8, 일반폭탄,클릭폭탄,X폭탄,CLOSE=12,FLAG=13,QUESTION=14
	int picPos[16]={240,224,208,192,176,160,144,128,112,80,48,64,0,16,32};

	/////////////////////////////////////////////////////////	
	static int WMFlagCount;							// 왼쪽 상단에 표시할 플래그 갯수카운트용
	int flagF;	int flagS; 	int flagT;				// 

	static int MINEMINE = INITMINE;					// 


	// 스마일 그림들.
	// 속성을.. 클릭된 스마일,	썬글라스 스마일 , 놀란 스마일, 찌프린 스마일 ,일반 스마일
	//NORMAL_ON_SMILE	0		//	일반 스마일        - 스마일 버튼 클릭시 -
	//GLASS_SMILE		1		//  선글라쓰 쓴 스마일 - 성공적으로 종료했을경우
	//SUPRSISE_SMILE	2		//  놀란 스마일		   - 마우스 - LBUTTON_DOWN 시 
	//DISAPEAR_SMILE	3		//	찌프린 스마일	   - LBUTTON_UP시. 폭탄일경우.

	//NORMAL_SMILE		4		//	일반 스마일		   - LBUTTON_UP시. 폭탄이 아닐경우
	int smilePos[5] = {0,24,48,72,96};				// x-24,y-24
	static int smileState = NORMAL_SMILE;					// 스마일 상태의 초기값을 기본 스마일로 둔다

	// 구조체 초기화 이후에 스마일에대해 갱신할 RECT 영역을 잡아준다
	RECT Invalid_SMILErect = {Client.smile.x,Client.smile.y+2,Client.smile.x+24,Client.smile.y+24};


	static int isOpenCount =0;						// isOpen 의 갯수를 샌다
	static int isDieFlag = FALSE;					// 게임이 죽었나 안죽었나 확인


	HDC hdc,MemDC;
	HBITMAP MyBitmap, OldBitmap;

	PAINTSTRUCT ps;
	
	
//////////////////////////////////////////////////////////////////////////////////////////////
	
	
	
	switch( msg )
	{
	case WM_CREATE:{
		Init(9,  9, 10);				// Xp Yp는 현제 9,9 전역 스테틱
										// INITMIND도 전역 10
		MINEMINE = INITMINE;
		
		RECT rc = { 0,0,Client.first.xx,Client.first.yy+35 };	// +35는 아래 여백
		WMFlagCount=INITMINE;			// 깃발의 카운터 - 초기화로 기본 폭탄개수
		style = GetWindowLong( hwnd, GWL_STYLE);
		style &= ~WS_THICKFRAME;
		SetWindowLong( hwnd, GWL_STYLE, style );
		AdjustWindowRect( &rc, style, FALSE );
		MoveWindow(hwnd, 0, 0, rc.right - rc.left, rc.bottom - rc.top, TRUE);
		
		
		}
		return 0;

	case WM_COMMAND :{

	case ID_GAME_BEGIN : {
				
				//if(g_board !)
		
				free(g_board);
				Init(9,9,10);

				timeCount=0;				  // 타이머 변수 초기화	
				timeF=timeS=timeT=0;		  // 타이머를 카운터할 변수
				
				RECT rc2 = { 0,0,Client.first.xx,Client.first.yy+35 };	// +35는 아래 여백
				WMFlagCount=INITMINE;			// 깃발의 카운터 - 초기화로 기본 폭탄개수
				style = GetWindowLong( hwnd, GWL_STYLE);
				style &= ~WS_THICKFRAME;
				SetWindowLong( hwnd, GWL_STYLE, style );
				AdjustWindowRect( &rc2, style, FALSE );
				MoveWindow(hwnd, 0, 0, rc2.right - rc2.left, rc2.bottom - rc2.top, TRUE);

				
				
				WMFlagCount=INITMINE;			// 깃발의 카운터 - 초기화로 기본 폭탄개수
				
				KillTimer(hwnd,1);				// 타이머를 죽인다
				timeF=timeS=timeT=0;		  // 타이머를 카운터할 변수

				mineSettingFLAG = FALSE;		// 마우스 처음클랙해서 지뢰세팅하기 위해서 FALSE로 다시고친다


				RECT rc = { 0,0,Client.first.xx,Client.first.yy+35 };	// +35는 아래 여백
				InvalidateRect(hwnd,&rc,TRUE);
			  }
	//case 중급 : {
	case ID_GAME_INTER : {
				free(g_board);
				Init(16,16,40);

				timeCount=0;				  // 타이머 변수 초기화	
				timeF=timeS=timeT=0;		  // 타이머를 카운터할 변수
				
				RECT rc2 = { 0,0,Client.first.xx,Client.first.yy+35 };	// +35는 아래 여백
				WMFlagCount=INITMINE;			// 깃발의 카운터 - 초기화로 기본 폭탄개수
				style = GetWindowLong( hwnd, GWL_STYLE);
				style &= ~WS_THICKFRAME;
				SetWindowLong( hwnd, GWL_STYLE, style );
				AdjustWindowRect( &rc2, style, FALSE );
				MoveWindow(hwnd, 0, 0, rc2.right - rc2.left, rc2.bottom - rc2.top, TRUE);

				
				
				WMFlagCount=INITMINE;			// 깃발의 카운터 - 초기화로 기본 폭탄개수
				
				KillTimer(hwnd,1);				// 타이머를 죽인다
				timeF=timeS=timeT=0;		  // 타이머를 카운터할 변수

				mineSettingFLAG = FALSE;		// 마우스 처음클랙해서 지뢰세팅하기 위해서 FALSE로 다시고친다


				RECT rc = { 0,0,Client.first.xx,Client.first.yy+35 };	// +35는 아래 여백
				InvalidateRect(hwnd,&rc,TRUE);
			  }

	case ID_GAME_HIGH : {
				free(g_board);
				Init(30,16,99);

				timeCount=0;				  // 타이머 변수 초기화	
				timeF=timeS=timeT=0;		  // 타이머를 카운터할 변수
				
				RECT rc2 = { 0,0,Client.first.xx,Client.first.yy+35 };	// +35는 아래 여백
				WMFlagCount=INITMINE;			// 깃발의 카운터 - 초기화로 기본 폭탄개수
				style = GetWindowLong( hwnd, GWL_STYLE);
				style &= ~WS_THICKFRAME;
				SetWindowLong( hwnd, GWL_STYLE, style );
				AdjustWindowRect( &rc2, style, FALSE );
				MoveWindow(hwnd, 0, 0, rc2.right - rc2.left, rc2.bottom - rc2.top, TRUE);

				
				
				WMFlagCount=INITMINE;			// 깃발의 카운터 - 초기화로 기본 폭탄개수
				
				KillTimer(hwnd,1);				// 타이머를 죽인다
				timeF=timeS=timeT=0;		  // 타이머를 카운터할 변수

				mineSettingFLAG = FALSE;		// 마우스 처음클랙해서 지뢰세팅하기 위해서 FALSE로 다시고친다


				RECT rc = { 0,0,Client.first.xx,Client.first.yy+35 };	// +35는 아래 여백
				InvalidateRect(hwnd,&rc,TRUE);
			  }





		}

		return 0;

	case WM_LBUTTONDOWN :
		{
			
			// 배열에서의 상대 적인 좌표값.
			ClntPos = MAKEPOINTS(lParam);
			ArrayPos.x = (ClntPos.x-12) / 16 ;		
			ArrayPos.y = (ClntPos.y-55) / 16 ;

			


			
			
			// 미구현...
			ButtonPushMoveFLAG = TRUE;
			// 클릭한곳이 닫혀이씨다면 눌린그림으로 보여준다
			if (G_BOARD(ArrayPos.x,ArrayPos.y).isOpen == CLOSE){
				printf("ArrayPos.x = %d \n ArrayPos.y = %d\n" ,ArrayPos.x,ArrayPos.y);

				hdc = BeginPaint( hwnd, &ps );	
				MemDC=CreateCompatibleDC(hdc);

				RECT temp = {GetClntX(ArrayPos.x),GetClntY(ArrayPos.y),GetClntX(ArrayPos.x)+16,GetClntY(ArrayPos.y)+16};
				

				// 메모리 할당하고 리소스 불르고 로드한 비트맵 선택
				MemDC = CreateCompatibleDC(hdc);						
				MyBitmap = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP1));
				OldBitmap = (HBITMAP)SelectObject(MemDC,MyBitmap);
				
				// 읽어오는데 메모리상 y좌표를 배열에서 가져온다
				StretchBlt(hdc, 
					Client.miniPos.x+(ArrayPos.x*16) , Client.miniPos.y+(ArrayPos.y*16),	// 시작 x,y좌표
					16,16,												// X로 15 y로 15만큼 카피
					MemDC , 0,picPos[OPEN],16,16,SRCCOPY);
				
				InvalidateRect(hwnd,&temp,FALSE);
				// 자원해제
				SelectObject(MemDC,OldBitmap);
				DeleteObject(MyBitmap);
				DeleteDC(MemDC);
				
			}
			
			
			
						
			
			// 클라이언트 윈도우의 좌표가 스마일의 x1 좌표부다 크거나 같고
			// 클라이언트 윈도우의 좌표가 스마일의 x2 좌표부다 작거나 같고
			if( ( (ClntPos.x >= Client.smile.x+2) && (ClntPos.x <= Client.smile.xx-2) ) &&
				(ClntPos.y >= Client.smile.y+2) && (ClntPos.y <= Client.smile.yy-2) ) {
				// 눌린 스마일
				smileState = NORMAL_ON_SMILE;
				InvalidateRect(hwnd,&Invalid_SMILErect,FALSE);

			}
			else{

			// 스마일을 출력한다...놀란 스마일........
			smileState = SUPRSISE_SMILE;
			InvalidateRect(hwnd,&Invalid_SMILErect,TRUE);
			}




				


			
						
		}
		return 0;




	case WM_TIMER :{

		// 타이머를 보여줄 부분의 무효화영역
		RECT timeRect={109,14,150,40};
		
		// timer가 진행될때 마다 시카운트 증가
		timeCount++;
			
		// 시간을 999초까지만 보여준다 그이상일때는 타이머를 죽인다
		if (timeCount== 999)
			KillTimer(hwnd,1);			// timer카운트가 999까지 가면 타이머를 죽여라

		printf("INITMINE = %d \n",INITMINE);
		InvalidateRect(hwnd,&timeRect,TRUE);
		}
		return 0;
			



			
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

	case WM_RBUTTONDOWN	:
		{
			int i,j;
			// 클라이언트 윈도우 좌표값
			ClntPos = MAKEPOINTS(lParam);

			// 배열에서의 상대 적인 좌표값.
			ArrayPos.x = (ClntPos.x-12) / 16 ;
			ArrayPos.y = (ClntPos.y-55) / 16 ;


						
			// 지뢰찾을떄 쓸 마우스 좌표를 한계점을 둔다
			if( (  (ClntPos.x>=Client.miniPos.x)&&(ClntPos.x<=Client.miniPos.xx+15)  ) && 
				// 클라이언트 X좌표는 작은사각형의 시작좌표보다 크거나 같고
				// 클라이언틑 X좌표는 작은 윈도우의 X축 마지막좌표의 시작값보다 작거나 같아야된다
				(  (ClntPos.y>=Client.miniPos.y)&&(ClntPos.y<=Client.miniPos.yy+15)  )  ){
				// 클라이언트 Y좌표는 작은사각형의 시작좌표보다 크거나 같고
				// 클라이언트 Y좌표는 작은 사각형의 Y축 마지막좌표의 끝값보다 작거나 같으면된다
				
			
				// 만일 RBUTTONDOWN좌표
				

				// FLAG 좌표에 속성을 FLAG로 놓고 플래그 카운트를 줄이기
				if (G_BOARD(ArrayPos.x,ArrayPos.y).isOpen == CLOSE){
					G_BOARD(ArrayPos.x,ArrayPos.y).isOpen = FLAG;		// 속성을 FLAG로 놓는다
					WMFlagCount--;										// 플래그 카운트를 줄인다

					// 만일 플래그를 찍은곳이 지뢰 자리라면 지뢰 카운트를 하나 감소 시켜라
					if(G_BOARD(ArrayPos.x,ArrayPos.y).state == BOMB)
						INITMINE--;

					//------------------------------------------------------
					// 지뢰를 모두 찾은경우 ............
					//------------------------------------------------------
					//------------------------------------------------------
					//	지뢰 카운트 갯수가 0개이고 전체 좌표에서 실제로 찾은 지뢰갯수
					
					if (INITMINE == 0){
						// 성공했으므로
						isOpenCount=0;

						// 아래에 써먹기위해 OPEN이 몇개 되있나 세어본다
						for (i = 0 ; i < Xp ; i++){
							for (j = 0 ; j < Yp ; j++){
								if(G_BOARD(i,j).isOpen == OPEN)  isOpenCount++;
							}
						}

						if (Xp*Yp-MINEMINE == isOpenCount) {

							//////////////////////////////////////////////////////////////////
							// 썬글라스 쓴 스마일을 출력한다
							//////////////////////////////////////////////////////////////////
							smileState = GLASS_SMILE;
							InvalidateRect(hwnd,&Invalid_SMILErect,FALSE);

							G_BOARD(ArrayPos.x,ArrayPos.y).isOpen = FLAG;
							

							// 타이머를 죽인다
							KillTimer(hwnd,1);
							
							// 모두 연다
							for(j = 0; j <Yp ;++j)					// x축 = i = Xp
								for(i = 0; i <Xp ; ++i)
									G_BOARD(i,j).isOpen = OPEN;				
						}//	if (Xp*Yp-MINEMINE ) close
					}	// if (INITMINE == 0){ close


				}else if ( G_BOARD(ArrayPos.x,ArrayPos.y).isOpen == FLAG){
					G_BOARD(ArrayPos.x,ArrayPos.y).isOpen =QUESTION;
					WMFlagCount++;

					// 만일 플래그를 해제 한곳이 지뢰 자리라면 지뢰 카운트 하나를 증가 시켜라
					if(G_BOARD(ArrayPos.x,ArrayPos.y).state == BOMB)
						INITMINE++;

				}
				else if (G_BOARD(ArrayPos.x,ArrayPos.y).isOpen == QUESTION){
					//printf("X = %d Y= %d\n" ,ArrayPos.x,ArrayPos.y);
					//printf("QUESTION입니다---FLAG로 바꾸겠습니다\n");
					G_BOARD(ArrayPos.x,ArrayPos.y).isOpen = CLOSE;
				}


			}	// 좌표 제한 if끝

				
			RECT RBrect = {GetClntX(ArrayPos.x),GetClntY(ArrayPos.y),
				GetClntX(ArrayPos.x)+16,GetClntY(ArrayPos.y)+16};
			RECT FLAGRect = {Client.third.x,Client.third.y,Client.third.xx,Client.third.yy};
			printf("x1=%d,y1=%d,x2=%d,y2=%d\n",GetClntX(ArrayPos.x),GetClntY(ArrayPos.y),GetClntX(ArrayPos.x+16),GetClntY(ArrayPos.y+16));


			InvalidateRect(hwnd,&FLAGRect,FALSE);
			InvalidateRect(hwnd,&RBrect,FALSE);
		}

		return 0;
	case WM_LBUTTONUP : 
		{
			int i,j;
			
			// 클라이언트 윈도우 좌표값
			ClntPos = MAKEPOINTS(lParam);

			// 배열에서의 상대 적인 좌표값.
			ArrayPos.x = (ClntPos.x-12) / 16 ;
			ArrayPos.y = (ClntPos.y-55) / 16 ;


			


			if (mineSettingFLAG == FALSE){			 // 처음 마우스를 클릭하는경우
				
				
				// timer
				SetTimer(hwnd ,1, 1000, 0);	  // 1번 타이머 발생 1초에 한번씩.
											  // 타이머심기
				timeCount=0;				  // 타이머 변수 초기화	
				timeF=timeS=timeT=0;		  // 타이머를 카운터할 변수

						
				// 바인딩. 지뢰를 넣고 숫자를 초기화하는 함수 호출
				// 첫번째 좌표를 넘겨서 그자리엔 지뢰를 않넣는다
				Binding(INITMINE , Xp , Yp,ArrayPos.x,ArrayPos.y);	 
				//printf("--INITMINE = %d      ###########################\n ", INITMINE);

				WMFlagCount=INITMINE;			// 깃발의 카운터 - 초기화로 기본 폭탄개수

				// FLAG를 바꿔서 다음에 다시 못들어오게함
				mineSettingFLAG = TRUE;		  
			}
			




			// 클라이언트 윈도우의 좌표가 스마일의 x1 좌표부다 크거나 같고
			// 클라이언트 윈도우의 좌표가 스마일의 x2 좌표부다 작거나 같고
			if( ( (ClntPos.x >= Client.smile.x+2) && (ClntPos.x <= Client.smile.xx-2) ) &&
				(ClntPos.y >= Client.smile.y+2) && (ClntPos.y <= Client.smile.yy-2) ) {
				
				//smileState = NORMAL_SMILE;
				//InvalidateRect(hwnd,&Invalid_SMILErect,FALSE);
				//Init(Xp,Yp,INITMINE);

				//g_board = (BLOCK*)malloc(sizeof(BLOCK)*Xp*Yp);
				// 일단 메모리를 풀어주고 다시잡는

				// 초급
				//if(g_board !=NULL){
					free(g_board);
					printf("메모리 프리~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

				//}
				Init(9,9,10);

				timeCount=0;				  // 타이머 변수 초기화	
				timeF=timeS=timeT=0;		  // 타이머를 카운터할 변수
				
				RECT rc2 = { 0,0,Client.first.xx,Client.first.yy+35 };	// +35는 아래 여백
				WMFlagCount=INITMINE;			// 깃발의 카운터 - 초기화로 기본 폭탄개수
				style = GetWindowLong( hwnd, GWL_STYLE);
				style &= ~WS_THICKFRAME;
				SetWindowLong( hwnd, GWL_STYLE, style );
				AdjustWindowRect( &rc2, style, FALSE );
				MoveWindow(hwnd, 0, 0, rc2.right - rc2.left, rc2.bottom - rc2.top, TRUE);

				
				
				WMFlagCount=INITMINE;			// 깃발의 카운터 - 초기화로 기본 폭탄개수
				
				KillTimer(hwnd,1);				// 타이머를 죽인다
				timeF=timeS=timeT=0;		  // 타이머를 카운터할 변수

				mineSettingFLAG = FALSE;		// 마우스 처음클랙해서 지뢰세팅하기 위해서 FALSE로 다시고친다


				RECT rc = { 0,0,Client.first.xx,Client.first.yy+35 };	// +35는 아래 여백
				InvalidateRect(hwnd,&rc,TRUE);

			}

			//---------------------------------------------------------
			// 스마일 출력
			//---------------------------------------------------------
			// 클릭한 좌표에 폭탄이 없다면 일반 스마일을 출력한다
			//---------------------------------------------------------
			if (G_BOARD(ArrayPos.x,ArrayPos.y).state != BOMB ){
				smileState = NORMAL_SMILE;
				InvalidateRect(hwnd,&Invalid_SMILErect,FALSE);
			}
			else if ( isDieFlag ==TRUE){
				smileState = DISAPEAR_SMILE;
				InvalidateRect(hwnd,&Invalid_SMILErect,FALSE);
			}


			// 클릭한곳에 좌표가 있어도 게임에서 졌으면 스마일 모양을 출력해준다
							
			
			



			
			// 지뢰찾을떄 쓸 마우스 좌표를 한계점을 둔다

			if( (  (ClntPos.x>=Client.miniPos.x)&&(ClntPos.x<=Client.miniPos.xx+15)  ) && 
				// 클라이언트 X좌표는 작은사각형의 시작좌표보다 크거나 같고
				// 클라이언틑 X좌표는 작은 윈도우의 X축 마지막좌표의 시작값보다 작거나 같아야된다
				(  (ClntPos.y>=Client.miniPos.y)&&(ClntPos.y<=Client.miniPos.yy+15)  )  &&
				// 클라이언트 Y좌표는 작은사각형의 시작좌표보다 크거나 같고
				// 클라이언트 Y좌표는 작은 사각형의 Y축 마지막좌표의 끝값보다 작거나 같으면된다
				(isDieFlag ==FALSE)){				
				// 지뢰를 밝아 죽지않았어야 한다
			
				// 만일 CLICKUP좌표가 지뢰가아니 라면
				// 지뢰가 아닐경우가 더많기에 첫번째 if에 대입

	

				 if ( G_BOARD(ArrayPos.x,ArrayPos.y).isOpen == FLAG) {
					printf("플래그라면................\n");
					printf("플래그라면................\n");
					printf("플래그라면................\n");
					printf("플래그라면................\n");
					printf("플래그라면................\n");
					
				// 그리고 깃발 생태는 클릭하면안된다


					
				//------------------------------------------------------	
				//------------------------------------------------------
				// 클릭한곳이 폭탄이라면............
				//------------------------------------------------------
				//------------------------------------------------------

				}else if (G_BOARD(ArrayPos.x,ArrayPos.y).state == BOMB){
					
				
					

					//클릭하면 나오는 지뢰모양으로 속성을 바꿔준다
					G_BOARD(ArrayPos.x,ArrayPos.y).state = CLICKBOMB;
					// 자기자신을 열고
					G_BOARD(ArrayPos.x,ArrayPos.y).isOpen =OPEN;

					// 안열린 나머지 지뢰들을 모두 열어준다
					for(j = 0; j <Yp ;++j)					// x축 = i = Xp
						for(i = 0; i <Xp ; ++i)
							if (G_BOARD(i,j).state == BOMB)
								G_BOARD(i,j).isOpen = OPEN;	

					
					// 그리고 지뢰를 클릭했으므로 타이머를 죽인다
					KillTimer(hwnd,1);


					// 클릭한 좌표에 폭탄이 있다면 인상쓴 스마일
					smileState = DISAPEAR_SMILE;
					InvalidateRect(hwnd,&Invalid_SMILErect,FALSE);

					// 게임에서 졌다는 플래그..졌으므로 게임을 활성화 시킬때까지 게임영역에 못들어온다
					isDieFlag = TRUE;

							
					
				
				}	// if end
					

				//------------------------------------------------------	
				//------------------------------------------------------
				// 클릭한곳의 속성이 EMPTY라면 ............
				//------------------------------------------------------
				//------------------------------------------------------

				else if (G_BOARD(ArrayPos.x,ArrayPos.y).state == EMPTY||
					G_BOARD(ArrayPos.x,ArrayPos.y).state == ONE||
					G_BOARD(ArrayPos.x,ArrayPos.y).state == TWO||
					G_BOARD(ArrayPos.x,ArrayPos.y).state == THREE||
					G_BOARD(ArrayPos.x,ArrayPos.y).state == FOUR||
					G_BOARD(ArrayPos.x,ArrayPos.y).state == FIVE||
					G_BOARD(ArrayPos.x,ArrayPos.y).state == SIX||
					G_BOARD(ArrayPos.x,ArrayPos.y).state == SEVEN){
					
					// 서칭해서 주위에 폭탄이없으면 다열어준다
					G_BOARD(ArrayPos.x,ArrayPos.y).isOpen = OPEN;
					EmptySearching(ArrayPos.x,ArrayPos.y);
					
				}				
				

				//------------------------------------------------------	
				//------------------------------------------------------
				// 지뢰를 모두 찾은경우 ............
				//------------------------------------------------------
				//------------------------------------------------------
				//	지뢰 카운트 갯수가 0개이고 전체 좌표에서 실제로 찾은 지뢰갯수
				
				else if (INITMINE == 0){
					// 성공했으므로

					
					printf("INITMINE = %d\n", INITMINE);
					printf("isOpenCount= %d\n" , isOpenCount );
					printf("Xp*Yp-MINEMINE = %d\n" ,(Xp*Yp)-MINEMINE);

					printf("Xp = %d \n Yp = %d \n" , Xp, Yp);

					// 아래에 써먹기위해 OPEN이 몇개 되있나 세어본다
					for (i = 0 ; i < Xp ; i++){
						for (j = 0 ; j < Yp ; j++){
							if(G_BOARD(i,j).isOpen == OPEN)  isOpenCount++;
						}
					}

					
					// 총 배열의 갯수에서 지뢰의 갯수만큼을 뺀거. 
					// 8*8의 경우 64 -10 (지뢰갯수) 
					// 54개 만큼 OPEN이되야 있어야 한다
					
					if (Xp*Yp-MINEMINE == isOpenCount) {

						//////////////////////////////////////////////////////////////////
						// 썬글라스 쓴 스마일을 출력한다
						//////////////////////////////////////////////////////////////////
						smileState = GLASS_SMILE;
						InvalidateRect(hwnd,&Invalid_SMILErect,FALSE);
						


						// 타이머를 죽인다
						KillTimer(hwnd,1);
						
						// 모두 연다
						for(j = 0; j <Yp ;++j)					// x축 = i = Xp
							for(i = 0; i <Xp ; ++i)
								G_BOARD(i,j).isOpen = OPEN;				
					}//	if (Xp*Yp-MINEMINE ) close
				}	// if (INITMINE == 0){ close
						
				else{
					// LBUTTON한 곳의 배열의 속성을 ON으로 고침.. 
					G_BOARD(ArrayPos.x,ArrayPos.y).isOpen = OPEN;					
				}	// else close
						

				
					printf("\n\n\n----------------------------------------------\n");
					printf("------------------심은 지뢰-----------------\n");
					printf("----------------------------------------------\n");
					for ( j = 0 ; j < Yp ; ++j){
						for(i = 0 ; i < Xp ; ++i){
					printf("%2d ",G_BOARD(i,j).state);
					}
					printf("\n");
					}

					printf("\n\n\n----------------------------------------------\n");
					printf("------------------IS OPEN-----------------\n");
					printf("----------------------------------------------\n");
					for ( j = 0 ; j < Yp ; ++j){
						for(i = 0 ; i < Xp ; ++i){
					printf("%2d ",G_BOARD(i,j).isOpen);
					}
					printf("\n");
					}


				InvalidateRect(hwnd,0,FALSE);
				
			}
			
		}
		return 0;


//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------		
	case WM_PAINT:
		{		

		int i, j;

		hdc = BeginPaint( hwnd, &ps );	
		MemDC=CreateCompatibleDC(hdc);


//////////////////////////////////////////////////////////////////////////////////////////////
//									화면 전체의 틀
//////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////


		//===========================메뉴 및 버튼 그리기
		Draw3dRect( hdc, Client.first.x,   Client.first.y,  
			Client.first.xx, Client.first.yy , FALSE, 3 );		// 메뉴아래 클라이언트 시작부터
		Draw3dRect( hdc, Client.second.x,   Client.second.y,  
			Client.second.xx, Client.second.yy,  TRUE,  2 );		
		Draw3dRect( hdc, Client.third.x,  Client.third.y, 
			Client.third.xx,  Client.third.yy,  TRUE,  1 );
		Draw3dRect( hdc, Client.four.x, Client.four.y,
			Client.four.xx, Client.four.yy,  TRUE,  1 );
		Draw3dRect( hdc, Client.five.x, Client.five.y, 
			Client.five.xx, Client.five.yy, TRUE,  3 );

		//Draw3dRect(hdc , Client.smile.x ,Client.smile.y ,Client.smile.xx ,Client.smile.yy,
		//	FALSE, 2);

		// 마우스 클릭시 기록 CLOSE OPEN 정보를 가지고 여는 동작을 보여준다
		// 닫혀 있을경우에는 
		// 첫인자 dc , 두번째 그릴 x1, 두번째 y1. 세번째 x2 네번째 y2



//////////////////////////////////////////////////////////////////////////////////////////////
//									화면 상단에 출력되는 부분
//////////////////////////////////////////////////////////////////////////////////////////////
//=================타이머=========================
///////////////////////////////////////////////////


		// 카운트를 표시해준다..
		// time 카운트
		timeF = timeCount/100;			// 100단위
		timeS = (timeCount%100)/10;		// 10단위
		timeT = timeCount %10;			// 1단위

		
		MemDC = CreateCompatibleDC(hdc);
		MyBitmap = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP2));		// 숫자출력 비트맵
		OldBitmap = (HBITMAP)SelectObject(MemDC,MyBitmap);
		// timeF--100단위 타이머
		StretchBlt(hdc, Client.four.x+1 , Client.four.y+2,	// 시작 x,y좌표 4번째 프레임보다 +1,+2위치이다
						13,23,MemDC , 0,
						timePos[timeF],						// timePos(비트맵에서 읽어올 Y축좌표) 타임을 인덱스로)
						13,23,SRCCOPY);					    // x축으로 13만큼 Y축으로 23만큼 카피
		// timeS--10단위 타이머
		StretchBlt(hdc, Client.four.x+1+13 , Client.four.y+2,
						13,23,MemDC , 0,timePos[timeS],13,23,SRCCOPY);		
		// timeT--1단위 타이머
		StretchBlt(hdc, Client.four.x+1+13+13 , Client.four.y+2,		
						13,23,MemDC , 0,timePos[timeT],13,23,SRCCOPY);		

		SelectObject(MemDC,OldBitmap);
		DeleteObject(MyBitmap);
		DeleteDC(MemDC);



//////////////////////////////////////////////////////////////////////////////////////////////
//									화면 상단에 출력되는 부분
//////////////////////////////////////////////////////////////////////////////////////////////
//=================flag카운트=========================
//////////////////////////////////////////////////////


		// flag 카운트
		// 100단위플래그는 플래그 카운터가 0보다작을땐 -카운터용도로 인덱스 11을
		// 0보다 클땐 100단위 숫자를넣는다
		flagF = (WMFlagCount<0) ?	10 :  WMFlagCount/100;			// 100단위
		flagS = (WMFlagCount%100)/10;		// 10단위
		flagS = (flagS<0) ? -flagS : flagS;
		flagT = WMFlagCount %10;			// 1단위
		flagT = (flagT<0) ? -flagT : flagT;

		printf("flagF=%d , flagS=%d ,flagT=%d \n", flagF,flagS,flagT);

		MemDC = CreateCompatibleDC(hdc);
		MyBitmap = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP2));		// 숫자출력 비트맵
		OldBitmap = (HBITMAP)SelectObject(MemDC,MyBitmap);
		// flagF--100단위 타이머 or 부호
		StretchBlt(hdc, Client.third.x+1 , Client.third.y+2,	// 시작 x,y좌표 4번째 프레임보다 +1,+2위치이다
						13,23,MemDC , 0,
						timePos[flagF],						// timePos(비트맵에서 읽어올 Y축좌표) 타임을 인덱스로)
						13,23,SRCCOPY);					    // x축으로 13만큼 Y축으로 23만큼 카피
		// flagS--10단위 타이머
		StretchBlt(hdc, Client.third.x+1+13 , Client.third.y+2,
						13,23,MemDC , 0,timePos[flagS],13,23,SRCCOPY);		
		// flagT--1단위 타이머
		StretchBlt(hdc, Client.third.x+1+13+13 , Client.third.y+2,
						13,23,MemDC , 0,timePos[flagT],13,23,SRCCOPY);		

		SelectObject(MemDC,OldBitmap);
		DeleteObject(MyBitmap);
		DeleteDC(MemDC);


//////////////////////////////////////////////////////////////////////////////////////////////
//									화면 상단에 출력되는 부분
//////////////////////////////////////////////////////////////////////////////////////////////
//=================스마일=========================
///////////////////////////////////////////////////
		// 스마일의 상태에 따라 스마일을 출력한다.
		//			smileState

		MemDC = CreateCompatibleDC(hdc);
		MyBitmap = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP3));		// 스마일 출력 비트맵
		OldBitmap = (HBITMAP)SelectObject(MemDC,MyBitmap);
		StretchBlt(hdc, Client.smile.x , Client.smile.y+2,	// 시작 x,y좌표 
						24,24,MemDC , 0,
						smilePos[smileState],				// smilePos배열의 인덱스로 smileState를 선택한다
						24,24,SRCCOPY);					    // x축으로 26만큼 Y축으로 26만큼 카피
		SelectObject(MemDC,OldBitmap);
		DeleteObject(MyBitmap);
		DeleteDC(MemDC);




//////////////////////////////////////////////////////////////////////////////////////////////
//									화면 하단에 출력되는 부분
//////////////////////////////////////////////////////////////////////////////////////////////
//=================지뢰찾기=========================
/////////////////////////////////////////////////////



		// 상대적인 좌표 i는 0부터 시작해서 i값이 Xp.. 즉 가로 사이즈만큼..(배열이니까 Xp는 포함안된다)
		// 상대적인 좌표 j는 0부터 시작해서 j값이 Yp.. 즉 세로 사이즈만큼
		// 클라이언트 윈도우의 주소로 바꾸어서 연산....
		
		for(j = 0 ; j < Yp ; j++){			// x축 = i  = Xp
			for(i = 0 ; i < Xp ; i++){
				//Draw3dRect( hdc, x, y, x + 15, y + 15, FALSE, 2 );

				if(G_BOARD(i,j).isOpen == CLOSE)							//isOpen == CLOSE일경우
				{
					//Draw3dRect( hdc,12+(i*16),55+(j*16),12+((i+1)*16)-1,55+((j+1)*16)-1,FALSE,2);
					MemDC = CreateCompatibleDC(hdc);
					MyBitmap = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP1));
					OldBitmap = (HBITMAP)SelectObject(MemDC,MyBitmap);
					StretchBlt(hdc, 
						Client.miniPos.x+(i*16) , Client.miniPos.y+(j*16),	// 시작 x,y좌표
						16,16,												// X로 15 y로 15만큼 카피
						MemDC , 0,0,16,16,SRCCOPY);
					SelectObject(MemDC,OldBitmap);
					DeleteObject(MyBitmap);
					DeleteDC(MemDC);
				//	printf("G_BOARD(%d,%d).isOpen ===CLOSE = %d\n",i,j,G_BOARD(i,j).isOpen);
					
				}					
				else if ( G_BOARD(i,j).isOpen == OPEN )						//isOpen == CLOSE
				{
				//	printf("G_BOARD(%d,%d).isOpen ===OPEN = %d\n",i,j,G_BOARD(i,j).isOpen);

					// 메모리 할당하고 리소스 불르고 로드한 비트맵 선택
					MemDC = CreateCompatibleDC(hdc);						
					MyBitmap = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP1));
					OldBitmap = (HBITMAP)SelectObject(MemDC,MyBitmap);
					
					// 읽어오는데 메모리상 y좌표를 배열에서 가져온다
					StretchBlt(hdc, 
						Client.miniPos.x+(i*16) , Client.miniPos.y+(j*16),	// 시작 x,y좌표
						16,16,												// X로 15 y로 15만큼 카피
						MemDC , 0,picPos[G_BOARD(i,j).state],16,16,SRCCOPY);
					
					// 자원해제
					SelectObject(MemDC,OldBitmap);
					DeleteObject(MyBitmap);
					DeleteDC(MemDC);
					
				}
				else if ( G_BOARD(i,j).isOpen == FLAG )						//isOpen == CLOSE
				{
				//	printf("G_BOARD(%d,%d).isOpen ===FLAG = %d\n",i,j,G_BOARD(i,j).isOpen);

					// 메모리 할당하고 리소스 불르고 로드한 비트맵 선택
					MemDC = CreateCompatibleDC(hdc);						
					MyBitmap = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP1));
					OldBitmap = (HBITMAP)SelectObject(MemDC,MyBitmap);
					
					// 읽어오는데 메모리상 y좌표를 배열에서 가져온다
					StretchBlt(hdc, 
						Client.miniPos.x+(i*16) , Client.miniPos.y+(j*16),	// 시작 x,y좌표
						16,16,												// X로 15 y로 15만큼 카피
						MemDC , 0,picPos[FLAG],16,16,SRCCOPY);
					
					// 자원해제
					SelectObject(MemDC,OldBitmap);
					DeleteObject(MyBitmap);
					DeleteDC(MemDC);
					
				}else if ( G_BOARD(i,j).isOpen == QUESTION )						//isOpen == CLOSE
				{
					//printf("G_BOARD(%d,%d).isOpen ===OPEN = %d\n",i,j,G_BOARD(i,j).isOpen);

					// 메모리 할당하고 리소스 불르고 로드한 비트맵 선택
					MemDC = CreateCompatibleDC(hdc);						
					MyBitmap = LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP1));
					OldBitmap = (HBITMAP)SelectObject(MemDC,MyBitmap);
					
					// 읽어오는데 메모리상 y좌표를 배열에서 가져온다
					StretchBlt(hdc, 
						Client.miniPos.x+(i*16) , Client.miniPos.y+(j*16),	// 시작 x,y좌표
						16,16,												// X로 15 y로 15만큼 카피
						MemDC , 0,picPos[QUESTION],16,16,SRCCOPY);
					
					// 자원해제
					SelectObject(MemDC,OldBitmap);
					DeleteObject(MyBitmap);
					DeleteDC(MemDC);
					
				}// if문 종료	
			}		// 두번째 for
		}		//	첫번째 for

  


		TextOut(hdc,ClntPos.x+5 , ClntPos.y+10 , str , lstrlen(str));
		EndPaint( hwnd, &ps );
		}
		return 0;

		


//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------


	case WM_DESTROY: 
		PostQuitMessage(0);					 
		return 0;
	}
	
	return DefWindowProc( hwnd, msg, wParam, lParam);
}






//=======================WinMain=================================
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPSTR	  lpCmdLine, int nShowCmd )
	{

	// GUI도 Console을 생성할수 있다
	AllocConsole();

	// stdout을 console과 연결한다
	freopen("CONOUT$","wt",stdout);		// CONSOUT$ : console 출력
										// wt : write text 텍스트 쓰기 모드
										// 파일 포인터를 다시 오픈할때 쓰는 모드



	g_hInst = hInstance;
	WNDCLASS wc;
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hbrBackground= (HBRUSH)GetStockObject( WHITE_BRUSH ); 
	wc.hCursor		= LoadCursor( 0, IDC_ARROW );
	wc.hIcon		= LoadIcon( hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hInstance	= hInstance;
	wc.lpfnWndProc  = WndProc; 
	wc.lpszClassName= "First";
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.style		= 0;
	ATOM atom = RegisterClass( &wc); 
	if ( atom == 0 ) 
	{
		MessageBox( 0, "Fail To RegisterClass", "Error", MB_OK);
		return 0;
	}

	HWND hwnd = CreateWindowEx( WS_EX_APPWINDOW , "first", "지뢰 찾기", WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT, CW_USEDEFAULT,0 ,0, 0, 0, 
								hInstance, 0); 
	ShowWindow( hwnd, nShowCmd); 
	UpdateWindow( hwnd );

	MSG msg;
	while ( GetMessage( &msg, 0, 0, 0) ) 
	{									 
		DispatchMessage( &msg );
	}

	return 0;
}


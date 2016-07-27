#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include "declar.h"



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
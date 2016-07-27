/**************************************************************

	[ 스택 풀기(Stack Unwinding) ]


	발생한 예외에 대하여 처리할 catch 블록이 없는 경우, 자동으로 
	상위 함수로 예외 객체를 전달한다.

	만약 main()함수까지 전달되었을 때에도 처리할 catch() 블록이 없으면 
	프로그램이 종료된다.
 

**************************************************************/

//[예제] 스택 풀기의 예제 프로그램

#include <iostream.h>

void f3(double n); 
void f2(double n); 
void f1(double n);

int main(void)
{
    try
	{
        double num = -100;
        f1(num);
    }
    catch (char* msg)  
	{
		cout << "[main()] ##Error!! : " << msg << endl;
	}
    return 0;
}


void f1(double n)
{
    try		{    		f2(n);    	}
    catch (int no) {cout << "[f1()] ##Error!! : " << no << endl;};
}

void f2(double n)
{
    try{    f3(n);    }
    catch (int no) {cout << "[f2()] ##Error!! : " << no << endl;};
}

void f3(double n)
{
    try{    if (n < 0) throw "The number is too small.";    }
    catch (int no) {cout << "[f3()] ##Error!! : " << no << endl;};
}

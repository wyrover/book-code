/**************************************************************

	[ C++의 예외 처리 방식 ]

	try 블록 설정
		하나의 함수 내에 둘 이상의 try블록이 설정될 수 있다.
		하나의 try 블록 내부에 다른 try블록이 내포될 수 있다.
		try 블록도 명시적인 블록이므로 해당 블록 내에서 선언된 변수들은 
		  그 블록이 끝나면 사용할 수 없다.

	catch 블록 설정
		catch 블록 설정 시 해당 블록에서 처리할 예외의 타입를 명시한다.
		명확한 타입 대신 생략 기호(...)를 사용하면 모든 종류의 예외를 
		포착할 수 있다.
		하나의 try블록 뒤에 둘 이상의 catch블록들이 올 수 있으며, 앞에서부터 
		   차례대로 예외의 타입이 맞는지 검사 하게 된다.
		경우에 따라 catch블록에서 해당 예외에 대한 처리를 직접 하지 않고 다시 
		throw할 수 있다.

			catch (int num) {
				if (num ==0) cout << “This Number is Zero” << endl;
				else throw; 
			}

**************************************************************/

//[예제] 앞의 예제에 여러 개의 try블록과 catch블록을 설정해 보았다.

#include <iostream.h>
#include <stdlib.h>
double divide(double, double);

int main(void)
{
    try
	{
		double d1 = 10, d2 = 20;
		cout << d1 << " / " << d2 << " = " << divide(d1,d2) << endl;
		d2 = 0; 
		cout << d1 << " / " << d2 << " = " << divide(d1,d2) << endl;
    }
    catch (char* msg)
	{  //문자열 예외 처리
		cout << "## Error~!! : " << msg <<" ##" << endl;
    }
    try
	{
		int d2 = 10; 
		if (d2 < 100)	
			throw -1;
    }
    catch (...)
	{  //모든 종류의 예외 처리
		cout << "## Error~!! ##" << endl;
    }
    return 0;
}

double divide(double n, double m)
{
    if (m == 0)    
		throw "0으로 나눌 수 없습니다.";
    return n/m;
}

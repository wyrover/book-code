/**************************************************************

	[ C++의 예외 명세 함수 (exception specification) ]


	예외 명세를 함으로써, 해당 함수가 발생시킬 수 있는 예외들의 
	종류를 지정할 수 있다.

	인터페이스 사용자들에게 어떤 예외가 발생되는 지 알려주기 위한 
	것이다.


    ex)

	void f (int a) throw (x1, x2) {......}
	double g (double n, double m) throw ( ){......}


   - f() 함수에서는 x1, x2 타입의 예외나 혹은 그로부터 파생된객체
      타입의 예외가 발생할 수 있다.
   - g() 함수에서는 예외를 발생시키지 않는다. 

**************************************************************/


//[예제] 앞의 나눗셈 예제에서 divide()함수에 예외 명세를 해보았다.

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
	{
		cout << "## Error~!! : " << msg <<" ##" << endl;
    }
    return 0;
}


//문자열 예외의 발생을 의미하는 명세
double divide(double n, double m) throw (char*) 
{
    if (m == 0) 
		throw "0으로 나눌 수 없습니다.";
//		throw 10;
    return n/m;
}


/**************************************************************

	[ C++의 예외 처리 방식 ]

	예외 처리 관련 키워드

	try
	예외 발생에 대해 검사를 수행할 범위를 설정한다.
	범위(블록)는 try 키워드로 시작하고 바로 뒤이어 나오는 한 쌍의 중괄호({})
	로 설정된다.

	catch
	발생한 예외를 포착하고 처리하는 블럭을 선언할 때 사용한다.
	catch 키워드로 시작하고 중괄호로 이루어지는 블록내부에 처리 내용을 코딩
	   한다.
	catch 블록은 try블록의 바로 뒤에 위치시킨다.

	throw
	특정 예외 상황이 발생하였음을 알린다.
	throw 키워드로 시작하며, 바로 뒤에 그 예외의 특징을 나타내는 문자열이나 
	   객체와  같은 하나의 값을 반환한다.
	이 값의 타입으로 여러 개의 catch 블록 중 실제 실행될 블록을 찾아낸다.

**************************************************************/

//[예제] 나눗셈에서 0으로 나누는 예외에 대한 C++ 의 에러 처리 방식

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
    catch (const char* msg)
	{
		cout << "## Error~!! : " << msg <<" ##" << endl;
    }
    return 0;
}

double divide(double n, double m)
{
    if (m == 0) 
		throw "0으로 나눌 수 없습니다.";
    return n/m;
}

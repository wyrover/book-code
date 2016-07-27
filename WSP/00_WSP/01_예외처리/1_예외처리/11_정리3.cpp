#include <iostream>
using namespace std;


// c++에는 기본적으로 exception 클래스가 있다.
// exception 클래스를 계승받아 사용하는 것이 좋다.
class MyException : public exception
{
public:
	// 사용시에는 what() 함수를 재정의해준다. 
	// 부모가 virtual 이므로 virtual 로 재정의한다.
	virtual const char* what()const throw()
	{  
		return "MyException";
	}
};


void foo() throw(exception*)
{
	throw new MyException;
}


void main()
{
	try{
		foo();
	}
	catch(exception* p)
	{
		cout << p->what() << endl;
		delete p;
	}
}

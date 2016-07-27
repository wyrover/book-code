//----------------
// Sample 1
//----------------


// SMART POINTER
/*
#include <iostream>
using namespace std;

class Test
{
	int x;
public:
	~Test() { cout << "Test::~Test" << endl; }
	void foo() { cout << "Test::Foo" << endl; }

};

// 스마트 포인터 : 다른 객체의 포인터 역할
template<typename T> class SPTR
{
	T *_t;
public:
	SPTR( T *t) : _t(t) {}
	~SPTR()				{ delete _t; }
	T* operator->()     { return _t; }
	T & operator*()     { return *_t;}
};

void main()
{
	SPTR<Test> p(new Test);

	p->foo();

//	delete p;
}
*/


/*
#include <memory>
// memory 헤더 파일 include 하면
// auto_ptr 을 사용할수 있음. smart pointer 사용 가능


class Test
{
public:
	Test()		{ cout << "Test" << endl;  }
	~Test()		{ cout << "~Test" << endl; }
	void goo()	{cout << "goo" << endl;    }
};

void foo()
{
//	auto_ptr<Test> p(new Test[100]); // error
	// auto_ptr은 반드시 객체만 사용가능하다.
//	auto_ptr<Test> p = new Test; // explict constructor이므로 error
	// 즉 변환이 불가능하다. 엄밀하게 아래와 같지 않음

	auto_ptr<Test> p(new Test); // smart pointer..
	p->goo(); 
	throw "exception";
//  delete p; // error 
}


void main()
{
	try
	{
		foo();		
	}
	catch(char* msg)
	{	
		cout << msg << endl;		
	}
}
*/


//(ex) 예외 처리 : 실행시 에러를 처리하는 표준적인 방법
/*
#include <iostream>		
using namespace std;

#define ERRROR   -1		
#define SUCCEEDED 0


int errorCode;
void SetErrrorCode(int a){		errorCode = a;		}

int GetErrorCode(){		return errorCode;		}

int foo()
{
	int *p = new int[1000];
	if(p==0)
	{
		SetErrrorCode(1);		
		return ERRROR;		
	}
	return SUCCEEDED;
}


void main()
{
	if(foo() == ERRROR)
	{
		int error = GetErrorCode();   
		cout << "error : " << error << endl;	
	}
}

*/


/*
// ex3)	처리 되지 않은 예외 발생
#include <iostream>		
using namespace std;


void myHandler()
{
	cout << "처리 되지 않은 예외 발생" << endl;	
	exit(-1);	
}


void foo() throw(int, char*)
{
	if (1) throw 1; 		
	if (2) throw "a";	
	throw new string("aa");
}

void main()
{
	// set_unexpected() 는 예상치 못한 결과 발생 처리하는 함수
	// set_terminate() 처리되지 않은 예외를 처리하는 함수	

//	set_terminate(myHandler); // 처리 되지 않은 예외 발생시
	// set_terminate() 함수를 사용하여 myHandler()부름 
	try
	{		
		foo();		
	} 
	catch(char* a)
	{		
		cout << a << endl;		
	}
	// throw 한 char을 catch 하지 못하면 그 순간  
	//abort()가 호출되고 프로그램이 종료된다. 

//	catch(...)
//	{ // ... catch는 반드시 맨 밑에 있어야 한다.
		
//	}
}
*/
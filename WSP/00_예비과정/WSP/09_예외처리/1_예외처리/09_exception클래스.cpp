/**************************************************************

	[ exception 클래스 ]
	
	exception 헤더 파일에 선언되어 있다.
	다른 예외 클래스들의 기본 클래스로 사용된다.
	what() 이라는 가상(virtual function) 함수가 주어진다. 
	이 함수를 이용하여 반환할 문자열을 지정해 줄 수 있다.

**************************************************************/

//[예제1] 앞에서 본 ESize 클래스를 exception 클래스의 파생 클래스로
//       정의하고 사용하였다.
/*
#include <iostream.h>
#include <exception>  //exception 헤더파일 포함


class ESize : public exception
{
	int index;
public:
	ESize(int i):index(i){}
	const char* what() {return "[Out of Range]" ;}
	//가상 함수 what()을 재정의 하여 문자열을 지정하였다.
};


class Array
{
	int size;
	int* arr;
public:


	Array(){arr=NULL; size=0;};
	Array(int n);
	~Array(){ delete[] arr; }
	int getSize() { return size; }
	int summary();

	int& operator[](int i);
	const int& operator[](int i) const;
};
Array::Array(int n):size(n)
{
    arr = new int[size];
    for(int i=0; i<size; i++)  arr[i]=0;
}

int Array::summary()
{
    int sum=0;
    for (int i=0; i<size; i++)
        sum += arr[i];
    return sum;
}

int& Array::operator[](int i)
{
    if (i < 0 || i >= size)  throw ESize(i); 
    return arr[i];
}

const int& Array::operator[](int i) const
{
    if (i < 0 || i >= size)  throw ESize(i);
        return arr[i];
}

int main(void)
{
    try
	{
        Array mydata(3);

        for (int i=0; i<3; i++)
            mydata[i] = i*10;

        for (i=0; i<4; i++)
            cout << mydata[i] << endl;
    }

    catch(ESize &err)
    {	
        cout << "## Exception ##\t" << err.what() << endl;
    }

    return 0;
}
*/





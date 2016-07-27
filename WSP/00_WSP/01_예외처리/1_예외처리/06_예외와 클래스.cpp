/**************************************************************

	[ 예외와 클래스 ]


	다른 클래스의 멤버로 내포되는 예외 클래스 정의
	어떤 함수들이 예외를 발생시킨다면, 발생되는 예외의 타입으로 
	사용할 클래스를 public 멤버로 정의하는 것이 좋다.
 

**************************************************************/

//[예제] 배열(Array) 클래스의 멤버로 내포되어 배열 첨자 범위에 어긋날 때 
//		사용되는 예외 클래스를 정의한 프로그램

#include <iostream.h>

class Array{
	int size;    
	int* arr;
public:

	class ESize
	{
		int index;
	public:
		ESize(int i):index(i){}
		void print() const;
	};

	Array(){arr=NULL; size=0;};
	Array(int n);
	~Array(){ delete[] arr; }
	int getSize() { return size; }
	int summary();

	int& operator[](int i);
	const int& operator[](int i) const;
};

void Array::ESize::print() const
{
    cout << "Out of Range : " << index << endl;
}

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
    if (i < 0 || i >= size)  throw ESize(i); //ESize 예외 발생 
    return arr[i];
}

const int& Array::operator[](int i) const
{
    if (i < 0 || i >= size)  throw ESize(i); //ESize 예외 발생 
    return arr[i];
}

int main(void)
{
    try
	{
		Array mydata(3); //배열 사이즈가 3으로 설정됨

		for (int i=0; i<3; i++)  mydata[i] = i*10;

		for (i=0; i<=3; i++) //i=3이면 범위를 초과하게 된다.
			cout << mydata[i] << endl;
    }

    catch(Array::ESize &err) //ESize 예외 처리 
    {
		cout << "## Exception ##\t" ;
		err.print();
    }

	cout << "계속 실행" << endl;

    return 0;
}

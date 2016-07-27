/**************************************************************

	[ 예외의 상속 ]


	예외 클래스의 상속

	어떤 클래스가 public 멤버로 예외 클래스를 가지고 있다면, 
	그것을 기본으로 하여 파생되는 클래스들은 그 예외 클래스들을 
	상속한다.

	기존에 정의되어 있는 예외 클래스들로부터 새로운 예외 클래스를 
	파생시킬 수 있다.

	동일한 범주의 예외 상황들을 클래스 계층 구조로 표현함으로써 
	관리가 용이해진다.


	class MathErr {......};  
	class Overflow : public MathErr {......};
	class Underflow : public MathErr {......};
	class ZeroDivide : public MathErr {......};



   [ catch블록 구성시 주의사항 ]

   catch 블록은 선언된 순서대로 예외를 검사한다.

   catch 블록은 가장 구체화된 예외 클래스에 대한 처리가 먼저 나오고, 
   가장 일반화된 예외 클래스에 대한 처리가 나중에 오도록 구성해야 한다. 
   만약 일반화된 예외 클래스가 먼저 온다면 구체화된 예외 처리 구문은 결코 
   수행되지 않는다.


   catch블록을 다음처럼 구성하면 ZeroDivide에 대한 catch블록은 
   결코 실행되지 않는다.

	try { ...... }
	catch (Overflow) { ...... }
	catch (MathErr) { ...... }
	catch (ZeroDivide) { ...... }


**************************************************************/
//[예제] 배열 첨자 범위에 어긋날 때 사용되는 ESize 예외 클래스를 
//       전역으로 정의하고, 그것으로
//      부터 파생시킨 ChildESize를 정의한 후 에러를 발생시켰다. 


#include <iostream.h>

class ESize
{
protected:
	int index;
public:
	ESize(int i):index(i){}
	virtual void print() const;
};

void ESize::print() const
{
    cout << "Out of Range : " << index << endl;
}
//--------------------------------------------------------


class ChildESize : public ESize
{
    	int lbound;
    	int ubound;
public:
    	ChildESize(int i,int lidx, int uidx) : 
                        ESize(i), lbound(lidx), ubound(uidx) {};
	void print() const;
};

void ChildESize::print() const
{
    cout << "Out of Range [" << lbound << " to " << ubound << "]" ;
    cout << ", Current Index : " << index << endl;
}
//--------------------------------------------------------


class Array{
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
    if (i < 0 || i >= size)  throw ChildESize(i,0,size);
    return arr[i];
}

const int& Array::operator[](int i) const
{
    if (i < 0 || i >= size)  throw ChildESize(i,0,size);
    return arr[i];
}
//------------------------------------------------------


int main(void)
{
    Array mydata(3);

    try
	{
        for (int i=0; i<=3; i++) //예외발생을 위해 일부러 (i <= 3)으로 비교함
			mydata[i] = i*10;
	}
    catch(ESize &err)
    {
        cout << "## Exception ##\t" ;
        err.print();
    }
    try
	{
        for (int i=0; i<=3; i++) //예외발생을 위해 일부러 (i <= 3)으로 비교함
            cout << mydata[i] << endl;
    }
    catch(ChildESize &err)
    {
        cout << "## Exception ##\t" ;
        err.print();
    }
    return 0;
}

//[예제] MathErr, Overflow, Underflow, ZeroDivide 간의 상속관계를 
//        나타내어 산술 연산과 관련된 에러를 처리하였다.


#include <iostream.h>

const int MAXINT = 32767;
const int MININT = -32768;


class MathErr
{
	int first, second;
public:
	MathErr(int f=0, int s=0):first(f), second(s) {}
	int getfirst() { return first; }
	int getsecond() { return second; }
	virtual void print() { cout << "This is Math Error " << endl; }
};

class Overflow : public MathErr
{
public:
	Overflow(int a, int b) : MathErr(a,b) {}
	void print() { cout << "This is Overflow Error" << endl; }
};

class Underflow : public MathErr
{
public:
	Underflow(int a, int b):MathErr(a,b) {}
	void print() {cout << "This is Underflow Error" << endl; }
};

class ZeroDivide : public MathErr
{
public:
	ZeroDivide(int a, int b) : MathErr(a,b) {}
	void print() {cout << "This is Zero Division Error" << endl; }
};

class MathClass
{
public:
	int add(int, int);
	int subtract(int, int);
	int multiply(int, int);
	int divide(int, int);
};

int MathClass::add(int a, int b)
{
	int tmp = a + b;
	if (tmp > MAXINT) throw Overflow(a,b);
	return tmp;
}

int MathClass::subtract(int a, int b)
{
    int tmp = a - b;
    if (tmp < MININT) throw Underflow(a,b);
        return tmp;
}

int MathClass::multiply(int a, int b)
{
    int tmp = a * b;
    if (tmp > MAXINT) throw Overflow(a,b);
        return tmp;
}
 
int MathClass::divide(int a, int b)
{
    if (b==0) throw ZeroDivide(a,b);
    return (int)(a/b);
}

int main(void)
{
    MathClass mc;
    cout << "Maxint = " << MAXINT << endl;
    cout << "Minint = " << MININT << endl;

    try{
        cout << "Add   : " << mc.add(10000,20000) << endl;
        cout << "Subract : " << mc.subtract(10000,20000) << endl;
        cout << "Multiply : " << mc.multiply(10, 100) << endl;
        cout <<"Divide : " << mc.divide(10000,0) << endl;
     }

    catch (Overflow o)
    {cout << "Overflow : " << o.getfirst() << " + " << o.getsecond() << endl;}
	
    catch (Underflow u)
    {cout << "Under flow : " << u.getfirst() << " - " << u.getsecond() << endl;}

    catch (ZeroDivide z)
    {cout << "ZeroDivide : " << z.getfirst() << " + " << z.getsecond() << endl;}

    return 0;
}


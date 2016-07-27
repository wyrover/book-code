
//[예제] Vector 클래스에 예외를 처리할 멤버 함수를 두는 예제 프로그램

#include <iostream.h>

class Vector
{
    int* p;  	int sz;
public:
    enum {MAX = 100};

    class ERange //exception class
    {
    public:
		int index;
		ERange(int i):index(i) {}
    };

    class ESize //exception class
    {
    public:
		int size;
		ESize(int s):size(s) {}
    };

    Vector(int s);
    ~Vector() {delete[] p;}
    int& operator [] (int i);
};
inline Vector::Vector(int s)
{   if (s < 0 || s > MAX) throw ESize(s);
    p = new int [sz = s];
}
inline int& Vector::operator[](int i)
{
    if (i >= 0 && i < sz) return p[i];
    throw ERange(i);
}
void use_vector()
{
    try {  Vector vec(10000);  }
    catch(Vector::ESize s) {throw Vector::ESize(s.size);}
}

int main(void)
{
    Vector vec(10);
    try{
        use_vector();
        cout << vec[11] << endl;
    }
    catch(Vector::ESize s)
    {
        cout << "Size Error : " << s.size << endl;
        try { cout << vec[11] << endl; }
        catch (Vector::ERange r){cout << "Range Error : " << r.index << endl;}
    }
    catch (Vector::ERange r) { cout << "Range Error : " << r.index << endl; }
    catch (...) { cout << "Another Error" << endl; }
    return 0;
}
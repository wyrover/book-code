
//[예제] Vector 클래스 템플릿의 멤버로 예외 클래스가 들어간 
//       예제 프로그램

#include <iostream.h>

template <typename T> class Vector 
{
    T* p;    int sz;
public:
    enum { max = 100 };
    class ERange   // exception class
    {
    public:
	int index;
	ERange(int i) : index(i) {}
    };
    class ESize    // exception class
    {
    public:
	int size;
	ESize(int s) : size(s) {}
    };
    Vector(int s);      ~Vector() { delete[] p; }
    T& operator[](int i);
};


template <typename T> 
inline Vector<T>::Vector(int s)
{
    if ( s < 0 || s > max )    throw ESize(s);
    p = new T[sz =s];
};

template <typename T> 
inline T& Vector<T>::operator[](int i)
{
    if ( 0 <= i && i < sz )    return p[i];
    throw ERange(i);	
};

int main(void)
{
    try
    {
		Vector<char> cvec(10);
		Vector<int> ivec(1000);
		cout << cvec[11] << endl;
    }
    catch (Vector<int>::ESize s)
    { cout << "Size Error : " << s.size << endl; }
    catch (Vector<int>::ERange r)
    { cout << "Range Error : " << r.index << endl; }
    catch (Vector<char>::ESize s)
    { cout << "Size Error : " << s.size << endl; }
    catch (Vector<char>::ERange r)
    { cout << "Range Error : " << r.index << endl; }
    catch (...)
    { cout << "Another Error" << endl; }
    return 0;
}

#include <iostream.h>
class B1
{
public:
	void output();
};
class B2
{
public:
	void output();
};
void B1::output()
{
	cout<<"call the class B1"<<endl;
}
void B2::output()
{
	cout<<"call the class B2"<<endl;
}
class A:public B1,public B2
{
public:
	void show();
};
void A::show()
{
	cout<<"call the class A"<<endl;
}
void main()
{
	A a;
	a.output();         //该语句编译时会报错
	a.show();
}
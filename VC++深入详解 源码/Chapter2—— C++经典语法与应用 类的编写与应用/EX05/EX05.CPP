#include <iostream.h>
class animal
{
public:
	void eat()
	{
		cout<<"animal eat"<<endl;
	}
	void sleep()
	{
		cout<<"animal sleep"<<endl;
	}
	void breathe()
	{
		cout<<"animal breathe"<<endl;
	}
};
class fish:public animal
{
};
void main()
{
	animal an;
	fish fh;
	an.breathe();
	fh.breathe();
}
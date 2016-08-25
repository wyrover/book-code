#include <iostream.h>
class animal
{
public:
	animal(int height, int weight)
	{
		cout<<"animal construct"<<endl;
	}
	~animal()
	{
		cout<<"animal destruct"<<endl;
	}
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
public:
	fish():animal(400,300)
	{
		cout<<"fish construct"<<endl;
	}
	~fish()
	{
		cout<<"fish destruct"<<endl;
	}	
};
void main()
{
	fish fh;
}
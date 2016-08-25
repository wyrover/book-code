#include <iostream.h>
class point
{
public:
	int x;
	int y;
	point()
	{
		x=0;
		y=0;
	}
	point(int a,int b)
	{
		x=a;
		y=b;
	}
	void output()
	{
		cout<<x<<endl<<y<<endl;
	}
	void input(int x,int y)
	{
		//x=x;
		//y=y;
		this->x=x;
		this->y=y;
	}
};


void main()
{
	point pt(5,5);
	pt.input(10,10);
	pt.output();
}
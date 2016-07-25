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
};


void main()
{
	point pt(5,5);
	pt.output();
}
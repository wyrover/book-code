#include <iostream.h>
class Point
{
public:
	int x;
	int y;
/*	void init()
	{
		x=0;
		y=0;
	}*/
	Point()
	{
		x=0;
		y=0;
	}
	Point(int a,int b)
	{
		x=a;
		y=b;
	}
	~Point()
	{
	}
	void output()
	{
		cout<<x<<endl<<y<<endl;
	}
	void output(int x,int y)
	{
		this->x=x;
		this->y=y;
	}
};

void main()
{
	Point pt(3,3);
	pt.output(5,5);
//	pt.init();
	//pt.x=5;
	//pt.y=5;
//	cout<<pt.x<<endl<<pt.y<<endl;
	pt.output();
}
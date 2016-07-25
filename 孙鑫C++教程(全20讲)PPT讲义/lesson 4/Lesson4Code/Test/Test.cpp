#include <iostream.h>

class Point
{
public:
	void output()
	{
		
		init();
	}
	static void init()
	{
		x=0;
		y=0;
	}
private:
	static int x,y;
};

//int Point::x=0;
//int Point::y=0;
void main()
{
/*	Point pt;
	pt.init();
	pt.output();*/

//	Point::init();
	//Point::output();

}
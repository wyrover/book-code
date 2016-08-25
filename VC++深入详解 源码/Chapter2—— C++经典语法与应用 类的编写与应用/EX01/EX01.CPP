#include <iostream.h>
struct point
{
	int x;
	int y;
	void output()
	{
		cout<<x<<endl<<y<<endl;
	}
};

void main()
{
	point pt;
	pt.x=0;
	pt.y=0;
//	cout<<pt.x<<endl<<pt.y<<endl;
	pt.output();
}
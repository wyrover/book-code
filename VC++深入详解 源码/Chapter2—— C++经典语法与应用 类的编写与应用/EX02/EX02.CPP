#include <iostream.h>
class point
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
	pt.output();
}
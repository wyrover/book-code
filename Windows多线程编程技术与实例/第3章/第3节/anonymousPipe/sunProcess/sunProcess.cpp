#include "stdio.h"
#include "iostream.h"
#include "windows.h"

void main()
{
	int i;
	Sleep(800);
	for(i=0;i<10;i++)
	{
		printf("Logging %d\n",i);
		cout<<"Output Line :"<<i<<endl;
		cerr<<"error line:"<<i<<endl;
		Sleep(100);
	}
}
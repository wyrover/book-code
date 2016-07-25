#include "Fish.h"
#include <iostream.h>
Fish::Fish():Animal(300,400)
{
}

void Fish::breathe()
{
	cout<<"fish bubble"<<endl;
}
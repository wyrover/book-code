#include <iostream.h>



#include "Animal.h"
#include "Fish.h"

void fn(Animal *pAn)
{
	pAn->breathe();
}


void main()
{
	//Animal an;
	//an.eat();

//	an.breathe();
	Fish fh;
	Animal *pAn;
	pAn=&fh;
	fn(pAn);

}

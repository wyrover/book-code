#include "animal.h"
#include "fish.h"
void fn(animal *pAn)
{
	pAn->breathe();
}
void main()
{
	animal *pAn;
	fish fh;
	pAn=&fh;
	fn(pAn);
}

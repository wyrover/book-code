#include <iostream.h>
class animal
{
public:
	 void eat()
	 {
		  cout<<"animal eat"<<endl;
	 }
	 void sleep()
	 {
		  cout<<"animal sleep"<<endl;
	 }
	 virtual void breathe()
	 {
		  cout<<"animal breathe"<<endl;
	 }
};
class fish:public animal
{
public:
	 void breathe()
	 {
		  cout<<"fish bubble"<<endl;
	 }
};

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
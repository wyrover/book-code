// Person.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>

class Person{
	char * ID;
public:
	Person(char * aID);
	~Person();
	char * getID(){ return ID;}
};

Person::Person(char * aID){
	ID = aID;	
};


Person * createPerson1()
{
	Person * p = new Person("123");
	return p;
}

Person * createPerson2()
{
	char Id[4] = "123";
	return new Person(Id);	
}

void main()
{
	Person *p2 = createPerson2();
	Person *p1 = createPerson1();
	printf("p2->Id=%s p1->Id=%s\n", p2->getID(), p1->getID());	
}


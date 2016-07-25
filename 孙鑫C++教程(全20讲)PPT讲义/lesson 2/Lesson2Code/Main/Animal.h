#ifndef ANIMAL_H_H
#define ANIMAL_H_H
class Animal
{
public:
	Animal(int height,int weight);


	void eat();


	void sleep();
	
	virtual void breathe();//=0;
};

#endif
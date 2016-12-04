#ifndef Common_XXXXX_H
#define Common_XXXXX_H

class Parent
{
public:
	virtual void setNum(int v) = 0;
	virtual int getNum() = 0;
};

Parent* createObj();

#endif
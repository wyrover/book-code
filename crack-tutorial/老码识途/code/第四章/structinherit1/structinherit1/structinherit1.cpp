// structinherit1.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>

class Parent;
class Child;

typedef void(*PtrParentIntSetter)(struct Parent *self, int v);
typedef void(*PtrChildIntSetter)(struct Child *self, int v);

struct Parent{
	int a;
	int b;	
	PtrParentIntSetter setB;
};

void setB(struct Parent *self, int v){
	self->b = v;
}

void initParent(struct Parent * obj)
{
	obj->setB =setB;
}


struct Child{
	struct Parent super;
	int c;
	PtrChildIntSetter setC;
};



void setC(struct Child * self, int v){
	self->c = v;
}

void initChild(struct Child * obj)
{
	initParent((struct Parent *)obj);
	obj->setC = setC;
}

void main()
{
	struct Parent p;
	struct Child c;
	struct Parent *pp;
	initParent(&p);
	initChild(&c);
	p.setB(&p, 1);
	pp = (Parent *)(&c);
	pp->setB(pp, 2);
	c.setC(&c, 3);
	printf("p.b=%d, c.b=%d c.c=%d\n", p.b, c.super.b, c.c);
}
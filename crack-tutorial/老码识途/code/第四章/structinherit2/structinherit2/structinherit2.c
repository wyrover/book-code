// structinherit2.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>
#include "utils.h"
CLASS Parent;
CLASS Child;

typedef void(*PtrParentIntSetter)(struct Parent *self, int v);
typedef void(*PtrChildIntSetter)(struct Child *self, int v);

#define SUPERCLASS_Parent Object

CLASS Parent EXTENDS(Object)  
  int a;
  int b;
  PtrParentIntSetter setB;
};

CLASS_NEW_FUNC_DEFINE(Parent);

void Parent_dtor(void * self)
{
	//do the staff of this class, let the SUPERCLASS_DTOR call at the last
	SUPERCLASS_DTOR(self, Object);
}

void Parent_setB(struct Parent *self, int v){
	self->b = v;
}


BOOL Parent_init(struct Parent * self)
{
	SUPERCLASS_INIT(self, Object, Parent);
	((struct Object *)self)->dtor = Parent_dtor;
	self->setB = Parent_setB;
	return TRUE;
}


CLASS Child EXTENDS(Parent)  
  int c;
  PtrChildIntSetter setC;
CLASS_END;

CLASS_NEW_FUNC_DEFINE(Child);

void Child_dtor(void * self)
{
	//do the staff of this class, let the SUPERCLASS_DTOR call at the last
	SUPERCLASS_DTOR(self, Parent);
}

void Child_setC(struct Child *self, int v){
	self->c = v;
}


BOOL Child_init(struct Child * self)
{
	SUPERCLASS_INIT(self, Parent, Child);
	self->setC = Child_setC;
	return TRUE;
}



void main()
{
	CLASS Child * pc;
	CLASS Parent * pp;
	CLASS Parent * p2;
	pc = NULL;
	pc = Child_new();
	pp = Parent_new();
	pp->setB(pp, 1);
	p2 = (CLASS Parent *) pc;
	p2->setB(p2, 2);
	pc->setC(pc, 3);
	printf("pp->b=%d, pc->b=%d pc->c=%d\n", pp->b, pc->super.b, pc->c);
	OBJ_DELETE(pc);
	OBJ_DELETE(pp);
}


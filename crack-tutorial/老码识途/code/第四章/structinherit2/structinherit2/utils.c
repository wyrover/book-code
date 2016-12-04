#include "utils.h"

void Object_dtor(void * self)
{
	//FREE(self);
}

BOOL Object_init(struct Object * self)
{
	self->dtor = Object_dtor;
	return TRUE;
}
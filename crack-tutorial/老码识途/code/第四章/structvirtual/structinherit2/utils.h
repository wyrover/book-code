#ifndef UTILS_HDR_XXX
#define UTILS_HDR_XXX

#include <memory.h>
#include <malloc.h>

#define BOOL int
#define TRUE 1
#define FALSE 0



typedef void (*class_dtor_func)(void * self);

struct Object
{
	class_dtor_func dtor;
};

void Object_dtor(void * self);

BOOL Object_init(struct Object * self);


#define CLASS struct

#define EXTENDS(super_class) { \
struct super_class super;

#define CLASS_END };
	

#define CLASS_INSTANCE(class_name) (struct class_name *)malloc(sizeof(struct class_name))

#define CLASS_SET_ZERO(self, class_name) memset(self, 0, sizeof(struct class_name))

#define CLASS_NEW(class_name)  \
{\
struct class_name * ret = CLASS_INSTANCE(class_name); \
	if (ret == NULL) \
{   \
	return NULL; \
}   \
	if (!class_name##_init(ret)) return NULL;\
	return ret;\
}

#define CLASS_NEW_FUNC_DEFINE(class_name) struct class_name *  class_name##_new(){ \
	CLASS_NEW(class_name){}\
};

#define CLASS_NEW_1ARG(class_name, arg)  \
{\
struct class_name * ret = CLASS_INSTANCE(class_name); \
	if (ret == NULL) \
	{   \
	return NULL; \
	}   \
	if (!class_name##_init(ret, arg)) return NULL;\
	return ret;\
 }


#define CLASS_NEW_2ARGS(class_name, arg1, arg2)  \
{\
struct class_name * ret = CLASS_INSTANCE(class_name); \
	if (ret == NULL) \
	{   \
	return NULL; \
	}   \
	if (!class_name##_init(ret, arg1, arg2)) return NULL;\
	return ret;\
 }

#define SUPERCLASS(class_name) SUPERCLASS_##class_name

#define SUPERCLASS_PTR(class_name) struct SUPERCLASS(class_name) *

#define  SUPERCLASS_INIT(self, super_class, self_class) \
	BOOL r_xxxx;\
	r_xxxx = super_class##_init((struct super_class *)self); \
	if (!r_xxxx) return FALSE;\
	memset((char *)(self) + sizeof(self->super), 0, sizeof(struct self_class) - sizeof(self->super));\
	((struct Object *)self)->dtor = self_class##_dtor;

#define  SUPERCLASS_INIT_1ARG(self, super_class, self_class, arg) \
	BOOL r_xxxx;\
	r_xxxx = super_class##_init((struct super_class *)self, arg); \
	if (!r_xxxx) return FALSE;\
	memset((char *)(self) + sizeof(self->super), 0, sizeof(struct self_class) - sizeof(self->super));\
	((struct Object *)self)->dtor = self_class##_dtor;

#define  SUPERCLASS_INIT_2ARGS(self, super_class, self_class, arg1, arg2) \
	BOOL r_xxxx;\
	r_xxxx = super_class##_init((struct super_class *)self, arg1, arg2); \
	if (!r_xxxx) return FALSE;\
	memset((char *)(self) + sizeof(self->super), 0, sizeof(struct self_class) - sizeof(self->super));\
	((struct Object *)self)->dtor = self_class##_dtor;



#define SUPERCLASS_DTOR(self, super_class) \
	super_class##_dtor((struct super_class *)self); 

#define OBJ_DELETE(self){ \
	if (self != NULL) \
	{\
	  ((struct Object *)self)->dtor(self);\
	  free(self);\
	}\
   }

#define DEFINE_THIS(class_cast) struct class_cast * _this = (struct class_cast *)self

#define SUPER_FUNC(self, super_class, func_name_without_class_prex){\
	super_class##_##func_name_without_class_prex((struct super_class *)self);\
}

#define SUPER_FUNC_1ARG(self, super_class, func_name_without_class_prex, arg){\
	super_class##_##func_name_without_class_prex((struct super_class *)self, arg);\
}

#define SUPER_FUNC_2ARGS(self, super_class, func_name_without_class_prex, arg1, arg2){\
	super_class##_##func_name_without_class_prex((struct super_class *)self, arg1, arg2);\
}
	
#endif

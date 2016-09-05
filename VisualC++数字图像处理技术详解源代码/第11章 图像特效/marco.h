// 此文件定义了一些全局的宏

#define SAFE_DELETE(p)	\
{						\
	if ((p) != NULL)	\
	{					\
		delete (p);		\
		(p) = NULL;		\
	}					\
}

#define SAFE_DELETE_ARRAY(p)	\
{								\
	if ((p) != NULL)			\
	{							\
		delete[] (p);			\
		(p) = NULL;				\
	}							\
}
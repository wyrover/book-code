#define FUN(a, b) \
   do {           \
                  \
   } while(0)


#define offset(TYPE, MEMEBER) ((size_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({ \
       const typeof(((type*)0)->member) *__mptr = ptr; \
       (type*)((char *)__mptr - offset(type, member)); })

#ifndef OHTBL_H
#define OHTBL_H
#include<stdlib.h>

typedef struct OHTbl_ {
    int positions;
    void *vacated;
    //防止出现下面这种情况：
    //1，哈系表中有   1，2，3，4，5，6，7
    //2，现在删除表中3，4。如果以NULL表示删除了元素。则
    //表为1，2，NULL，NULL，5，6，7。
    //3，查找哈系表中元素5，lookup找到了NULL，返回找不到元素5。
    //4，现在插入5，得到哈希表为1，2，5，NULL，5，6，7。
    //5，哈希表出错，重复了元素5。

    int (*h1)(const void *key);
    int (*h2)(const void *key);
    int (*match)(const void *key1, const void *key2);
    void (*destroy)(void *data);

    int size;
    void **table;
} OHTbl;

int ohtbl_init(OHTbl *htbl, int positions, int (*h1)(const void *key), int (*h2)(const void *key), int (*match)(const void *key1, const void *key2), void (*destroy)(void *data));
void ohtbl_destroy(OHTbl *htbl);
int ohtbl_insert(OHTbl *htbl, const void *data);
int ohtbl_remove(OHTbl *htbl, void **data);
int ohtbl_lookup(const OHTbl *htbl, void **data);

#define ohtbl_size(htbl) ((htbl)->size)
#endif


#ifndef HEAP_H
#define HEAP_H

typedef struct Heap_ {
    int size;

    int (*compare)(const void *key1, const void *key2);
    void (*destroy)(void *data);

    void **tree;
} Heap;

void heap_init(Heap *heap, int (*compare)(const void *key1, const void *key2), void (*destory)(void *data));
void heap_destroy(Heap *heap);
int heap_insert(Heap *heap, const void *data);
int heap_extract(Heap *heap, void **data);

#define heap_size(heap) ((heap)->size)
#endif

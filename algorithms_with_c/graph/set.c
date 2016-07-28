#include<stdlib.h>
#include<string.h>
#include"list.h"
#include"set.h"

void set_init(Set *set, int (*match)(const void *key1, const void *key2), void (*destroy)(void *data))
{
    list_init(set, destroy);
    set->match = match;
    return;
}
int set_insert(Set *set, const void *data)
{
    if (set_is_member(set, data))
        return 1;

    return list_ins_next(set, list_tail(set), data);
}
int set_remove(Set *set, void **data)
{
    ListElmt *member, *prev;
    prev = NULL;

    for (member = list_head(set); member != NULL; member = list_next(member)) {
        if (set->match(*data, list_data(member)))
            break;

        prev = member;
    }

    if (member == NULL)
        return -1;

    return list_rem_next(set, prev, data);
}
int set_union(Set *setu, const Set *set1, const Set *set2)
{
    ListElmt *member;
    void *data;
    //set_init(setu,set1->match,set1->destroy);
    set_init(setu, set1->match, NULL); //此处不能传set1->destroy因为之后会调用list_destroy函数中list->destroy(data);将会销毁set1，set2中的data数据，程序后面全错。NULL传过去，list_destroy函数中if判断总是0，导致list_destroy函数只调用list_rem_next。

    for (member = list_head(set1); member != NULL; member = list_next(member)) {
        data = list_data(member);

        if (list_ins_next(setu, list_tail(setu), data) != 0) {
            set_destroy(setu);
            return -1;
        }
    }

    for (member = list_head(set2); member != NULL; member = list_next(member)) {
        if (set_is_member(set1, list_data(member))) {
            continue;
        } else {
            data = list_data(member);

            if (list_ins_next(setu, list_tail(setu), data) != 0) {
                set_destroy(setu);
                return -1;
            }
        }
    }

    return 0;
}

int set_intersection(Set *seti, const Set *set1, const Set *set2)
{
    ListElmt *member;
    void *data;
    //set_init(seti,set1->match,set1->destroy);
    set_init(seti, set1->match, NULL); //****same to line 37.

    for (member = list_head(set1); member != NULL; member = list_next(member)) {
        if (set_is_member(set2, list_data(member))) {
            data = list_data(member);

            if (list_ins_next(seti, list_tail(seti), data) != 0) {
                set_destroy(seti);
                return -1;
            }
        }
    }

    return 0;
}

int set_difference(Set *setd, const Set *set1, const Set *set2)
{
    ListElmt *member;
    void *data;
    set_init(setd, set1->match, NULL); //****same to line 37.
    //set_init(setd,set1->match,set1->destroy);

    for (member = list_head(set1); member != NULL; member = list_next(member)) {
        if (!set_is_member(set2, list_data(member))) {
            data = list_data(member);

            if (list_ins_next(setd, list_tail(setd), data) != 0) {
                set_destroy(setd);
                return -1;
            }
        }
    }

    return 0;
}

int set_is_member(const Set *set, const void *data)
{
    ListElmt *member;

    for (member = list_head(set); member != NULL; member = list_next(member)) {
        if (set->match(data, list_data(member)))
            return 1;
    }

    return 0;
}

int set_is_subset(const Set *set1, const Set *set2)
{
    ListElmt *member;

    if (set_size(set1) > set_size(set2))
        return 0;

    for (member = list_head(set1); member != NULL; member = list_next(member)) {
        if (!set_is_member(set2, list_data(member)))
            return 0;
    }

    return 1;
}

int set_is_equal(const Set *set1, const Set *set2)
{
    if (set_size(set1) != set_size(set2))
        return 0;

    return set_is_subset(set1, set2);
}

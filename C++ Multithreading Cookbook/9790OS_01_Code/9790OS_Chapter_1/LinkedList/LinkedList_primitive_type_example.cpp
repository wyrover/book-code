#include <windows.h>
#include "CList.h"

DWORD Predicate(int* iElement)
{
    return (DWORD)(*iElement);
}

int main()
{
    CList<int>* list = new CList<int>();
    list->Insert(new int(1));
    list->Insert(new int(2));
    list->Insert(new int(3));
    int* iElement = list->Find(Predicate, 2);

    if (iElement != NULL) {
        // iElement FOUND
    }

    return 0;
}

#include <windows.h>
#include "CList.h"

class CStudent
{
public:
    CStudent(DWORD dwStudentId) : dwStudentId(dwStudentId) { }
    static DWORD GetStudentId(CStudent* student)
    {
        DWORD dwValue = student->GetId();
        return dwValue;
    }
    DWORD GetId() const
    {
        return dwStudentId;
    }
private:
    DWORD dwStudentId;
};

int __main()
{
    CList<CStudent>* list = new CList<CStudent>();
    list->Insert(new CStudent(1));
    list->Insert(new CStudent(2));
    list->Insert(new CStudent(3));
    CStudent* s = list->Find(&CStudent::GetStudentId, 2);

    if (s != NULL) {
        // s FOUND
    }

    return 0;
}

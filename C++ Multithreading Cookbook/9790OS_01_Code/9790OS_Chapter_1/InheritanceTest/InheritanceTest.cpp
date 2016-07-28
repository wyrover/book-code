#include "stdafx.h"
#include <iostream>

using namespace std;

class CPerson
{
public:
    CPerson(int iAge, char* sName)
    {
        this->iAge = iAge;
        strcpy_s(this->sName, 32, sName);
    }
    virtual char* WhoAmI()
    {
        return "I am a person";
    }
private:
    int iAge;
    char sName[32];
};

class CWorker : public CPerson
{
public:
    CWorker(int iAge, char* sName, char* sEmploymentStatus)
        : CPerson(iAge, sName)
    {
        strcpy_s(this->sEmploymentStatus, 32, sEmploymentStatus);
    }
    virtual char* WhoAmI()
    {
        return "I am a worker";
    }
private:
    char sEmploymentStatus[32];
};

class CStudent : public CPerson
{
public:
    CStudent(int iAge, char* sName, char* sStudentIdentityCard)
        : CPerson(iAge, sName)
    {
        strcpy_s(this->sStudentIdentityCard, 32, sStudentIdentityCard);
    }
    virtual char* WhoAmI()
    {
        return "I am a student";
    }
private:
    char sStudentIdentityCard[32];
};

int _tmain(int argc, TCHAR* argv[])
{
    CPerson cPerson(10, "John");
    cout << cPerson.WhoAmI() << endl;
    CWorker cWorker(35, "Mary", "On wacation");
    cout << cWorker.WhoAmI() << endl;
    CStudent cStudent(22, "Sandra", "Phisician");
    cout << cStudent.WhoAmI() << endl;
    return 0;
}

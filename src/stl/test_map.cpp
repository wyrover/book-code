// Program: Map Own Class
// Purpose: To demonstrate a map of classes

#include <string>
#include <iostream>
#include <vector>
#include <map>
using namespace std;

class CStudent
{
public :
    int nStudentID;
    int nAge;
public :
    // Default Constructor - Empty
    CStudent()  {  }
    // Full constructor
    CStudent(int nSID, int nA)
    {
        nStudentID = nSID;
        nAge = nA;
    }
    // Copy constructor
    CStudent(const CStudent& ob)
    {
        nStudentID = ob.nStudentID;
        nAge = ob.nAge;
    }
    // Overload =
    void operator = (const CStudent& ob)
    {
        nStudentID = ob.nStudentID;
        nAge = ob.nAge;
    }
};

int main(int argc, char* argv[])
{
    map <string, CStudent> mapStudent;
    mapStudent["Joe Lennon"] = CStudent(103547, 22);
    mapStudent["Phil McCartney"] = CStudent(100723, 22);
    mapStudent["Raoul Starr"] = CStudent(107350, 24);
    mapStudent["Gordon Hamilton"] = CStudent(102330, 22);
    // Access via the name
    cout << "The Student number for Joe Lennon is " <<
         (mapStudent["Joe Lennon"].nStudentID) << endl;
    return 0;
}
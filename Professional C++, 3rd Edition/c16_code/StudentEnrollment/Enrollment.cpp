#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

// courseStudents is a vector of lists, one for each course. The lists
// contain the students enrolled in those courses. They are not sorted.
//
// droppedStudents is a list of students who failed to pay their
// tuition and so were dropped from their courses.
//
// The function returns a list of every enrolled (non-dropped) student in
// all the courses.
list<string> getTotalEnrollment(const vector<list<string>>& courseStudents,
                                const list<string>& droppedStudents)
{
    list<string> allStudents;

    // Concatenate all the course lists onto the master list
    for (auto& lst : courseStudents) {
        allStudents.insert(cend(allStudents), cbegin(lst), cend(lst));
    }

    // If your compiler does not support the above C++11 range-based for
    // loop, you need the following.
    //for (vector<list<string> >::const_iterator it = courseStudents.begin();
    //  it != courseStudents.end(); ++it) {
    //      allStudents.insert(allStudents.end(), (*it).begin(), (*it).end());
    //}
    // Sort the master list
    allStudents.sort();
    // Remove duplicate student names (those who are in multiple courses).
    allStudents.unique();

    // Remove students who are on the dropped list.
    // Iterate through the drop list, calling remove on the
    // master list for each student in the dropped list.
    for (auto& str : droppedStudents) {
        allStudents.remove(str);
    }

    // If your compiler does not support the above C++11 range-based for
    // loop, you need the following.
    //for (list<string>::const_iterator it = droppedStudents.begin();
    //  it != droppedStudents.end(); ++it) {
    //      allStudents.remove(*it);
    //}
    // done!
    return allStudents;
}

void readStudentList(list<string>& students, ifstream& istr)
{
    string name;

    while (getline(istr, name)) {
        cout << "Read name " << name << endl;
        students.push_back(name);
    }
}

void readCourseLists(vector<list<string>>& lists)
{
    for (int i = 1;; i++) {
        ostringstream ostr;
        ostr << "course" << i << ".txt";
        ifstream istr(ostr.str().c_str());

        if (!istr) {
            cout << "Failed to open " << ostr.str() << endl;
            break;
        }

        list<string> newList;
        readStudentList(newList, istr);
        lists.push_back(newList);
    }
}

void readDroppedStudents(list<string>& students)
{
    ifstream istr("dropped.txt");
    readStudentList(students, istr);
}

int main()
{
    vector<list<string>> courseStudents;
    list<string> droppedStudents;
    readCourseLists(courseStudents);
    readDroppedStudents(droppedStudents);
    list<string> finalList = getTotalEnrollment(courseStudents, droppedStudents);

    for (const auto& name : finalList) {
        cout << name << endl;
    }

    return 0;
}

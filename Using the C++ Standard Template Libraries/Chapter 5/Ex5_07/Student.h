// Student.h
// Student class definition for Ex5_01
#ifndef STUDENT_H
#define STUDENT_H
#include <string>                                // For string class
#include <ostream>                               // For output streams

class Student
{
private:
    std::string first {};
    std::string second {};

public:
    Student(const std::string& name1, const std::string& name2) : first(name1), second(name2) {}
    Student(Student&& student) : first(std::move(student.first)), second(std::move(student.second)) {} // Move constructor
    Student(const Student& student) : first(student.first), second(student.second) {}                 // Copy constructor
    Student() {}                                                                                      // Default constructor

    // Less-than operator
    bool operator<(const Student& student) const
    {
        return second < student.second || (second == student.second && first < student.first);
    }

    friend std::ostream& operator<<(std::ostream& out, const Student& student);
};

// Insertion operator overload
inline std::ostream& operator<<(std::ostream& out, const Student& student)
{
    out << student.first + " " + student.second;
    return out;
}
#endif
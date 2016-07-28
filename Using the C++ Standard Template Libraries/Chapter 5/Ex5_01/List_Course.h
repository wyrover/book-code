// List_Courses.h
// Function object to output the students in a group for Ex5_01
#ifndef LIST_COURSE_H
#define LIST_COURSE_H
#include <iostream>                              // For standard streams
#include <string>                                // For string class
#include <set>                                   // For set container
#include <algorithm>                             // For copy()
#include <iterator>                              // For ostream_iterator
#include "Student.h"

using Subject = std::string;                     // A course subject
using Group = std::set<Student>;                 // A student group for a subject
using Course = std::pair<Subject, Group>;        // A pair representing a course

class List_Course
{
public:
    void operator()(const Course& course)
    {
        std::cout << "\n\n" << course.first << "  " << course.second.size() << " students:\n  ";
        std::copy(std::begin(course.second), std::end(course.second), std::ostream_iterator<Student>(std::cout, "  "));
    }
};
#endif
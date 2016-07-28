//
//  Matrix_Py.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "Matrix_Py.h"

#if 0
// include this header file for access to boost::python templates and macros
#include <boost/python.hpp>

// add the using clause to reduce namespace clutter
using namespace boost::python;


MatrixP::MatrixP(int a)
: Matrix(a)
{

}

MatrixP::MatrixP(int a, int b)
: Matrix(a, b)
{

}

MatrixP::MatrixP(const MatrixP &p)
: Matrix(p)
{

}

MatrixP::~MatrixP()
{
}

void MatrixP::set(int a, int b, double v)
{
    (*this)[a][b] = v;
}

double MatrixP::get(int a, int b)
{
    return (*this)[a][b];
}


// this macro generates all the boilerplate required by the Python API
BOOST_PYTHON_MODULE(matrix)
{

    // defines a new class to be exported
    class_<MatrixP>("Matrix",
                   init<int>())    // the init form defines a constructor

        // another constructor with two int parameters
        .def(init<int, int>())

        // here are some standard functions (name first, member function second)
        .def("add", &MatrixP::add)
        .def("subtract", &MatrixP::subtract)
        .def("multiply", &MatrixP::multiply)
        .def("numRows", &MatrixP::numRows)
        .def("trace", &MatrixP::trace)
        .def("transpose", &MatrixP::transpose)
        .def("set", &MatrixP::set)
        .def("get", &MatrixP::get)
    ;
}

#endif
//
//  VectorOperations.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "VectorOperations.h"

#include <boost/numeric/ublas/vector.hpp>


VectorOperations::VectorOperations(const std::vector<double> &p)
: m_data(p)
{
}

VectorOperations::VectorOperations(const VectorOperations &p)
: m_data(p.m_data)
{

}

VectorOperations::~VectorOperations()
{
}

VectorOperations &VectorOperations::operator=(const VectorOperations &p)
{
    if (this != &p)
    {
        m_data = p.m_data;
    }
    return *this;
}

std::vector<double> VectorOperations::scalarMult(double scalar)
{
    using namespace boost::numeric::ublas;
    vector<double> vx;

    std::copy(m_data.begin(), m_data.end(), vx.end());


    vector<double> res = vx * scalar;

    std::vector<double> v;
    std::copy(res.begin(), res.end(), v.end());

    return v;
}

std::vector<double> VectorOperations::addVector(const std::vector<double> &vec)
{
    using namespace boost::numeric::ublas;

    vector<double> v1;
    std::copy(m_data.begin(), m_data.end(), v1.end());

    vector<double> v2;
    std::copy(vec.begin(), vec.end(), v2.end());

    vector<double> v3 = v1 + v2;

    std::vector<double> v;
    std::copy(v3.begin(), v3.end(), v.end());
    return v;
}

std::vector<double> VectorOperations::subtractVector(const std::vector<double> &vec)
{
    using namespace boost::numeric::ublas;

    vector<double> v1;
    std::copy(m_data.begin(), m_data.end(), v1.end());

    vector<double> v2;
    std::copy(vec.begin(), vec.end(), v2.end());

    vector<double> v3 = v1 - v2;

    std::vector<double> v;
    std::copy(v3.begin(), v3.end(), v.end());
    return v;
}

double VectorOperations::dotProd(const std::vector<double> &v)
{

    using namespace boost::numeric::ublas;

    vector<double> v1;
    std::copy(m_data.begin(), m_data.end(), v1.end());

    vector<double> v2;
    std::copy(v.begin(), v.end(), v2.end());

    double res = inner_prod(v1, v2);
    return res;
}


double VectorOperations::norm()
{
    using namespace boost::numeric::ublas;

    vector<double> v1;
    std::copy(m_data.begin(), m_data.end(), v1.end());

    double res = norm_2(v1);
    return res;
}




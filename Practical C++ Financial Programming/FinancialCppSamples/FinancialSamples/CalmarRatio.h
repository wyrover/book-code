//
// CalmarRatio.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net


#ifndef CALMARRATIO_H_
#define CALMARRATIO_H_

#include <boost/ratio.hpp>

typedef boost::ratio<1, 1>::type CalmarRatioType;
typedef boost::ratio<1, 100>::type CalmarRatioBPS;
typedef boost::ratio<1, 1>::type CalmarRatioPerc;

template <class Ratio>
class CalmarRatio {
public:
	CalmarRatio(double calmar, double ret) : m_calmar(calmar), m_return(ret) {}
	virtual ~CalmarRatio() {}

	double getReturn()
	{
		return m_return;
	}

	double getDrawDown()
	{
		return m_return / m_calmar * m_ratio.den;
	}
private:
	Ratio m_ratio;
	double m_calmar;
	double m_return;
};

template <>
double CalmarRatio<CalmarRatioBPS>::getDrawDown()
{
	return  m_return / m_calmar * m_ratio.den * 100;
}

#endif /* CALMARRATIO_H_ */

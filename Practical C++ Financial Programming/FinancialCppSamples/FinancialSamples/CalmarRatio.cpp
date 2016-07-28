//
// CalmarRatio.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net


#include "CalmarRatio.h"

#include <iostream>



boost::ratio<1, 2> one_half;
boost::ratio<1, 3> one_third;



void createCalmarRatio()
{
	CalmarRatio<CalmarRatioPerc> ratio(0.15, 11);
}


void printRatios()
{
	std::cout << "one_third numerator: " << one_third.num
	          << " denominator: " << one_third.den;
}

//void printCalmarRatios()
int main_calmar()
{
	CalmarRatio<CalmarRatioPerc> ratio(0.110, 3.12);
	std::cout << "return: " << ratio.getReturn()
	          << " drawdown: " << ratio.getDrawDown() << std::endl;

	CalmarRatio<CalmarRatioBPS> bpsRatio(480, 2.15);
	std::cout << "return: " << bpsRatio.getReturn()
	          << " drawdown: " << bpsRatio.getDrawDown() << std::endl;
    return 0;
}


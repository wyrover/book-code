//
//  CreditRisk.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "CreditRisk.h"

#include <iostream>


CreditRisk::CreditRisk()
{

}

CreditRisk::~CreditRisk()
{

}

CreditRisk::CreditRisk(const CreditRisk &a)
{

}

CreditRisk &CreditRisk::operator =(const CreditRisk &a)
{
    return *this;
}

CreditRisk::RiskType CreditRisk::getRating()
{
    RiskType type = RiskType::AAA;
    // define the right type here...
    return type;
}

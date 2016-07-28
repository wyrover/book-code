//
//  CreditRisk.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__CreditRisk__
#define __FinancialSamples__CreditRisk__

class CreditRisk {
public:
    enum RiskType {
        AAA,
        AAPlus,
        AA,
        APlus,
        A,
        BPlus,
        B,
        CPlus,
        C
    };


    RiskType getRating();

    CreditRisk();
    ~CreditRisk();
    CreditRisk(const CreditRisk &);
    CreditRisk &operator =(const CreditRisk &);

};



#endif /* defined(__FinancialSamples__CreditRisk__) */

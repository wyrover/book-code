//
//  Recommendation.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__Recommendation__
#define __FinancialSamples__Recommendation__

#include <string>

enum RecommendationType {
    BUY,
    SELL,
    HOLD,
    NO_RECOMMENDATION
};

class Recommendation {
public:
    Recommendation();
    Recommendation(const std::string &ticker, RecommendationType rec, double target);
    ~Recommendation();
    Recommendation(const Recommendation &r);
    Recommendation &operator =(const Recommendation &r);

    double getTarget() const;
    RecommendationType getRecommendation() const;
    std::string getTicker() const;

private:
    std::string m_ticker;
    RecommendationType m_recType;
    double m_target;
};

#endif /* defined(__FinancialSamples__Recommendation__) */

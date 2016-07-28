//
//  RecommendationsProcessor.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__RecommendationsProcessor__
#define __FinancialSamples__RecommendationsProcessor__


#include <map>
#include <vector>

#include "Recommendation.h"


class RecommendationsProcessor {
public:
    RecommendationsProcessor();
    ~RecommendationsProcessor();
    RecommendationsProcessor(const RecommendationsProcessor &);
    RecommendationsProcessor &operator =(const RecommendationsProcessor &);

    void addRecommendation(const std::string &ticker, RecommendationType rec, double targetPrice);
    double averageTargetPrice(const std::string &ticker);
    RecommendationType averageRecommendation(const std::string &ticker);
private:
    std::map<std::string, std::vector<Recommendation> > m_recommendations;
};



#endif /* defined(__FinancialSamples__RecommendationsProcessor__) */

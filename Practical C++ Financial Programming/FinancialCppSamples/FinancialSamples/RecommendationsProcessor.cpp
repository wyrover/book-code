//
//  RecommendationsProcessor.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "RecommendationsProcessor.h"
#include <numeric>

RecommendationsProcessor::RecommendationsProcessor()
{

}

RecommendationsProcessor::~RecommendationsProcessor()
{

}

RecommendationsProcessor::RecommendationsProcessor(const RecommendationsProcessor &r)
: m_recommendations(r.m_recommendations)
{

}

RecommendationsProcessor &RecommendationsProcessor::operator =(const RecommendationsProcessor &r)
{
    if (this != &r)
    {
        m_recommendations = r.m_recommendations;
    }
    return *this;
}

void RecommendationsProcessor::addRecommendation(const std::string &ticker,
                                                 RecommendationType rec, double targetPrice)
{
    Recommendation r(ticker, rec, targetPrice);
    m_recommendations[ticker].push_back(r);
}

double RecommendationsProcessor::averageTargetPrice(const std::string &ticker)
{
    if (m_recommendations.find(ticker) == m_recommendations.end())
        return 0;
    auto vrec = m_recommendations[ticker];
    std::vector<double> prices;
    for (auto i=0; i<vrec.size(); ++i)
    {
        prices.push_back(vrec[i].getTarget());
    }
    return std::accumulate(prices.begin(), prices.end(), 0) / prices.size();
}

RecommendationType RecommendationsProcessor::averageRecommendation(const std::string &ticker)
{
    double avg = 0;
    if (m_recommendations.find(ticker) == m_recommendations.end())
    {
        return NO_RECOMMENDATION;
    }
    auto vrec = m_recommendations[ticker];
    std::vector<int> recommendations;
    for (auto i=0; i<vrec.size(); ++i)
    {
        recommendations.push_back((int)vrec[i].getRecommendation()+1);
    }

    return (RecommendationType) (int) (avg / recommendations.size());
}


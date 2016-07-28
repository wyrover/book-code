//
//  OptionsProbabilities.h

#ifndef __FinancialSamples__OptionsProbabilities__
#define __FinancialSamples__OptionsProbabilities__

#include <vector>

class OptionsProbabilities {
public:
    OptionsProbabilities(double initialPrice, double strike, double avgStep, int nDays);
    OptionsProbabilities(const OptionsProbabilities &p);
    ~OptionsProbabilities();
    OptionsProbabilities &operator=(const OptionsProbabilities &p);

    void setNumIterations(int n);

    double probFinishAboveStrike();
    double probFinishBellowStrike();
    double probFinalPriceBetweenPrices(double lowPrice, double highPrice);
    std::vector<double> getWalk();
private:
    double m_initialPrice;
    double m_strike;
    double m_avgStep;
    int m_numDays;
    int m_numIterations;

    double gaussianValue(double mean, double sigma);
    double getLastPriceOfWalk();
};

#endif /* defined(__FinancialSamples__OptionsProbabilities__) */

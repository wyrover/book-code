//
//  QtPlotter.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__QtPlotter__
#define __FinancialSamples__QtPlotter__

#include <string>
#include <vector>

class QtPlotter {
public:
    QtPlotter();
    QtPlotter(const QtPlotter &p);
    ~QtPlotter();
    QtPlotter &operator=(const QtPlotter &p);
    void setData(const std::vector<double> &xdata, const std::vector<double> &ydata);
    int plotWindowRun(int argc, char **argv);
    int plotWindowRun();
private:
    std::vector<double> m_xdata;
    std::vector<double> m_ydata;
};




#endif /* defined(__FinancialSamples__QtPlotter__) */

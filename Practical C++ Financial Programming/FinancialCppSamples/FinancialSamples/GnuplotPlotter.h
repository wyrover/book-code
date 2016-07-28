//
//  GnuplotPlotter.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__GnuplotPlotter__
#define __FinancialSamples__GnuplotPlotter__

#include <vector>
#include <string>

class GnuplotPlotter {
public:
    GnuplotPlotter(const std::string &fileName);
    GnuplotPlotter(const GnuplotPlotter &p);
    ~GnuplotPlotter();
    GnuplotPlotter &operator=(const GnuplotPlotter &p);
    void generateCmds(const std::string &cmdFileName);
    void setHeaders(const std::string &xheader, const std::string &yheader);
    void setData(const std::vector<double> &xdata, const std::vector<double> &ydata);
    void setData(const std::vector<std::string> &xdata, const std::vector<double> &ydata);
    void csvWrite();
private:
    std::string m_fileName;
    std::string m_xheader;
    std::string m_yheader;
    std::vector<std::string> m_xdata;
    std::vector<double> m_ydata;
    bool m_isDate;
};

#endif /* defined(__FinancialSamples__GnuplotPlotter__) */

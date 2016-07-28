//
//  IntRateEngine.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__IntRateEngine__
#define __FinancialSamples__IntRateEngine__

#include <vector>

template <class T>
class IntRateEngine {
public:
    IntRateEngine();
    ~IntRateEngine();
    IntRateEngine(const IntRateEngine<T> &a);
    IntRateEngine<T> &operator =(const IntRateEngine<T> &a);

    void setInstrument(T &inv);
    double getAnnualIntRate();
private:
    T m_instrument;
};

template <class T>
IntRateEngine<T>::IntRateEngine()
{

}

template <class T>
IntRateEngine<T>::~IntRateEngine()
{

}

template <class T>
IntRateEngine<T>::IntRateEngine(const IntRateEngine<T> &a)
: m_instrument(a.m_instrument)
{

}

template <class T>
IntRateEngine<T> &IntRateEngine<T>::operator =(const IntRateEngine<T> &a)
{
    if (this != &a)
    {
        m_instrument = a.m_instrument;
    }
    return *this;
}

template <class T>
void IntRateEngine<T>::setInstrument(T &inv)
{
    m_instrument = inv;
}

template <class T>
double IntRateEngine<T>::getAnnualIntRate()
{
    double payment = m_instrument.getMonthlyPayment();
    double principal = m_instrument.getPrincipal();
    return (12 *payment) / principal;
}


#endif /* defined(__FinancialSamples__IntRateEngine__) */

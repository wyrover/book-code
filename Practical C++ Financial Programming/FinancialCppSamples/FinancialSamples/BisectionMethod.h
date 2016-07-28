//
// BisectionMethod.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef BISECTIONMETHOD_H_
#define BISECTIONMETHOD_H_

template <class T>
class MathFunction;

class BisectionMethod {
public:
	BisectionMethod(MathFunction<double> &f);
	BisectionMethod(const BisectionMethod &p);
	~BisectionMethod();
	BisectionMethod &operator=(const BisectionMethod &p);
	double getRoot(double x1, double x2);

private:
	MathFunction<double> &m_f;
	double m_error;
};

#endif /* BISECTIONMETHOD_H_ */

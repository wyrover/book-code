//
// SecantMethod.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef SECANTMETHOD_H_
#define SECANTMETHOD_H_

template <class T>
class MathFunction;

class SecantMethod {
public:
	SecantMethod(MathFunction<double> &f);
	SecantMethod(const SecantMethod &p);
	SecantMethod &operator=(const SecantMethod &p);
	~SecantMethod();

	double getRoot(double x1, double x2);
private:
	MathFunction<double> &m_f;
	double m_error;
};

#endif /* SECANTMETHOD_H_ */

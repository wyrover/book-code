//
// NewtonMethod.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef NEWTONMETHOD_H_
#define NEWTONMETHOD_H_

template <typename T>
class MathFunction;

class NewtonMethod {
public:
	NewtonMethod(MathFunction<double> &f, MathFunction<double> &derivative);
	NewtonMethod(const NewtonMethod &p);
	virtual ~NewtonMethod();
	NewtonMethod &operator=(const NewtonMethod &p);

	double getRoot(double initialValue);
private:
	MathFunction<double> &m_f;
	MathFunction<double> &m_derivative;
	double m_error;
};


#endif /* NEWTONMETHOD_H_ */

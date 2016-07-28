//
// MathFunction.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef MATHFUNCTION_H_
#define MATHFUNCTION_H_

template <class Res>
class MathFunction {
public:
	MathFunction() {}
	virtual ~MathFunction(){}
	virtual Res operator()(Res x) = 0;
    virtual Res operator()(Res x, Res y); // version with two variables
};

template <class Res>
Res MathFunction<Res>::operator()(Res x, Res y)
{
    return 0;
}

#endif /* MATHFUNCTION_H_ */

#include <iostream>
#include <functional>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/utility/enable_if.hpp>

struct Constant {
    Constant(double val = 0.0) : val_(val) {}
    double operator()(double) const
    {
        return val_;
    }

    const double val_;
};

struct Variable {
    double operator()(double x)
    {
        return x;
    }
};

template <typename E, typename Enable = void>
struct Expr {
    Expr(E e) : expr(e) {}

    double operator()(double d)
    {
        return expr(d);
    }

private:
    E expr;
};

template <typename E>
struct Expr<E,
           typename boost::enable_if<boost::is_arithmetic<E>>::type> {
    Expr(E e) : expr(e) {}
    double operator()(double d)
    {
        return expr(d);
    }

private:
    Constant expr;
};

template <typename E1, typename E2, typename Op>
struct ComplexExpr {
    ComplexExpr(Expr<E1> left, Expr<E2> right) : left_(left), right_(right) {}

    double operator()(double d)
    {
        return Op()(left_(d), right_(d));
    }

private:
    Expr<E1> left_;
    Expr<E2> right_;
};

template <typename E1, typename E2>
Expr<ComplexExpr<E1, E2, std::plus<double>>>
operator+ (E1 left, E2 right)
{
    typedef ComplexExpr<E1, E2, std::plus<double>> ExprType;
    return ExprType(Expr<E1>(left), Expr<E2>(right));
}

template <typename E1, typename E2>
Expr<ComplexExpr<E1, E2, std::minus<double>>>
operator- (E1 left, E2 right)
{
    typedef ComplexExpr<E1, E2, std::minus<double>> ExprType;
    return ExprType(Expr<E1>(left), Expr<E2>(right));
}

template <typename E1, typename E2>
Expr<ComplexExpr<E1, E2, std::multiplies<double>>>
operator* (E1 left, E2 right)
{
    typedef ComplexExpr<E1, E2, std::multiplies<double>> ExprType;
    return ExprType(Expr<E1>(left), Expr<E2>(right));
}

template <typename E1, typename E2>
Expr<ComplexExpr<E1, E2, std::divides<double>>>
operator/ (E1 left, E2 right)
{
    typedef ComplexExpr<E1, E2, std::divides<double>> ExprType;
    return ExprType(Expr<E1>(left), Expr<E2>(right));
}


int main()
{
    Variable x;
    std::cout << (x + 5)(1) << '\n';
    std::cout << ((x + 5 - x * x) / (x * 2))(10.0) << '\n';
    std::cout << ((x * x - x + 4) / (x * 2))(10.0) << '\n';
}


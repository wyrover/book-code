#include "stdafx.h"
#include <iostream>

#define M_PI 3.14159265358979323846

using namespace std;

class CFigure
{
public:
    virtual char* FigureType() = 0;
    virtual double Circumference() = 0;
    virtual double Area() = 0;
    virtual ~CFigure() { }
};

class CTriangle : public CFigure
{
public:
    CTriangle()
    {
        a = b = c = 0;
    }
    CTriangle(double a, double b, double c) : a(a), b(b), c(c) { }
    virtual char* FigureType()
    {
        return "Triangle";
    }
    virtual double Circumference()
    {
        return a + b + c;
    }
    virtual double Area()
    {
        double S = Circumference() / 2;
        return sqrt(S * (S - a) * (S - b) * (S - c));
    }
private:
    double a, b, c;
};
class CSquare : public CFigure
{
public:
    CSquare()
    {
        a = b = 0;
    }
    CSquare(double a, double b) : a(a), b(b) { }
    virtual char* FigureType()
    {
        return "Square";
    }
    virtual double Circumference()
    {
        return 2 * a + 2 * b;
    }
    virtual double Area()
    {
        return a * b;
    }
private:
    double a, b;
};

class CCircle : public CFigure
{
public:
    CCircle()
    {
        r = 0;
    }
    CCircle(double r) : r(r) { }
    virtual char* FigureType()
    {
        return "Circle";
    }
    virtual double Circumference()
    {
        return 2 * r * M_PI;
    }
    virtual double Area()
    {
        return r * r * M_PI;
    }
private:
    double r;
};

int _tmain(int argc, _TCHAR* argv[])
{
    CFigure* figures[3];
    figures[0] = new CTriangle(2.1, 3.2, 4.3);
    figures[1] = new CSquare(5.4, 6.5);
    figures[2] = new CCircle(8.8);

    for (int i = 0; i < 3; i++) {
        cout << "Figure type:\t" << figures[i]->FigureType()
             << "\nCircumference:\t" <<
             figures[i]->Circumference()
             << "\nArea:\t\t" << figures[i]->Area()
             << endl << endl;
    }

    return 0;
}

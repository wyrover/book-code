#include <complex>
#include <iostream>

using namespace std;

int main()
{
    double rho = 3.0; // magnitude
    double theta = 3.141592 / 2; // angle
    complex<double> coord = polar(rho, theta);
    cout << "rho = " << abs(coord) << ", theta = " << arg(coord) << endl;
    coord += polar(4.0, 0.0);
    cout << "rho = " << abs(coord) << ", theta = " << arg(coord) << endl;
}
#include <iostream>
#include <complex>

int main()
{
	std::complex<float> c(1, 2);      // Both arguments are optional (default: 0)
	std::cout << "c=" << c.real() << '+' << c.imag() << 'i' << '\n';   // c=1+2i
	c.real(3); c.imag(3); c += 1;
	std::cout << "norm(" << c << ") = " << std::norm(c);     // norm((4,3)) = 25

	std::cout << std::endl;
}

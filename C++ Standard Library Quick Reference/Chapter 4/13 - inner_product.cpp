#define _SCL_SECURE_NO_WARNINGS		// To make the example work in Visual C++.
#include <iostream>
#include <numeric>
#include <iterator>

int main()
{
	double v1[] = { 0,1,2 };
	double v2[] = { 1,0,2 };
	double dot = std::inner_product(std::begin(v1), std::end(v1),
		std::begin(v2), 0.0);    // 0*1 + 1*0 + 2*2 = 4.0

	std::cout << dot << std::endl;
}

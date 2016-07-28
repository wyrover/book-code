#include <iostream>
#include <locale>

class yes_no_numpunct : public std::numpunct<char> {
protected:
	virtual string_type do_truename() const override { return "yes"; }
	virtual string_type do_falsename() const override { return "no"; }
};

int main()
{
	std::cout.imbue(std::locale(std::cout.getloc(), new yes_no_numpunct));
	std::cout << std::boolalpha << true << " / " << false << std::endl;


	std::cout << std::endl;
}

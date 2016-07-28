#include <iostream>
#include <locale>
#include <iomanip>

class accounting_moneypunct : public std::moneypunct_byname<char, false> {
public:
	accounting_moneypunct(const std::string& name)
		: moneypunct_byname(name) { }
protected:
	// Put negative numbers between parentheses:
	virtual string_type do_negative_sign() const override { return "()"; }
	// Override formats to facilitate accounting-style padding:
	static pattern acc_format() { return{ symbol, space, sign, value }; }
	virtual pattern do_neg_format() const override { return acc_format(); }
	virtual pattern do_pos_format() const override { return acc_format(); }
};

int main()
{
	const auto name = "en_US";    // en-US on Windows.
	std::locale my_locale(std::locale(name), new accounting_moneypunct(name));
	std::cout.imbue(my_locale);
	std::cout << std::showbase << std::internal; //show $ sign + tweak padding 
	for (auto val : { 100000, -500 })
		std::cout << std::setw(12) << std::put_money(val) << '\n';


	std::cout << std::endl;
}

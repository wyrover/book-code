#include <iostream>
#include <locale>
#include <iomanip>

using namespace std;

int main()
{
	int bigValue = 10000;
	long double money = 123456;
	cout << bigValue << " " << put_money(money) << '\n'; // 10000 123456

	locale chinese("zh_CN");  // For Windows use "zh-CN"
	cout.imbue(chinese);
	cout << bigValue << ' ' << put_money(money) << '\n';  // 10,000 1,234.56

	// Use the neutral "C" locale, but with Chinese monetary punctuation:
	locale combined = locale::classic().combine<moneypunct<char>>(chinese);
	// Or:
	// locale combined(locale::classic(), &use_facet<moneypunct<char>>(chinese));
	// Or:
	// locale combined(locale::classic(), chinese, locale::monetary);
	cout.imbue(combined);
	cout << bigValue << ' ' << put_money(money) << '\n';  // 10000  1,234.56


	std::cout << std::endl;
}

#include <iostream>
#include <iomanip>
#include <locale>

int main()
{
	using namespace std;
	cout.imbue(locale(""));    // Use the user's preferred locale, see Chapter 6
	cout << setfill('_') << hex << showbase;
	cout << "Left:     " << left << setw(7) << put_money(123) << '\n';
	cout << "Right:    " << right << setw(7) << put_money(123) << '\n';
	cout << "Internal: " << internal;
	cout.width(7);
	cout << 123 << '\n';


	std::cout << std::endl;
}

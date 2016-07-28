// Listing 8-3. Using Alternative Fill Characters 
#include <iomanip>
#include <iostream>

int main()
{
  using namespace std;

  int day{14};
  int month{3};
  int year{2006};
  int dollars{42};
  int cents{7};

  // Print date in USA order. Later in the book, you will learn how to
  // handle internationalization.
  cout << "Date: " << setfill('0') << setw(2) << month
                            << '/' << setw(2) << day
                            << '/' << setw(2) << year << '\n';
  cout << "Pay to the order of: CASH\n";
  cout << "The amount of $" << setfill('*') << setw(8) << dollars << '.'
                            << setfill('0') << setw(2) << cents << '\n';
}

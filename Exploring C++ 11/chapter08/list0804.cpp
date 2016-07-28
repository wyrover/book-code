// Listing 8-4. Exploring Field Width, Fill Character, and Alignment 
#include <iomanip>
#include <iostream>

int main()
{
  using namespace std;
  cout << '|' << setfill('*') << setw(6) <<  1234 << '|' << '\n';
  cout << '|' << left <<         setw(6) <<  1234 << '|' << '\n';
  cout << '|' <<                 setw(6) << -1234 << '|' << '\n';
  cout << '|' << right <<        setw(6) << -1234 << '|' << '\n';
}

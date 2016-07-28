// Listing 70-3. Using a Function Parameter Pack to Print Arbitrary Values
#include <iostream>
#include <utility>

// Forward declaration.
template<class... Types>
void print(std::ostream& stream, Types&&...);

// Print the first value in the list, then recursively
// call print() to print the rest of the list.
template<class T, class... Types>
void print_split(std::ostream& stream, T&& head, Types&& ... rest)
{
   stream << head << ' ';
   print(stream, std::forward<Types>(rest)...);
}

// End recursion when there are no more values to print.
void print_split(std::ostream&)
{}

// Print an arbitrary list of values to a stream.
template<class... Types>
void print(std::ostream& stream, Types&&... args)
{
   print_split(stream, std::forward<Types>(args)...);
}

int main()
{
   print(std::cout, 42, 'x', "hello", 3.14159, 0, '\n');
}

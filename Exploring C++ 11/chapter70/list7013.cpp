// Listing 70-13. Using enable_if to Direct Overload Resolution
#include <iostream>
#include <type_traits>

template<class T>
void encode(std::ostream& stream, T const& int_value,
   typename std::enable_if<std::is_integral<T>::value, T>::type* = nullptr)
{
   // All integer types end up here.
   stream << "int: " << int_value << '\n';
}

template<class T>
void encode(std::ostream& stream, T const& enum_value,
   typename std::enable_if<std::is_enum<T>::value>::type* = nullptr)
{
   // All enumerated types end up here.
   // Record the underlying integer value.
   stream << "enum: " <<
      static_cast<typename std::underlying_type<T>::type>(enum_value) << '\n';
}

template<class T>
void encode(std::ostream& stream, T const& float_value,
   typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr)
{
   // All floating-point types end up here.
   stream << "float: " << float_value << '\n';
}

// enable_if forms cooperate with normal overloading
void encode(std::ostream& stream, std::string const& string_value)
{
   stream << "str: " << string_value << '\n';
}

int main()
{
   encode(std::cout, 1);
   enum class color { red, green, blue };
   encode(std::cout, color::blue);
   encode(std::cout, 3.0);
   encode(std::cout, std::string("string"));
}

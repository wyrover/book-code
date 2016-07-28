#include <iostream>
#include <vector>
#include <string>

// use the efficient memcpy() if allowed (i.e., if T is trivially copyable):
template<typename T, size_t N>
typename std::enable_if<std::is_trivially_copyable<T>::value>::type
copy(T(&from)[N], T(&to)[N])
{
	std::memcpy(to, from, N * sizeof(T));
}

// otherwise, copy elements one by one using copy assignment:
template<typename T, size_t N>
std::enable_if_t<!std::is_trivially_copyable<T>::value>
copy(T(&from)[N], T(&to)[N])
{
	for (size_t i = 0; i < N; ++i)
		to[i] = from[i];
}

int main()
{
	int arrayFrom[12] = { 0 };
	int arrayTo[12] = { 0 };
	copy(arrayFrom, arrayTo);	// Calls the first copy() implementation.

	std::string vecFrom[2] = { "A", "B" };
	std::string vecTo[2];
	copy(vecFrom, vecTo);		// Calls the second copy() implementation.


	std::cout << std::endl;
}

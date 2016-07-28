#include <iostream>
#include <memory>

void DoSomethingWith(int* data)
{
	/* ... */
}

int main()
{
	{
		std::unique_ptr<int[]> array(new int[123]);	// or make_unique<int[]>(123)
		for (int i = 0; i < 123; ++i)
			array[i] = i;
		DoSomethingWith(array.get());				// Pass raw int* pointer
	}												// array is delete[]'d, even if DoSomethingWith() throws

	std::cout << std::endl;
}

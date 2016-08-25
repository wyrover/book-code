#include <stdio.h>

float fib(int);

void main()
{
	int number;
	printf("pleace input a number:");
	scanf("%d",&number);
	printf("The fib(%d) = %g",number,fib(number));
}

float fib(int number)
{
	if (number <= 2)
	{
		return 1;
	}
	else
	{
		return (fib(number-2) + fib(number-1));
	}
}

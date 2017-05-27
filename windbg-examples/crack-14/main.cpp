#include "windows.h"
#include "tchar.h"
#include <stdio.h>
#include <iostream>

void if_example_01()
{
	printf("if_example_01 begin ----------------------------\n");

	int a = 3, b = 5;

	std::cout << a << b << std::endl;

	if (a < b)
		printf("%s", "yes"); 

	printf("if_example_01 end ----------------------------\n");
}

void if_example_02()
{
	printf("if_example_02 begin ----------------------------\n");

	int a = 3, b = 5;

	std::cout << a << b << std::endl;

	if (a < b)
		printf("%s", "yes");
	else 
		printf("%s", "no");

	printf("if_example_02 end ----------------------------\n");
}

void if_example_03()
{
	printf("if_example_03 begin ----------------------------\n");

	int a = 3, b = 5;

	std::cout << a << b << std::endl;

	if (a < b)
		printf("%s", "yes");
	else if (a > b)
		printf("%s", "no");
	else if (a == 3)
		printf("%s", "3");
	else if (b == 5)
		printf("%s", "5");
	else
		printf("%s", "default");

	printf("if_example_03 end ----------------------------\n");
}

void if_example_04()
{
	printf("if_example_04 begin ----------------------------\n");

	int i = 1, j = 2;

	std::cout << i << j << std::endl;

	if (i < 2 || i == 1 || j != 2 || i + j < 3) {
		printf("%s", "yes");
	} else {
		printf("%s", "no");
	}

	printf("if_example_04 end ----------------------------\n");
}

void if_example_05()
{
	printf("if_example_05 begin ----------------------------\n");

	int i = 1, j = 2;

	std::cout << i << j << std::endl;

	if (i < 2 && i == 1 && j != 2 && i + j < 3) {
		printf("%s", "yes");
	} else {
		printf("%s", "no");
	}

	printf("if_example_05 end ----------------------------\n");
}



int _tmain(int argc, TCHAR *argv[])
{
	if_example_01();

	if_example_02();

	if_example_03();

	if_example_04();

	if_example_05();
	
	return 0;
}
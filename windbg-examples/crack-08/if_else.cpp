// if_else.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// if 结构
void if_example_01(int argc)
{
	printf("if_example_01 begin ----------------------------\n");

    if (argc == 0) {
        printf("%d \r\n", argc);
    }

    if (argc > 0) {
        printf("argc >= 0");
    }

	printf("if_example_01 end ----------------------------\n");
    
}

// if else 结构
void if_example_02(int argc)
{
	printf("if_example_02 begin ----------------------------\n");
//  if (argc == 0)
//  {
//      printf("argc == 0");
//  }
//  else
//  {
//      printf("argc != 0");
//  }

    // 模拟条件表达式 转换方式1
    if (argc == 0) {
        argc = 5;
    } else {
        argc = 6;
    }

    printf("%d \r\n", argc);

	printf("if_example_02 end ----------------------------\n");
}

// if else if 组合
void if_example_03(int argc)
{
	printf("if_example_03 begin ----------------------------\n");

    argc = 0;

    if (argc > 0) {
        printf("argc > 0");
    } else if (argc == 0) {
        printf("argc == 0");
    } else {
        printf("argc <= 0");
    }

	printf("if_example_03 end ----------------------------\n");
}

void if_example_04(int argc)
{
	printf("if_example_04 begin ----------------------------\n");

	int x = 0;
	int y = 1;
	int z = 2;

	if (x == y) {
		if (z == 0) {
			printf("z is zero and x = y.\n");
		} else {
			printf("z is non-zero and x = y.\n");
		}
	} else {
		if (z == 0) {
			printf("z zero and x != y.\n");
		} else {
			printf("z non-zero and x != y.\n");
		}
	}

	printf("if_example_04 end ----------------------------\n");
}

int main(int argc, char* argv[])
{
    int a = 7;
    int b = !!a;
    printf("%f", 10);


    if_example_01(argc);
    if_example_02(argc);
    if_example_03(argc);

    return 0;
}


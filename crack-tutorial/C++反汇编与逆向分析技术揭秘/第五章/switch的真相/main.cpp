#include <stdio.h>

// if else 方式
void test_switch_if()
{
	int i = 1;
	scanf("%d", &i);
	switch(i){
	case 1:
		printf("i == 1");
		break;
	case 3:
		printf("i == 3");
		break;
	case 100:
		printf("i == 100");
		break;
	}
}

// 线性处理
void SwitchLine()
{
	int nIndex = 0;
	scanf("%d", &nIndex);
	switch(nIndex)
	{
	case 1:
		printf("nIndex == 1");
		break;
	case 2:
		printf("nIndex == 2");
		break;
	case 3:
		printf("nIndex == 3");
		break;
	case 5:
		printf("nIndex == 5");
		break;
	case 6:
		printf("nIndex == 6");
		break;
	case 7:
		printf("nIndex == 7");
		break;
	}
}

// 非线性的处理
void test_switch_nonline()
{
	int i = 0;
	scanf("%d", &i);
	switch(i){
	case 1:
		printf("i == 1");
		break;
	case 2:
		printf("i == 2");
		break;
	case 3:
		printf("i == 3");
		break;
	case 5:
		printf("i == 5");
		break;
	case 6:
		printf("i == 6");
		break;
	case 15:
		printf("i == 15");
		break;
	}
}

// 平衡树处理
void test_switch_tree()
{
	int i = 8;
	scanf("%d", &i);
	switch(i){
	case 2:
		printf("i == 2\n");
		break;
	case 3:
		printf("i == 3\n");
		break;
	case 8:
		printf("i == 8\n");
		break;
	case 10:
		printf("i == 10\n");
		break;
	case 35:
		printf("i == 35\n");
		break;
	case 37:
		printf("i == 37\n");
		break;
	case 666:
		printf("i == 666\n");
		break;
	case 10000:
		printf("i == 10000\n");
		break;
	default:
		printf("default\n");
		break;
	}
}


int main()
{
	// 线性处理
	//SwitchLine();
	//test_switch_if();
	test_switch_nonline();
	//test_switch_tree();
	return 0;
}
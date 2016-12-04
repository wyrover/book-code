// copy_constructortest.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>

class TestClass{
public:
	TestClass(){
		printf("do constructor\n");
	}
	~TestClass(){
		printf("do destructor\n");
	}
	int i1;
	int i2;
};

void test(TestClass a)
{
	printf("a.i1=%d a.i2=%d\n", a.i1, a.i2);
}

struct TestStruct
{
	int i1;
	int i2;
};

void test2(TestStruct s)
{
	printf("s.i1=%d s.i2=%d\n", s.i1, s.i2);
};

void test3(TestStruct* ps)
{
	printf("ps.i1=%d ps.i2=%d\n", ps->i1, ps->i2);
};

void main()
{
	TestClass a;
	a.i1 = 1;
	a.i2 = 2;
	test(a);
	TestStruct s;
	s.i1 = 3;
	s.i2 = 4;
	test2(s);
	test3(&s);
}


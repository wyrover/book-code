// test.cpp : 定义控制台应用程序的入口点。
//

int gi;

void testfunc()
{

}

void main()
{
	void (*f)() = testfunc;
	gi = 12;
	testfunc();
}

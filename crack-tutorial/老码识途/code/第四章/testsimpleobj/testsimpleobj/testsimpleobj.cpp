// testsimpleobj.cpp : 定义控制台应用程序的入口点。
//

class TestObjClass
{
public:
	int a;
	int b;
	int c;
	void setC(int v);
};

struct TestObjStruct
{
	int a;
	int b;
	int c;
};

void TestObjClass::setC(int v)
{
	c = v;
}

int main()
{
	TestObjClass t1;
	TestObjStruct t2;
	t1.c = 1;
	t1.setC(2);
	t2.c = 3;
}


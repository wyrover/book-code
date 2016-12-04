// testsimplediskarray.cpp : 定义控制台应用程序的入口点。
//
#include <windows.h>
#include "../include/types.h"
#include "../simplediskarray/simplediskarray.h"
#include <process.h>
#include <io.h>


#define ARRAY_FILE "test.dat"

void showError(char * msg)
{
	printf("%s", msg);
	exit(0);
};

struct MyTest
{
	int age;
	int no;
};

void initEnv()
{
	DeleteFileA(ARRAY_FILE);
}

void testInsert_get()
{
	initEnv();
	MyTest t;
	t.age = 10;
	t.no = 11;
	SimpleDiskArray array;
	initSimpleDiskArray(&array, sizeof(MyTest), ARRAY_FILE);
	if (openSimpleDiskArray(&array) == FALSE)
	{
		showError("fail to open\n");
	}
	insertSimpleDiskArray(&array, &t);
	t.age = 12;
	t.no = 13;
	insertSimpleDiskArray(&array, &t);
	if (getSimpleDiskArrayCount(&array) != 2)
		showError("count error\n");
	getSimpleDiskArray(&array, 0, &t);
	if (t.age != 10 || t.no != 11)
		showError("get error\n");
	getSimpleDiskArray(&array, 1, &t);
	if (t.age != 12 || t.no != 13)
		showError("get error\n");
	closeSimpleDiskArray(&array);
	finalizeSimpleDiskArray(&array);

//--------相当于重新打开,如果有缓冲都无效，保证肯定从硬盘读取---
	initSimpleDiskArray(&array, sizeof(MyTest), ARRAY_FILE);
	if (openSimpleDiskArray(&array) == FALSE)
	{
		showError("fail to open\n");
	}
	if (getSimpleDiskArrayCount(&array) != 2)
		showError("count error\n");
	getSimpleDiskArray(&array, 0, &t);
	if (t.age != 10 || t.no != 11)
		showError("get error\n");
	getSimpleDiskArray(&array, 1, &t);
	if (t.age != 12 || t.no != 13)
		showError("get error\n");
	closeSimpleDiskArray(&array);
	finalizeSimpleDiskArray(&array);
}

void testupdate()
{
	initEnv();
	MyTest t;
	t.age = 10;
	t.no = 11;
	SimpleDiskArray array;
	initSimpleDiskArray(&array, sizeof(MyTest), ARRAY_FILE);
	if (openSimpleDiskArray(&array) == FALSE)
	{
		showError("fail to open\n");
	}
	insertSimpleDiskArray(&array, &t);
	t.age = 12;
	t.no = 13;
	insertSimpleDiskArray(&array, &t);
	
	t.age = 30;
	t.no = 40;
	updateSimpleDiskArray(&array, 0, &t);	
	t.age = 50;
	t.no = 60;
	if (updateSimpleDiskArray(&array, 1, &t) == FALSE)
		showError("fail to update\n");
	closeSimpleDiskArray(&array);
	finalizeSimpleDiskArray(&array);

	//--------相当于重新打开,如果有缓冲都无效，保证肯定从硬盘读取---
	initSimpleDiskArray(&array, sizeof(MyTest), ARRAY_FILE);
	if (openSimpleDiskArray(&array) == FALSE)
	{
		showError("fail to open\n");
	}
	if (getSimpleDiskArrayCount(&array) != 2)
		showError("count error\n");
	getSimpleDiskArray(&array, 0, &t);
	if (t.age != 30 || t.no != 40)
		showError("get error\n");
	getSimpleDiskArray(&array, 1, &t);
	if (t.age != 50 || t.no != 60)
		showError("get error\n");
	closeSimpleDiskArray(&array);
	finalizeSimpleDiskArray(&array);
}

void testdeletefirst()
{
	initEnv();
	MyTest t;
	t.age = 10;
	t.no = 11;
	SimpleDiskArray array;
	initSimpleDiskArray(&array, sizeof(MyTest), ARRAY_FILE);
	if (openSimpleDiskArray(&array) == FALSE)
	{
		showError("fail to open\n");
	}
	insertSimpleDiskArray(&array, &t);
	t.age = 12;
	t.no = 13;
	if (insertSimpleDiskArray(&array, &t) == FALSE)
		showError("fail to insert\n");
	if (deleteSimpleDiskArray(&array, 0) == FALSE)
		showError("fail to del\n");
	if (getSimpleDiskArrayCount(&array) != 1)
		showError("count error\n");
	if (_filelength(array.fp->_file) != (sizeof(MyTest) + sizeof(array.count)))
		showError("size error\n");
	getSimpleDiskArray(&array, 0, &t);
	if (t.age != 12 || t.no != 13)
		showError("get error\n");
	closeSimpleDiskArray(&array);
	finalizeSimpleDiskArray(&array);

	//--------相当于重新打开,如果有缓冲都无效，保证肯定从硬盘读取---
	initSimpleDiskArray(&array, sizeof(MyTest), ARRAY_FILE);
	if (openSimpleDiskArray(&array) == FALSE)
	{
		showError("fail to open\n");
	}
	if (_filelength(array.fp->_file) != (sizeof(MyTest) + sizeof(array.count)))
		showError("size error\n");
	if (getSimpleDiskArrayCount(&array) != 1)
		showError("count error\n");
	getSimpleDiskArray(&array, 0, &t);
	if (t.age != 12 || t.no != 13)
		showError("get error\n");
	closeSimpleDiskArray(&array);
	finalizeSimpleDiskArray(&array);
}

void testdeletelast()
{
	initEnv();
	MyTest t;
	t.age = 10;
	t.no = 11;
	SimpleDiskArray array;
	initSimpleDiskArray(&array, sizeof(MyTest), ARRAY_FILE);
	if (openSimpleDiskArray(&array) == FALSE)
	{
		showError("fail to open\n");
	}
	insertSimpleDiskArray(&array, &t);
	t.age = 12;
	t.no = 13;
	insertSimpleDiskArray(&array, &t);	
	if (deleteSimpleDiskArray(&array, 1) == FALSE)
		showError("fail to del\n");
	if (getSimpleDiskArrayCount(&array) != 1)
		showError("count error\n");
	if (_filelength(array.fp->_file) != (sizeof(MyTest) + sizeof(array.count)))
		showError("size error\n");
	getSimpleDiskArray(&array, 0, &t);
	if (t.age != 10 || t.no != 11)
		showError("get error\n");
	closeSimpleDiskArray(&array);
	finalizeSimpleDiskArray(&array);

	//--------相当于重新打开,如果有缓冲都无效，保证肯定从硬盘读取---
	initSimpleDiskArray(&array, sizeof(MyTest), ARRAY_FILE);
	if (openSimpleDiskArray(&array) == FALSE)
	{
		showError("fail to open\n");
	}
	if (_filelength(array.fp->_file) != (sizeof(MyTest) + sizeof(array.count)))
		showError("size error\n");
	if (getSimpleDiskArrayCount(&array) != 1)
		showError("count error\n");
	getSimpleDiskArray(&array, 0, &t);
	if (t.age != 10 || t.no != 11)
		showError("get error\n");
	closeSimpleDiskArray(&array);
	finalizeSimpleDiskArray(&array);
}

void main()
{
	printf("begin test\n");
	testInsert_get();
	testupdate();
	testdeletefirst();
	testdeletelast();
	printf("end test\n");
}
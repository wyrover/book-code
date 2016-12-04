
#include <windows.h> 
#include <stdio.h> 
#include <iostream>
#include "PluginController.h"

using namespace std;

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		cout << "Parameters error" << endl;
		return 0;
	}

	if(strcmp(argv[1], "help") == 0)
	{
		CPluginController pc;
		pc.ProcessHelp();

		return 0;
	}

	int FunctionID = atoi(argv[1]);
	CPluginController pc;

	pc.InitializeController();

	pc.ProcessRequest(FunctionID);

	pc.UninitializeController();

	return 0;
}

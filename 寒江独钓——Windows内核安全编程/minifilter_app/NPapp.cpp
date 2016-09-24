#include "NPapp.h"
#include <iostream>
using namespace std;

int (__stdcall* pNPSendMessage)(PVOID pInBuffer);
int (__stdcall* pInitialCommunicationPort)(VOID);

CNPApp::CNPApp()
{
	m_hModule = NULL;
	LoadNPminifilterDll();
}

CNPApp::~CNPApp()
{
	if (m_hModule) {
		FreeLibrary(m_hModule);
	}
}
bool CNPApp::LoadNPminifilterDll(void)
{	
	m_hModule = LoadLibrary("minifilter_dll.dll");

	if (m_hModule != NULL) {
		
		pNPSendMessage = (int (__stdcall*)(PVOID)) GetProcAddress(GetModuleHandle("minifilter_dll.dll"), "NPSendMessage");
		if (!pNPSendMessage) {
			return false;
		}
		return true;
	}
	return false;
}

void CNPApp::NPMessage(COMMAND_MESSAGE data)
{
	if (m_hModule == NULL) {
		if (LoadNPminifilterDll() == false) {
			return;
		}
	}
	pNPSendMessage(&data);
}

void main(void)
{
	CNPApp ControlObj;
	char input;
	while (true) {
		cout << "Enter 'a' for PASS MODE, 'b' for BLOCKMODE or 'q' to EXIT" << endl;
		cin >> input;
		if (input == 'a' || input == 'A') {
			COMMAND_MESSAGE data;
			data.Command = ENUM_PASS;
			ControlObj.NPMessage(data);	
			printf("==>NOTEPAD.EXE PASS MODE\n");
		} else if (input == 'b' || input == 'B') {
			COMMAND_MESSAGE data;
			data.Command = ENUM_BLOCK;
			ControlObj.NPMessage(data);	
			printf("==>NOTEPAD.EXE BLOCK MODE\n");
		} else if (input == 'q' || input == 'Q') {
			printf("EXIT\n");
			break;
		} else {
			printf("Wrong Parameter!!!\n");
		}
	};

	system("pause");
}
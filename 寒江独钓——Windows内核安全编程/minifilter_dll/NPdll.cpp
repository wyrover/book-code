#include "NPdll.h"

HANDLE g_hPort = INVALID_HANDLE_VALUE;

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		InitialCommunicationPort();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

int InitialCommunicationPort(void)
{
    DWORD hResult = FilterConnectCommunicationPort( 
		NPMINI_PORT_NAME, 
		0, 
		NULL, 
		0, 
		NULL, 
		&g_hPort );

	if (hResult != S_OK) {
		return hResult;
	}
	return 0;
}

int NPSendMessage(PVOID InputBuffer)
{
	DWORD bytesReturned = 0;
	DWORD hResult = 0;
	PCOMMAND_MESSAGE commandMessage = (PCOMMAND_MESSAGE) InputBuffer;	
	
    hResult = FilterSendMessage(
		g_hPort,
        commandMessage,
        sizeof(COMMAND_MESSAGE),
        NULL,
        NULL,
        &bytesReturned );

	if (hResult != S_OK) {
		return hResult;
	}
	return 0;
}
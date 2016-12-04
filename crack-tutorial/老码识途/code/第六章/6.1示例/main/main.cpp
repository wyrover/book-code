
#include <windows.h> 
#include <stdio.h> 

typedef void (__cdecl *PROC_PRINT)(void); 

int main(int argc, char **argv)
{
	HINSTANCE hinstLib; 
	PROC_PRINT Proc; 
	BOOL fFreeResult;

	// Get a handle to the DLL module.
	hinstLib = LoadLibrary(TEXT("function.dll")); 

	if (hinstLib != NULL) 
	{ 
		Proc = (PROC_PRINT) GetProcAddress(hinstLib, "Print"); 

		// If the function address is valid, call the function.
		if (NULL != Proc) 
		{
			(Proc)(); 
		}

		// Free the DLL module.
		fFreeResult = FreeLibrary(hinstLib); 
	} 

	getchar();
}

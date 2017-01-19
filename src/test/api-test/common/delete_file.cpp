#include "delete_file.h"

void cactus::DeleteFileReboot( const wchar_t* filename )
{
	WCHAR TempPath[MAX_PATH];
	WCHAR TempName[MAX_PATH];

	GetTempPath(RTL_NUMBER_OF(TempPath), TempPath);
	GetTempFileName(TempPath, NULL, 0, TempName);

	if (!MoveFileEx(filename, TempName, MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH))
	{
		//DbgMsg(__FUNCTION__"(): MoveFileEx: 0x%x\n", GetLastError());
	}
	else
		DeleteFile(TempName);

	if (!MoveFileEx(TempName, NULL, MOVEFILE_DELAY_UNTIL_REBOOT|MOVEFILE_WRITE_THROUGH))
	{
		//DbgMsg(__FUNCTION__"(): MoveFileEx: 0x%x\n", GetLastError());
	}
}

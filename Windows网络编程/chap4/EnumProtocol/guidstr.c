//
// GUIDSTR.C -- Returns a formatted string for a GUID
//

#include <windows.h>

////////////////////////////////////////////////////////////

LPCTSTR GUIDtoString(GUID *pGUID)
{
	static char szStr[33];
	wsprintf(szStr, 
	    "%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		pGUID->Data1,
		pGUID->Data2,
		pGUID->Data3,
		pGUID->Data4[0],
		pGUID->Data4[1],
		pGUID->Data4[2],
		pGUID->Data4[3],
		pGUID->Data4[4],
		pGUID->Data4[5],
		pGUID->Data4[6],
		pGUID->Data4[7]);
	return(szStr);
}

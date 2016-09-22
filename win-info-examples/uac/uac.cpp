#include "uac.h"
#include <tchar.h>

#define SIZE_OF_ARRAY(Array) (sizeof((Array)) / sizeof(*(Array)))
#define ARRAY_CONTAINS(Array, Index) (Index>= 0 && Index <SIZE_OF_ARRAY(Array))

void* LocalAllocZero(size_t cb)
{
	return LocalAlloc(LPTR, cb);
}
inline void* LocalAlloc(size_t cb)
{
	return LocalAllocZero(cb);
}

unsigned myatou(const TCHAR *s)
{
	unsigned int v=0;

	for (;;)
	{
		unsigned int c=*s++;
		if (c>= _T('0') && c <= _T('9')) c-=_T('0');
		else break;
		v*=10;
		v+=c;
	}
	return v;
}

static FARPROC AdvApi32_GetProcAddress(LPCSTR Name)
{
	return GetProcAddress(LoadLibraryA("ADVAPI32"), Name);
}


TCHAR* CharPos(TCHAR* szStr, int cchStrLen, TCHAR chFind)
{
	for (int i = 0; i < cchStrLen && NULL != *(szStr + i); i++)
		if (*(szStr + i) == chFind)
			return szStr + i;

	return NULL;
}

BYTE FromHex(TCHAR* szHex)
{
	int a = 0;
	int b = 0;

	if (szHex[0] >= TEXT('0') && szHex[0] <= TEXT('9'))
		a = szHex[0] - TEXT('0');
	else if (szHex[0] >= TEXT('A') && szHex[0] <= TEXT('F'))
		a = szHex[0] - TEXT('A') + 10;
	else if (szHex[0] >= TEXT('a') && szHex[0] <= TEXT('f'))
		a = szHex[0] - TEXT('a') + 10;

	if (szHex[1] >= TEXT('0') && szHex[1] <= TEXT('9'))
		b = szHex[1] - TEXT('0');
	else if (szHex[1] >= TEXT('A') && szHex[1] <= TEXT('F'))
		b = szHex[1] - TEXT('A') + 10;
	else if (szHex[1] >= TEXT('a') && szHex[1] <= TEXT('f'))
		b = szHex[1] - TEXT('a') + 10;

	return a * 16 + b;
}

static void SetIdentifierAuthority(SID_IDENTIFIER_AUTHORITY&sia, DWORD val32)
{
	sia.Value[5] = (BYTE)((val32 & 0x000000FF) >> 0);
	sia.Value[4] = (BYTE)((val32 & 0x0000FF00) >> 8);
	sia.Value[3] = (BYTE)((val32 & 0x00FF0000) >> 16);
	sia.Value[2] = (BYTE)((val32 & 0xFF000000) >> 24);
	sia.Value[1] = sia.Value[0] = 0;
}

// Based on GetBinarySid function from http://www.codeguru.com/cpp/w-p/system/security/article.php/c5659.
BOOL Compat_ConvertStringSidToSidT(LPTSTR szSid, PSID* ppSid)
{
	// Try to call the real function on 2000+,
	// this will enable support for more SID Strings (Those will not work on NT4 so script carefully)
	FARPROC fp = AdvApi32_GetProcAddress(sizeof(TCHAR) > 1 ? "ConvertStringSidToSidW" : "ConvertStringSidToSidA");
	bool ret = fp && ((BOOL(WINAPI*)(LPCTSTR, PSID*))fp)(szSid, ppSid);

	if (ret) return ret;

	*ppSid = NULL;
	BYTE nByteAuthorityCount = 0;
	PSID pSid = LocalAllocZero(8 + (sizeof(DWORD) * SID_MAX_SUB_AUTHORITIES));

	if (!pSid) return false;

	// Building a revision 1 SID in memory, the rest of the code assumes that pSid is zero filled!
	((char*)pSid)[0] = 1;
	SID_IDENTIFIER_AUTHORITY &sidIA = *(SID_IDENTIFIER_AUTHORITY*)((char*)pSid + 2);
	DWORD *pSidSA = (DWORD*)((char*)pSid + 8);
	static const struct {
		char id0, id1;
		BYTE ia, sa0, sa1;
	} sidmap[] = {
		{'A' | 32, 'N' | 32, (5), (7) , 0}, // NT AUTHORITY\ANONYMOUS LOGON
		{'A' | 32, 'U' | 32, (5), (11), 0}, // NT AUTHORITY\Authenticated Users
		{'B' | 32, 'A' | 32, (5), (32), (544) - 500}, // BUILTIN\Administrators
		{'B' | 32, 'U' | 32, (5), (32), (545) - 500}, // BUILTIN\Users
		{'I' | 32, 'U' | 32, (5), (4) , 0}, // NT AUTHORITY\INTERACTIVE
		{'S' | 32, 'Y' | 32, (5), (18), 0}, // NT AUTHORITY\SYSTEM
		{'W' | 32, 'D' | 32, (1), (0) , 0}, // Everyone
	};

	// Try to lookup a SID string
	for (int i = 0; i < SIZE_OF_ARRAY(sidmap); ++i) {
		if ((szSid[0] | 32) != sidmap[i].id0 || (szSid[1] | 32) != sidmap[i].id1 || szSid[2]) continue;

		SetIdentifierAuthority(sidIA, sidmap[i].ia);
		pSidSA[nByteAuthorityCount++] = sidmap[i].sa0;

		if (sidmap[i].sa1) pSidSA[nByteAuthorityCount++] = (DWORD)sidmap[i].sa1 + 500;

		goto done;
	}

	// S-SID_REVISION- + identifierauthority- + subauthorities + NULL
	// Skip S
	PTSTR ptr;

	if (!(ptr = CharPos(szSid, lstrlen(szSid), TEXT('-'))))
		return FALSE;

	ptr++;

	// Skip SID_REVISION
	if (!(ptr = CharPos(ptr, lstrlen(ptr), TEXT('-'))))
		return FALSE;

	ptr++;
	// Skip identifierauthority
	PTSTR ptr1;

	if (!(ptr1 = CharPos(ptr, lstrlen(ptr), TEXT('-'))))
		return FALSE;

	*ptr1 = 0;

	if ((*ptr == TEXT('0')) && (*(ptr + 1) == TEXT('x'))) {
		sidIA.Value[0] = FromHex(ptr);
		sidIA.Value[1] = FromHex(ptr + 2);
		sidIA.Value[2] = FromHex(ptr + 4);
		sidIA.Value[3] = FromHex(ptr + 8);
		sidIA.Value[4] = FromHex(ptr + 10);
		sidIA.Value[5] = FromHex(ptr + 12);
	} else {
		SetIdentifierAuthority(sidIA, myatou(ptr));
	}

	// Skip -
	*ptr1 = TEXT('-'), ptr = ptr1, ptr1++;

	for (int i = 0; i < 8; i++) {
		// Get subauthority count.
		if (!(ptr = CharPos(ptr, lstrlen(ptr), TEXT('-')))) break;

		*ptr = 0, ptr++, nByteAuthorityCount++;
	}

	for (int i = 0; i < nByteAuthorityCount; i++) {
		// Get subauthority.
		pSidSA[i] = myatou(ptr1);
		ptr1 += lstrlen(ptr1) + 1;
	}

done:

	if (nByteAuthorityCount) {
		*ppSid = pSid, ((char*)pSid)[1] = nByteAuthorityCount;
		return TRUE;
	}

	LocalFree(pSid);
	return FALSE;
}

// Based on GetTextualSid function from http://www.codeguru.com/cpp/w-p/system/security/article.php/c5659.
BOOL ConvertSidToStringSidNoAlloc(const PSID pSid, LPTSTR pszSid)
{
	// Validate the binary SID
	if (!IsValidSid(pSid)) return FALSE;

	// Get the identifier authority value from the SID
	PSID_IDENTIFIER_AUTHORITY psia = GetSidIdentifierAuthority(pSid);
	// Get the number of subauthorities in the SID.
	DWORD dwSubAuthorities = *GetSidSubAuthorityCount(pSid);
	// Compute the buffer length
	// S-SID_REVISION- + IdentifierAuthority- + subauthorities- + NULL
	DWORD dwSidSize = (15 + 12 + (12 * dwSubAuthorities) + 1) * sizeof(TCHAR);
	// Add 'S' prefix and revision number to the string
	dwSidSize = wsprintf(pszSid, TEXT("S-%lu-"), SID_REVISION);

	// Add SID identifier authority to the string.
	if ((psia->Value[0] != 0) || (psia->Value[1] != 0)) {
		dwSidSize += wsprintf(pszSid + lstrlen(pszSid),
			TEXT("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
			(USHORT)psia->Value[0],
			(USHORT)psia->Value[1],
			(USHORT)psia->Value[2],
			(USHORT)psia->Value[3],
			(USHORT)psia->Value[4],
			(USHORT)psia->Value[5]);
	} else {
		dwSidSize += wsprintf(pszSid + lstrlen(pszSid),
			TEXT("%lu"),
			(ULONG)(psia->Value[5]) +
			(ULONG)(psia->Value[4] << 8) +
			(ULONG)(psia->Value[3] << 16) +
			(ULONG)(psia->Value[2] << 24));
	}

	// Add SID subauthorities to the string
	for (DWORD dwCounter = 0; dwCounter < dwSubAuthorities; dwCounter++) {
		dwSidSize += wsprintf(pszSid + dwSidSize, TEXT("-%lu"),
			*GetSidSubAuthority(pSid, dwCounter));
	}

	return TRUE;
}
// include
#include "stdio.h"
#include "wchar.h"
#include "windows.h"


// typedef
typedef BOOL (WINAPI *PFSETWINDOWTEXTW)(HWND hWnd, LPWSTR lpString);


// globals
FARPROC g_pOrgFunc = NULL;


// 사용자 후킹 함수
BOOL WINAPI MySetWindowTextW(HWND hWnd, LPWSTR lpString)
{
    wchar_t* pNum = L"영일이삼사오육칠팔구";
    wchar_t temp[2] = {0,};
    int i = 0, nLen = 0, nIndex = 0;

    nLen = wcslen(lpString);
    for(i = 0; i < nLen; i++)
    {
        // '수'문자를 '한글'문자로 변환
        //   lpString 은 wide-character (2 byte) 문자열
        if( L'0' <= lpString[i] && lpString[i] <= L'9' )
        {
            temp[0] = lpString[i];
            nIndex = _wtoi(temp);
            lpString[i] = pNum[nIndex];
        }
    }

    // user32!SetWindowTextW() API 호출
    //   (위에서 lpString 버퍼 내용을 변경하였음)
    return ((PFSETWINDOWTEXTW)g_pOrgFunc)(hWnd, lpString);
}


// hook_iat
//   현재 프로세스의 IAT 를 검색해서
//   pfnOrg 값을 pfnNew 값으로 변경시킴
BOOL hook_iat(LPCSTR szDllName, PROC pfnOrg, PROC pfnNew)
{
	HMODULE hMod;
	LPCSTR szLibName;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc; 
	PIMAGE_THUNK_DATA pThunk;
	DWORD dwOldProtect, dwRVA;
	PBYTE pAddr;

    // hMod, pAddr = ImageBase of calc.exe
    //             = VA to MZ signature (IMAGE_DOS_HEADER)
	hMod = GetModuleHandle(NULL);
	pAddr = (PBYTE)hMod;

    // pAddr = VA to PE signature (IMAGE_NT_HEADERS)
	pAddr += *((DWORD*)&pAddr[0x3C]);

    // dwRVA = RVA to IMAGE_IMPORT_DESCRIPTOR Table
	dwRVA = *((DWORD*)&pAddr[0x80]);

    // pImportDesc = VA to IMAGE_IMPORT_DESCRIPTOR Table
	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)hMod+dwRVA);

	for( ; pImportDesc->Name; pImportDesc++ )
	{
        // szLibName = VA to IMAGE_IMPORT_DESCRIPTOR.Name
		szLibName = (LPCSTR)((DWORD)hMod + pImportDesc->Name);
		if( !_stricmp(szLibName, szDllName) )
		{
            // pThunk = IMAGE_IMPORT_DESCRIPTOR.FirstThunk
            //        = VA to IAT(Import Address Table)
			pThunk = (PIMAGE_THUNK_DATA)((DWORD)hMod + 
                                         pImportDesc->FirstThunk);

            // pThunk->u1.Function = VA to API
			for( ; pThunk->u1.Function; pThunk++ )
			{
				if( pThunk->u1.Function == (DWORD)pfnOrg )
				{
                    // 메모리 속성을 E/R/W 로 변경
					VirtualProtect((LPVOID)&pThunk->u1.Function, 
                                   4, 
                                   PAGE_EXECUTE_READWRITE, 
                                   &dwOldProtect);

                    // IAT 값을 변경
                    pThunk->u1.Function = (DWORD)pfnNew;
					
                    // 메모리 속성 복원
                    VirtualProtect((LPVOID)&pThunk->u1.Function, 
                                   4, 
                                   dwOldProtect, 
                                   &dwOldProtect);						

					return TRUE;
				}
			}
		}
	}

	return FALSE;
}



BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch( fdwReason )
	{
		case DLL_PROCESS_ATTACH : 
            // original API 주소 저장
           	g_pOrgFunc = GetProcAddress(GetModuleHandle(L"user32.dll"), 
                                        "SetWindowTextW");

            // # hook
            //   user32!SetWindowTextW() 를 hookiat!MySetWindowText() 로 후킹
			hook_iat("user32.dll", g_pOrgFunc, (PROC)MySetWindowTextW);
			break;

		case DLL_PROCESS_DETACH :
            // # unhook
            //   calc.exe 의 IAT 를 원래대로 복원
            hook_iat("user32.dll", (PROC)MySetWindowTextW, g_pOrgFunc);
			break;
	}

	return TRUE;
}
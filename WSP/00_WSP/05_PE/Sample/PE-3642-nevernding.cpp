//---------------------------------------------------------------
// PE Section 분석 프로그램
// 제작 : 배영희(nevernding)
// 메일 : kodoc11 at hanmail dot net
// 관련문서 : http://blog.naver.com/nevernding/
// 제작일 : 2006/05/22
// 개발환경 : Microsoft Visual Studio 2003 Profecional(Visual C++ .NET)
//---------------------------------------------------------------

//#include "stdafx.h"
#include <windows.h>
#include <iostream>
using namespace std;
#include <TCHAR.h>

void restore_filemapping_resources(HANDLE hFile, HANDLE hFileMap, LPVOID pMapView)
{
	UnmapViewOfFile(pMapView);
	CloseHandle(hFileMap);
	CloseHandle(hFile);
}

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hFile, hFileMap;
	LPVOID pBaseAddr;
	
	// 섹션을 조사할 PE파일을 File-mapping을 이용하여 메모리에 올린다.
	hFile = CreateFile("C:\\Windows\\system32\\notepad.exe", GENERIC_READ, FILE_SHARE_READ, 
					NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(!hFile)		return 0;
	hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, "PE");
	if(!hFileMap)
	{
		CloseHandle(hFile);
		return 0;
	}
	// Create Map-view pointer
	pBaseAddr = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
	if(!pBaseAddr)
	{
		CloseHandle(hFileMap);
		CloseHandle(hFile);
		return 0;
	}

	// 올바른 PE파일인지 Dos/NT 헤더의 Signature를 조사한다.
	PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)pBaseAddr;
	if(pDosHdr->e_magic != IMAGE_DOS_SIGNATURE)
		restore_filemapping_resources(hFile, hFileMap, pBaseAddr);
	PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)((DWORD)pBaseAddr + (DWORD)pDosHdr->e_lfanew);
	if(pNtHdr->Signature != IMAGE_NT_SIGNATURE)
		restore_filemapping_resources(hFile, hFileMap, pBaseAddr);

	int iNumberOfSections;
	DWORD dwSectionAlignment, dwFileAlignment;
		
	// 섹션 정보(섹션갯수, Raw&Virtual상의 alignment) 저장과 출력
	iNumberOfSections = pNtHdr->FileHeader.NumberOfSections;
	dwSectionAlignment = pNtHdr->OptionalHeader.SectionAlignment;
	dwFileAlignment = pNtHdr->OptionalHeader.FileAlignment;
	printf("- Section Information -\n");
	printf(" Number of sections: %d\n", iNumberOfSections);
	printf(" Section Alignment : 0x%08X\n", dwSectionAlignment);
	printf(" File Alignment    : 0x%08X\n", dwFileAlignment);

	// 섹션헤더로 이동(섹션헤더는 IMAGE_OPTIONAL_HEADER, 즉 IMAGE_NT_HEADER 바로 뒤에 위치한다)
	PIMAGE_SECTION_HEADER pSecHdr = (PIMAGE_SECTION_HEADER)((DWORD)pNtHdr +
									sizeof(IMAGE_NT_HEADERS));

	// 섹션이름과 위치/크기 속성 정보를 출력.
	printf("%8s %10s %10s %10s %10s %10s\n", 
			"Name", "Raw Addr", "Raw Size", "Vir Addr", "Vir Size", "Characer");
	for(int i = 0; i < iNumberOfSections; i++)
	{
		printf("%8s 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X",
					pSecHdr->Name,										// 섹션이름
					pSecHdr->PointerToRawData, pSecHdr->SizeOfRawData,	// Raw데이터 주소/크기
					pSecHdr->VirtualAddress, pSecHdr->Misc.VirtualSize,	// Virtual데이터 주소/크기
					pSecHdr->Characteristics);							// 섹션 속성
		
		// 섹션 속성 플래그의 종류(Characteristics flags)
		// IMAGE_SCN_CTN_CODE				0x00000020		코드로 채워진 섹션
		// IMAGE_SCN_CTN_INITIALIZED_DATA	0x00000040		데이터가 초기화된 섹션
		// IMAGE_SCN_CTN_UNINITIALIZED_DATA	0x00000080		데이터가 비초기화된 섹션
		// IMAGE_SCN_MEM_EXECUTE			0x20000000		코드로서 실행될 수 있는 섹션
		// IMAGE_SCN_MEM_READ				0x40000000		읽기 가능영역 섹션
		// IMAGE_SCN_MEM_WRITE				0x80000000		쓰기 가능영역 섹션
		
		// 섹션 속성 검사
		// if(0x00000040 & pSecHdr->Characteristics)	printf("/Initialized");
		// if(0x00000080 & pSecHdr->Characteristics)	printf("/Uninitialized");
		if(0x20000000 & pSecHdr->Characteristics)	printf("/Code");
		if(0x40000000 & pSecHdr->Characteristics)	printf("/Read");
		if(0x80000000 & pSecHdr->Characteristics)	printf("/Write");
		puts("");

		pSecHdr++;		// 다음 섹션 정보로 이동
	}

	
	// 자원반환
	restore_filemapping_resources(hFile, hFileMap, pBaseAddr);

	return 0;
}
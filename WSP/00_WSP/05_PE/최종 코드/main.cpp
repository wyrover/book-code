#include <windows.h>
#include <iostream>
using namespace std;


void restore_filemapping_resources(HANDLE hFile, HANDLE hFileMap, LPVOID pMapView)
{
	UnmapViewOfFile(pMapView);
	CloseHandle(hFileMap);
	CloseHandle(hFile);
}

void main()
{
	// MMF
	HANDLE hFile, hFileMap;
	void *pBaseAddr;
	
	hFile = CreateFile("C:\\Windows\\system32\\notepad.exe", 
					GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);

	hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, "PE");

	pBaseAddr = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);

	//
	PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)pBaseAddr;

	PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)
		((DWORD)pBaseAddr + (DWORD)pDosHdr->e_lfanew);

	if(IMAGE_DOS_SIGNATURE != pDosHdr->e_magic || IMAGE_NT_SIGNATURE != pNtHdr->Signature)
	{
		printf("ERROR\n");
		restore_filemapping_resources(hFile, hFileMap, pBaseAddr);
	}

	//-----------------------------------------------------------------
	
	unsigned short iNumberOfSections;
	unsigned long dwSectionAlignment;
	unsigned long dwFileAlignment;
	// 섹션의 갯수와 alignment 정보를 알아낸다
	
	iNumberOfSections = pNtHdr->FileHeader.NumberOfSections;
	dwSectionAlignment = pNtHdr->OptionalHeader.SectionAlignment;
	dwFileAlignment = pNtHdr->OptionalHeader.FileAlignment;


	// 알아낸 정보를 출력

	printf("- Section Information -\n");
	printf(" Number of sections: %d\n", iNumberOfSections);
	printf(" Section Alignment : 0x%08X\n", dwSectionAlignment);
	printf(" File Alignment    : 0x%08X\n", dwFileAlignment);

	//-----------------------------------------------------------------

	//-----------------------------------------------------------------
	PIMAGE_SECTION_HEADER pSecHdr = (PIMAGE_SECTION_HEADER)
		((DWORD)pNtHdr + sizeof(IMAGE_NT_HEADERS));

	printf("%8s %10s %10s %10s %10s %10s\n",
		"Name", "Raw Addr", "Raw Size", "Vir Addr", "Vir Size", "Characer");

	for(int i = 0; i < iNumberOfSections; i++)
	{
		printf("%8s 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X",
			pSecHdr->Name,pSecHdr->PointerToRawData,
			pSecHdr->SizeOfRawData,
			pSecHdr->VirtualAddress,
			pSecHdr->Misc.VirtualSize,
			pSecHdr->Characteristics);
		// 섹션 속성 플래그의 종류(Characteristics flags)
		// IMAGE_SCN_CTN_CODE    0x00000020  코드로 채워진 섹션
		// IMAGE_SCN_CTN_INITIALIZED_DATA 0x00000040  데이터가 초기화된 섹션
		// IMAGE_SCN_CTN_UNINITIALIZED_DATA 0x00000080  데이터가 비초기화된 섹션
		// IMAGE_SCN_MEM_EXECUTE   0x20000000  코드로서 실행될 수 있는 섹션
		// IMAGE_SCN_MEM_READ    0x40000000  읽기 가능영역 섹션
		// IMAGE_SCN_MEM_WRITE    0x80000000  쓰기 가능영역 섹션
		
		// 섹션 속성 검사
		//if(0x00000040 & pSecHdr->Characteristics)	printf("/Initialized");
		//if(0x00000080 & pSecHdr->Characteristics)	printf("/Uninitialized");
		if(0x20000000 & pSecHdr->Characteristics)	printf("/Code");
		if(0x40000000 & pSecHdr->Characteristics)	printf("/Read");
		if(0x80000000 & pSecHdr->Characteristics)	printf("/Write");
		puts("");
		pSecHdr++;  // 다음 섹션 정보로 이동
	}
	//---------------------------------------------------------------- 

}
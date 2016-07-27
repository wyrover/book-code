/***************************************************************

  Windows에는 호환성을 위해 100개 가까운 파일 조작 API를 제공함 

  기본이 되는 것이 정리된 열기, 읽기, 쓰기, 닫기 처리를 하는
  API이다.

  이는 표준 C라이브러리 함수의 fopen, fread, fwrite, fclose에 
  해당된다.

***************************************************************/

//---------------------------------------------------
//  CreateFile API로 파일 접근
//---------------------------------------------------

/*

  기능     API			해당 라이브러리 함수
  --------------------------------------------------
  열기  CreateFile		fopen
  읽기	ReadFile		fread
  쓰기	WriteFile		fwrite
  닫기	CloseHandle		fclose
  --------------------------------------------------


  CreateFile 인자와 반환값

  HANDLE CreateFile(			// 반환값:파일 핸들
   LPCTSTR lpFileName,			// 파일명
   DWORD dwDesiredAccess,		// 접근 모드
   DWORD dwShareMode,			// 배타적 제어 지정
   LPSECURITY_ATTRIBUTES lpSA	// 보안
   DWORD dwCreationDisposition,	// 여는 방법
   DWORD dwFlagsAndAttributes,	// 파일 속성 플래그(거의 사용 안함)
   HANDLE hTemplateFile			// 템플릿 파일 핸들(보통 NULL사용)
   );

   접근모드 : 읽기(GENERIC_READ)/쓰기(GENERIC_WRITE)

   여는방법 : CREATE_NEW		- 신규파일 생성,같은 파일존재시에러
			  CREATE_ALWAYS		- 같은 파일 존재시 기존 파일 사라짐
								  항상 성공
			  OPEN_EXISTING		- 기존 파일 열기, 없으면 에러
			  OPEN_ALWAYS		- 기존 파일 열기, 없으면 생성
			  TRUNCATE_EXISTING	- 기존 파일 열기, 기존 파일 제거

*/


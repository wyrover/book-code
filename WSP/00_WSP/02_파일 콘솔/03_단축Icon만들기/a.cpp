#include <iostream.h>
#include <windows.h>
#include <shlobj.h>

void Create( char* path, wchar_t* lnk)
{
	HRESULT ret;		
	// ShellLink 컴포넌트를 Load한다. IShellLink 인터페이스를 요청한다.
	IShellLink* pLink;  
	ret = CoCreateInstance( CLSID_ShellLink, // 컴포넌트 ID
								0,               // 통합.(0 사용안함.
								CLSCTX_SERVER,  // Process안에서 수행.
								IID_IShellLink,  // 원하는 Interface ID
								(void**)&pLink );
	if ( FAILED(ret))
	{
		cout << "Component를 Load할수 없습니다." << endl;
		return ;
	}
	// component사용
	pLink->SetPath( path );
	pLink->SetDescription("");
	pLink->SetArguments("");
	
	// IPersistFile 인터페이스를 얻는다.
	IPersistFile* pFile;
	pLink->QueryInterface( IID_IPersistFile, (void**)&pFile );
	
	pFile->Save( lnk, TRUE );
	
	// 이제 interface를 해지한다.
	pFile->Release();
	pLink->Release();
}

void main()
{
	CoInitialize(0);
	
	Create("C:\\WINDOWS\\system32\\calc.exe", 
		
		L"C:\\Documents and Settings\\All Users\\바탕 화면\\a.lnk");//Unicode로 넘기기위해 L 지시어를 붙인다.
	
	CoUninitialize();
}
// MSDN에서 IShellLink




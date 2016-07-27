#pragma comment(linker, "/subsystem:windows")

#include <windows.h>

#include <commctrl.h> // 공용 컨트롤 헤더.
#pragma comment( lib, "comctl32.lib") // comctl32.dll을 사용하기 위해.

// Tree 에 항목을 추가하는 함수.
HTREEITEM AddItem( HWND hTree, HTREEITEM hParent, char* s )
{
	TVINSERTSTRUCT tv;
	tv.hParent      = hParent;  // << -- 수정 하세요.
	tv.hInsertAfter = TVI_SORT;
	tv.item.pszText = s;  // << ------ 수정.
	tv.item.iImage  = 0;
	tv.item.iSelectedImage = 1;
	tv.item.mask    = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	HTREEITEM hItem = (HTREEITEM)SendMessage( hTree, TVM_INSERTITEM, 0,
												(LPARAM)&tv);	
	return hItem;  // << 추가하세요.
}


void EnumFiles( HWND hTree, HTREEITEM hParent, char* path ) // << 수정.
{
	SetCurrentDirectory( path );
	WIN32_FIND_DATA wfd;

	HANDLE hFind = FindFirstFile( "*.*", &wfd );
	do
	{
		if ( ( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) &&
			!( wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ) )
		{
			//=========================================
			if ( strcmp( wfd.cFileName, ".")  != 0 &&
				 strcmp( wfd.cFileName, "..") != 0 )
			{
				
				HTREEITEM hItem = AddItem( hTree, hParent, wfd.cFileName);
					
				EnumFiles( hTree, hItem, wfd.cFileName ); // 재귀 호출.



				SetCurrentDirectory("..");  // 다시 상위 폴더로.!
			}
			//=========================================
		}

	} while ( FindNextFile( hFind, &wfd ) );

	FindClose( hFind );
}








LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hTree;
	
	switch( msg )
	{	
	case WM_CREATE:
			
		InitCommonControls(); // 공용 컨트롤 초기화.

		// 1. Tree Control 생성하기.
		hTree = CreateWindow( WC_TREEVIEW, "",
			
			WS_CHILD | WS_VISIBLE | WS_BORDER | TVS_HASLINES | TVS_LINESATROOT |
			TVS_HASBUTTONS,

							10,10,300,500, hwnd, (HMENU)1, 0, 0);

		{
			// 2. Tree에 ImageList 연결하기
			HIMAGELIST hImg = ImageList_Create( 16,15,	// image 크기.
								ILC_COLOR16 | ILC_MASK, // 색상 | Mask 사용여부
											2,  // 초기 크기
											0); // Grow Size
			// 비트맵 Load
			HBITMAP hBitmap = (HBITMAP)LoadImage( 0, "folder.bmp",
												IMAGE_BITMAP, 0, 0, 
												LR_LOADFROMFILE );

			ImageList_AddMasked( hImg, hBitmap, RGB(0,128,128));
			DeleteObject( hBitmap ); 

			// Tree에 ImageList를 연결한다.
			SendMessage( hTree, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM)hImg);
		}


		// 2. Tree 항목 추가.!! - 해당하는 구조체를 만들어서 메세지로 전달한다.
		{
			EnumFiles( hTree, TVI_ROOT, "C:\\Program Files");

		}	
		return 0;











	case WM_DESTROY:
		PostQuitMessage(0); 
		return 0;
	}
	return DefWindowProc( hwnd, msg, wParam, lParam);
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPSTR	  lpCmdLine, int nShowCmd )
{
	ATOM atom;
	WNDCLASS wc;
	HWND hwnd;
	MSG msg;
	
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hbrBackground= (HBRUSH)GetStockObject( WHITE_BRUSH ); 
	wc.hCursor		= LoadCursor( 0, IDC_ARROW );
	wc.hIcon		= LoadIcon( 0, IDI_APPLICATION);
	wc.hInstance	= hInstance;
	wc.lpfnWndProc  = WndProc; 
	wc.lpszClassName= "First";
	wc.lpszMenuName = 0;
	wc.style		= 0;

	atom = RegisterClass( &wc);
	
	if ( atom == 0 ) 
	{
		MessageBox( 0, "Fail To RegisterClass", "Error", MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx( 0, "first", "Hello", WS_OVERLAPPEDWINDOW,
								CW_USEDEFAULT, 0, CW_USEDEFAULT,0, 0, 0, 
								hInstance, 0); 
	ShowWindow( hwnd, nShowCmd); 
	UpdateWindow( hwnd );

	while ( GetMessage( &msg, 0, 0, 0) ) 
	{							
		TranslateMessage(&msg);
		DispatchMessage( &msg);
	}

	return 0;
}


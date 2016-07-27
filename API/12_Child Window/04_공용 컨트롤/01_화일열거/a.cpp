#include <stdio.h>
#include <windows.h>

void EnumFiles( char* path )
{
	SetCurrentDirectory( path );
	WIN32_FIND_DATA wfd;

	HANDLE hFind = FindFirstFile( "*.*", &wfd );
	do
	{
		if ( ( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) &&
			!( wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ) )
			printf( "%s\n", wfd.cFileName );

	} while ( FindNextFile( hFind, &wfd ) );

	FindClose( hFind );
}


/*
// 재귀 호출 버전.
void EnumFiles( char* path )
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
			if ( strcmp( wfd.cFileName, ".") != 0 &&
				 strcmp( wfd.cFileName, "..")!= 0 )
			{
				printf( "%s\n", wfd.cFileName );
					
				EnumFiles( wfd.cFileName ); // 재귀 호출.
				SetCurrentDirectory("..");  // 다시 상위 폴더로.!
			}
			//=========================================
		}

	} while ( FindNextFile( hFind, &wfd ) );

	FindClose( hFind );
}
*/











void main()
{
	EnumFiles( "C:\\");
}











#include "stdio.h"
#include "windows.h"

int get_folder_count(LPCSTR szPath)
{
	int				    nCount = 0;
	char			    szFile[MAX_PATH] = {0,};
	HANDLE			    hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATAA	fd;

	wsprintfA(szFile, "%s\\*.*", szPath);

	hFind = FindFirstFileA(szFile, &fd);
	if( INVALID_HANDLE_VALUE == hFind )
	{
		return 0;
	}

	do
	{
		if( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			(fd.cFileName[0] != '.') ) 
		{
			nCount++;
		}
	} while( FindNextFileA(hFind, &fd) );

	FindClose(hFind);

	return nCount;
}

int main(int argc, char* argv[])
{
    int     nFolderCount = 0;

    if( argc != 2 )
    {
        printf(" USAGE : FolderCount.exe <path>\n");
        return 1;
    }

    nFolderCount = get_folder_count(argv[1]);
    printf("folder count of <%s> is %d\n", argv[1], nFolderCount);

    return 0;
}
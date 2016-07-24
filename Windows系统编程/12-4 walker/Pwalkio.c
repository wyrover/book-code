
#include "pwalk.h"
#include <commdlg.h>


/* global file name variables */
char	szExeFilterSpec[128] = "Executable Files (*.EXE)\0*.EXE\0";


/* call the OpenFile common dialog to get a filename */
BOOL WINAPI GetFileName (
    HWND    hWnd,
    char    *lpszFilePath,
    char    *lpszExt)
{
    OPENFILENAME    ofn;
    char	    szFileOpen[25];
    char	    szExt[10];
    char	    szTitle[MAX_PATH];

    *szTitle = 0;

    if (lpszExt && *lpszExt)
	strcpy (szExt, lpszExt);
    else
	LoadString ((HANDLE)GetModuleHandle (NULL),
	    IDS_EXEFILEEXT,
	    szExt,
	    sizeof (szExt));

    LoadString ((HANDLE)GetModuleHandle (NULL),
        IDS_FILEOPENTITLE,
	szFileOpen,
	sizeof (szFileOpen));

    ofn.lStructSize	  = sizeof (OPENFILENAME);
    ofn.hwndOwner	  = NULL;
    ofn.lpstrFilter	  = szExeFilterSpec;
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter    = 0;
    ofn.nFilterIndex      = 0;
    ofn.lpstrFile	  = lpszFilePath;
    ofn.nMaxFile	  = MAX_PATH;
    ofn.lpstrInitialDir   = NULL;
    ofn.lpstrFileTitle	  = szTitle;
    ofn.nMaxFileTitle     = MAX_PATH;
    ofn.lpstrTitle	  = szFileOpen;
    ofn.lpstrDefExt	  = szExt;
    ofn.Flags		  = OFN_FILEMUSTEXIST;

    /* call common open dialog and return result */
    return (GetOpenFileName ((LPOPENFILENAME)&ofn));
}



/* invokes the saveas common dialog to retrieve a file name */
/* function retrieves the filename from the path */
void WINAPI GetFileFromPath (
    char    *lpszFullPath,
    char    *lpszFile)
{
    char    *lpPtr = lpszFullPath + strlen (lpszFullPath);

    /* file is at end of path, so search backwards to first \ or : char */
    while (lpPtr > lpszFullPath)
    {
    if (*lpPtr == '\\' ||
        *lpPtr == ':')
        {
        lpPtr++;
        break;
        }
    lpPtr = CharPrev(lpszFullPath, lpPtr);
    }

    /* return filename if found, or full path passed in */
    strcpy (lpszFile, lpPtr);
}




/* validate filename as executable image  */
BOOL WINAPI IsValidFile (
    char    *lpszFilename)
{
    OFSTRUCT	of;
    int 	nLen;
    char	*pStr;

    /* validate filename pointer */
    if (lpszFilename == NULL ||
	!*lpszFilename)
	return FALSE;

    /* open the file for existance */
    if (OpenFile (lpszFilename, &of, OF_EXIST) == -1)
	/* fail validation */
	return FALSE;

    /* test the extension is .EXE */
    nLen = strlen (lpszFilename);
    pStr = lpszFilename + nLen - 4;
    if (!stricmp (pStr, ".EXE"))
	/* pass validation */
	return TRUE;

    /* fail validation */
    return FALSE;
}




/* get win32 command line parameters */
BOOL WINAPI GetCmdLine(
    char    *lpStr,
    char    *lpszCmdLine,
    BOOL    *bBkgnd)
{
    if (*lpStr)
	{
	/* skip application name which precedes parameters */
	while (*lpStr != ' ' && *lpStr != 0)
		lpStr = CharNext(lpStr);

	/* skip spaces */
	while (*lpStr == ' ' && *lpStr != 0)
		lpStr = CharNext(lpStr);

	/* indeed command line parameter(s) present */
	if (*lpStr != 0)
	    {
	    /* if background switch, set flag and remove switch from command line */
	    if ((*lpStr == '/' || *lpStr == '-') &&
		(*(lpStr+1) == 'b' || *(lpStr+1) == 'B'))
		{
		*bBkgnd = TRUE;
		lpStr += 2;

		if (*lpStr == 0)
		    *lpszCmdLine = 0;
		else
		    strcpy (lpszCmdLine, lpStr);
		}
	    /* maybe switch is embedded in parameter(s) somewhere */
	    else
		{
		char	*pStr = lpStr;
		char	*pCmdLine = lpszCmdLine;
		int	i, nCnt;

		while (*pStr != 0)
		    {
		    /* background switch is set, so prepare parameters and set flag */
		    if ((*pStr == '/' || *pStr == '-') &&
			(*(pStr+1) == 'b' || *(pStr+1) == 'B'))
			{
			*bBkgnd = TRUE;

			/* copy from beg. of lpStr to *pStr to lpszCmdLine */
			nCnt = pStr - lpStr;
			for (i=0; i<nCnt; i++)
			    lpszCmdLine[i] = lpStr[i];
			lpszCmdLine[i] = 0;
			strcat (lpszCmdLine, (pStr+2));

			/* break from loop */
			break;
			}

		    pStr = CharNext(pStr);
		    }

		/* no switch found, can only edit one file, remove extra parameters */
		if (*pStr == 0)
		    {
		    pStr = lpStr;

		    while (*pStr != ' ' && *pStr != 0)
			pStr = CharNext(pStr);

		    if (*pStr == ' ')
			*pStr = 0;

		    strcpy (lpszCmdLine, lpStr);
		    }
		}
	    }
	else
	    return FALSE;
	}
    else
	return FALSE;

    return TRUE;
}

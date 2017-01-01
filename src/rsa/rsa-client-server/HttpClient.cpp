/*++

Copyright (c) Cheetah Mobile.  All rights reserved.

Module Name:

    HttpClient.cpp

Abstract:

    communicate with the remote server via http protocol

--*/


#include "HttpClient.h"
//#include "LocalDNS.h"

#include <WinCred.h>


namespace {

	///////////////////////////////////////////////////////////////////////////////
	// CmStringHelper
	class CmStringHelper
	{
	public:
		static BOOL isValidPointer(LPCTSTR szString)
		{
			return szString && *szString;
		}

		static CString D2A(int nValue)
		{
			CString strValue;
			strValue.Format(_T("%d"), nValue);
			return strValue;
		}

		static CString D2A(DWORD nValue)
		{
			CString strValue;
			strValue.Format(_T("%d"), nValue);
			return strValue;
		}

		static CString GUID2Str(GUID stGuid)
		{
			CString strGuid;
			strGuid.Format(_T("{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}"), 
				stGuid.Data1, 
				stGuid.Data2,
				stGuid.Data3,
				stGuid.Data4[0],
				stGuid.Data4[1],
				stGuid.Data4[2],
				stGuid.Data4[3],
				stGuid.Data4[4],
				stGuid.Data4[5],
				stGuid.Data4[6],
				stGuid.Data4[7]);

			return strGuid;
		}

		static GUID Str2GUID(LPCTSTR szGuid)
		{
			DWORD v[11];
			GUID stGuid; 
			_stscanf_s(szGuid, _T("{%8x-%4x-%4x-%2x%2x-%2x%2x%2x%2x%2x%2x}"), 
				&(v[0]), 
				&(v[1]), 
				&(v[2]), 
				&(v[3]), 
				&(v[4]), 
				&(v[5]), 
				&(v[6]), 
				&(v[7]), 
				&(v[8]), 
				&(v[9]), 
				&(v[10]));

			stGuid.Data1		= v[0];
			stGuid.Data2		= WORD(v[1]);
			stGuid.Data3		= WORD(v[2]);
			stGuid.Data4[0]		= BYTE(v[3]);
			stGuid.Data4[1]		= BYTE(v[4]);
			stGuid.Data4[2]		= BYTE(v[5]);
			stGuid.Data4[3]		= BYTE(v[6]);
			stGuid.Data4[4]		= BYTE(v[7]);
			stGuid.Data4[5]		= BYTE(v[8]);
			stGuid.Data4[6]		= BYTE(v[9]);
			stGuid.Data4[7]		= BYTE(v[10]);

			return stGuid;
		}

		static DWORDLONG Str2Ver(LPCTSTR szVersion)
		{
			DWORDLONG dwVersion = 0;
			CString strVersion = szVersion;
			std::vector<CString> vecVersion;
			WORD wVer[4] = {0};

			strVersion += _T(".0.0.0");
			if(Split(strVersion, '.', vecVersion))
			{				
				for (size_t i = 0; i < vecVersion.size() && i < 4; ++i)
				{
					wVer[i] = _ttoi(vecVersion[i]);
				}

				dwVersion = (DWORDLONG(wVer[0]) << 48) | (DWORDLONG(wVer[1]) << 32) | (DWORDLONG(wVer[2]) << 16) | wVer[3];
			}

			return dwVersion;
		}

		static CString Ver2Str(DWORDLONG dwVersion)
		{
			CString strVersion;
			DWORD dwHi = DWORD(dwVersion >> 32);
			DWORD dwLo = DWORD(dwVersion);

			strVersion.Format(_T("%d.%d.%d.%d"),
				dwHi >> 16,
				dwHi & 0xffff,
				dwLo >> 16,
				dwLo & 0xffff);

			return strVersion;
		}	

		static CString Ver2Str(DWORD dwHighVersion, DWORD dwLowVersion)
		{
			CString strVersion;

			strVersion.Format(_T("%d.%d.%d.%d"),
				dwHighVersion >> 16,
				dwHighVersion & 0xffff,
				dwLowVersion >> 16,
				dwLowVersion & 0xffff);

			return strVersion;
		}

		static CString Size2Str(ULONGLONG ulSize)
		{
			CString strSize;
			float fSize = 0.0f;

			if (ulSize > 1024*1024*1024)
			{
				fSize = (float)ulSize / (1024*1024*1024);
				strSize.Format(L"%1.1fGB", fSize);
			}
			else if (ulSize > 1024*1024)
			{
				fSize = (float)ulSize / (1024*1024);
				strSize.Format(L"%1.1fMB", fSize);
			}
			else if (ulSize > 1024)
			{
				fSize = (float)ulSize / 1024;
				strSize.Format(L"%1.1fKB", fSize);
			}
			else
			{
				strSize.Format(L"%dB", ulSize);
			}
			return strSize;
		}

		static BOOL Split(LPCTSTR szStrings, TCHAR cSeparator, std::vector<CString>& vecStrList, BOOL bCanEmpty = FALSE)
		{
			BOOL bResult = FALSE;

			if(isValidPointer(szStrings))			
			{
				CString strSrc(szStrings);
				int nStart	= 0;
				int nEnd	= nStart;
				int nLen    = 0;
				int nSize	= strSrc.GetLength();

				do
				{
					nStart		= nEnd;
					nEnd		= strSrc.Find(cSeparator, nStart);
					nLen    	= 0;
					if (-1 == nEnd)
					{
						nEnd	= nSize;
						nLen	= nEnd - nStart;
					}
					else
						nLen	= nEnd - nStart;

					if (nLen > 0 || bCanEmpty)
					{
						CString strElement;
						strElement = strSrc.Mid(nStart, nLen);
						vecStrList.push_back(strElement);
					}

				}while (++ nEnd < nSize || (bCanEmpty && nEnd == nSize));

				bResult = TRUE;
			}

			return bResult;
		}

		static CStringA Unicode2Utf8(LPCWSTR szValue)
		{	
			CStringA strResult;

			if ( !szValue || !*szValue )
				return strResult;

			int nSize = ::WideCharToMultiByte(CP_UTF8, 0, szValue, -1, NULL, 0, NULL, NULL);	
			if(nSize > 0)
			{
				nSize++;			
				if(::WideCharToMultiByte(CP_UTF8, 0, szValue, -1, strResult.GetBuffer(nSize), nSize, NULL, NULL) == 0)
				{
					strResult.ReleaseBuffer(0);
					strResult.Empty();
				}
				else
					strResult.ReleaseBuffer();
			}

			return strResult;
		}

		static CString Utf82Unicode(LPCSTR szValue)
		{
			CString strResult;

			if ( !szValue || !*szValue )
				return strResult;

			int nSize = int(strlen(szValue));	
			std::vector<WCHAR> buf(++nSize);

			if(::MultiByteToWideChar(CP_UTF8, 0, szValue, nSize, &buf[0], nSize) != 0)
			{
				strResult = CString(&buf[0]);
			}

			return strResult;
		}

		static CStringA Unicode2GB(LPCWSTR szValue)
		{
			CStringA strResult;

			if ( !szValue || !*szValue )
				return strResult;

			int nSize = int(wcslen(szValue) * sizeof(WCHAR));	
			std::vector<CHAR> buf(++nSize);

			if(::WideCharToMultiByte(936, 0, szValue, int(wcslen(szValue)), &buf[0], nSize, NULL, NULL) != 0)
			{
				strResult = CStringA(&buf[0]);
			}

			return strResult;
		}

		static CString GB2Unicode(LPCSTR szValue)
		{
			CString strResult;

			if ( !szValue || !*szValue )
				return strResult;

			int nSize = int(strlen(szValue));	
			std::vector<WCHAR> buf(++nSize);

			if(::MultiByteToWideChar(936, 0, szValue, nSize, &buf[0], nSize) != 0)
			{
				strResult = CString(&buf[0]);
			}

			return strResult;
		}
	};



	///////////////////////////////////////////////////////////////////////////////
	// CmDirectoryHelper

#define PATH_DELIM                      _T("\\")
#define DRIVE_DELIM                     _T(":")
#define URI_DELIM                       _T("/")
#define WILDCARD          			    _T("*")
#define RELATIVE_CURRENT_PATH           _T(".")
#define RELATIVE_PARENT_PATH            _T("..")

#define PATH_DELIM_CHAR                 '\\'
#define URI_DELIM_CHAR                  '/'
#define EXTENSION_DELIM_CHAR            '.'

	typedef BOOL (*DirectoryWalkFunc) (LPCTSTR szPath, const LPWIN32_FIND_DATA lpFindFileData);

	class CmDirectoryHelper 
	{
	private:
		static BOOL ExistsLockedOrShared(LPCTSTR szFileName)
		{
			WIN32_FIND_DATA fdFindData;
			HANDLE hFindFile = ::FindFirstFile(szFileName, &fdFindData);
			if(hFindFile != INVALID_HANDLE_VALUE)
			{
				::FindClose(hFindFile);
				return (fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
			}
			else
				return FALSE;
		}

	public:
		static BOOL DirectoryExists(LPCTSTR szDirectory)
		{
			DWORD dwAttr = ::GetFileAttributes(szDirectory);
			return dwAttr != INVALID_FILE_ATTRIBUTES && (FILE_ATTRIBUTE_DIRECTORY & dwAttr) != 0;
		}

		static BOOL Delete(LPCTSTR szDirectory)
		{
			WIN32_FIND_DATA fdFindData;
			HANDLE hFindFile;
			CString strPath = IncludeTrailingPathDelimiter(szDirectory), strSubPath;

			if(strPath.IsEmpty())
				return FALSE;

			hFindFile = ::FindFirstFile(strPath + WILDCARD, &fdFindData);
			if(hFindFile != INVALID_HANDLE_VALUE)
			{
				do
				{
					if(_tcsicmp(fdFindData.cFileName, RELATIVE_CURRENT_PATH) == 0 ||
						_tcsicmp(fdFindData.cFileName, RELATIVE_PARENT_PATH) == 0)
						continue;

					// go recursive in subdirectories
					strSubPath = strPath + fdFindData.cFileName;
					if((fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
					{
						Delete(strSubPath);
					}
					else if((fdFindData.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0)
					{
						::SetFileAttributes(strSubPath, fdFindData.dwFileAttributes & (~FILE_ATTRIBUTE_READONLY));		
						::DeleteFile(strSubPath);       
					} 
					else 
					{
						::DeleteFile(strSubPath); 
					}

				}
				while(::FindNextFile(hFindFile, &fdFindData));

				::FindClose(hFindFile);
			}

			return ::RemoveDirectory(strPath);
		}

		static CString ChangeFileExt(LPCTSTR szPath, LPCTSTR szNewExt)
		{
			CString strFileName = szPath;
			int nIndex;

			if(!strFileName.IsEmpty())
			{
				nIndex = strFileName.ReverseFind(EXTENSION_DELIM_CHAR);
				if(nIndex > -1)
				{
					nIndex++;
					strFileName.Delete(nIndex, strFileName.GetLength() - nIndex);
					strFileName += szNewExt;
				}
				else
					strFileName += szNewExt;
			}

			return strFileName;
		}

		static CString ExtractFileExt(LPCTSTR szPath)
		{
			CString strFileExt = szPath;
			int nIndex = strFileExt.ReverseFind(EXTENSION_DELIM_CHAR);
			if(nIndex > -1)
				strFileExt.Delete(0, nIndex);
			return strFileExt;
		}

		static CString ExtractFilePath(LPCTSTR szFileName)
		{
			CString strPath = szFileName;
			int nIndex = strPath.ReverseFind(PATH_DELIM_CHAR);
			if(nIndex > -1)
				strPath.Delete(nIndex, strPath.GetLength() - nIndex);
			return strPath;
		}

		static CString ExtractFileName(LPCTSTR szPath)
		{
			CString strFileName = szPath;
			int nIndex = strFileName.ReverseFind(PATH_DELIM_CHAR);
			if(nIndex > -1)
				strFileName.Delete(0, nIndex + 1);
			return strFileName;
		}

		static CString ExtractFileNameFromURI(LPCTSTR szUri)
		{
			CString strFileName = szUri;
			int nIndex = strFileName.ReverseFind(URI_DELIM_CHAR);
			if(nIndex > -1)
				strFileName.Delete(0, nIndex + 1);
			return strFileName;
		}

		static CString ExtractFileNameNoExt(LPCTSTR szFileName)
		{
			CString strName = szFileName;
			int nIndex = strName.ReverseFind(PATH_DELIM_CHAR);		
			if(nIndex > -1)
				strName.Delete(0, nIndex + 1);
			nIndex = strName.ReverseFind(EXTENSION_DELIM_CHAR);	
			if(nIndex > -1)
				strName.Delete(nIndex, strName.GetLength() - nIndex);
			return strName;
		}

		static CString ExcludeTrailingPathDelimiter(LPCTSTR szPath)
		{		
			CString strNewPath = szPath;
			int nIndex;
			if(!strNewPath.IsEmpty())
			{
				nIndex = strNewPath.GetLength() - 1;
				if(strNewPath[nIndex] == PATH_DELIM_CHAR)
					strNewPath.Delete(nIndex);
			}
			return strNewPath;
		}

		static BOOL FileExists(LPCTSTR szFileName)
		{
			BOOL bResult = FALSE;
			DWORD dwLastError;
			DWORD dwAttr = ::GetFileAttributes(szFileName);

			if(dwAttr != INVALID_FILE_ATTRIBUTES)
				bResult = (FILE_ATTRIBUTE_DIRECTORY & dwAttr) == 0;
			else
			{
				dwLastError = ::GetLastError();
				if(dwLastError != ERROR_FILE_NOT_FOUND && dwLastError != ERROR_PATH_NOT_FOUND &&
					dwLastError != ERROR_INVALID_NAME)
					bResult = ExistsLockedOrShared(szFileName);
			}

			return bResult;
		}

		static BOOL FindDirectory(LPCTSTR szDirectory, LPCTSTR szFilter, std::vector<CString> &vecFileNames)
		{
			BOOL bResult = FALSE;

			WIN32_FIND_DATA fdFindData;
			CString strSearchFullPath = IncludeTrailingPathDelimiter(szDirectory);
			CString strPath;
			CString strSubPath;

			strPath = strSearchFullPath + szFilter;
			HANDLE hFindFile = ::FindFirstFile(strPath, &fdFindData);
			if(hFindFile != INVALID_HANDLE_VALUE)
			{			
				do
				{
					if(_tcsicmp(fdFindData.cFileName, RELATIVE_CURRENT_PATH) == 0 ||
						_tcsicmp(fdFindData.cFileName, RELATIVE_PARENT_PATH) == 0)
						continue;

					if(fdFindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
					{
						vecFileNames.push_back(strSearchFullPath + fdFindData.cFileName);
						bResult = TRUE;
					}		
				}
				while(::FindNextFile(hFindFile, &fdFindData));

				::FindClose(hFindFile);
			}
			return bResult;
		}

		static BOOL FindFile(LPCTSTR szDirectory, LPCTSTR szFilter, std::vector<CString> &vecFileNames, BOOL Recursive = TRUE)
		{
			BOOL bResult = FALSE;

			WIN32_FIND_DATA fdFindData;
			CString strSearchFullPath = IncludeTrailingPathDelimiter(szDirectory);
			CString strPath;
			CString strSubPath;

			strPath = strSearchFullPath + szFilter;
			HANDLE hFindFile = ::FindFirstFile(strPath, &fdFindData);
			if(hFindFile != INVALID_HANDLE_VALUE)
			{			
				do
				{
					if(_tcsicmp(fdFindData.cFileName, RELATIVE_CURRENT_PATH) == 0 ||
						_tcsicmp(fdFindData.cFileName, RELATIVE_PARENT_PATH) == 0)
						continue;

					if((fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
					{
						vecFileNames.push_back(strSearchFullPath + fdFindData.cFileName);
						bResult = TRUE;
					}		
				}
				while(::FindNextFile(hFindFile, &fdFindData));

				::FindClose(hFindFile);
			}

			if(Recursive)
			{
				strPath = strSearchFullPath + WILDCARD;
				hFindFile = ::FindFirstFile(strPath, &fdFindData);
				if(hFindFile != INVALID_HANDLE_VALUE)
				{
					do
					{
						if(_tcsicmp(fdFindData.cFileName, RELATIVE_CURRENT_PATH) == 0 ||
							_tcsicmp(fdFindData.cFileName, RELATIVE_PARENT_PATH) == 0)
							continue;

						if((fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
						{
							strSubPath = strSearchFullPath + fdFindData.cFileName;
							bResult |= FindFile(strSubPath, szFilter, vecFileNames, Recursive);
						}		
					}
					while(::FindNextFile(hFindFile, &fdFindData));

					::FindClose(hFindFile);
				}
			}		

			return bResult;
		}

		static CString FindFirstFile(LPCTSTR szDirectory, LPCTSTR szFilter, BOOL Recursive = TRUE)
		{
			CString strResult;

			WIN32_FIND_DATA fdFindData;
			CString strSearchFullPath = IncludeTrailingPathDelimiter(szDirectory);
			CString strPath;
			CString strSubPath;

			strPath = strSearchFullPath + szFilter;
			HANDLE hFindFile = ::FindFirstFile(strPath, &fdFindData);
			if(hFindFile != INVALID_HANDLE_VALUE)
			{			
				do
				{
					if(_tcsicmp(fdFindData.cFileName, RELATIVE_CURRENT_PATH) == 0 ||
						_tcsicmp(fdFindData.cFileName, RELATIVE_PARENT_PATH) == 0)
						continue;

					if((fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
					{
						strResult = strSearchFullPath + fdFindData.cFileName;
						break;
					}		
				}
				while(::FindNextFile(hFindFile, &fdFindData));

				::FindClose(hFindFile);
			}

			if(strResult.IsEmpty() && Recursive)
			{
				strPath = strSearchFullPath + WILDCARD;
				hFindFile = ::FindFirstFile(strPath, &fdFindData);
				if(hFindFile != INVALID_HANDLE_VALUE)
				{
					do
					{
						if(_tcsicmp(fdFindData.cFileName, RELATIVE_CURRENT_PATH) == 0 ||
							_tcsicmp(fdFindData.cFileName, RELATIVE_PARENT_PATH) == 0)
							continue;

						if((fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
						{
							strSubPath = strSearchFullPath + fdFindData.cFileName;
							strResult = FindFirstFile(strSubPath, szFilter, Recursive);
							if(!strResult.IsEmpty())
								break;
						}		
					}
					while(::FindNextFile(hFindFile, &fdFindData));

					::FindClose(hFindFile);
				}
			}		

			return strResult;
		}

		static BOOL ForceDirectories(LPCTSTR szDirectory)
		{
			CString strDir = ExcludeTrailingPathDelimiter(szDirectory);

			if(strDir.IsEmpty())
				return FALSE;

			// avoid 'xyz:\' problem.
			if(strDir.GetLength() < 3 || DirectoryExists(strDir) || ExtractFilePath(strDir).Compare(strDir) == 0)
				return TRUE; 

			return ForceDirectories(ExtractFilePath(strDir)) && CreateDirectory(strDir, NULL);
		}

		static CString IncludeTrailingPathDelimiter(LPCTSTR szPath)
		{		
			CString strNewPath = szPath;
			if(!strNewPath.IsEmpty())
			{
				if(strNewPath[strNewPath.GetLength() - 1] != PATH_DELIM_CHAR)
					strNewPath += PATH_DELIM;
			}
			return strNewPath;
		}

		static void WalkThroughDirectory(LPCTSTR szSearchPath, LPCTSTR szFilter,
			DirectoryWalkFunc PreCallback, DirectoryWalkFunc PostCallback, BOOL Recursive = TRUE)
		{
			WIN32_FIND_DATA fdFindData;
			CString strSearchFullPath = IncludeTrailingPathDelimiter(szSearchPath);
			CString strPath = strSearchFullPath, strSubPath;
			HANDLE hFindFile = INVALID_HANDLE_VALUE;

			if(szFilter == NULL)
			{
				strPath += WILDCARD;
				hFindFile = ::FindFirstFile(strPath, &fdFindData);
				if(hFindFile != INVALID_HANDLE_VALUE)
				{
					do
					{
						if(_tcsicmp(fdFindData.cFileName, RELATIVE_CURRENT_PATH) == 0 ||
							_tcsicmp(fdFindData.cFileName, RELATIVE_PARENT_PATH) == 0)
							continue;

						// call the preorder callback method
						if(PreCallback != NULL && (fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
						{
							if(PreCallback(szSearchPath, &fdFindData) == FALSE)
								break;
						}

						// go recursive in subdirectories
						if(Recursive && (fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
						{
							strSubPath = strSearchFullPath;
							strSubPath += fdFindData.cFileName;
							WalkThroughDirectory(strSubPath, szFilter, PreCallback, PostCallback, Recursive);
						}											

						// call the post-order callback method
						if(PostCallback != NULL && (fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
						{
							if(PostCallback(szSearchPath, &fdFindData) == FALSE)
								break;
						}	
					}
					while(::FindNextFile(hFindFile, &fdFindData));

					::FindClose(hFindFile);
				}
			}
			else
			{
				strPath += szFilter;			
				hFindFile = ::FindFirstFile(strPath, &fdFindData);
				if(hFindFile != INVALID_HANDLE_VALUE)
				{
					do
					{
						// call the preorder callback method
						if(PreCallback != NULL)
						{
							if(PreCallback(szSearchPath, &fdFindData) == FALSE)
								break;
						}

						// call the post-order callback method
						if(PostCallback != NULL)
						{
							if(PostCallback(szSearchPath, &fdFindData) == FALSE)
								break;
						}	
					}
					while(::FindNextFile(hFindFile, &fdFindData));

					::FindClose(hFindFile);
				}

				strPath = strSearchFullPath;
				strPath += WILDCARD;
				hFindFile = ::FindFirstFile(strPath, &fdFindData);
				if(hFindFile != INVALID_HANDLE_VALUE)
				{
					do
					{
						if(_tcsicmp(fdFindData.cFileName, RELATIVE_CURRENT_PATH) == 0 ||
							_tcsicmp(fdFindData.cFileName, RELATIVE_PARENT_PATH) == 0)
							continue;

						// go recursive in subdirectories
						if(Recursive && (fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)					
						{
							strSubPath = strSearchFullPath;
							strSubPath += fdFindData.cFileName;
							WalkThroughDirectory(strSubPath, szFilter, PreCallback, PostCallback, Recursive);
						}
					}
					while(::FindNextFile(hFindFile, &fdFindData));

					::FindClose(hFindFile);
				}
			}		
		}
	};



	///////////////////////////////////////////////////////////////////////////////
	// CmPathHelper

#define        SOFTWARE_DOWNLOAD_PATH                       0
#define        COMPONENT_DOWNLOAD_PATH						1
#define        DRIVER_DOWNLOAD_PATH							2
#define        DRIVERZIP_DOWNLOAD_PATH						3

	class CmPathHelper
	{
	public:
		static CString GetCurrentModuleDirectory()
		{
			CString strResult;

			::GetModuleFileName(NULL, strResult.GetBuffer(MAX_PATH + 10), MAX_PATH);
			strResult.ReleaseBuffer();
			strResult = CmDirectoryHelper::ExtractFilePath(strResult);

			return strResult;
		}

		

		static const wchar_t* GetAppFileName()
		{
			static std::wstring file;

			if (file.empty()) {
				wchar_t filename[MAX_PATH + 10] = {0};
				GetModuleFileNameW((HMODULE)&__ImageBase, filename, MAX_PATH);
				file = filename;
			}

			return file.c_str();
		}
	};

	///////////////////////////////////////////////////////////////////////////////
	// CmIEHelper

#define  REG_PATH_IESETTINGS					_T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings")
#define  REG_VALUE_PROXYSERVER_PREFIX_HTTP		_T("http=")
#define  REG_VALUE_PROXYSERVER_HTTP_DEFAULTPORT	_T("80")

	class CmIEHelper
	{
	public:
		static BOOL IsProxyEnabled()
		{
			BOOL    bResult = FALSE;
			CRegKey reg;
			CString strKey;
			DWORD   dwValue;

			if(reg.Open(HKEY_CURRENT_USER, REG_PATH_IESETTINGS, KEY_READ) == ERROR_SUCCESS)
			{
				dwValue = 0;
				if(reg.QueryDWORDValue(_T("ProxyEnable"), dwValue) == ERROR_SUCCESS)
				{
					bResult = dwValue != 0;
				}
				reg.Close();
			}

			return bResult;
		}

		static BOOL GetProxyHttpServer(CString &strHost, CString &strPort)
		{
			BOOL    bResult = FALSE;
			CRegKey reg;
			CString strKey, strValue, strServer;
			DWORD   dwSize;
			long    nRetCode;
			int     nIdx;
			std::vector<CString> strServers;

			strHost.Empty();
			strPort.Empty();

			if(reg.Open(HKEY_CURRENT_USER, REG_PATH_IESETTINGS, KEY_READ) == ERROR_SUCCESS)
			{
				dwSize = MAX_PATH;
				nRetCode = reg.QueryStringValue(_T("ProxyServer"), strValue.GetBuffer(MAX_PATH), &dwSize);
				strValue.ReleaseBuffer();
				reg.Close();

				if(nRetCode == ERROR_SUCCESS && !strValue.IsEmpty())
				{
					if(CmStringHelper::Split(strValue, ';', strServers))
					{
						if(strServers.size() == 1)
						{
							strValue = strServers[0];
							strValue.Trim();
							strValue.MakeLower();
							if(strValue.Find('=') < 0)
								strServer = strValue;
							else
								if(strValue.Find(REG_VALUE_PROXYSERVER_PREFIX_HTTP) == 0)
									strServer = strValue.Mid(5);
						}
						else
						{
							for(DWORD i = 0; i < strServers.size();i++)
							{
								strValue = strServers[i];
								strValue.Trim();
								strValue.MakeLower();
								if(strValue.Find(REG_VALUE_PROXYSERVER_PREFIX_HTTP) == 0)
								{
									strServer = strValue.Mid(5); 
									break;
								}
							}
						}

						if(!strServer.IsEmpty())
						{
							nIdx = strServer.Find(':');
							if(nIdx < 0)
							{
								strHost = strServer;
								strPort = REG_VALUE_PROXYSERVER_HTTP_DEFAULTPORT;
								bResult = TRUE;
							}
							else
								if(nIdx > 0)
								{
									strHost = strServer.Left(nIdx);
									strPort = strServer.Mid(nIdx + 1);
									if(strPort.IsEmpty())
										strPort = REG_VALUE_PROXYSERVER_HTTP_DEFAULTPORT;
									bResult = TRUE;
								}							
						}
					}
				}
			}

			return bResult;
		}

		static BOOL GetProxyCredential(LPCTSTR szHost, CString &strUserName, CString &strPassword)
		{
			BOOL			bResult		= FALSE;
			DWORD			dwCount		= 0;  
			PCREDENTIAL		pCredential;
			PCREDENTIAL *	pCredArray	= NULL ; 
			CString			strTarget;

			if(::CredEnumerate(NULL, 0, &dwCount, &pCredArray))  
			{  
				for(DWORD i = 0; i < dwCount; i++)  
				{  
					pCredential = pCredArray[i];  
					strTarget = pCredential->TargetName;  

					if(strTarget.CompareNoCase(szHost) == 0)
					{  
						strUserName = pCredential->UserName;
#ifdef _UNICODE 
						strPassword = (LPCWSTR)pCredential->CredentialBlob;  
#else  
						strPassword = CW2A((LPCWSTR)pCredential->CredentialBlob);  
#endif  
						bResult = TRUE;
						break;  
					}  
				}

				::CredFree(pCredArray);
			}  
			return bResult;
		}
	};
}




using namespace cactus;

/* CmHttpClient class */

CmHttpClient::CmHttpClient()
{
    
}

CmHttpClient::~CmHttpClient()
{
    
}

BOOL CmHttpClient::Get(LPCTSTR szUrl, CStringA &strReceiveContent, DWORD *pErrorCode)
{
	BOOL bResult = FALSE;
	std::string strUrl, strRecContent;

    strUrl	= CmStringHelper::Unicode2GB(szUrl).GetString();

	// 设置代理
	SetProxy();

	if (m_pClient.performGet(strUrl) == true) 
	{
        m_pClient.getLastWebResponse(strRecContent);
        strReceiveContent = strRecContent.c_str();
        bResult = TRUE;
    }
	else
	{
		if(pErrorCode != NULL)
			*pErrorCode = m_pClient.last_curl_code;
	}

	return bResult;
}



BOOL CmHttpClient::Post2(LPCTSTR szUrl, BYTE *pData, ULONG ulSize, CStringA &strReceiveContent, DWORD *pErrorCode)
{
    BOOL		bResult = FALSE;
	std::string strUrl, strRecContent;

    strUrl	= CmStringHelper::Unicode2GB(szUrl).GetString();

	// 设置代理
	SetProxy();

    if (m_pClient.perform_dg_post_bin2(strUrl, (char *)pData, ulSize) == true) 
	{
        m_pClient.getLastWebResponse(strRecContent);
        strReceiveContent = strRecContent.c_str();
        bResult = TRUE;
    }
	else
	{
		if(pErrorCode != NULL)
			*pErrorCode = m_pClient.last_curl_code;
	}

    return bResult;
}

int CmHttpClient::GetProxyModeConfig()
{
	CString strConfig = CmPathHelper::GetCurrentModuleDirectory() + _T("\\config.ini");
	return ::GetPrivateProfileInt(_T("ProxyMode"), _T("Mode"), 0, strConfig);
}

BOOL CmHttpClient::GetProxyNeedAuthConfig()
{
	CString strConfig = CmPathHelper::GetCurrentModuleDirectory() + _T("\\config.ini");
	return ::GetPrivateProfileInt(_T("ProxyMode"), _T("Needauth"), 0, strConfig) > 0;
}

CString CmHttpClient::GetProxyHostConfig()
{
	CString strResult;
	CString strConfig = CmPathHelper::GetCurrentModuleDirectory() + _T("\\config.ini");

	if(::GetPrivateProfileString(_T("ProxyMode"), _T("Host"), NULL, strResult.GetBuffer(MAX_PATH), MAX_PATH, strConfig) == 0)
	{
		strResult.ReleaseBuffer();
		strResult.Empty();		
	}
	else
		strResult.ReleaseBuffer();

	return strResult;
}

CString CmHttpClient::GetProxyPortConfig()
{
	CString strResult;
	CString strConfig = CmPathHelper::GetCurrentModuleDirectory() + _T("\\config.ini");

	if(::GetPrivateProfileString(_T("ProxyMode"), _T("Port"), NULL, strResult.GetBuffer(MAX_PATH), MAX_PATH, strConfig) == 0)
	{
		strResult.ReleaseBuffer();
		strResult.Empty();		
	}
	else
		strResult.ReleaseBuffer();

	return strResult;
}

CString CmHttpClient::GetProxyUserNameConfig()
{
	CString strResult;
	CString strConfig = CmPathHelper::GetCurrentModuleDirectory() + _T("\\config.ini");

	if(::GetPrivateProfileString(_T("ProxyMode"), _T("Login"), NULL, strResult.GetBuffer(MAX_PATH), MAX_PATH, strConfig) == 0)
	{
		strResult.ReleaseBuffer();
		strResult.Empty();		
	}
	else
		strResult.ReleaseBuffer();

	return strResult;
}

CString CmHttpClient::GetProxyPasswordConfig()
{
	CString strResult;
	CString strConfig = CmPathHelper::GetCurrentModuleDirectory() + _T("\\config.ini");

	if(::GetPrivateProfileString(_T("ProxyMode"), _T("Password"), NULL, strResult.GetBuffer(MAX_PATH), MAX_PATH, strConfig) == 0)
	{
		strResult.ReleaseBuffer();
		strResult.Empty();		
	}
	else
		strResult.ReleaseBuffer();

	return strResult;
}

BOOL CmHttpClient::SetProxy()
{
	BOOL		bResult = FALSE;
	CString     strHost, strPort, strUserName, strPassword;
    std::string strProxyServerIp, strProxyServerPort, strProxyUserName, strProxyPassword;    

	// 设置代理
	switch(GetProxyModeConfig())
	{
	case 1:
		if(CmIEHelper::IsProxyEnabled() && CmIEHelper::GetProxyHttpServer(strHost, strPort))
		{
			strProxyServerIp = CmStringHelper::Unicode2GB(strHost).GetString();
			strProxyServerPort = CmStringHelper::Unicode2GB(strPort).GetString();
			m_pClient.setProxyServerIp(strProxyServerIp);
			m_pClient.setProxyServerPort(strProxyServerPort);
			if(CmIEHelper::GetProxyCredential(strHost, strUserName, strPassword))
			{
				strProxyUserName = CmStringHelper::Unicode2GB(strUserName).GetString();
				strProxyPassword = CmStringHelper::Unicode2GB(strPassword).GetString();
				m_pClient.setProxyUserName(strProxyUserName);
				m_pClient.setProxyPassword(strProxyPassword);
			}

			bResult = TRUE;
		}
		break;
	case 2:
		strProxyServerIp = CmStringHelper::Unicode2GB(GetProxyHostConfig()).GetString();
		strProxyServerPort = CmStringHelper::Unicode2GB(GetProxyPortConfig()).GetString();
		m_pClient.setProxyServerIp(strProxyServerIp);
		m_pClient.setProxyServerPort(strProxyServerPort);
		if(GetProxyNeedAuthConfig())
		{
			strProxyUserName = CmStringHelper::Unicode2GB(GetProxyUserNameConfig()).GetString();
			strProxyPassword = CmStringHelper::Unicode2GB(GetProxyPasswordConfig()).GetString();
			m_pClient.setProxyUserName(strProxyUserName);
			m_pClient.setProxyPassword(strProxyPassword);
		}

		bResult = TRUE;
		break;
	}

	return bResult;
}

/* CmDgClient class */

#define		PREFERRED_DG_HOST		_T("http://liveupdate5.drivergenius.com")	
#define		RESERVED_DG_HOST		_T("http://liveupdate51.drivergenius.com")


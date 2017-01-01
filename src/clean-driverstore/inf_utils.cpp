#include "inf_utils.h"
#include <Shlwapi.h>
#include <SetupApi.h>
#include <cfgmgr32.h>
#include <atlstr.h>
#include <iostream>
#include <set>
#include <algorithm>
#include <boost/regex.hpp>
#include <tracetool/tracetool.h>

namespace
{
	string16 convert_to(const char* const& from)
	{
		int len = static_cast<int>(strlen(from) + 1);
		std::vector<char16> wideBuf(len);
		int ret = MultiByteToWideChar(CP_ACP, 0, from, len, &wideBuf[0], len);

		if (ret == 0)
			return L"";
		else
			return string16(&wideBuf[0]);
	}
}


void WINAPI enum_oem_inf_files(const std::wstring& filename, void* param)
{
    std::vector<std::wstring>* list = reinterpret_cast<std::vector<std::wstring>* >(param);
    string16 base_name = cactus::ExtractFileName(filename);
    string16 ext = cactus::ExtractFileExt(filename);

    if ((base_name.find(L"oem") == 0) && (_tcsicmp(ext.c_str(), L"inf") == 0)) {
        list->push_back(filename);
    }

    /*if (filename.rfind(L".inf") + 4 == filename.length()) {
        list->push_back(filename);
    }*/
}

bool less_inf(const string16& str1, const string16& str2)
{
    string16 base_name_num1 = cactus::ExtractFileName(str1);
    string16 base_name_num2 = cactus::ExtractFileName(str2);
    int num1 = _wtoi(base_name_num1.substr(3, base_name_num1.length() - 3).c_str());
    int num2 = _wtoi(base_name_num2.substr(3, base_name_num2.length() - 3).c_str());
    return num1 < num2;
}

const char16 PathDelim  = L'\\';

namespace cactus
{
void enumFile(const std::wstring& searchPath, EnumCallback callback, void* param)
{
    WIN32_FIND_DATA fd;
    HANDLE findFile = ::FindFirstFile((IncludeTrailingPathDelimiter(searchPath) + L"*").c_str(), &fd);

    if (findFile == INVALID_HANDLE_VALUE) {
        ::FindClose(findFile);
        return;
    }

    std::wstring fullpath = GetFullPathName(IncludeTrailingPathDelimiter(searchPath));

    while (1) {
        if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
            //file
            callback(fullpath + fd.cFileName, param);
        } else if (wcscmp(fd.cFileName, L".") != 0 && wcscmp(fd.cFileName, L"..")  != 0) {
            //folder
            enumFile(IncludeTrailingPathDelimiter(searchPath) + fd.cFileName + L"/", callback, param);
        }

        if (!FindNextFile(findFile, &fd)) {
            ::FindClose(findFile);
            return;
        }
    }

    ::FindClose(findFile);
}

std::wstring GetFullPathName(const std::wstring & path)
{
    LPTSTR szFilePart;
    wchar_t buffer[MAX_PATH + 1];
    ::GetFullPathName(path.c_str(), MAX_PATH, buffer, &szFilePart);
    return std::wstring(buffer);
}

std::wstring get_systemroot_inf_dir()
{
    std::wstring retval;
    wchar_t* sysroot = _wgetenv(L"SystemRoot");
    retval.append(std::wstring(sysroot) + std::wstring(L"\\inf"));
    return retval;
}

const char16* GetAllPathDelimiters()
{
    return L"\\/";
}

bool EndsWithPathDelimiter(const string16& path)
{
    return (path.find_last_of(GetAllPathDelimiters()) + 1 == path.length());
}

string16 IncludeTrailingPathDelimiter(const string16& S)
{
    string16 Result = S;

    if (!EndsWithPathDelimiter(Result))
        Result = Result + PathDelim;

    return Result;
}

string16 ExtractFileName(const string16& FileName)
{
    string16 Result = FileName;
    return Result.erase(0, Result.find_last_of(GetAllPathDelimiters()) + 1);
}

string16 ExtractFileExt(const string16& FileName)
{
    string16 Result = FileName;
    return Result.erase(0, Result.find_last_of(L'.') + 1);
}

BOOL IsWow64()
{
    typedef BOOL (WINAPI * LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
    LPFN_ISWOW64PROCESS fnIsWow64Process;
    BOOL bIsWow64 = FALSE;
    HMODULE hModule = GetModuleHandle(L"kernel32");

    if (hModule == NULL)
        return FALSE;

    fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(hModule, "IsWow64Process");

    if (NULL != fnIsWow64Process) {
        if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64)) {
            return FALSE;
        } else {
            return bIsWow64;
        }
    }

    return bIsWow64;
}

string16 get_pnputil_exe_fullpath()
{
    string16 retval;
    wchar_t* sysroot = _wgetenv(L"SystemRoot");

    if (IsWow64())
        retval.append(std::wstring(sysroot) + std::wstring(L"\\sysnative\\PnPutil.exe"));
    else
        retval.append(std::wstring(sysroot) + std::wstring(L"\\System32\\PnPutil.exe"));

    return retval;
}

BOOL getRunExeOutput(const std::wstring& exe_filename, const std::wstring& args, DWORD* out_exitcode, std::wstring& result)
{
	static const int MAX_CMD = 1024;
	STARTUPINFOW si = { sizeof(STARTUPINFOW) };
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	BOOL bUsePipes = FALSE;
	HANDLE FWritePipe = NULL;
	HANDLE FReadPipe = NULL;
	SECURITY_ATTRIBUTES pa = { sizeof(pa), NULL, TRUE };
	bUsePipes = ::CreatePipe(&FReadPipe, &FWritePipe, &pa, 0);

	if (bUsePipes != FALSE) {
		si.hStdOutput = FWritePipe;
		si.hStdInput = FReadPipe;
		si.hStdError = FWritePipe;
		si.dwFlags = STARTF_USESTDHANDLES | si.dwFlags;
	}

	std::wstring cmdline;
	cmdline.append(L"\"");
	cmdline.append(exe_filename.c_str());
	cmdline.append(L"\"");
	cmdline.append(L" ");
	cmdline.append(args.c_str());
	

	PROCESS_INFORMATION pi = { 0 };
	BOOL RetCode = ::CreateProcess(NULL,
		(LPWSTR)cmdline.c_str(),
		NULL,
		NULL,
		TRUE,
		0,
		NULL, // Use parent's environment block
		NULL, //path.GetDirectory(),
		&si, // STARTUPINFO
		&pi);  // PROCESS_INFORMATION

	if (RetCode == FALSE) {
		::CloseHandle(FReadPipe);
		::CloseHandle(FWritePipe);
		return FALSE;
	}

	::CloseHandle(pi.hThread);

	try {
		DWORD BytesToRead = 0;
		DWORD BytesRead = 0;
		DWORD TotalBytesAvail = 0;
		DWORD PipeReaded = 0;
		DWORD exit_code = 0;
		CHAR  szBuffer[256] = {0};
		std::vector<char> buffer(MAX_CMD);

		while (::PeekNamedPipe(FReadPipe, NULL, 0, &BytesRead, &TotalBytesAvail, NULL)) {
			if (TotalBytesAvail == 0) {
				if (::GetExitCodeProcess(pi.hProcess, &exit_code) == FALSE ||
					exit_code != STILL_ACTIVE) {
						break;
				} else {
					continue;
				}
			} else {
				while (TotalBytesAvail> BytesRead) {
					if (TotalBytesAvail - BytesRead> MAX_CMD - 1) {
						BytesToRead = MAX_CMD - 1;
					} else {
						BytesToRead = TotalBytesAvail - BytesRead;
					}

					if (::ReadFile(FReadPipe,
						&buffer[0],
						BytesToRead,
						&PipeReaded,
						NULL) == FALSE) {
							break;
					}

					if (PipeReaded <= 0) continue;

					BytesRead += PipeReaded;
					buffer[PipeReaded] = '\0';					
					result.append(convert_to(&buffer[0]).c_str());
				}
			}
		}
	} catch (...) {
	}



	::GetExitCodeProcess(pi.hProcess, out_exitcode);
	::CloseHandle(pi.hProcess);
	::CloseHandle(FReadPipe);
	::CloseHandle(FWritePipe);
	return TRUE;
}



int runExe(const std::wstring& exe_filename, const std::wstring& args)
{
    int retval = 0;
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.lpReserved = NULL;
    si.lpDesktop = NULL;
    si.lpTitle = NULL;
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    si.cbReserved2 = NULL;
    si.lpReserved2 = NULL;
    ZeroMemory(&pi, sizeof(pi));

    do {
        //TRACE_SEND_W(cmdline.c_str());
        if (!CreateProcess(exe_filename.c_str(), (LPWSTR)args.c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
            retval = -1;
            break;
        }
    } while (0);

    // Esperamos hasta que el proceso creado finalice.
    WaitForSingleObject(pi.hProcess, INFINITE);

    if (retval == 0) {
        DWORD exitCode;

        if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
            retval = exitCode;
        } else {
            retval = -2;
        }
    }

    // Se cierra el proceso.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return retval;
}

BOOL IsWinNT()
{
    static int dwPlatformId = -1;

    if (dwPlatformId == -1) {
        OSVERSIONINFO ver;
        ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        ::GetVersionEx((LPOSVERSIONINFO)&ver);
        dwPlatformId = ver.dwPlatformId;
    }

    return (dwPlatformId == VER_PLATFORM_WIN32_NT ? TRUE : FALSE);
}


CString getSoftwareKeyFromRegistry(const CString& instanceID)
{
    CString strKey;

    // 是否是 nt 系统
    if (IsWinNT()) {
        strKey.Format(_T("SYSTEM\\CurrentControlSet\\Enum\\%s"), instanceID);
    } else {
        strKey.Format(_T("Enum\\%s"), instanceID);
    }

    CString softwareKey;
    DWORD dwSize = LINE_LEN;
    CRegKey reg;

    // 查找设备的 sofeware key
    if (reg.Open(HKEY_LOCAL_MACHINE, strKey, KEY_READ) == ERROR_SUCCESS) {
        reg.QueryStringValue(_T("Driver"), softwareKey.GetBuffer(LINE_LEN), &dwSize);
        softwareKey.ReleaseBuffer();
        softwareKey.TrimRight();
        reg.Close();
    }

    return softwareKey;
}


CString getInfName(LPCTSTR szSoftwareKey)
{
    CString strKey, strValue;

    if (IsWinNT()) {
        strKey.Format(_T("SYSTEM\\CurrentControlSet\\Control\\Class\\%s"), szSoftwareKey);
    } else {
        strKey.Format(_T("System\\CurrentControlSet\\Services\\Class\\%s"), szSoftwareKey);
    }

    CString infName;
    DWORD dwSize;
    CRegKey reg;

    if (reg.Open(HKEY_LOCAL_MACHINE, strKey, KEY_READ) == ERROR_SUCCESS) {
        dwSize = LINE_LEN;
        reg.QueryStringValue(_T("InfPath"), infName.GetBuffer(LINE_LEN), &dwSize);
        infName.ReleaseBuffer();
        infName.TrimRight();
        reg.Close();
    }

    return infName;
}


ULONG get_device_status(HDEVINFO Devs, PSP_DEVINFO_DATA DevInfo)
{
    ULONG retval = 0;
    CONFIGRET cr;
    ULONG ulProblemNumber, ulStatus;

    if (Devs != INVALID_HANDLE_VALUE) {
        if ((cr = ::CM_Get_DevNode_Status(&ulStatus, &ulProblemNumber, DevInfo->DevInst, 0)) == CR_SUCCESS) {
            //m_ulProblemNumber = ulProblemNumber;
            retval = ulStatus;
        }
    }

    return retval;
}


void enum_device_oem_infs(std::set<string16>& infset)
{
    HDEVINFO devs = INVALID_HANDLE_VALUE;
    DWORD devIndex;
    SP_DEVINFO_DATA devInfo = { 0 };
    //m_dlDevices.clear();
    devs = ::SetupDiGetClassDevs(NULL,
                                 NULL,
                                 NULL,
                                 DIGCF_ALLCLASSES); // DIGCF_ALLCLASSES | DIGCF_PRESENT

    if (devs == INVALID_HANDLE_VALUE) {
        goto final;
    }

    devInfo.cbSize = sizeof(devInfo);

    for (devIndex = 0; ::SetupDiEnumDeviceInfo(devs, devIndex, &devInfo); devIndex++) {
        CString instanceID;
        TCHAR szBuffer[LINE_LEN] = { 0 };

        if (::CM_Get_Device_ID(devInfo.DevInst, szBuffer, LINE_LEN, 0) == CR_SUCCESS) {
            instanceID = szBuffer;

            if (instanceID.GetLength() > 0) {
                CString infName;
                infName = getInfName(getSoftwareKeyFromRegistry(instanceID));
                infName.MakeLower();

                if (infName.Find(L"oem") == 0) {
                    infset.insert((LPCWSTR)infName);
                    //TRACE_SEND_FORMAT(L"设备inf:%s", (LPCWSTR)infName);
                }
            }
        }
    }

final:

    if (devs != INVALID_HANDLE_VALUE) {
        ::SetupDiDestroyDeviceInfoList(devs);
    }
}


void get_difference_infs(std::vector<string16>& ivec)
{
    std::set<string16> infset;
    enum_device_oem_infs(infset);
    std::vector<std::wstring> oem_inf_list;
    enumFile(get_systemroot_inf_dir(), enum_oem_inf_files, &oem_inf_list);
    //sort(oem_inf_list.begin(), oem_inf_list.end(), less_inf);
    std::vector<std::wstring>::const_iterator it = oem_inf_list.begin();
    std::set<string16> inf_files_set;

    for (; it != oem_inf_list.end(); ++it) {
        //std::wcout << (*it).c_str() << std::endl;
        string16 base_name = ExtractFileName(*it);
        //std::wcout << base_name.c_str() << std::endl;
        inf_files_set.insert(base_name);
    }

    ivec.resize(inf_files_set.size() + infset.size());
    std::vector<string16>::iterator target_end = std::set_difference(inf_files_set.begin(), inf_files_set.end(), infset.begin(), infset.end(), ivec.begin());
    ivec.resize(target_end - ivec.begin());
}

void get_difference_infs2(std::vector<string16>& ivec)
{
	std::wstring result = L"";
	DWORD exit_code;

	if (getRunExeOutput(get_pnputil_exe_fullpath(), L" -e", &exit_code, result)) {		
		boost::wregex r2(L"(oem\\d+\\.inf)");
		boost::wsmatch matches;
		std::wstring::const_iterator start = result.begin(),
			end = result.end();

		std::set<string16> infset;
		enum_device_oem_infs(infset);
		std::set<string16> inf_files_set;

		while (boost::regex_search(start, end, matches, r2)) {
			//std::cout << matches.str() << '\n';
			inf_files_set.insert(matches.str());
			start = matches[0].second;
		}

		ivec.resize(inf_files_set.size() + infset.size());
		std::vector<string16>::iterator target_end = std::set_difference(inf_files_set.begin(), inf_files_set.end(), infset.begin(), infset.end(), ivec.begin());
		ivec.resize(target_end - ivec.begin());
	}
}

} // namespace cactus



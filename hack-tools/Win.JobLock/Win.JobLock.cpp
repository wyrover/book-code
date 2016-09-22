/*
A Microsoft Windows Process Lockdown Tool using Job Objects 

Released as open source by NCC Group Plc - http://www.nccgroup.com/

Developed by Ollie Whitehouse, ollie dot whitehouse at nccgroup dot com

https://github.com/nccgroup/WindowsJobLock

Released under AGPL see LICENSE for more information
*/

#include "stdafx.h"
#include "XGetopt.h"
#include "Wtsapi32.h"

// Global
BOOL bListProcesses=FALSE;
HANDLE hProcess = NULL;
TCHAR *strProcess = NULL;
DWORD dwPID = 0;
TCHAR *strName = NULL;
DWORD dwProcessLimit = 0;
DWORD dwJobMemory = 0;
DWORD dwProcessMemory = 0;
BOOL  bKillProcOnJobClose = FALSE;
BOOL  bBreakAwayOK = FALSE;
BOOL  bSilentBreakAwayOK = FALSE;
BOOL  bUILimitDesktop = FALSE;
BOOL  bUILimitDispSettings = FALSE;
BOOL  bUILimitExitWindows = FALSE;
BOOL  bUILimitUserHandles = FALSE;
BOOL  bUILimitGlobalAtoms = FALSE;
BOOL  bUILimitReadClip = FALSE;
BOOL  bUILimitSystemParams = FALSE;
BOOL  bUILimitWriteClip = FALSE;
HANDLE hJob = NULL;


//
// Function	: FindProcess
// Purpose	: Find a process by name
//
DWORD FindProcess(TCHAR *strName)
{
	 DWORD dwPIDArray[2048], dwCount = 0, dwRet;
	 PWTS_PROCESS_INFO ppProcessInfo;

     if(WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE,0,1,&ppProcessInfo,&dwRet)){
		for(dwCount=0;dwCount<dwRet;dwCount++){

			if(strName == NULL){
				_ftprintf(stdout,L"[i] %s (%d) in session %d\n",ppProcessInfo[dwCount].pProcessName,ppProcessInfo[dwCount].ProcessId,ppProcessInfo[dwCount].SessionId);
			} else {
				if(lstrcmp(ppProcessInfo[dwCount].pProcessName,strName) == 0 ){
					return ppProcessInfo[dwCount].ProcessId;
				}
			}
		}
	 }

	return FALSE;
}

//
// Function	: FindProcess
// Purpose	: Find as processes name by PID
//
BOOL FindProcessName(DWORD dwPID, TCHAR *strName)
{
	 DWORD dwPIDArray[2048], dwCount = 0, dwRet;
	 PWTS_PROCESS_INFO ppProcessInfo;

     if(WTSEnumerateProcesses(WTS_CURRENT_SERVER_HANDLE,0,1,&ppProcessInfo,&dwRet)){
		for(dwCount=0;dwCount<dwRet;dwCount++){

			if(ppProcessInfo[dwCount].ProcessId== dwPID ){
				_tcscpy_s(strName,MAX_PATH,ppProcessInfo[dwCount].pProcessName);
				return TRUE;
			}
		}
	 }

	return FALSE;
}

BOOL BuildAndDeploy()
{
	TCHAR strFinalName[MAX_PATH] = { 0 };
	JOBOBJECT_EXTENDED_LIMIT_INFORMATION jelInfo = { 0 };
	JOBOBJECT_BASIC_UI_RESTRICTIONS jbuiRestrictions = { 0 };

	jelInfo.BasicLimitInformation.LimitFlags = 0;
	jbuiRestrictions.UIRestrictionsClass = 0;

	// Risk of truncation in theory
	if(strName != NULL){
		_tcscpy_s(strFinalName,MAX_PATH,L"Local\\");
		_tcscat_s(strFinalName,MAX_PATH,strName);
		hJob = CreateJobObject(NULL,strFinalName);
	} else {
		hJob = CreateJobObject(NULL,NULL);
	}
	

	_ftprintf(stdout,L"[i] Final job name                - %s\n",strName == NULL ? L"NONAME" : strFinalName);

	if(hJob == NULL){
		if(GetLastError() ==  ERROR_INVALID_HANDLE){
			_ftprintf(stdout,L"[!] Couldn't create job %s due to a object name conflict\n",strFinalName);
		} else if (GetLastError() ==ERROR_ALREADY_EXISTS){ 
			_ftprintf(stdout,L"[!] Couldn't create job %s due to a job already existing with that name\n",strFinalName);
		} else {
			_ftprintf(stdout,L"[!] Couldn't create job %s due to an unknown error %d\n",GetLastError());
		}
		return FALSE;
	}

	if(dwProcessLimit){
		jelInfo.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_ACTIVE_PROCESS;
		jelInfo.BasicLimitInformation.ActiveProcessLimit = dwProcessLimit;
	}

	if(dwJobMemory){
		jelInfo.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_JOB_MEMORY;
		jelInfo.JobMemoryLimit = dwJobMemory;
	}
		

	if(dwProcessMemory){
		jelInfo.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_PROCESS_MEMORY;
		jelInfo.ProcessMemoryLimit = dwProcessMemory;
	}

	if(bKillProcOnJobClose) jelInfo.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
	if(bBreakAwayOK) jelInfo.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_BREAKAWAY_OK;
	if(bSilentBreakAwayOK) jelInfo.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK;
	if(bUILimitDesktop)  jbuiRestrictions.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_DESKTOP;
	if(bUILimitDispSettings) jbuiRestrictions.UIRestrictionsClass|= JOB_OBJECT_UILIMIT_DISPLAYSETTINGS;
	if(bUILimitExitWindows) jbuiRestrictions.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_EXITWINDOWS;
	if(bUILimitGlobalAtoms) jbuiRestrictions.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_GLOBALATOMS;
	if(bUILimitUserHandles) jbuiRestrictions.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_HANDLES;
	if(bUILimitReadClip) jbuiRestrictions.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_READCLIPBOARD;
	if(bUILimitSystemParams) jbuiRestrictions.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_SYSTEMPARAMETERS;
	if(bUILimitWriteClip) jbuiRestrictions.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_WRITECLIPBOARD;

	
	if(!SetInformationJobObject(hJob,JobObjectExtendedLimitInformation,&jelInfo,sizeof(jelInfo))){
		_ftprintf(stdout,L"[!] Couldn't set job extended limits to job object %s due to an error %d\n",(_tcslen(strFinalName) ==0 ) ? L"Unknown" : strFinalName, GetLastError());
		return FALSE;
	} else {
		_ftprintf(stdout,L"[*] Applied job exended limits to job object\n");
	}

	if(!SetInformationJobObject(hJob,JobObjectBasicUIRestrictions,&jbuiRestrictions,sizeof(jbuiRestrictions))){
		_ftprintf(stdout,L"[!] Couldn't set UI limits to job object %s due to an error %d\n",(_tcslen(strFinalName) ==0 ) ? L"Unknown" : strFinalName, GetLastError());
		return FALSE;
	} else {
		_ftprintf(stdout,L"[*] Applied UI limits to job object\n");
	}


	// Duplicate the handle into the target process
	if(!DuplicateHandle(GetCurrentProcess(),hJob,hProcess,NULL,JOB_OBJECT_QUERY,TRUE,NULL)){
		_ftprintf(stdout,L"[!] Couldn't duplicate job handle into target process due to an error %d\n",GetLastError());
		return FALSE;
	} else {
		_ftprintf(stdout,L"[*] Duplicated handle of job (with restricted access) into target process!\n");
	}

	// Now assign the process to the job object
	if(!AssignProcessToJobObject(hJob,hProcess)){
		// this is where I wanted to be lazy
		if(GetLastError() == ERROR_ACCESS_DENIED){ // Windows 7 and Server 2008 R2 and below
			_ftprintf(stdout,L"[!] Couldn't apply job object to %s Looks like a job object has already been applied\n", strProcess);
			return FALSE;
		} else {
			_ftprintf(stdout,L"[!] Couldn't apply job object to %s due to an error %d - %d\n", strProcess, GetLastError(),sizeof(jbuiRestrictions));
		}
	} else {
		_ftprintf(stdout,L"[*] Applied job object to process!\n");
	}


	return TRUE;
}

//
// Function	: PrintSettings
// Purpose	: Print the settings we will apply
//
void PrintSettings()
{
	fprintf(stdout,"[i] Process Limit                 - %s - %d\n", dwProcessLimit > 0 ? "True " : "False", dwProcessLimit);
	fprintf(stdout,"[i] Job Memory Limit              - %s - %d\n", dwJobMemory > 0 ? "True " : "False", dwJobMemory);
	fprintf(stdout,"[i] Process Memory Limit          - %s - %d\n", dwProcessMemory > 0 ? "True " : "False", dwProcessMemory);
	fprintf(stdout,"[i] Kill Process on Job Close     - %s\n", bKillProcOnJobClose == TRUE ? "True ": "False");
	fprintf(stdout,"[i] Break Away from Job OK        - %s\n", bBreakAwayOK == TRUE ? "True ": "False");
	fprintf(stdout,"[i] Silent Break Away from Job OK - %s\n", bSilentBreakAwayOK == TRUE ? "True ": "False");
	fprintf(stdout,"[i] Limit Desktop Operations      - %s\n", bUILimitDesktop == TRUE ? "True ": "False");
	fprintf(stdout,"[i] Limit Display Changes         - %s\n", bUILimitDispSettings == TRUE ? "True ": "False");
	fprintf(stdout,"[i] Limit Exit Windows            - %s\n", bUILimitExitWindows == TRUE ? "True ": "False");
	fprintf(stdout,"[i] Limit Global Atoms            - %s\n", bUILimitGlobalAtoms == TRUE ? "True ": "False");
	fprintf(stdout,"[i] Limit User Handles            - %s\n", bUILimitUserHandles == TRUE ? "True ": "False");
	fprintf(stdout,"[i] Limit Reading of Clipboard    - %s\n", bUILimitReadClip == TRUE ? "True ": "False");
	fprintf(stdout,"[i] Limit System Parameter Change - %s\n", bUILimitSystemParams == TRUE ? "True ": "False");
	fprintf(stdout,"[i] Limit Writing to Clipboard    - %s\n", bUILimitWriteClip == TRUE ? "True ": "False");
}

//
// Function	: PrintHelp
// Purpose	: Print the help out
//
void PrintHelp(TCHAR *strExe){

        _ftprintf(stdout,L"    i.e. %s [-h] \n",strExe);
		fprintf (stdout,"\n");
		fprintf (stdout," [.General Settings / Options.]\n");
		fprintf (stdout,"    -g          - Get process list\n");
		fprintf (stdout,"    -P <name>   - Process name to apply the job to\n");
		fprintf (stdout,"    -p <PID>    - PID to apply the job to\n");
        fprintf (stdout,"    -n <name>   - What the job will be called (optional)\n");
		fprintf (stdout," [.Process Limits.]\n");
		// JOBOBJECT_BASIC_LIMIT_INFORMATION.LimitFlags - JOB_OBJECT_LIMIT_ACTIVE_PROCESS
        fprintf (stdout,"    -l <number> - Limit the number of process to this many\n");
		fprintf (stdout," [.Memory.]\n");
		// JOBOBJECT_BASIC_LIMIT_INFORMATION.LimitFlags - JOB_OBJECT_LIMIT_JOB_MEMORY
        fprintf (stdout,"    -m <bytes>  - Limit the total memory in bytes for the entire job\n");
		// JOBOBJECT_BASIC_LIMIT_INFORMATION.LimitFlags - JOB_OBJECT_LIMIT_PROCESS_MEMORY
		fprintf (stdout,"    -M <bytes>  - Limit the total memory in bytes for each process in the job\n");
		fprintf (stdout," [.Process Control.]\n");
		// JOBOBJECT_BASIC_LIMIT_INFORMATION.LimitFlags - JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE
        fprintf (stdout,"    -k          - Kill all process when the job handle dies\n");
		// JOBOBJECT_BASIC_LIMIT_INFORMATION.LimitFlags - JOB_OBJECT_LIMIT_BREAKAWAY_OK
		fprintf (stdout,"    -B          - Allow child process to be created with CREATE_BREAKAWAY_FROM_JOB (weak security)\n");
		// JOBOBJECT_BASIC_LIMIT_INFORMATION.LimitFlags - JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK
		fprintf (stdout,"    -b          - Allow child process which aren't part of the job (weak security)\n");
		fprintf (stdout," [.UI Security Controls.]\n");
		// JOBOBJECT_BASIC_UI_RESTRICTIONS - JOB_OBJECT_UILIMIT_DESKTOP
        fprintf (stdout,"    -d          - Prevent processes within the job from switching or creating desktops\n");
		// JOBOBJECT_BASIC_UI_RESTRICTIONS - JOB_OBJECT_UILIMIT_DISPLAYSETTINGS
        fprintf (stdout,"    -D          - Prevent processes within the job from calling the change display setting function\n");
        // JOBOBJECT_BASIC_UI_RESTRICTIONS - JOB_OBJECT_UILIMIT_EXITWINDOWS
		fprintf (stdout,"    -x          - Prevent processes within job from calling the exit Windows function\n");
		// JOBOBJECT_BASIC_UI_RESTRICTIONS - JOB_OBJECT_UILIMIT_GLOBALATOMS
		fprintf (stdout,"    -a          - Prevent processes within job from accessing global atoms\n");
		// JOBOBJECT_BASIC_UI_RESTRICTIONS - JOB_OBJECT_UILIMIT_HANDLES
		fprintf (stdout,"    -u          - Prevent processes within job from using user handles\n");
		// JOBOBJECT_BASIC_UI_RESTRICTIONS - JOB_OBJECT_UILIMIT_READCLIPBOARD
		fprintf (stdout,"    -c          - Prevent processes within job from reading the clipboard\n");
		// JOBOBJECT_BASIC_UI_RESTRICTIONS - JOB_OBJECT_UILIMIT_SYSTEMPARAMETERS
		fprintf (stdout,"    -s          - Prevent processes within job from changing system parameters\n");
		// JOBOBJECT_BASIC_UI_RESTRICTIONS - JOB_OBJECT_UILIMIT_WRITECLIPBOARD
		fprintf (stdout,"    -C          - Prevent processes within job from writing the clipboard\n");

		fprintf (stdout,"\n");
        ExitProcess(1);
}


//
// Function	: _tmain
// Purpose	: entry point
//
int _tmain(int argc, _TCHAR* argv[])
{
	DWORD dwPID = 0;
	char  chOpt;
	TCHAR strProcName[MAX_PATH];

	printf("[*] A Microsoft Windows Process Lockdown Tool using Job Objects - https://github.com/nccgroup/WindowsJobLock\n");
	printf("[*] NCC Group Plc - http://www.nccgroup.com/ \n");
	printf("[*] -h for help \n");

	// Extract all the options
	while ((chOpt = getopt(argc, argv, _T("P:p:l:m:M:n:gkBbdDxaucsCh"))) != EOF) 
		switch(chOpt)
		{
			case _T('g'):
				bListProcesses = TRUE;
				break;
			case _T('P'):
				strProcess = optarg;
				break;
			case _T('p'):
				dwPID = _tstoi(optarg);
				break;
			case _T('l'):
				dwProcessLimit = _tstoi(optarg);
				break;
			case _T('m'):
				dwJobMemory = _tstoi(optarg);
				break;
			case _T('M'):
				dwProcessMemory = _tstoi(optarg);
				break;
			case _T('n'):
				strName = optarg;
				break;
			case _T('k'):
				bKillProcOnJobClose = TRUE;
				break;
			case _T('B'):
				bBreakAwayOK = TRUE;
				break;
			case _T('b'):
				bSilentBreakAwayOK= TRUE;
				break;
			case _T('d'):
				bUILimitDesktop = TRUE;
				break;
			case _T('D'):
				bUILimitDispSettings = TRUE;
				break;
			case _T('x'):
				bUILimitExitWindows = TRUE;
				break;
			case _T('a'):
				bUILimitGlobalAtoms = TRUE;
				break;
			case _T('u'):
				bUILimitUserHandles = TRUE;
				break;
			case _T('c'):
				bUILimitReadClip = TRUE;
				break;
			case _T('s'):
				bUILimitSystemParams = TRUE;
				break;
			case _T('C'):
				bUILimitWriteClip = TRUE;
				break;
			case _T('h'):
				PrintHelp(argv[0]);
				return 0;
			default:
				fwprintf(stderr,L"[!] No handler - %c\n", chOpt);
				break;
        }

	if(bListProcesses) {
		FindProcess(NULL);
		return 0;
	}
	
	if(strProcess!=NULL){
		dwPID = FindProcess(strProcess);
	}

	if(dwPID == 0){
		if(strProcess != NULL) {
			_ftprintf(stderr,L"[!] Could not find the process %s\n",strProcess);
		} else {
			_ftprintf(stderr,L"[!] You need to specify a PID or valid process name (use -g to list processes)\n");
		}
		return -1;
	}

	if(!FindProcessName(dwPID,strProcName)){
		_ftprintf(stderr,L"[!] Could not find the name of the process for PID %d!\n",dwPID);
		return -1;
	} else {
		// this is so I can be lazy later
		strProcess = strProcName;
	}

	hProcess = OpenProcess(PROCESS_SET_QUOTA|PROCESS_TERMINATE|PROCESS_DUP_HANDLE,false,dwPID);
	if(hProcess == NULL || hProcess == INVALID_HANDLE_VALUE){
		_ftprintf(stderr,L"[!] Could not open process %s (PID %d) - %d\n",strProcName,dwPID,GetLastError());
		return -1;
	} else {
		_ftprintf(stdout,L"[*] Opened process %s\n",strProcName);
	}

	PrintSettings();
	if(!BuildAndDeploy()){
		_ftprintf(stderr,L"[!] Failed to build and deploy job object to %s..\n",strProcName);
		return -1;
	} else {
		_ftprintf(stdout,L"[*] Successfully built and deployed job object to %s!\n",strProcName);
	}

	return 0;
}


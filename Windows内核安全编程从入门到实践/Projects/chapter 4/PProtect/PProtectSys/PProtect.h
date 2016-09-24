#ifndef _pprotect_h_
#define _pprotect_h_


/************************************************************************/
/* º¯ÊýÉùÃ÷                                                             */
/************************************************************************/
NTSTATUS InitVerion();
NTSTATUS Init_HCode();
NTSTATUS InitAppRunValue();
ULONG    Hook_SSDT(ULONG fnID, PVOID fnName);
VOID     Unhook(ULONG fnID, ULONG RealAddr);

PCWSTR  GetFullProcessName();
ULONG   GetSysServiceId(IN PUNICODE_STRING uDllName, IN char* cSearchFnName);
BOOLEAN IsUACRunning();



typedef NTSTATUS(*fnNtCreateUserProcess)(
    OUT PHANDLE ProcessHandle,
    OUT PHANDLE ThreadHandle,
    IN ACCESS_MASK ProcessDesiredAccess,
    IN ACCESS_MASK ThreadDesiredAccess,
    IN POBJECT_ATTRIBUTES ProcessObjectAttributes OPTIONAL,
    IN POBJECT_ATTRIBUTES ThreadObjectAttributes OPTIONAL,
    IN ULONG CreateProcessFlags,
    IN ULONG CreateThreadFlags,
    IN PRTL_USER_PROCESS_PARAMETERS ProcessParameters,
    IN PVOID Parameter9,
    IN PVOID AttributeList
);
fnNtCreateUserProcess RealNtCreateUserProcess;



NTSTATUS myNtCreateUserProcess(
    OUT PHANDLE ProcessHandle,
    OUT PHANDLE ThreadHandle,
    IN ACCESS_MASK ProcessDesiredAccess,
    IN ACCESS_MASK ThreadDesiredAccess,
    IN POBJECT_ATTRIBUTES ProcessObjectAttributes OPTIONAL,
    IN POBJECT_ATTRIBUTES ThreadObjectAttributes OPTIONAL,
    IN ULONG CreateProcessFlags,
    IN ULONG CreateThreadFlags,
    IN PRTL_USER_PROCESS_PARAMETERS ProcessParameters,
    IN PVOID Parameter9,
    IN PVOID AttributeList
);



#endif
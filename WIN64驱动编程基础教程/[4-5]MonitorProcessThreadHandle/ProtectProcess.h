
NTKERNELAPI PEPROCESS IoThreadToProcess(PETHREAD Thread);
NTKERNELAPI char* PsGetProcessImageFileName(PEPROCESS Process);

void BypassCheckSign(PDRIVER_OBJECT pDriverObj)
{
    //STRUCT FOR WIN64
    typedef struct _LDR_DATA {                                  // 24 elements, 0xE0 bytes (sizeof)
        struct _LIST_ENTRY InLoadOrderLinks;                     // 2 elements, 0x10 bytes (sizeof)
        struct _LIST_ENTRY InMemoryOrderLinks;                   // 2 elements, 0x10 bytes (sizeof)
        struct _LIST_ENTRY InInitializationOrderLinks;           // 2 elements, 0x10 bytes (sizeof)
        VOID*        DllBase;
        VOID*        EntryPoint;
        ULONG32      SizeOfImage;
        UINT8        _PADDING0_[0x4];
        struct _UNICODE_STRING FullDllName;                      // 3 elements, 0x10 bytes (sizeof)
        struct _UNICODE_STRING BaseDllName;                      // 3 elements, 0x10 bytes (sizeof)
        ULONG32      Flags;
    } LDR_DATA, *PLDR_DATA;
    PLDR_DATA ldr;
    ldr = (PLDR_DATA)(pDriverObj->DriverSection);
    ldr->Flags |= 0x20;
}

BOOLEAN IsProtectedProcessName(PEPROCESS eprocess)
{
    char *Name = PsGetProcessImageFileName(eprocess);

    if (!_stricmp("calc.exe", Name))
        return TRUE;
    else
        return FALSE;
}

PVOID obHandle = NULL, obHandle2 = NULL;

OB_PREOP_CALLBACK_STATUS preCall(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION pOperationInformation)
{
#define PROCESS_TERMINATE 0x1
    HANDLE pid;

    if (pOperationInformation->ObjectType != *PsProcessType)
        goto exit_sub;

    pid = PsGetProcessId((PEPROCESS)pOperationInformation->Object);
    DbgPrint("[OBCALLBACK][Process]PID=%ld\n", pid);
    UNREFERENCED_PARAMETER(RegistrationContext);

    if (IsProtectedProcessName((PEPROCESS)pOperationInformation->Object)) {
        if (pOperationInformation->Operation == OB_OPERATION_HANDLE_CREATE) {
            //pOperationInformation->Parameters->CreateHandleInformation.DesiredAccess=0;
            if ((pOperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & PROCESS_TERMINATE) == PROCESS_TERMINATE) {
                pOperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
            }
        }

        if (pOperationInformation->Operation == OB_OPERATION_HANDLE_DUPLICATE) {
            //pOperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess=0;
            if ((pOperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess & PROCESS_TERMINATE) == PROCESS_TERMINATE) {
                pOperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~PROCESS_TERMINATE;
            }
        }
    }

exit_sub:
    return OB_PREOP_SUCCESS;
}

OB_PREOP_CALLBACK_STATUS preCall2(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION pOperationInformation)
{
#define THREAD_TERMINATE2 0x1
    PEPROCESS ep;
    PETHREAD et;
    HANDLE pid;

    if (pOperationInformation->ObjectType != *PsThreadType)
        goto exit_sub;

    et = (PETHREAD)pOperationInformation->Object;
    ep = IoThreadToProcess(et);
    pid = PsGetProcessId(ep);
    DbgPrint("[OBCALLBACK][Thread]PID=%ld; TID=%ld\n", pid, PsGetThreadId(et));
    UNREFERENCED_PARAMETER(RegistrationContext);

    if (IsProtectedProcessName(ep)) {
        if (pOperationInformation->Operation == OB_OPERATION_HANDLE_CREATE) {
            //pOperationInformation->Parameters->CreateHandleInformation.DesiredAccess=0;
            if ((pOperationInformation->Parameters->CreateHandleInformation.OriginalDesiredAccess & THREAD_TERMINATE2) == THREAD_TERMINATE2) {
                pOperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= ~THREAD_TERMINATE2;
            }
        }

        if (pOperationInformation->Operation == OB_OPERATION_HANDLE_DUPLICATE) {
            //pOperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess=0;
            if ((pOperationInformation->Parameters->DuplicateHandleInformation.OriginalDesiredAccess & THREAD_TERMINATE2) == THREAD_TERMINATE2) {
                pOperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess &= ~THREAD_TERMINATE2;
            }
        }
    }

exit_sub:
    return OB_PREOP_SUCCESS;
}

NTSTATUS ObProtectProcess(BOOLEAN Enable)
{
    if (Enable == TRUE) {
        NTSTATUS obst1 = 0, obst2 = 0;
        OB_CALLBACK_REGISTRATION obReg, obReg2;
        OB_OPERATION_REGISTRATION opReg, opReg2;
        //reg ob callback 1
        memset(&obReg, 0, sizeof(obReg));
        obReg.Version = ObGetFilterVersion();
        obReg.OperationRegistrationCount = 1;
        obReg.RegistrationContext = NULL;
        RtlInitUnicodeString(&obReg.Altitude, L"321124");
        obReg.OperationRegistration = &opReg;
        memset(&opReg, 0, sizeof(opReg));
        opReg.ObjectType = PsProcessType;
        opReg.Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
        opReg.PreOperation = (POB_PRE_OPERATION_CALLBACK)&preCall;
        obst1 = ObRegisterCallbacks(&obReg, &obHandle);
        //reg ob callback 2
        memset(&obReg2, 0, sizeof(obReg2));
        obReg2.Version = ObGetFilterVersion();
        obReg2.OperationRegistrationCount = 1;
        obReg2.RegistrationContext = NULL;
        RtlInitUnicodeString(&obReg2.Altitude, L"321125");
        obReg2.OperationRegistration = &opReg2;
        memset(&opReg2, 0, sizeof(opReg2));
        opReg2.ObjectType = PsThreadType;
        opReg2.Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
        opReg2.PreOperation = (POB_PRE_OPERATION_CALLBACK)&preCall2;
        obst1 = ObRegisterCallbacks(&obReg2, &obHandle2);
        return NT_SUCCESS(obst1) & NT_SUCCESS(obst2);
    } else {
        if (obHandle != NULL)
            ObUnRegisterCallbacks(obHandle);

        if (obHandle2 != NULL)
            ObUnRegisterCallbacks(obHandle2);

        return TRUE;
    }
}
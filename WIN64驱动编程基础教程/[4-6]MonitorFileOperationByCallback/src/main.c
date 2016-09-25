//Author: Tesla.Angela

#include "obstruct.h"

DRIVER_INITIALIZE DriverEntry;
PVOID obHandle;

OB_PREOP_CALLBACK_STATUS preCall(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation)
{
    UNICODE_STRING DosName;
    PFILE_OBJECT fileo = OperationInformation->Object;
    HANDLE CurrentProcessId = PsGetCurrentProcessId();
    UNREFERENCED_PARAMETER(RegistrationContext);

    if (OperationInformation->ObjectType != *IoFileObjectType)
        return OB_PREOP_SUCCESS;

    //过滤无效指针
    if (fileo->FileName.Buffer == NULL             ||
        !MmIsAddressValid(fileo->FileName.Buffer)   ||
        fileo->DeviceObject == NULL                   ||
        !MmIsAddressValid(fileo->DeviceObject))
        return OB_PREOP_SUCCESS;

    //过滤无效路径
    if (!_wcsicmp(fileo->FileName.Buffer, L"\\Endpoint")  ||
        !_wcsicmp(fileo->FileName.Buffer, L"?")          ||
        !_wcsicmp(fileo->FileName.Buffer, L"\\.\\.")     ||
        !_wcsicmp(fileo->FileName.Buffer, L"\\"))
        return OB_PREOP_SUCCESS;

    //阻止访问readme.txt
    if (wcsstr(_wcslwr(fileo->FileName.Buffer), L"readme.txt")) {
        if (OperationInformation->Operation == OB_OPERATION_HANDLE_CREATE) {
            OperationInformation->Parameters->CreateHandleInformation.DesiredAccess = 0;
        }

        if (OperationInformation->Operation == OB_OPERATION_HANDLE_DUPLICATE) {
            OperationInformation->Parameters->DuplicateHandleInformation.DesiredAccess = 0;
        }
    }

    RtlVolumeDeviceToDosName(fileo->DeviceObject, &DosName);
    DbgPrint("[FILE_MONITOR_X64][PID]%ld [File]%wZ%wZ\n", (ULONG64)CurrentProcessId, &DosName, &fileo->FileName);
    return OB_PREOP_SUCCESS;
}

VOID EnableObType(POBJECT_TYPE ObjectType)
{
    PMY_OBJECT_TYPE myobtype = (PMY_OBJECT_TYPE)ObjectType;
    myobtype->TypeInfo.SupportsObjectCallbacks = 1;
}

VOID Unload(IN PDRIVER_OBJECT DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);
    DbgPrint("[FILE_MONITOR_X64] driver unloading...\n");
    ObUnRegisterCallbacks(obHandle);
}

NTSTATUS DriverEntry
(
    __in PDRIVER_OBJECT   DriverObject,
    __in PUNICODE_STRING  RegistryPath
)
{
    NTSTATUS status = STATUS_SUCCESS;
    OB_CALLBACK_REGISTRATION obReg;
    OB_OPERATION_REGISTRATION opReg;
    PLDR_DATA ldr;
    UNREFERENCED_PARAMETER(RegistryPath);
    DriverObject->DriverUnload = &Unload;
    // enable IoFileObjectType
    EnableObType(*IoFileObjectType);
    // bypass MmVerifyCallbackFunction
    ldr = (PLDR_DATA)DriverObject->DriverSection;
    ldr->Flags |= 0x20;
    // init callbacks
    memset(&obReg, 0, sizeof(obReg));
    obReg.Version = ObGetFilterVersion();
    obReg.OperationRegistrationCount = 1;
    obReg.RegistrationContext = NULL;
    RtlInitUnicodeString(&obReg.Altitude, L"321000");
    obReg.OperationRegistration = &opReg;
    memset(&opReg, 0, sizeof(opReg));
    opReg.ObjectType = IoFileObjectType;
    opReg.Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
    opReg.PreOperation = (POB_PRE_OPERATION_CALLBACK)&preCall;
    //opReg.PostOperation = (POB_POST_OPERATION_CALLBACK)&postCall; //不需要
    // register callbacks
    status = ObRegisterCallbacks(&obReg, &obHandle);

    if (!NT_SUCCESS(status)) {
        DbgPrint("[FILE_MONITOR_X64] ERROR: events sources not installed\n");
        status = STATUS_UNSUCCESSFUL;
    } else
        DbgPrint("[FILE_MONITOR_X64] SUCCESS: callbacks registered\n");

    return status;
}
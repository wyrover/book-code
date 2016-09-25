#include <ntddk.h>
#include <windef.h>

PVOID GetFunctionAddr(PCWSTR FunctionName)
{
    UNICODE_STRING UniCodeFunctionName;
    RtlInitUnicodeString(&UniCodeFunctionName, FunctionName);
    return MmGetSystemRoutineAddress(&UniCodeFunctionName);
}

//NTKERNELAPI NTSTATUS ZwRenameKey(HANDLE KeyHandle, PUNICODE_STRING NewName);
typedef NTSTATUS(__fastcall *ZWRENAMEKEY)(HANDLE KeyHandle, PUNICODE_STRING NewName);
ZWRENAMEKEY MyZwRenameKey = NULL;

void RegCreateKey(LPWSTR KeyName)
{
    OBJECT_ATTRIBUTES objectAttributes;
    UNICODE_STRING usKeyName;
    NTSTATUS ntStatus;
    HANDLE hRegister;
    RtlInitUnicodeString(&usKeyName, KeyName);
    InitializeObjectAttributes(&objectAttributes,
                               &usKeyName,
                               OBJ_CASE_INSENSITIVE,//对大小写敏感
                               NULL,
                               NULL);
    ntStatus = ZwCreateKey(&hRegister, KEY_ALL_ACCESS, &objectAttributes, 0, NULL, REG_OPTION_NON_VOLATILE, NULL);

    if (NT_SUCCESS(ntStatus)) {
        ZwClose(hRegister);
        DbgPrint("ZwCreateKey success!\n");
    } else {
        DbgPrint("ZwCreateKey failed!\n");
    }
}

void RegRenameKey(LPWSTR OldKeyName, LPWSTR NewKeyName)
{
    OBJECT_ATTRIBUTES objectAttributes;
    HANDLE hRegister;
    NTSTATUS ntStatus;
    UNICODE_STRING usOldKeyName, usNewKeyName;
    RtlInitUnicodeString(&usOldKeyName, OldKeyName);
    RtlInitUnicodeString(&usNewKeyName, NewKeyName);
    InitializeObjectAttributes(&objectAttributes,
                               &usOldKeyName,
                               OBJ_CASE_INSENSITIVE,//对大小写敏感
                               NULL,
                               NULL);
    ntStatus = ZwOpenKey(&hRegister, KEY_ALL_ACCESS, &objectAttributes);

    if (NT_SUCCESS(ntStatus)) {
        ntStatus = MyZwRenameKey(hRegister, &usNewKeyName);
        ZwFlushKey(hRegister);
        ZwClose(hRegister);
        DbgPrint("ZwRenameKey success!\n");
    } else {
        DbgPrint("ZwRenameKey failed!\n");
    }
}

void RegDeleteKey(LPWSTR KeyName)
{
    OBJECT_ATTRIBUTES objectAttributes;
    UNICODE_STRING usKeyName;
    NTSTATUS ntStatus;
    HANDLE hRegister;
    RtlInitUnicodeString(&usKeyName, KeyName);
    InitializeObjectAttributes(&objectAttributes,
                               &usKeyName,
                               OBJ_CASE_INSENSITIVE,//对大小写敏感
                               NULL,
                               NULL);
    ntStatus = ZwOpenKey(&hRegister, KEY_ALL_ACCESS, &objectAttributes);

    if (NT_SUCCESS(ntStatus)) {
        ntStatus = ZwDeleteKey(hRegister);
        ZwClose(hRegister);
        DbgPrint("ZwDeleteKey success!\n");
    } else {
        DbgPrint("ZwDeleteKey failed!\n");
    }
}

void RegSetValueKey(LPWSTR KeyName, LPWSTR ValueName, DWORD DataType, PVOID DataBuffer, DWORD DataLength)
{
    OBJECT_ATTRIBUTES objectAttributes;
    UNICODE_STRING usKeyName, usValueName;
    NTSTATUS ntStatus;
    HANDLE hRegister;
    ULONG Type;
    RtlInitUnicodeString(&usKeyName, KeyName);
    RtlInitUnicodeString(&usValueName, ValueName);
    InitializeObjectAttributes(&objectAttributes,
                               &usKeyName,
                               OBJ_CASE_INSENSITIVE,//对大小写敏感
                               NULL,
                               NULL);
    ntStatus = ZwOpenKey(&hRegister, KEY_ALL_ACCESS, &objectAttributes);

    if (NT_SUCCESS(ntStatus)) {
        ntStatus = ZwSetValueKey(hRegister, &usValueName, 0, DataType, DataBuffer, DataLength);
        ZwFlushKey(hRegister);
        ZwClose(hRegister);
        DbgPrint("ZwSetValueKey success!\n");
    } else {
        DbgPrint("ZwSetValueKey failed!\n");
    }
}

/*
typedef struct _KEY_VALUE_PARTIAL_INFORMATION {
  ULONG TitleIndex;
  ULONG Type;
  ULONG DataLength;
  UCHAR Data[1];
} KEY_VALUE_PARTIAL_INFORMATION, *PKEY_VALUE_PARTIAL_INFORMATION;
*/
NTSTATUS RegQueryValueKey(LPWSTR KeyName, LPWSTR ValueName, PKEY_VALUE_PARTIAL_INFORMATION *pkvpi)
{
    ULONG ulSize;
    NTSTATUS ntStatus;
    PKEY_VALUE_PARTIAL_INFORMATION pvpi;
    OBJECT_ATTRIBUTES objectAttributes;
    HANDLE hRegister;
    UNICODE_STRING usKeyName;
    UNICODE_STRING usValueName;
    RtlInitUnicodeString(&usKeyName, KeyName);
    RtlInitUnicodeString(&usValueName, ValueName);
    InitializeObjectAttributes(&objectAttributes,
                               &usKeyName,
                               OBJ_CASE_INSENSITIVE,//对大小写敏感
                               NULL,
                               NULL);
    ntStatus = ZwOpenKey(&hRegister, KEY_ALL_ACCESS, &objectAttributes);

    if (!NT_SUCCESS(ntStatus)) {
        DbgPrint("[RegQueryValueKey]ZwOpenKey failed!\n");
        return ntStatus;
    }

    ntStatus = ZwQueryValueKey(hRegister,
                               &usValueName,
                               KeyValuePartialInformation ,
                               NULL,
                               0,
                               &ulSize);

    if (ntStatus == STATUS_OBJECT_NAME_NOT_FOUND || ulSize == 0) {
        DbgPrint("ZwQueryValueKey 1 failed!\n");
        return STATUS_UNSUCCESSFUL;
    }

    pvpi = (PKEY_VALUE_PARTIAL_INFORMATION)ExAllocatePool(PagedPool, ulSize);
    ntStatus = ZwQueryValueKey(hRegister,
                               &usValueName,
                               KeyValuePartialInformation ,
                               pvpi,
                               ulSize,
                               &ulSize);

    if (!NT_SUCCESS(ntStatus)) {
        DbgPrint("ZwQueryValueKey 2 failed!\n");
        return STATUS_UNSUCCESSFUL;
    }

    //这里的pvpi是没有释放的用完要释放。ExFreePool(pvpi);
    *pkvpi = pvpi;
    DbgPrint("ZwQueryValueKey success!\n");
    return STATUS_SUCCESS;
}

void RegDeleteValueKey(LPWSTR KeyName, LPWSTR ValueName)
{
    OBJECT_ATTRIBUTES objectAttributes;
    UNICODE_STRING usKeyName, usValueName;
    NTSTATUS ntStatus;
    HANDLE hRegister;
    RtlInitUnicodeString(&usKeyName, KeyName);
    RtlInitUnicodeString(&usValueName, ValueName);
    InitializeObjectAttributes(&objectAttributes,
                               &usKeyName,
                               OBJ_CASE_INSENSITIVE,//对大小写敏感
                               NULL,
                               NULL);
    ntStatus = ZwOpenKey(&hRegister, KEY_ALL_ACCESS, &objectAttributes);

    if (NT_SUCCESS(ntStatus)) {
        ntStatus = ZwDeleteValueKey(hRegister, &usValueName);
        ZwFlushKey(hRegister);
        ZwClose(hRegister);
        DbgPrint("ZwDeleteValueKey success!\n");
    } else {
        DbgPrint("ZwDeleteValueKey failed!\n");
    }
}

VOID EnumSubKeyTest()
{
    WCHAR MY_KEY_NAME[] = L"\\Registry\\Machine\\Software";
    UNICODE_STRING RegUnicodeString;
    HANDLE hRegister;
    OBJECT_ATTRIBUTES objectAttributes;
    NTSTATUS ntStatus;
    ULONG ulSize, i;
    UNICODE_STRING uniKeyName;
    PKEY_FULL_INFORMATION pfi;
    //初始化UNICODE_STRING字符串
    RtlInitUnicodeString(&RegUnicodeString, MY_KEY_NAME);
    //初始化objectAttributes
    InitializeObjectAttributes(&objectAttributes,
                               &RegUnicodeString,
                               OBJ_CASE_INSENSITIVE,//对大小写敏感
                               NULL,
                               NULL);
    //打开注册表
    ntStatus = ZwOpenKey(&hRegister,
                         KEY_ALL_ACCESS,
                         &objectAttributes);

    if (NT_SUCCESS(ntStatus)) {
        DbgPrint("Open register successfully\n");
    }

    //第一次调用ZwQueryKey为了获取KEY_FULL_INFORMATION数据的长度
    ZwQueryKey(hRegister, KeyFullInformation, NULL, 0, &ulSize);
    pfi = (PKEY_FULL_INFORMATION)ExAllocatePool(PagedPool, ulSize);
    //第二次调用ZwQueryKey为了获取KEY_FULL_INFORMATION数据的数据
    ZwQueryKey(hRegister, KeyFullInformation, pfi, ulSize, &ulSize);

    for (i = 0; i < pfi->SubKeys; i++) {
        PKEY_BASIC_INFORMATION pbi;
        //第一次调用ZwEnumerateKey为了获取KEY_BASIC_INFORMATION数据的长度
        ZwEnumerateKey(hRegister, i, KeyBasicInformation, NULL, 0, &ulSize);
        pbi = (PKEY_BASIC_INFORMATION)ExAllocatePool(PagedPool, ulSize);
        //第二次调用ZwEnumerateKey为了获取KEY_BASIC_INFORMATION数据的数据
        ZwEnumerateKey(hRegister, i, KeyBasicInformation, pbi, ulSize, &ulSize);
        uniKeyName.Length = (USHORT)pbi->NameLength;
        uniKeyName.MaximumLength = (USHORT)pbi->NameLength;
        uniKeyName.Buffer = pbi->Name;
        DbgPrint("The %d sub item name:%wZ\n", i, &uniKeyName);
        ExFreePool(pbi);
    }

    ExFreePool(pfi);
    ZwClose(hRegister);
}

VOID EnumSubValueTest()
{
    WCHAR MY_KEY_NAME[] = L"\\Registry\\Machine\\Software\\Microsoft\\.NETFramework";
    UNICODE_STRING RegUnicodeString;
    HANDLE hRegister;
    OBJECT_ATTRIBUTES objectAttributes;
    ULONG ulSize, i;
    UNICODE_STRING uniKeyName;
    PKEY_FULL_INFORMATION pfi;
    NTSTATUS ntStatus;
    //初始化UNICODE_STRING字符串
    RtlInitUnicodeString(&RegUnicodeString, MY_KEY_NAME);
    //初始化objectAttributes
    InitializeObjectAttributes(&objectAttributes,
                               &RegUnicodeString,
                               OBJ_CASE_INSENSITIVE,//对大小写敏感
                               NULL,
                               NULL);
    //打开注册表
    ntStatus = ZwOpenKey(&hRegister, KEY_ALL_ACCESS, &objectAttributes);

    if (NT_SUCCESS(ntStatus)) {
        DbgPrint("Open register successfully\n");
    }

    //查询VALUE的大小
    ZwQueryKey(hRegister, KeyFullInformation, NULL, 0, &ulSize);
    pfi = (PKEY_FULL_INFORMATION)   ExAllocatePool(PagedPool, ulSize);
    ZwQueryKey(hRegister, KeyFullInformation, pfi, ulSize, &ulSize);

    for (i = 0; i < pfi->Values; i++) {
        PKEY_VALUE_BASIC_INFORMATION pvbi;
        //查询单个VALUE的大小
        ZwEnumerateValueKey(hRegister, i, KeyValueBasicInformation, NULL, 0, &ulSize);
        pvbi = (PKEY_VALUE_BASIC_INFORMATION)ExAllocatePool(PagedPool, ulSize);
        //查询单个VALUE的详情
        ZwEnumerateValueKey(hRegister, i, KeyValueBasicInformation, pvbi, ulSize, &ulSize);
        uniKeyName.Length = (USHORT)pvbi->NameLength;
        uniKeyName.MaximumLength = (USHORT)pvbi->NameLength;
        uniKeyName.Buffer = pvbi->Name;
        DbgPrint("The %d sub value name:%wZ\n", i, &uniKeyName);

        if (pvbi->Type == REG_SZ) {
            DbgPrint("The sub value type:REG_SZ\n");
        } else if (pvbi->Type == REG_MULTI_SZ) {
            DbgPrint("The sub value type:REG_MULTI_SZ\n");
        } else if (pvbi->Type == REG_DWORD) {
            DbgPrint("The sub value type:REG_DWORD\n");
        } else if (pvbi->Type == REG_BINARY) {
            DbgPrint("The sub value type:REG_BINARY\n");
        }

        ExFreePool(pvbi);
    }

    ExFreePool(pfi);
    ZwClose(hRegister);
}


VOID DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
    DbgPrint("[MyDriver]Unloaded...\n");
    return;
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
    DWORD dw = 1234, dw0 = 0;
    PKEY_VALUE_PARTIAL_INFORMATION pkvi;
    DriverObject->DriverUnload = DriverUnload;
    DbgPrint("[MyDriver]Loaded...\n");
    //
    MyZwRenameKey = (ZWRENAMEKEY)GetFunctionAddr(L"ZwRenameKey"); //DbgPrint("ZwRenameKey: %p\n",MyZwRenameKey);
    //
    DbgPrint("枚举子KEY测试\n");
    EnumSubKeyTest();
    DbgPrint("\n");
    DbgPrint("枚举子VALUE测试\n");
    EnumSubValueTest();
    DbgPrint("\n");
    DbgPrint("创建KEY测试\n");
    RegCreateKey(L"\\Registry\\Machine\\Software\\0000");
    DbgPrint("\n");
    DbgPrint("重命名KEY测试\n");
    RegRenameKey(L"\\Registry\\Machine\\Software\\0000", L"1111");
    DbgPrint("\n");
    DbgPrint("新建/设置VALUE测试\n");
    RegSetValueKey(L"\\Registry\\Machine\\Software\\1111", L"dw", REG_DWORD, &dw, sizeof(dw));
    DbgPrint("\n");
    DbgPrint("查询VALUE测试\n");
    RegQueryValueKey(L"\\Registry\\Machine\\Software\\1111", L"dw", &pkvi);
    memcpy(&dw0, pkvi->Data, pkvi->DataLength);
    DbgPrint("dw0: %ld\n", dw0);
    ExFreePool(pkvi);
    DbgPrint("\n");
    DbgPrint("删除VALUE测试\n");
    RegDeleteValueKey(L"\\Registry\\Machine\\Software\\1111", L"dw");
    DbgPrint("\n");
    DbgPrint("删除KEY测试\n");
    RegDeleteKey(L"\\Registry\\Machine\\Software\\1111");
    DbgPrint("\n");
    //
    return STATUS_SUCCESS;
}
#ifndef NT_SUCCESS
    #define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#endif
#ifndef STATUS_SUCCESS
    #define STATUS_SUCCESS       ((NTSTATUS)0x00000000L)
#endif
#ifndef STATUS_UNSUCCESSFUL
    #define STATUS_UNSUCCESSFUL ((NTSTATUS)0xC0000001L)
#endif

#define SystemModuleInformation 11
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)

typedef struct _SYSTEM_MODULE_INFORMATION_ENTRY {
    ULONG Unknow1;
    ULONG Unknow2;
    ULONG Unknow3;
    ULONG Unknow4;
    PVOID64 Base;
    ULONG Size;
    ULONG Flags;
    USHORT Index;
    USHORT NameLength;
    USHORT LoadCount;
    USHORT ModuleNameOffset;
    char ImageName[256];
} SYSTEM_MODULE_INFORMATION_ENTRY, *PSYSTEM_MODULE_INFORMATION_ENTRY;

typedef struct _SYSTEM_MODULE_INFORMATION {
    ULONG Count;//内核中以加载的模块的个数
    SYSTEM_MODULE_INFORMATION_ENTRY Module[1];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

typedef long (__stdcall *ZWQUERYSYSTEMINFORMATION)(
    IN ULONG SystemInformationClass,
    IN OUT PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    IN PULONG ReturnLength OPTIONAL);
ZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation;

ULONGLONG GetNtosBaseAndPath(char *ModuleName)
{
    ULONG NeedSize, i, ModuleCount, BufferSize = 0x5000;
    PVOID pBuffer = NULL;
    ULONGLONG qwBase = 0;
    NTSTATUS Result;
    PSYSTEM_MODULE_INFORMATION pSystemModuleInformation;

    do {
        //分配内存
        pBuffer = malloc(BufferSize);

        if (pBuffer == NULL) {
            //printf("malloc Error\n");
            return FALSE;
        }

        //查询模块信息
        Result = ZwQuerySystemInformation(SystemModuleInformation, pBuffer, BufferSize, &NeedSize);

        if (Result == STATUS_INFO_LENGTH_MISMATCH) {
            free(pBuffer);
            BufferSize *= 2;
        } else if (!NT_SUCCESS(Result)) {
            //查询失败则退出
            //printf( "ZwQueryInformation Error,the error code:%8X\n", Result );
            free(pBuffer);
            return FALSE;
        }
    } while (Result == STATUS_INFO_LENGTH_MISMATCH);

    pSystemModuleInformation = (PSYSTEM_MODULE_INFORMATION)pBuffer;
    //获得模块的总数量
    ModuleCount = pSystemModuleInformation->Count;

    //遍历所有的模块
    //printf("\n");
    //printf("ID\tBase Address\t\tFull Path\n");
    //printf("--\t------------\t\t---------\n");
    /*for( i = 0; i < ModuleCount; i++ )
    {
        //获得模块的路径
        drv_path=KmGetDrvPath(Replace(pSystemModuleInformation->Module[i].ImageName,"\\??\\","",1,1,1));
        if(OnlyShowNoSign==0) //显示所有模块
        {
            if((ULONG64)(pSystemModuleInformation->Module[i].Base) > (ULONG64)0x8000000000000000)
                printf("%0.4ld\t0x%0.16llx\t%s\n",i,pSystemModuleInformation->Module[i].Base,AddTab(drv_path,48,4)); //+pSystemModuleInformation->Module[i].ModuleNameOffset
        }
        else //仅仅显示不带签名的模块
        {
            if(CheckFileTrust(drv_path)==FALSE && (ULONG64)(pSystemModuleInformation->Module[i].Base) > (ULONG64)0x8000000000000000)
                printf("%0.4ld\t0x%0.16llx\t%s\n",i,pSystemModuleInformation->Module[i].Base,AddTab(drv_path,48,4));
        }

    }*/
    if (ModuleName != NULL)
        strcpy(ModuleName, pSystemModuleInformation->Module[0].ImageName + pSystemModuleInformation->Module[0].ModuleNameOffset);

    qwBase = (ULONGLONG)pSystemModuleInformation->Module[0].Base;
    free(pBuffer);
    return qwBase;
}
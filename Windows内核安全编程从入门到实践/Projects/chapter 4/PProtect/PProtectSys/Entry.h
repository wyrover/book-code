#ifndef _entry_h_
#define _entry_h_

/************************************************************************/
/* 宏                                                                   */
/************************************************************************/
#define fPProtect                   0x01<<0
#define fAppRun                     0x01<<1


#define FlagOn(_a,_b) (_a&_b)

#define GetProcessImageName(_a) ((char*)((ULONG)_a + ImageFileNameOffset))

/************************************************************************/
/* 全局变量及数据结构                                                   */
/************************************************************************/
extern  HANDLE  g_hMeID;
extern  PKEVENT gEvent_A;
extern  PKEVENT gEvent_B;
extern  PKMUTEX gMutex;
extern  int     iChoose;
extern  PVOID   pShareMM_SYS;
extern  PVOID   pShareMM_User;
extern  PMDL    pShareMM_MDL;

extern  ULONG   ImageFileNameOffset;

extern ULONG NtCreateUserProcessID;

extern ULONG *MmSectionObjectType;

typedef struct _GlobalFlag_ {
    BOOLEAN bPProtect ;
    BOOLEAN bAppRun ;

} GlobalFlag;
extern GlobalFlag GFlag;

typedef struct _RTL_USER_PROCESS_PARAMETERS_ {
    char Reserved1[16];
    PVOID Reserved2[10];
    UNICODE_STRING ImagePathName;
    UNICODE_STRING CommandLine;

} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _SystemServiceDescriptorTable {
    PVOID    ServiceTableBase;
    PULONG    ServiceCounterTableBase;
    ULONG    NumberOfService;
    ULONG    ParamTableBase;
} SystemServiceTable, *PSystemServiceTable;
extern    PSystemServiceTable    KeServiceDescriptorTable;

typedef struct _RealServiceAddr {

    ULONG NtCreateUserProcess;


} RealServiceAddr, *pRealServiceAddr;
extern RealServiceAddr RealAddr;

enum _NotifyType_ {
    ProcessMonitor = 1,
    AntiZero
};

typedef struct _NotifyBlock {
    ULONG   NotifyType;
    WCHAR   CurPro[260];                //当前进程名

    union {
        struct _ProcessMonitor_ {
            WCHAR    R_Pro[100];
            BOOLEAN  SHAChanged;        //在白名单中查看是否SHA值被修改
            char     SHA1[20];          //SHA值
        } ProMon;

//      struct _AntiZero_
//      {
//          WCHAR dll[260];
//          ULONG AttackType;
//      }Anti0;
    } u;

} NotifyBlock, *pNotifyBlock;

/************************************************************************/
/* 函数声明                                                             */
/************************************************************************/
VOID PProtect_Unload(IN PDRIVER_OBJECT DriverObject);
NTSTATUS PProtect_Create(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp);
NTSTATUS PProtect_Close(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp);
NTSTATUS PProtect_Cleanup(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp);
NTSTATUS PProtect_DeivceIoControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp);
NTSTATUS PProtect_DispatchShutdown(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp);


#endif

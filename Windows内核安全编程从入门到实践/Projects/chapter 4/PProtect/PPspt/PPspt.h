// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 PPSPT_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// PPSPT_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef PPSPT_EXPORTS
    #define PPSPT_API __declspec(dllexport)
#else
    #define PPSPT_API __declspec(dllimport)
#endif


#define fPProtect                   0x01<<0
#define fAppRun                     0x01<<1

#define SetBit(_a,_b) (_a |= _b)
#define ClearBit(_a,_b) (_a &= ~_b)

typedef struct _ProMonItem_ {
    WCHAR wProcessName[50];
    int oper;
    WCHAR sha1[21];
    WCHAR wProcessPath[200];
    WCHAR wLastTime[20];

} ProMonItem;

typedef struct _NotifyBlock {
    ULONG   NotifyType;
    WCHAR   CurPro[260];                //当前进程名

    union {
        struct _ProcessMonitor_ {
            WCHAR    R_Pro[100];
            BOOL     SHAChanged;        //在白名单中查看是否SHA值被修改
            char     SHA1[20];          //SHA值
        } ProMon;

//      struct _AntiZero_
//      {
//          WCHAR dll[260];
//          ULONG AttackType;
//      }Anti0;
    } u;

} NotifyBlock, *pNotifyBlock;

extern "C"
PPSPT_API int fnPPspt(void);

extern "C"
PPSPT_API BOOL fnFirstRun();

extern "C"
PPSPT_API BOOL fnInitApp(
    IN OUT PVOID* pShareMemory
);

extern "C"
PPSPT_API BOOL fnInitReg(OUT int* flagControl);

extern "C"
PPSPT_API BOOL fnGetAntiZInfo(OUT WCHAR* detailInfo);

extern "C"
PPSPT_API BOOL fnUpdateFlag(
    IN int bit,
    IN BOOL bDirect
);

extern "C"
PPSPT_API int fnGetRegProItemCount();

extern "C"
PPSPT_API BOOL fnGetRegProItem(
    IN int index,
    OUT ProMonItem* item
);



extern "C"
PPSPT_API BOOL fnStartChoose();

extern "C"
PPSPT_API BOOL fnSigVerif();

extern "C"
PPSPT_API BOOL fnGetIcoPath(OUT WCHAR* icoPath);

extern "C"
PPSPT_API BOOL fnSetASLRFlag(BOOL bDirect);

BOOL Add2ProMonRules(
    IN BOOL bAllowRun,
    IN WCHAR *wPath
);

BOOL GetSHA1(
    IN WCHAR *wPath,
    OUT char *SHA1
);

DWORD
WINAPI
InitFnSigVerif(LPVOID lpParameter);

BOOL RegDelnode(HKEY hKeyRoot, WCHAR* lpSubKey);
BOOL
DelRegKey(
    HKEY hKeyRoot,
    WCHAR* lpSubKey
);

/************************************************************************/
/* 日志打印                                                             */
/************************************************************************/
#ifdef Debug

VOID DebugPrintA(
    CHAR *Format,
    ...
);

VOID DebugPrintW(
    WCHAR *Format,
    ...
);

#define DbgPrintA(args) DebugPrintA args
#define DbgPrintW(args) DebugPrintW args

#else

#define DbgPrintA(args)
#define DbgPrintW(args)

#endif
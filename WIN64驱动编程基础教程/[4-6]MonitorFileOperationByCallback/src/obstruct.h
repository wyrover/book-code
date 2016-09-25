#include <ntddk.h>
#include <string.h>

typedef struct _CALLBACK_ENTRY {
    LIST_ENTRY CallbackList;
    OB_OPERATION  Operations;
    ULONG Active;
    PVOID Handle;
    POBJECT_TYPE ObjectType;
    POB_PRE_OPERATION_CALLBACK  PreOperation;
    POB_POST_OPERATION_CALLBACK PostOperation;
    ULONG unknown;
} CALLBACK_ENTRY, *PCALLBACK_ENTRY;

/*typedef struct _OB_HANDLE {
    USHORT Version;
    USHORT OperationRegistrationCount;
    PVOID RegistrationContext;
    UNICODE_STRING Altitude;
    CALLBACK_ENTRY entries[1]; // array of OperationRegistrationCount entries
} OB_HANDLE, *POB_HANDLE;*/

//STRUCT FOR WIN7X64
typedef struct _LDR_DATA {                       // 24 elements, 0xE0 bytes (sizeof)
    /*0x000*/     struct _LIST_ENTRY InLoadOrderLinks;                     // 2 elements, 0x10 bytes (sizeof)
    /*0x010*/     struct _LIST_ENTRY InMemoryOrderLinks;                   // 2 elements, 0x10 bytes (sizeof)
    /*0x020*/     struct _LIST_ENTRY InInitializationOrderLinks;           // 2 elements, 0x10 bytes (sizeof)
    /*0x030*/     VOID*        DllBase;
    /*0x038*/     VOID*        EntryPoint;
    /*0x040*/     ULONG32      SizeOfImage;
    /*0x044*/     UINT8        _PADDING0_[0x4];
    /*0x048*/     struct _UNICODE_STRING FullDllName;                      // 3 elements, 0x10 bytes (sizeof)
    /*0x058*/     struct _UNICODE_STRING BaseDllName;                      // 3 elements, 0x10 bytes (sizeof)
    /*0x068*/     ULONG32      Flags;
    /*0x06C*/     UINT16       LoadCount;
    /*0x06E*/     UINT16       TlsIndex;
    union {                                                  // 2 elements, 0x10 bytes (sizeof)
        /*0x070*/         struct _LIST_ENTRY HashLinks;                        // 2 elements, 0x10 bytes (sizeof)
        struct {                                             // 2 elements, 0x10 bytes (sizeof)
            /*0x070*/             VOID*        SectionPointer;
            /*0x078*/             ULONG32      CheckSum;
            /*0x07C*/             UINT8        _PADDING1_[0x4];
        };
    };
    union {                                                  // 2 elements, 0x8 bytes (sizeof)
        /*0x080*/         ULONG32      TimeDateStamp;
        /*0x080*/         VOID*        LoadedImports;
    };
    /*0x088*/     struct _ACTIVATION_CONTEXT* EntryPointActivationContext;
    /*0x090*/     VOID*        PatchInformation;
    /*0x098*/     struct _LIST_ENTRY ForwarderLinks;                       // 2 elements, 0x10 bytes (sizeof)
    /*0x0A8*/     struct _LIST_ENTRY ServiceTagLinks;                      // 2 elements, 0x10 bytes (sizeof)
    /*0x0B8*/     struct _LIST_ENTRY StaticLinks;                          // 2 elements, 0x10 bytes (sizeof)
    /*0x0C8*/     VOID*        ContextInformation;
    /*0x0D0*/     UINT64       OriginalBase;
    /*0x0D8*/     union _LARGE_INTEGER LoadTime;                           // 4 elements, 0x8 bytes (sizeof)
} LDR_DATA, *PLDR_DATA;

typedef struct _OBJECT_TYPE_INITIALIZER {                                                                                                                                       // 25 elements, 0x70 bytes (sizeof)
    /*0x000*/     UINT16       Length;
    union {                                                                                                                                                                     // 2 elements, 0x1 bytes (sizeof)
        /*0x002*/         UINT8        ObjectTypeFlags;
        struct {                                                                                                                                                                // 7 elements, 0x1 bytes (sizeof)
            /*0x002*/             UINT8        CaseInsensitive : 1;                                                                                                                                   // 0 BitPosition
            /*0x002*/             UINT8        UnnamedObjectsOnly : 1;                                                                                                                                // 1 BitPosition
            /*0x002*/             UINT8        UseDefaultObject : 1;                                                                                                                                  // 2 BitPosition
            /*0x002*/             UINT8        SecurityRequired : 1;                                                                                                                                  // 3 BitPosition
            /*0x002*/             UINT8        MaintainHandleCount : 1;                                                                                                                               // 4 BitPosition
            /*0x002*/             UINT8        MaintainTypeList : 1;                                                                                                                                  // 5 BitPosition
            /*0x002*/             UINT8        SupportsObjectCallbacks : 1;                                                                                                                           // 6 BitPosition
        };
    };
    /*0x004*/     ULONG32      ObjectTypeCode;
    /*0x008*/     ULONG32      InvalidAttributes;
    /*0x00C*/     struct _GENERIC_MAPPING GenericMapping;                                                                                                                                     // 4 elements, 0x10 bytes (sizeof)
    /*0x01C*/     ULONG32      ValidAccessMask;
    /*0x020*/     ULONG32      RetainAccess;
    /*0x024*/     enum _POOL_TYPE PoolType;
    /*0x028*/     ULONG32      DefaultPagedPoolCharge;
    /*0x02C*/     ULONG32      DefaultNonPagedPoolCharge;
    /*0x030*/     PVOID DumpProcedure;
    /*0x038*/     PVOID OpenProcedure;
    /*0x040*/     PVOID CloseProcedure;
    /*0x048*/     PVOID DeleteProcedure;
    /*0x050*/     PVOID ParseProcedure;
    /*0x058*/     PVOID SecurityProcedure;
    /*0x060*/     PVOID QueryNameProcedure;
    /*0x068*/     PVOID OkayToCloseProcedure;
} OBJECT_TYPE_INITIALIZER, *POBJECT_TYPE_INITIALIZER;

typedef struct _EX_PUSH_LOCK {               // 7 elements, 0x8 bytes (sizeof)
    union {                                  // 3 elements, 0x8 bytes (sizeof)
        struct {                             // 5 elements, 0x8 bytes (sizeof)
            /*0x000*/             UINT64       Locked : 1;         // 0 BitPosition
            /*0x000*/             UINT64       Waiting : 1;        // 1 BitPosition
            /*0x000*/             UINT64       Waking : 1;         // 2 BitPosition
            /*0x000*/             UINT64       MultipleShared : 1; // 3 BitPosition
            /*0x000*/             UINT64       Shared : 60;        // 4 BitPosition
        };
        /*0x000*/         UINT64       Value;
        /*0x000*/         VOID*        Ptr;
    };
} EX_PUSH_LOCK, *PEX_PUSH_LOCK;

typedef struct _MY_OBJECT_TYPE {                 // 12 elements, 0xD0 bytes (sizeof)
    /*0x000*/     struct _LIST_ENTRY TypeList;              // 2 elements, 0x10 bytes (sizeof)
    /*0x010*/     struct _UNICODE_STRING Name;              // 3 elements, 0x10 bytes (sizeof)
    /*0x020*/     VOID*        DefaultObject;
    /*0x028*/     UINT8        Index;
    /*0x029*/     UINT8        _PADDING0_[0x3];
    /*0x02C*/     ULONG32      TotalNumberOfObjects;
    /*0x030*/     ULONG32      TotalNumberOfHandles;
    /*0x034*/     ULONG32      HighWaterNumberOfObjects;
    /*0x038*/     ULONG32      HighWaterNumberOfHandles;
    /*0x03C*/     UINT8        _PADDING1_[0x4];
    /*0x040*/     struct _OBJECT_TYPE_INITIALIZER TypeInfo; // 25 elements, 0x70 bytes (sizeof)
    /*0x0B0*/     struct _EX_PUSH_LOCK TypeLock;            // 7 elements, 0x8 bytes (sizeof)
    /*0x0B8*/     ULONG32      Key;
    /*0x0BC*/     UINT8        _PADDING2_[0x4];
    /*0x0C0*/     struct _LIST_ENTRY CallbackList;          // 2 elements, 0x10 bytes (sizeof)
} MY_OBJECT_TYPE, *PMY_OBJECT_TYPE;
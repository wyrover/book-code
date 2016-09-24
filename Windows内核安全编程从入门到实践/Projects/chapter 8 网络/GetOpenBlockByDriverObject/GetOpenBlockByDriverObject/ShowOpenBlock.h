#pragma once

/************************************************************************/
/* 宏                                                                    */
/************************************************************************/

#define ObpDirectoryObjectTypeIndex 0x03
#define ObjectHeaderTypeIndexToObject(_a) (_a+0x0c)
#define ObjectDirectoryEntryCount   0x25
#define ClientDriverExtensionOffset 0x14
#define ClientIdentificationAddressOffset 0x04
#define MinoprtDriverSignal 0x4E4D4944
#define ClientDriverExtensionToMDriverBlock(_a) (_a+0x08)

#define NdisProtocolBlock50NameOffset 0x44
#define NdisProtocolBlock60NameOffset 0x24
/************************************************************************/
/* 数据结构                                                             */
/************************************************************************/
typedef struct _Version_ {
    ULONG       OsMajorVersion;
    ULONG       OsMinorVersion;
    UCHAR       MajorNdisVersion;
    UCHAR       MiniorNdisVersion;

} Version;

typedef struct  _ndisMiniDriverList_Item_ {
    PVOID       Next;

    PVOID       MDriverBlock;

} ndisMiniDriverList_Item;

typedef struct _GlobolData_ {
    Version     Ver;

    PVOID       ndisMiniDriverList;


} GlobolData, *PGlobolData;
//
//XP sp3、Win7 7600通用
//

typedef struct _OBJECT_DIRECTORY_ENTRY_ {
    struct _OBJECT_DIRECTORY_ENTRY_     *ChainLink;

    PVOID   Object;

} OBJECT_DIRECTORY_ENTRY;

typedef struct _LDR_DATA_TABLE_ENTRY_ {
    LIST_ENTRY  InLoadOrderLinks;
    LIST_ENTRY  InMemoryOrderLinks;
    LIST_ENTRY  InInitializationOrderLinks;
    PVOID       DllBase;
    PVOID       EntryPoint;
    ULONG       SizeOfImage;
    UNICODE_STRING  FullDllName;
    UNICODE_STRING  BaseDllName;
    UCHAR           Buf[0];         //else

} LDR_DATA_TABLE_ENTRY;


//
//NDIS 5.1\6.20
//
typedef struct _NDIS_50_M_Driver_Block_ {
    PVOID   NextDriver;
    PVOID   MiniportQueue;
    PVOID   NdisDriverInfo;
    PVOID   AssociatedProtocol;
    UCHAR   Buf[0];
} NDIS_50_M_Driver_Block;

typedef struct _NDIS_60_M_Driver_Block_ {
    PVOID   Header;
    PVOID   NextDriver;
    PVOID   MiniportQueue;
    ULONG   Resever1;
    PVOID   NdisDriverInfo;
    PVOID   DriverObject;
    UNICODE_STRING  ServiceRegPath;
    PVOID   MiniportDriverContext;
    PVOID   AssociatedProtocol;
    UCHAR   Buf[0];

} NDIS_60_M_Driver_Block;

typedef struct _NDIS_50_Miniport_Block_ {
    ULONG   Signature;
    PVOID   NextMiniport;
    PVOID   DriverHandle;
    PVOID   MiniportAdapterContext;
    UNICODE_STRING  MiniportName;
    PVOID   BindPaths;
    PVOID   OpenQueue;
    UCHAR   Reserved1[0xd8];
    ULONG   MediaType;
    UCHAR   Buf[0];

} NDIS_50_Miniport_Block;

typedef struct _NDIS_60_Miniport_Block_ {
    PVOID   Header;
    PVOID   NextMiniport;
    UCHAR   Reserved1[0x14];
    PVOID   OpenQueue;
    UCHAR   Reserved2[0xd8];
    ULONG   MediaType;
    UCHAR   Reserved3[0xd54];
    UNICODE_STRING  MiniportName;
    UCHAR   Buf[0];

} NDIS_60_Miniport_Block;

typedef struct _NDIS_50_Open_Block_ {
    PVOID   MacHandle;
    PVOID   BindingHandle;
    PVOID   MiniportHandle;
    PVOID   ProtocolHandle;
    PVOID   ProtocolBindingContext;
    PVOID   MiniportNextOpen;
    PVOID   ProtocolNextOpen;
    UCHAR   Reserved1[8];
    PVOID   BindDeviceName;
    PVOID   Reserved2;
    PVOID   RootDeviceName;
    UCHAR   Buf[0];

} NDIS_50_Open_Block;

typedef struct _NDIS_60_Open_Block_ {
    PVOID   MacHandle;
    PVOID   BindingHandle;
    PVOID   MiniportHandle;
    PVOID   ProtocolHandle;
    UCHAR   Reserved1[0xc8];
    PVOID   MiniportNextOpen;
    PVOID   ProtocolNextOpen;
    PVOID   BindDeviceName;
    PVOID   RootDeviceName;
    UCHAR   Buf[0];

} NDIS_60_Open_Block;



/************************************************************************/
/* 函数声明                                                             */
/************************************************************************/

typedef ULONG(_stdcall *pfnObGetObjectType)(PVOID pObj);

NTSTATUS
ObReferenceObjectByName(
    __in PUNICODE_STRING ObjectName,
    __in ULONG Attributes,
    __in_opt PACCESS_STATE AccessState,
    __in_opt ACCESS_MASK DesiredAccess,
    __in POBJECT_TYPE ObjectType,
    __in KPROCESSOR_MODE AccessMode,
    __inout_opt PVOID ParseContext,
    __out PVOID *Object
);

VOID
ShowOpenBlock(PDRIVER_OBJECT DriverObject);

BOOLEAN
InitGlobolData(GlobolData *gpData);

BOOLEAN
ValidateLegacyVerion(Version *Ver);

BOOLEAN
GetMDriverBlockList(GlobolData *gpData);

BOOLEAN
ShowOpenBlockBy50MDriverBlock(GlobolData *gpData);

BOOLEAN
ShowOpenBlockBy60MDriverBlock(GlobolData *gpData);

BOOLEAN
DeInitGlobolData(GlobolData *gpData);
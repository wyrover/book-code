#ifndef _OBJECTHOOKCHECK_H_
#define _OBJECTHOOKCHECK_H_

#include "ntifs.h"
#include "InitWindowsVersion.h"
#include "ntos.h"

#define NoObjectHook 0
#define IsObjectHook 1
#define IsObjectInlineHook 2

typedef PVOID (_stdcall *OBGETOBJECTTYPE)( IN PVOID pObject); //win7中添加的函数
//扫描这几个就可以了吧

extern POBJECT_TYPE *IoFileObjectType;
extern POBJECT_TYPE *IoDriverObjectType;
extern POBJECT_TYPE *IoDeviceObjectType;
extern POBJECT_TYPE *PsProcessType;
extern POBJECT_TYPE *PsThreadType;
extern POBJECT_TYPE *MmSectionObjectType;

POBJECT_TYPE CmpKeyObjectType;   //没有导出

extern BYTE *ImageModuleBase;
extern ULONG SystemKernelModuleBase;
extern ULONG SystemKernelModuleSize;

ULONG ulObjectHookCount;

//-----------------------------------------------------------
typedef struct _OBJECT_TYPE_INITIALIZER {
	USHORT Length;
	BOOLEAN UseDefaultObject;
	BOOLEAN CaseInsensitive;
	ULONG InvalidAttributes;
	GENERIC_MAPPING GenericMapping;
	ULONG ValidAccessMask;
	BOOLEAN SecurityRequired;
	BOOLEAN MaintainHandleCount;
	BOOLEAN MaintainTypeList;
	POOL_TYPE PoolType;
	ULONG DefaultPagedPoolCharge;
	ULONG DefaultNonPagedPoolCharge;
	PVOID DumpProcedure;
	PVOID OpenProcedure;
	PVOID CloseProcedure;
	PVOID DeleteProcedure;
	PVOID ParseProcedure;
	PVOID SecurityProcedure;
	PVOID QueryNameProcedure;
	PVOID OkayToCloseProcedure;
} OBJECT_TYPE_INITIALIZER, *POBJECT_TYPE_INITIALIZER;


typedef struct _OBJECT_TYPE { 
	ERESOURCE Mutex; 
	LIST_ENTRY TypeList; 
	UNICODE_STRING Name; // Copy from object header for convenience 
	PVOID DefaultObject; 
	ULONG Index; 
	ULONG TotalNumberOfObjects; 
	ULONG TotalNumberOfHandles; 
	ULONG HighWaterNumberOfObjects; 
	ULONG HighWaterNumberOfHandles; 
	OBJECT_TYPE_INITIALIZER TypeInfo; 
#ifdef POOL_TAGGING 
	ULONG Key; 
#endif //POOL_TAGGING 
} OBJECT_TYPE, *POBJECT_TYPE;
//////////////////////////////////////////////////////////////
typedef struct _OBJECT_TYPE_INITIALIZER_WIN7 {

	USHORT Length     ;
	USHORT type       ;
	//+0x002 ObjectTypeFlags  : UChar
	//+0x002 CaseInsensitive  : Pos 0, 1 Bit
	//+0x002 UnnamedObjectsOnly : Pos 1, 1 Bit
	//+0x002 UseDefaultObject : Pos 2, 1 Bit
	//+0x002 SecurityRequired : Pos 3, 1 Bit
	//+0x002 MaintainHandleCount : Pos 4, 1 Bit
	//+0x002 MaintainTypeList : Pos 5, 1 Bit
	PVOID ObjectTypeCode   ;
	PVOID InvalidAttributes ;
	GENERIC_MAPPING GenericMapping   ;
	PVOID ValidAccessMask  ;
	PVOID RetainAccess     ;
	POOL_TYPE PoolType      ;
	PVOID DefaultPagedPoolCharge ;
	PVOID DefaultNonPagedPoolCharge ;
	PVOID DumpProcedure    ;
	PVOID OpenProcedure    ;
	PVOID CloseProcedure   ;
	PVOID DeleteProcedure  ;
	PVOID ParseProcedure   ;
	PVOID SecurityProcedure ;
	PVOID QueryNameProcedure ;
	USHORT OkayToCloseProcedure ;

} OBJECT_TYPE_INITIALIZER_WIN7, *POBJECT_TYPE_INITIALIZER_WIN7;

typedef struct _OBJECT_TYPE_WIN7 {
	LIST_ENTRY TypeList;         //         : _LIST_ENTRY
	UNICODE_STRING Name;         //             : _UNICODE_STRING
	PVOID DefaultObject;         //    : Ptr32 Void
	ULONG Index;         //            : UChar
	ULONG TotalNumberOfObjects;         // : Uint4B
	ULONG TotalNumberOfHandles;         // : Uint4B
	ULONG HighWaterNumberOfObjects;         // : Uint4B
	ULONG HighWaterNumberOfHandles;         // : Uint4B
	OBJECT_TYPE_INITIALIZER_WIN7 TypeInfo;         //         : _OBJECT_TYPE_INITIALIZER
	PVOID TypeLock;         //         : _EX_PUSH_LOCK
	ULONG Key;         //              : Uint4B
	LIST_ENTRY CallbackList;         //     : _LIST_ENTRY
} OBJECT_TYPE_WIN7, *POBJECT_TYPE_WIN7;

typedef struct _OBJECT_HEADER {
	//对象头部的指针计数，对对象头指针引用的计数
	LONG_PTR PointerCount;
	union {
		//句柄引用计数
		LONG_PTR HandleCount;
		PVOID NextToFree;
	};
	POBJECT_TYPE Type;
	//OBJECT_HEADER_NAME_INFO相对于此结构的偏移
	UCHAR NameInfoOffset;
	//OBJECT_HEADER_HANDLE_INFO相对于此结构的偏移
	UCHAR HandleInfoOffset;
	//OBJECT_HEADER_QUOTA_INFO相对于此结构的偏移
	UCHAR QuotaInfoOffset;
	UCHAR Flags;

	union {
		//创建对象是用于创建对象附加头的结构
		//里面保存了和附加对象头类似的信息
		PVOID ObjectCreateInfo;
		PVOID QuotaBlockCharged;
	};
	PSECURITY_DESCRIPTOR SecurityDescriptor;
	QUAD Body;
} OBJECT_HEADER, *POBJECT_HEADER;

//----------------------------------------------------------------------------------------

typedef struct _OBJECTHOOKINFO_INFORMATION {          //INLINEHOOKINFO_INFORMATION
	ULONG ulMemoryFunctionBase;    //原始地址
	ULONG ulMemoryHookBase;        //HOOK 地址
	CHAR lpszFunction[256];
	CHAR lpszHookModuleImage[256];
	ULONG ulObjectTypeBase;
	ULONG ulHookType;
	CHAR lpszObjectTypeName[256];

} OBJECTHOOKINFO_INFORMATION, *POBJECTHOOKINFO_INFORMATION;

typedef struct _OBJECTHOOKINFO {          //InlineHook
	ULONG ulCount;
	OBJECTHOOKINFO_INFORMATION ObjectHook[1];
} OBJECTHOOKINFO, *POBJECTHOOKINFO;

POBJECTHOOKINFO ObjectHookInfo;

BOOL IsAddressInSystem(
	ULONG ulDriverBase,
	ULONG *ulSysModuleBase,
	ULONG *ulSize,
	char *lpszSysModuleImage
	);

unsigned long __fastcall GetFunctionCodeSize(
	void *Proc
	);

#endif
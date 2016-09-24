#ifndef _DPMAIN_H_
#define _DPMAIN_H_

#define FS_OTHER	0
#define FS_FAT16	1
#define FS_FAT32	2
#define FS_NTFS		3

//用来存储一个卷所有的相关信息的数据结构，放在过滤设备的设备扩展中
typedef struct _DP_FILTER_DEV_EXTENSION_ 
{
	//卷的名字，例如"C:,D:"等中的字母部分
	WCHAR					VolumeLetter;
	//这个卷是否在保护状态
	BOOL					Protect;
	//这个卷的总大小，以byte为单位
	LARGE_INTEGER			TotalSizeInByte;
	//这个卷上文件系统的每簇大小，以byte为单位
	DWORD					ClusterSizeInByte;
	//这个卷的每个扇区大小，以byte为单位
	DWORD					SectorSizeInByte;
	//这个卷设备对应的过滤设备的设备对象
	PDEVICE_OBJECT			FltDevObj;
	//这个卷设备对应的过滤设备的下层设备对象
	PDEVICE_OBJECT			LowerDevObj;
	//这个卷设备对应的物理设备的设备对象
	PDEVICE_OBJECT			PhyDevObj;
	//这个数据结构是否已经被初始化完毕了
	BOOL					InitializeCompleted;
	//这个卷上的保护系统使用的位图的句柄
	PDP_BITMAP		Bitmap;	
	//用来转储的文件句柄
	HANDLE					TempFile;
	//这个卷上的保护系统使用的请求队列
	LIST_ENTRY				ReqList;
	//这个卷上的保护系统使用的请求队列的锁
	KSPIN_LOCK				ReqLock;
	//这个卷上的保护系统使用的请求队列的同步事件
	KEVENT					ReqEvent;
	//这个卷上的保护系统使用的请求队列的处理线程之线程句柄
	PVOID					ThreadHandle;
	//这个卷上的保护系统使用的请求队列的处理线程之结束标志
	BOOLEAN					ThreadTermFlag;
	//这个卷上的保护系统的关机分页电源请求的计数事件
	KEVENT					PagingPathCountEvent;
	//这个卷上的保护系统的关机分页电源请求的计数
	LONG					PagingPathCount;
} DP_FILTER_DEV_EXTENSION, *PDP_FILTER_DEV_EXTENSION;

typedef struct _VOLUME_ONLINE_CONTEXT_
{
	//在volume_online的DeviceIoControl中传给完成函数的设备扩展
	PDP_FILTER_DEV_EXTENSION	DevExt;
	//在volume_online的DeviceIoControl中传给完成函数的同步事件
	PKEVENT						Event;
}VOLUME_ONLINE_CONTEXT, *PVOLUME_ONLINE_CONTEXT;

#pragma pack(1)
typedef struct _DP_FAT16_BOOT_SECTOR
{
	UCHAR		JMPInstruction[3];
	UCHAR		OEM[8];
	USHORT		BytesPerSector;
	UCHAR		SectorsPerCluster;
	USHORT		ReservedSectors;
	UCHAR		NumberOfFATs;
	USHORT		RootEntries;
	USHORT		Sectors;
	UCHAR		MediaDescriptor;
	USHORT		SectorsPerFAT;
	USHORT		SectorsPerTrack;
	USHORT		Heads;
	DWORD		HiddenSectors;
	DWORD		LargeSectors;
	UCHAR		PhysicalDriveNumber;
	UCHAR		CurrentHead;
} DP_FAT16_BOOT_SECTOR, *PDP_FAT16_BOOT_SECTOR;

typedef struct _DP_FAT32_BOOT_SECTOR
{
	UCHAR		JMPInstruction[3];
	UCHAR		OEM[8];
	USHORT		BytesPerSector;
	UCHAR		SectorsPerCluster;
	USHORT		ReservedSectors;
	UCHAR		NumberOfFATs;
	USHORT		RootEntries;
	USHORT		Sectors;
	UCHAR		MediaDescriptor;
	USHORT		SectorsPerFAT;
	USHORT		SectorsPerTrack;
	USHORT		Heads;
	DWORD		HiddenSectors;
	DWORD		LargeSectors;
	DWORD		LargeSectorsPerFAT;
	UCHAR		Data[24];
	UCHAR		PhysicalDriveNumber;
	UCHAR		CurrentHead;
} DP_FAT32_BOOT_SECTOR, *PDP_FAT32_BOOT_SECTOR;

typedef struct _DP_NTFS_BOOT_SECTOR
{
	UCHAR		Jump[3];					//0
	UCHAR		FSID[8];					//3
	USHORT		BytesPerSector;				//11
	UCHAR		SectorsPerCluster;			//13
	USHORT		ReservedSectors;			//14
	UCHAR		Mbz1;						//16		
	USHORT		Mbz2;						//17
	USHORT		Reserved1;					//19
	UCHAR		MediaDesc;					//21
	USHORT		Mbz3;						//22
	USHORT		SectorsPerTrack;			//24
	USHORT		Heads;						//26
	ULONG		HiddenSectors;				//28
	ULONG		Reserved2[2];				//32
	ULONGLONG	TotalSectors;				//40
	ULONGLONG	MftStartLcn;				//48
	ULONGLONG	Mft2StartLcn;				//56
}DP_NTFS_BOOT_SECTOR, *PDP_NTFS_BOOT_SECTOR;
#pragma pack()

NTSTATUS
DPCompleteRequest(
	IN	PIRP			Irp,
	IN	NTSTATUS		Status,
	IN	CCHAR			Priority
	);
	
NTSTATUS
DPSendToNextDriver(
	IN	PDEVICE_OBJECT	TgtDevObj,
	IN	PIRP			Irp
	);

NTSTATUS
DPSendToNextDriverSynchronous(
	IN	PDEVICE_OBJECT	DeviceObject,
	IN	PDEVICE_OBJECT	TargetDeviceObject,
	IN	PIRP			Irp
	);

NTSTATUS DPDispatchShutdown(    
	IN PDEVICE_OBJECT  DeviceObject,
	IN PIRP  Irp
	);

NTSTATUS
DPAddDevice(
	IN	PDRIVER_OBJECT	DriverObject,
	IN	PDEVICE_OBJECT	PhysicalDeviceObject
	);

VOID
DPUnload(
	IN	PDRIVER_OBJECT	DriverObject
	);

NTSTATUS
DPDispatchAny(
    IN	PDEVICE_OBJECT	DeviceObject,
    IN	PIRP			Irp
    );
    
NTSTATUS
DPDispatchCreateClose(
    IN	PDEVICE_OBJECT	DeviceObject,
    IN	PIRP			Irp
    );
	
NTSTATUS
DPDispatchPower(
	IN	PDEVICE_OBJECT	DeviceObject,
	IN	PIRP			Irp
	);

NTSTATUS	
DPDispatchPnp(
	IN	PDEVICE_OBJECT	DeviceObject,
	IN	PIRP			Irp
	);

NTSTATUS
DPDispatchDeviceControl(
	IN	PDEVICE_OBJECT	DeviceObject,
	IN	PIRP			Irp
	);

NTSTATUS
DPDispatchReadWrite(
    IN	PDEVICE_OBJECT	DeviceObject,
    IN	PIRP			Irp
    );

NTSTATUS
DiskShldForwardIrpSynchronous(
	IN PDEVICE_OBJECT DeviceObject,
	IN PDEVICE_OBJECT TargetDeviceObject,
	IN PIRP Irp
	);

#endif
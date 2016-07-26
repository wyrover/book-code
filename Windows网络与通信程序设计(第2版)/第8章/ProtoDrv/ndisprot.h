/////////////////////////////////////////////
// ndisprot.h文件
// 定义此驱动程序数据结构，定义和函数原型




#ifndef __NDISPROT_H__
#define __NDISPROT_H__

// 驱动内部名称和符号连接名称
#define DEVICE_NAME L"\\Device\\devNdisProt"
#define LINK_NAME L"\\DosDevices\\slNdisProt"


typedef struct _GLOBAL
{
	PDRIVER_OBJECT pDriverObj;		// 驱动对象指针
	NDIS_HANDLE hNdisProtocol;		// 协议驱动句柄，是NdisRegisterProtocol函数返回的

	LIST_ENTRY AdapterList; 		// 为我们绑定的每个适配器创建的设备对象列表
	KSPIN_LOCK GlobalLock;			// 为了同步对这个表的访问
	PDEVICE_OBJECT pControlDevice;	// 此驱动程序的控制设备对象指针
} GLOBAL;



typedef struct _INTERNAL_REQUEST
{
	PIRP pIrp;				 
	NDIS_REQUEST Request;
} INTERNAL_REQUEST, *PINTERNAL_REQUEST;

// 每个适配器也要有自己的私有变量。下面的OPEN_INSTANCE结构描述了打开的适配器
typedef struct _OPEN_INSTANCE
{
	// 静态数据
	LIST_ENTRY AdapterListEntry;	// 用于连接到其它NIC设备对象，即连接到全局AdapterList列表
	PDEVICE_OBJECT pDeviceObj;		// 关联到的设备对象的指针
	UNICODE_STRING ustrAdapterName;	// 此适配器的名称
	UNICODE_STRING ustrLinkName;	// 此适配器对应适配器对象的符号连接名称
	NDIS_HANDLE hAdapter;			// 适配器句柄
	
	// 状态信息
	BOOLEAN bBound;			// 是否绑定
	NDIS_STATUS Status;		// 状态代码		
	ULONG nIrpCount;		// 当前用户在此适配器上的IRP请求数量
	
	// 封包池句柄
	NDIS_HANDLE	hPacketPool;

	// 接收列表
	LIST_ENTRY RcvList;
	KSPIN_LOCK	RcvSpinLock;
	// 重启列表
	LIST_ENTRY	ResetIrpList;
	KSPIN_LOCK ResetQueueLock;
	
	// 同步事件
	NDIS_EVENT BindEvent;
	NDIS_EVENT CleanupEvent;

	NDIS_MEDIUM Medium;		// 此适配器的类型
} OPEN_INSTANCE, *POPEN_INSTANCE;

typedef struct _PACKET_RESERVED
{
	LIST_ENTRY ListElement;		// 将各个封包描述表连在一起
	PIRP	pIrp;				// 记录此封包对应的未决的IRP请求
	PMDL	pMdl;				// 记录为此封包申请的MDL
}	PACKET_RESERVED, *PPACKET_RESERVED;


#define ETHERNET_HEADER_LENGTH	14
#define RESERVED(_p) ((PACKET_RESERVED*)((_p)->ProtocolReserved))



extern GLOBAL g_data;



VOID 
  ProtocolBindAdapter(
    OUT PNDIS_STATUS Status,
    IN NDIS_HANDLE  BindContext,
    IN PNDIS_STRING  DeviceName,
    IN PVOID  SystemSpecific1,
    IN PVOID  SystemSpecific2
    );
VOID
  ProtocolUnbindAdapter(
      OUT PNDIS_STATUS  Status,
      IN NDIS_HANDLE  ProtocolBindingContext,
      IN NDIS_HANDLE  UnbindContext
      );


// 自定义函数的声明
NTSTATUS DispatchCreate(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS DispatchClose(PDEVICE_OBJECT pDevObj, PIRP pIrp);
void DriverUnload(PDRIVER_OBJECT pDriverObj);

NTSTATUS DispatchRead(PDEVICE_OBJECT pDevObj, PIRP pIrp);
NTSTATUS DispatchWrite(PDEVICE_OBJECT pDevObj, PIRP pIrp);

void IoIncrement(OPEN_INSTANCE *pOpen);


void IoDecrement(OPEN_INSTANCE *pOpen);

VOID
ProtocolTransferDataComplete (
    IN NDIS_HANDLE   ProtocolBindingContext,
    IN PNDIS_PACKET  pPacket,
    IN NDIS_STATUS   Status,
    IN UINT          BytesTransfered
    );


NDIS_STATUS
ProtocolReceive(
    IN NDIS_HANDLE ProtocolBindingContext,
    IN NDIS_HANDLE MacReceiveContext,
    IN PVOID       HeaderBuffer,
    IN UINT        HeaderBufferSize,
    IN PVOID       LookAheadBuffer,
    IN UINT        LookaheadBufferSize,
    IN UINT        PacketSize
    );

int ProtocolReceivePacket(NDIS_HANDLE ProtocolBindingContext, PNDIS_PACKET Packet);

void CancelReadIrp(PDEVICE_OBJECT pDeviceObj);

NTSTATUS DispatchIoctl(PDEVICE_OBJECT pDevObj, PIRP pIrp);

NTSTATUS
GetAdapterList(
    IN  PVOID              Buffer,
    IN  ULONG              Length,
    IN  OUT PULONG         DataLength
    );

VOID
PacketResetComplete(
    IN NDIS_HANDLE  ProtocolBindingContext,
    IN NDIS_STATUS  Status
    );

VOID
  ProtocolUnbindAdapter(
      OUT PNDIS_STATUS  Status,
      IN NDIS_HANDLE  ProtocolBindingContext,
      IN NDIS_HANDLE  UnbindContext
      );

VOID
  ProtocolOpenAdapterComplete(
      IN NDIS_HANDLE  ProtocolBindingContext,
      IN NDIS_STATUS  Status,
      IN NDIS_STATUS  OpenErrorStatus
      );
VOID 
  ProtocolCloseAdapterComplete(
      IN NDIS_HANDLE  ProtocolBindingContext,
      IN NDIS_STATUS  Status
      );

VOID
ReadCancelRoutine (
    IN PDEVICE_OBJECT   pDeviceObject,
    IN PIRP             pIrp
    );

VOID
ProtocolSendComplete(
    IN NDIS_HANDLE   ProtocolBindingContext,
    IN PNDIS_PACKET  pPacket,
    IN NDIS_STATUS   Status
    );

VOID
ProtocolResetComplete(
    IN NDIS_HANDLE  ProtocolBindingContext,
    IN NDIS_STATUS  Status
    );

VOID
ProtocolRequestComplete(
    IN NDIS_HANDLE   ProtocolBindingContext,
    IN PNDIS_REQUEST NdisRequest,
    IN NDIS_STATUS   Status
    );

VOID
PacketRequestComplete(
    IN NDIS_HANDLE   ProtocolBindingContext,
    IN PNDIS_REQUEST NdisRequest,
    IN NDIS_STATUS   Status
    );

NTSTATUS
DispatchCleanup(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

VOID
ProtocolReceiveComplete(
    IN NDIS_HANDLE  ProtocolBindingContext
    );

VOID
ProtocolStatus(
    IN NDIS_HANDLE   ProtocolBindingContext,
    IN NDIS_STATUS   Status,
    IN PVOID         StatusBuffer,
    IN UINT          StatusBufferSize
    );

VOID
ProtocolStatusComplete(
    IN NDIS_HANDLE  ProtocolBindingContext
    );

NDIS_STATUS
ProtocolPNPHandler(
    IN    NDIS_HANDLE        ProtocolBindingContext,
    IN    PNET_PNP_EVENT     NetPnPEvent
    );

#endif // __NDISPROT_H__
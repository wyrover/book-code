///////////////////////////////////////////////////////
// PTEXTEND.h文件

#ifndef __PTEXTEND__H
#define __PTEXTEND__H








// 用户打开一个适配器后，我们就为其句柄关联一个OPEN_CONTEXT结构，
// 结构指针保存在FileObject的FsContext域中
typedef struct _OPEN_CONTEXT
{
	ULONG			RefCount;
	NDIS_SPIN_LOCK	Lock;
	BOOLEAN			bAdapterClosed;

	PADAPT			pAdapt;

	// 下面3个为的是处理Oid请求
	NDIS_REQUEST Request;
	NDIS_STATUS  RequestStatus;
    NDIS_EVENT   RequestEvent;

// BEGIN_PTEX_FILTER
    // 为过滤数据预留的空间  每个打开句柄的过滤相关数据
    // Per-Open-Handle Filter-Specific Area
    //
    ULONG               FilterReserved[16];

// END_PTEX_FILTER

} OPEN_CONTEXT, *POPEN_CONTEXT;





/////////////////////////////////////////////
// 下面是一些自定义函数的声明

NTSTATUS DevClose(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
NTSTATUS DevCleanup(PDEVICE_OBJECT    pDeviceObject,PIRP pIrp);
NTSTATUS DevOpen(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
NTSTATUS DevIoControl(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
NTSTATUS FltDevIoControl(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);

NTSTATUS DevHandleOidRequest(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
VOID DevRequestComplete(PADAPT pAdapt, PNDIS_REQUEST NdisRequest,NDIS_STATUS Status);

VOID PtRefAdapter(PADAPT pAdapt);
VOID PtDerefAdapter(PADAPT pAdapt);
POPEN_CONTEXT DevAllocateOpenContext(PADAPT pAdapt);
VOID DevDerefOpenContext(POPEN_CONTEXT pOpenContext);
VOID DevRefOpenContext(POPEN_CONTEXT pOpenContext) ;

NTSTATUS DevGetBindingList(PVOID Buffer,ULONG Length,PULONG DataLength);
PADAPT LookupAdaptByName(PUCHAR pNameBuffer, ULONG nNameLength);

#endif // __PTEXTEND__H

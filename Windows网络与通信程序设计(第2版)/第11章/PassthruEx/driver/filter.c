/////////////////////////////////////////////////////
// filter.c文件

// 这个文件包含了驱动程序中过滤相关的代码  过滤部分


#include "precomp.h"
#pragma hdrstop

#include "iocommon.h"


#include "protoinfo.h"

// 过滤规则列表
typedef struct _PassthruFilterList
{
	PassthruFilter filter;
	struct _PassthruFilterList *pNext;

} PassthruFilterList, *PPassthruFilterList;

// ADAPT结构中FilterReserved部分
typedef struct _ADAPT_FILTER_RSVD
{
	BOOLEAN     bFilterInitDone;
	
	//  Per-Adapter过滤相关成员
	PassthruStatistics Statistics;		// 记录网络状态，如传输了多少封包，丢弃了多少等等
	
	PPassthruFilterList pFilterList;	// 指向过滤列表
	
}ADAPT_FILTER_RSVD, *PADAPT_FILTER_RSVD;
C_ASSERT(sizeof(ADAPT_FILTER_RSVD) <= sizeof(((PADAPT)0)->FilterReserved));


// OPEN_CONTEXT结构中FilterReserved部分.
typedef struct _OPEN_CONTEXT_FILTER_RSVD
{
	BOOLEAN     bFilterInitDone;

	// 更多的Per-Open-Handle过滤相关成员

}OPEN_FILTER_RSVD, *POPEN_FILTER_RSVD;

C_ASSERT(sizeof(OPEN_FILTER_RSVD) <= sizeof(((POPEN_CONTEXT)0)->FilterReserved));



VOID FltOnInitAdapter(PADAPT pAdapt)
{
	PADAPT_FILTER_RSVD   pFilterContext;
	
	//
	// 初始化ADAPT结构中的FilterReserved域
	//
	pFilterContext = (PADAPT_FILTER_RSVD )&pAdapt->FilterReserved;
}


VOID FltOnDeinitAdapter(PADAPT pAdapt)
{
	PADAPT_FILTER_RSVD   pFilterContext;
	
	//
	// 反初始化ADAPT结构中的FilterReserved域
	//
	pFilterContext = (PADAPT_FILTER_RSVD)&pAdapt->FilterReserved;
	
	ClearFilterList(pFilterContext);
}


/////////////////////////////////////////////////////

// 向适配器过滤列表中添加一个过滤规则
NTSTATUS AddFilterToAdapter(PADAPT_FILTER_RSVD pFilterContext, PPassthruFilter pFilter)
{
	PPassthruFilterList pNew;
	// 为新的过滤规则申请内存空间
	if(NdisAllocateMemoryWithTag(&pNew, sizeof(PassthruFilterList), TAG) != NDIS_STATUS_SUCCESS)
		return STATUS_INSUFFICIENT_RESOURCES;

	// 填充这块内存
	NdisMoveMemory(&pNew->filter, pFilter, sizeof(PassthruFilter));
	
	// 连接到过滤列表中
	pNew->pNext = pFilterContext->pFilterList;
	pFilterContext->pFilterList = pNew;
	
	return STATUS_SUCCESS;
}

// 删除适配器过滤列表中的规则
void ClearFilterList(PADAPT_FILTER_RSVD pFilterContext)
{
	PPassthruFilterList pList = pFilterContext->pFilterList;
	PPassthruFilterList pNext;
	// 释放过滤列表占用的内存
	while(pList != NULL)
	{
		pNext = pList->pNext;
		
		NdisFreeMemory(pList, 0, 0); 
		pList = pNext;
	}
	pFilterContext->pFilterList = NULL;
}





// 对那些不能识别的IOCTL，PassThru从主要的DevIoControl例程调用此例程

NTSTATUS FltDevIoControl(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	// 假设失败
	NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;
	
	// 取得此IRP（pIrp）的I/O堆栈指针
	PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
	
	// 取得I/O控制代码
	ULONG uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
	// 取得I/O缓冲区指针和它的长度
	PVOID pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
	ULONG uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;
	
	ULONG uTransLen = 0;
	
	PADAPT              pAdapt = NULL;
    PADAPT_FILTER_RSVD  pFilterContext = NULL;
    POPEN_CONTEXT       pOpenContext = pIrpStack->FileObject->FsContext;
	
	if(pOpenContext == NULL || (pAdapt = pOpenContext->pAdapt) == NULL)
	{
		status = STATUS_INVALID_HANDLE;
		goto CompleteTheIRP;
	}
	
	pFilterContext = (PADAPT_FILTER_RSVD)&pAdapt->FilterReserved;

	
	//
	// Fail IOCTL If Unbind Is In Progress   Fail IOCTL If Adapter Is Powering Down
	//
	NdisAcquireSpinLock(&pAdapt->Lock);
	
	if( pAdapt->UnbindingInProcess || pAdapt->StandingBy == TRUE)
	{
		NdisReleaseSpinLock(&pAdapt->Lock);
		
		status = STATUS_INVALID_DEVICE_STATE;
		goto CompleteTheIRP;
	}
	
	// 当改变数据时,要拥有SpinLock
	
	// 最后,处理IO控制代码
	switch(uIoControlCode)
	{
	case IOCTL_PTUSERIO_QUERY_STATISTICS:		// 获取网络活动状态
		{
			uTransLen = sizeof(PassthruStatistics);
			if(uOutSize < uTransLen)
			{
				status =  STATUS_BUFFER_TOO_SMALL;
				break;
			}
			
			NdisMoveMemory(pIoBuffer, &pFilterContext->Statistics, uTransLen);
			status = STATUS_SUCCESS;
		}
		break;
	case IOCTL_PTUSERIO_RESET_STATISTICS:		// 重设网络活动状态
		{
			NdisZeroMemory(&pFilterContext->Statistics, sizeof(PassthruStatistics));
			status = STATUS_SUCCESS;
		}
		break;
	case IOCTL_PTUSERIO_ADD_FILTER:				// 添加一个过滤规则
		{
			if(uInSize >= sizeof(PassthruFilter))
			{
				DBGPRINT((" 添加一个过滤规则"));
				status = AddFilterToAdapter(pFilterContext, (PPassthruFilter)pIoBuffer);
			}
			else
			{
				status = STATUS_INVALID_DEVICE_REQUEST;
			}
		}
		break;
	case IOCTL_PTUSERIO_CLEAR_FILTER:			// 清除过滤规则
		{
			DBGPRINT((" 清除过滤规则"));
			ClearFilterList(pFilterContext);
			status = STATUS_SUCCESS;
		}
		break;
	}
	
	NdisReleaseSpinLock(&pAdapt->Lock);
	
CompleteTheIRP:
	
	if(status == STATUS_SUCCESS)
		pIrp->IoStatus.Information = uTransLen;
	else
		pIrp->IoStatus.Information = 0;
	
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return status;
}


////////////////////////////////////////////////////
// 读取封包中的数据
void FltReadPacketData(PNDIS_PACKET pPacket, 
					   PUCHAR lpBufferIn, ULONG nNumberToRead, PUINT lpNumberOfRead)
{	
	PUCHAR pBuf;
	ULONG nBufferSize;
	PNDIS_BUFFER pBufferDes = NULL;
	
	
	// 检查参数
	if(pPacket == NULL || lpBufferIn == NULL || nNumberToRead == 0)
	{
		if(lpNumberOfRead != NULL)
		{
			*lpNumberOfRead = 0;
			return ;
		}
	}

	// 设置返回数据
	*lpNumberOfRead = 0;
	
	
	// 遍历封包中的缓冲区描述表,将数据复制到用户缓冲区
	pBufferDes = pPacket->Private.Head;
	while(pBufferDes != pPacket->Private.Tail && pBufferDes != NULL)
	{
		// 获取此缓冲区描述表的缓冲区信息
		NdisQueryBufferSafe(pBufferDes, &pBuf, &nBufferSize, NormalPagePriority);
		if(pBuf == NULL)
			return;
		
		if(nNumberToRead > nBufferSize) // 复制整个缓冲区
		{
			NdisMoveMemory(lpBufferIn + *lpNumberOfRead, pBuf, nBufferSize);
			nNumberToRead -= nBufferSize;
			*lpNumberOfRead += nBufferSize;
		}
		else							// 仅复制剩下的部分
		{
			NdisMoveMemory(lpBufferIn + *lpNumberOfRead, pBuf, nNumberToRead);
			*lpNumberOfRead += nNumberToRead;
			return;
		}
		// 下一个缓冲区描述表
		pBufferDes = pBufferDes->Next;
	}
}

/////////////////////////////////////////////
// 检查过滤规则
BOOLEAN FltCheckFilterRules(PPassthruFilterList pFilterList, PUCHAR pPacketData, ULONG nDataLen, BOOLEAN bIncludeETHdr)
{	
	int nLeavingLen = nDataLen;
	
	PETHeader pEtherHdr;
	PIPHeader pIpHdr;
	PTCPHeader pTcpHdr;
	PUDPHeader pUdpHdr;


	
	// 从缓冲区中萃取出IP头
	// 如果包含以太头，就要先检查以太头
	if(bIncludeETHdr)
	{
		if(nLeavingLen < sizeof(ETHeader))
		{
			return TRUE;
		}
		nLeavingLen -= sizeof(ETHeader);
		
		pEtherHdr = (PETHeader)pPacketData;
		
		if(pEtherHdr->type != 0x8) // 如果不是IP协议，则不处理
			return TRUE;
		
		pIpHdr = (PIPHeader)(pEtherHdr + 1);
	}
	else
	{
		pIpHdr = (PIPHeader)pPacketData;
	}
	
	// 验证剩余数据长度，防止发生内核非法访问
	if(nLeavingLen < sizeof(IPHeader))
		return TRUE;
	nLeavingLen -= sizeof(IPHeader);
	
	
	// 检查版本信息，我们仅处理IPv4
	if(((pIpHdr->iphVerLen >> 4) & 0x0f) == 6)
	{
		return TRUE;
	}
	
	if(pIpHdr->ipProtocol == 6 && nLeavingLen >= sizeof(TCPHeader))  // 是TCP协议？
	{
		// 提取TCP头
		pTcpHdr = (PTCPHeader)(pIpHdr + 1);
		// 我们接受所有已经建立连接的TCP封包
		if(!(pTcpHdr->flags & 0x02))
		{
			return TRUE;
		}
	}
	
	// 与过滤规则比较，决定采取的行动
	while(pFilterList != NULL)
	{
		// 查看封包使用的协议是否和过滤规则相同
		if(pFilterList->filter.protocol == 0 || pFilterList->filter.protocol == pIpHdr->ipProtocol)
		{
			// 如果协议相同，再查看源IP地址
			if(pFilterList->filter.sourceIP != 0 &&
				pFilterList->filter.sourceIP != (pFilterList->filter.sourceMask & pIpHdr->ipSource))
			{
				pFilterList = pFilterList->pNext;
				continue;
			}
			
			// 再查看目的IP地址
			if(pFilterList->filter.destinationIP != 0 &&
				pFilterList->filter.destinationIP != (pFilterList->filter.destinationMask & pIpHdr->ipDestination))
			{
				pFilterList = pFilterList->pNext;
				continue;
			}
			
			// 如果是TCP封包，接着查看TCP端口号
			if(pIpHdr->ipProtocol == 6)
			{
				if(nLeavingLen < 4)
				{
					return TRUE;
				}
				pTcpHdr = (PTCPHeader)(pIpHdr + 1);
				// 如果源端口号和目的端口号都与规则中的一样，则按照规则的记录处理这个封包
				if(pFilterList->filter.sourcePort == 0 || pFilterList->filter.sourcePort == pTcpHdr->sourcePort)
				{
					if(pFilterList->filter.destinationPort == 0 ||
						pFilterList->filter.destinationPort == pTcpHdr->destinationPort)
					{
						DBGPRINT((" 按照规则处理一个TCP封包 \n "));
						return !pFilterList->filter.bDrop; 
					}
				}
				
			}
			// 如果是UDP封包，接着查看UDP端口号
			else if(pIpHdr->ipProtocol == 17)
			{
				if(nLeavingLen < 4)
				{
					return !pFilterList->filter.bDrop;
				}
				pUdpHdr = (PUDPHeader)(pIpHdr + 1);
				if(pFilterList->filter.sourcePort == 0 || 
					pFilterList->filter.sourcePort == pUdpHdr->sourcePort)
				{
					if(pFilterList->filter.destinationPort == 0 ||
							pFilterList->filter.destinationPort == pUdpHdr->destinationPort)
					{
						DBGPRINT((" 按照规则处理一个UDP封包 \n "));
						return !pFilterList->filter.bDrop; 
					}
				}
			}
			else
			{
				// 对于其它封包，我们直接处理
				return !pFilterList->filter.bDrop; 
			}
		}
		// 比较下一个封包
		pFilterList = pFilterList->pNext;
	}

	
	// 默认情况下接收所有封包
	return TRUE;
}



// 过滤向外发送的数据，从MPSendPackets或者MPSend函数调用
// 如果从MPSendPackets调用就运行在IRQL <= DISPATCH_LEVEL级别
// 如果从MPSend调用，就运行在IRQL == DISPATCH_LEVEL级别
BOOLEAN FltFilterSendPacket(
				IN PADAPT          pAdapt,
				IN PNDIS_PACKET   pSendPacket,
				IN BOOLEAN         bDispatchLevel  // TRUE -> IRQL == DISPATCH_LEVEL
	)
{
	BOOLEAN bPass = TRUE;
	PADAPT_FILTER_RSVD pFilterContext = (PADAPT_FILTER_RSVD)&pAdapt->FilterReserved;
	UCHAR buffer[MAX_PACKET_HEADER_LEN];
	ULONG nReadBytes;
	
	// 当使用过滤数据时，要获取旋转锁
	if(bDispatchLevel)
	{
		NdisDprAcquireSpinLock(&pAdapt->Lock);
	}
	else
	{
		NdisAcquireSpinLock(&pAdapt->Lock);
	}
	
	// 设置统计数字
	pFilterContext->Statistics.nMPSendPktsCt ++;
	
	// 如果没有设置过滤规则，则放行所有封包
	if(pFilterContext->pFilterList == NULL)
		goto ExitTheFilter;
	
	////////////////////////////////////////////////////
	// 读取封包中的数据，这里仅读取封包头即可
	FltReadPacketData(pSendPacket, buffer, MAX_PACKET_HEADER_LEN, &nReadBytes);
	// 检查过滤规则，看看是否允许这个封包通过
	bPass = FltCheckFilterRules(pFilterContext->pFilterList, buffer, nReadBytes, TRUE);
	
	if(!bPass)
	{
		// 拒绝了一个封包
		pFilterContext->Statistics.nMPSendPktsDropped ++;
	}
	
ExitTheFilter:
	// 过滤之后要释放旋转锁
	if(bDispatchLevel)
		NdisDprReleaseSpinLock(&pAdapt->Lock);
	else
		NdisReleaseSpinLock(&pAdapt->Lock);
	
	return bPass;
}



// 过滤接收到的数据,从PtReceivePacket函数调用，运行在DISPATCH_LEVEL IRQL级别
BOOLEAN FltFilterReceivePacket(
				IN PADAPT         pAdapt,
				IN	PNDIS_PACKET   pReceivedPacket
				)
{
	BOOLEAN bPass = TRUE;
	PADAPT_FILTER_RSVD pFilterContext = (PADAPT_FILTER_RSVD)&pAdapt->FilterReserved;
	UCHAR buffer[MAX_PACKET_HEADER_LEN];
	ULONG nReadBytes;
	
	// 当使用过滤数据时，要获取旋转锁
	NdisDprAcquireSpinLock(&pAdapt->Lock);
	
	
	// 设置统计数字
	pFilterContext->Statistics.nPTRcvPktCt ++;

	// 如果没有设置过滤规则，则放行所有封包
	if(pFilterContext->pFilterList == NULL)
		goto ExitTheFilter;
	
	////////////////////////////////////////////////////
	// 读取封包中的数据，这里仅读取封包头即可
	FltReadPacketData(pReceivedPacket, buffer, MAX_PACKET_HEADER_LEN, &nReadBytes);
	if(nReadBytes != MAX_PACKET_HEADER_LEN)
	{
		DBGPRINT(("  FltFilterReceivePacket:  nReadBytes != MAX_PACKET_HEADER_LEN"));
	}
	// 检查过滤规则，看看是否允许这个封包通过
	bPass = FltCheckFilterRules(pFilterContext->pFilterList,buffer, nReadBytes, TRUE);
	if(!bPass)
	{
		// 拒绝了一个封包
		pFilterContext->Statistics.nPTRcvPktDropped ++;
	}

ExitTheFilter:
	// 过滤之后要释放旋转锁
	NdisDprReleaseSpinLock(&pAdapt->Lock);
	
	return bPass;
}

// 过滤接收到的数据,从PtReceivePacket函数调用，运行在DISPATCH_LEVEL IRQL级别
BOOLEAN FltFilterReceive(
					IN PADAPT         pAdapt,
					IN NDIS_HANDLE    MacReceiveContext,
					IN PVOID          HeaderBuffer,
					IN UINT           HeaderBufferSize,
					IN PVOID          LookAheadBuffer,
					IN UINT           LookAheadBufferSize,
					IN UINT           PacketSize
					)
{
	BOOLEAN bPass = TRUE;
	PADAPT_FILTER_RSVD pFilterContext = (PADAPT_FILTER_RSVD)&pAdapt->FilterReserved;
	PETHeader pEtherHdr = (PETHeader)HeaderBuffer;
	
	// 当使用过滤数据时，要获取旋转锁
	NdisDprAcquireSpinLock(&pAdapt->Lock);
	
	
	// 设置统计数字
	pFilterContext->Statistics.nPTRcvCt ++;
	// 如果没有设置过滤规则，则放行所有封包
	if(pFilterContext->pFilterList == NULL)
		goto ExitTheFilter;
	
	// 如果不是IP协议，则放行
	if(pEtherHdr->type != 0x8)  
		goto ExitTheFilter;
	
	// 检查过滤规则，看看是否允许这个封包通过
	bPass = FltCheckFilterRules(pFilterContext->pFilterList,LookAheadBuffer, LookAheadBufferSize, FALSE);
	if(!bPass)
	{
		// 拒绝了一个封包
		pFilterContext->Statistics.nPTRcvDropped ++;
	}
	
ExitTheFilter:
	// 过滤之后要释放旋转锁
    NdisDprReleaseSpinLock(&pAdapt->Lock);

	return bPass;
}





//////////////////////////////////////////////////////////////////
// iocommon.h文件

#ifndef __IOCOMMON__H
#define __IOCOMMON__H


//////////////////////////////////////////////////////////
// 定义IO数据类型


typedef struct _PTUSERIO_OID_DATA 
{
    ULONG           Oid;
    ULONG           Length;
    UCHAR           Data[1];
}PTUSERIO_OID_DATA, *PPTUSERIO_OID_DATA;


// 网络活动状态
typedef struct _PassthruStatistics 
{
   ULONG    nMPSendPktsCt;      // 通过MPSendPackets的封包
   ULONG    nMPSendPktsDropped; // 在MPSendPackets中丢弃的封包
   ULONG    nPTRcvCt;           // 通过PTReceive的封包
   ULONG    nPTRcvDropped;      // 在PTReceive中丢弃的封包
   ULONG    nPTRcvPktCt;        // 通过PTReceivePacket的封包
   ULONG    nPTRcvPktDropped;   // 在PTReceivePacket中丢弃的封包
}PassthruStatistics, *PPassthruStatistics;

// 过滤规则
typedef struct _PassthruFilter
{
	USHORT protocol;		// 使用的协议

	ULONG sourceIP;			// 源IP地址
	ULONG sourceMask;		// 源地址屏蔽码     这里使用屏蔽码是为了能够设置一个IP地址范围

	ULONG destinationIP;	// 目的IP地址
	ULONG destinationMask;	// 目的地址屏蔽码

	USHORT sourcePort;		// 源端口号
	USHORT destinationPort;	// 目的端口号
	
	BOOLEAN bDrop;			// 是否丢弃此封包

}PassthruFilter, *PPassthruFilter;


////////////////////////////////////////////////////////////
// 定义IO控制代码

#define FSCTL_PTUSERIO_BASE      FILE_DEVICE_NETWORK

// 枚举绑定的适配器
#define IOCTL_PTUSERIO_ENUMERATE   \
            CTL_CODE(FSCTL_PTUSERIO_BASE, 0x201, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
 
// 打开一个适配器
#define IOCTL_PTUSERIO_OPEN_ADAPTER   \
            CTL_CODE(FSCTL_PTUSERIO_BASE, 0x202, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

#define IOCTL_PTUSERIO_SET_OID	\
		CTL_CODE(FSCTL_PTUSERIO_BASE, 0x203, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

#define IOCTL_PTUSERIO_QUERY_OID  \
		CTL_CODE(FSCTL_PTUSERIO_BASE, 0x204, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
    

    
// 获取网络活动状态
#define IOCTL_PTUSERIO_QUERY_STATISTICS   \
			CTL_CODE(FSCTL_PTUSERIO_BASE, \
				0x205, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
 
// 重设网络活动状态
#define IOCTL_PTUSERIO_RESET_STATISTICS   \
			CTL_CODE(FSCTL_PTUSERIO_BASE, \
				0x206, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
 
// 添加一个过滤规则
#define IOCTL_PTUSERIO_ADD_FILTER			\
			CTL_CODE(FSCTL_PTUSERIO_BASE,	\
				0x207, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

// 清除过滤规则
#define IOCTL_PTUSERIO_CLEAR_FILTER			\
			CTL_CODE(FSCTL_PTUSERIO_BASE,	\
				0x208, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)


#endif // __IOCOMMON__H







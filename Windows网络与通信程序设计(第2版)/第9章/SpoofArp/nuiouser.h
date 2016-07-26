// nuiouser.h文件
// 访问ProtoDrv驱动需要的常量和类型，用户程序必须包含


//#include "ntddndis.h"


#ifndef __NUIOUSER__H
#define __NUIOUSER__H

//#include <devioctl.h>



#define        MAX_LINK_NAME_LENGTH   124

typedef struct _PROTOCOL_OID_DATA	// 设置和获取适配器OID信息所需的结构
{

    ULONG           Oid;
    ULONG           Length;
    UCHAR           Data[1];

} PROTOCOL_OID_DATA, *PPROTOCOL_OID_DATA;


#define FILE_DEVICE_PROTOCOL      0x8000

// 4个IOCTL的功能分别是：设置适配器的OID信息，获取适配器的OID信息，重置适配器，枚举绑定的适配器
#define IOCTL_PROTOCOL_SET_OID      CTL_CODE(FILE_DEVICE_PROTOCOL, 0 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROTOCOL_QUERY_OID    CTL_CODE(FILE_DEVICE_PROTOCOL, 1 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_PROTOCOL_RESET        CTL_CODE(FILE_DEVICE_PROTOCOL, 2 , METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_ENUM_ADAPTERS         CTL_CODE(FILE_DEVICE_PROTOCOL, 3 , METHOD_BUFFERED, FILE_ANY_ACCESS)


#endif // __NUIOUSER__H
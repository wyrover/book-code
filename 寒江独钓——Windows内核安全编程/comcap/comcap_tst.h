///
/// @file		comcap_tst.c
/// @author	crazy_chu
/// @date		2008-6-20
/// 

#ifndef _COMCAP_TST_HEADER_
#define _COMCAP_TST_HEADER_

// 获取包指令码
#define CCPT_CMD_GETPCK	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
// 使能
#define CCPT_CMD_ENABLE	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)
// 禁止
#define CCPT_CMD_DISABLE	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)

// 两种操作类型。
#define CCPT_IN	1
#define CCPT_OUT 2

// 缓冲区中的包结构
typedef struct CCPT_PCK_ {
	unsigned long com_id;			// com口序列号
	unsigned long opr_type;			// 操作类型（读和写）
	unsigned long pid;					// 操作进程pid
	unsigned long data_length;		// 数据长度		
	unsigned char buf[1];				// 数据缓冲
} CCPT_PCK,*PCCPT_PCK;

#endif // _COMCAP_TST_HEADER_
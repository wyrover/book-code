//////////////////////////////////////////////////
// comm.h文件

// 包含一些公共函数



#ifndef __COMM_H__
#define __COMM_H__


// 校验和的计算
// 以16位的字为单位将缓冲区的内容相加，如果缓冲区长度为奇数，
// 则再加上一个字节。它们的和存入一个32位的双字中
USHORT checksum(USHORT* buff, int size);

BOOL SetTTL(SOCKET s, int nValue);
BOOL SetTimeout(SOCKET s, int nTime, BOOL bRecv = TRUE);


#endif // __COMM_H__
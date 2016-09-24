#ifndef _DP_BITMAP_H_
#define _DP_BITMAP_H_

#define BITMAP_ERR_OUTOFRANGE	-1
#define BITMAP_ERR_ALLOCMEMORY	-2
#define BITMAP_SUCCESS			0
#define BITMAP_BIT_SET			1
#define BITMAP_BIT_CLEAR		2
#define BITMAP_BIT_UNKNOW		3
#define BITMAP_RANGE_SET		4
#define BITMAP_RANGE_CLEAR		5
#define BITMAP_RANGE_BLEND		6
#define BITMAP_RANGE_SIZE		25600
#define BITMAP_RANGE_SIZE_SMALL 256
#define BITMAP_RANGE_SIZE_MAX	51684
#define BITMAP_RANGE_AMOUNT		16*1024

typedef unsigned char tBitmap;

#pragma pack(1)

typedef struct _DP_BITMAP_
{
	//这个卷中的每个扇区有多少字节，这同样也说明了bitmap中一个位所对应的字节数
    unsigned long sectorSize; 
	//每个byte里面有几个bit，一般情况下是8
    unsigned long byteSize; 
	//每个块是多大byte，
    unsigned long regionSize;
	//这个bitmap总共有多少个块
    unsigned long regionNumber;
	//这个块对应了多少个实际的byte，这个数字应该是sectorSize*byteSize*regionSize
    unsigned long regionReferSize;
	//这个bitmap对应了多少个实际的byte，这个数字应该是sectorSize*byteSize*regionSize*regionNumber
    __int64 bitmapReferSize;
	//指向bitmap存储空间的指针
    tBitmap** Bitmap; 
	//用于存取bitmap的锁
    void* lockBitmap; 
} DP_BITMAP, * PDP_BITMAP;

#pragma pack()
NTSTATUS DPBitmapInit(
	DP_BITMAP **	  bitmap,
	unsigned long     sectorSize,
	unsigned long     byteSize,
	unsigned long     regionSize,
	unsigned long     regionNumber
	);

void DPBitmapFree(DP_BITMAP* bitmap);

NTSTATUS DPBitmapSet(
  DP_BITMAP *		bitmap,
  LARGE_INTEGER     offset,
  unsigned long     length
  );

NTSTATUS DPBitmapGet(
  DP_BITMAP *    bitmap,
  LARGE_INTEGER     offset,
  unsigned long     length,
  void *            bufInOut,
  void *            bufIn
  );

long DPBitmapTest(
  DP_BITMAP *    bitmap,
  LARGE_INTEGER     offset,
  unsigned long     length
  );
#endif

#include <ntifs.h>
#include <windef.h>
#include "DPBitmap.h"

static tBitmap bitmapMask[8] =
{
	//需要用到的bitmap的位掩码
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
};

void * DPBitmapAlloc(
	int             poolType,
	unsigned long   length
	)
{
	//根据参数来分配不同类型的内存，这里全部分配的是非分页类型内存
	if (0 == poolType)
	{
		return ExAllocatePoolWithTag(NonPagedPool, length , 'mbpD');
	}
	else if (1 == poolType)
	{
		return ExAllocatePoolWithTag(PagedPool, length , 'mbpD');
	}
	else
	{
		return NULL;
	}
}

void DPBitmapFree(DP_BITMAP* bitmap)
{
	//释放bitmap
	DWORD i = 0;

	if (NULL != bitmap)
	{
		if (NULL != bitmap->Bitmap)
		{
			for (i = 0; i < bitmap->regionNumber; i++)
			{
				if (NULL != *(bitmap->Bitmap + i))
				{
					//从最底层的块开始释放，所有块都轮询一次				
					ExFreePool(*(bitmap->Bitmap + i));
				}
			}
			//释放块的指针
			ExFreePool(bitmap->Bitmap);
		}	
		//释放bitmap本身
		ExFreePool(bitmap);
	}
}

void DPBitmapInitLock(void * lock){}

void DPBitmapLock(void * lock){}

void DPBitmapUnlock(void * lock){}

NTSTATUS DPBitmapInit(
	DP_BITMAP **     bitmap,
	unsigned long       sectorSize,
	unsigned long       byteSize,
	unsigned long       regionSize,
	unsigned long       regionNumber
	)
{
	int i = 0;
	DP_BITMAP * myBitmap = NULL;
	NTSTATUS status = STATUS_SUCCESS;

	//检查参数，以免使用了错误的参数导致发生处零错等错误
	if (NULL == bitmap || 0 == sectorSize ||
		0 == byteSize || 0 == regionSize  || 0 == regionNumber)
	{
		return STATUS_UNSUCCESSFUL;
	}
	__try
	{
		//分配一个bitmap结构，这是无论如何都要分配的，这个结构相当于一个bitmap的handle	
		if (NULL == (myBitmap = (DP_BITMAP*)DPBitmapAlloc(0, sizeof(DP_BITMAP))))
		{
			status = STATUS_INSUFFICIENT_RESOURCES;
			__leave;
		}
		//清空结构
		memset(myBitmap, 0, sizeof(DP_BITMAP));
		//根据参数对结构中的成员进行赋值
		myBitmap->sectorSize = sectorSize;
		myBitmap->byteSize = byteSize;
		myBitmap->regionSize = regionSize;
		myBitmap->regionNumber = regionNumber;
		myBitmap->regionReferSize = sectorSize * byteSize * regionSize;
		myBitmap->bitmapReferSize = (__int64)sectorSize * (__int64)byteSize * (__int64)regionSize * (__int64)regionNumber;
		//分配出regionNumber那么多个指向region的指针，这是一个指针数组
		if (NULL == (myBitmap->Bitmap = (tBitmap **)DPBitmapAlloc(0, sizeof(tBitmap*) * regionNumber)))
		{
			status = STATUS_INSUFFICIENT_RESOURCES;
			__leave;
		}
		//清空指针数组
		memset(myBitmap->Bitmap, 0, sizeof(tBitmap*) * regionNumber);
		* bitmap = myBitmap;
		status = STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		status = STATUS_UNSUCCESSFUL;
	}
	if (!NT_SUCCESS(status))
	{
		if (NULL != myBitmap)
		{
			DPBitmapFree(myBitmap);
		}
		* bitmap = NULL;
	}
	return status;
}

NTSTATUS DPBitmapSet(
	DP_BITMAP *      bitmap,
	LARGE_INTEGER       offset,
	unsigned long       length
	)
{
	__int64 i = 0;
	unsigned long myRegion = 0, myRegionEnd = 0;
	unsigned long myRegionOffset = 0, myRegionOffsetEnd = 0;
	unsigned long myByteOffset = 0, myByteOffsetEnd = 0;
	unsigned long myBitPos = 0;
	NTSTATUS status = STATUS_SUCCESS;
	LARGE_INTEGER setBegin = { 0 }, setEnd = { 0 };

	__try
	{
		//检查变量
		if (NULL == bitmap || offset.QuadPart < 0)
		{
			status = STATUS_INVALID_PARAMETER;
			__leave;
		}
		if (0 != offset.QuadPart % bitmap->sectorSize || 0 != length % bitmap
			->sectorSize)
		{
			status = STATUS_INVALID_PARAMETER;
			__leave;
		}

		//根据要设置的偏移量和长度来计算需要使用到哪些region，如果需要的话，就分配他们指向的内存空间
		myRegion = (unsigned long)(offset.QuadPart / (__int64)bitmap->regionReferSize);
		myRegionEnd = (unsigned long)((offset.QuadPart + (__int64)length) / (__int64)bitmap->regionReferSize);
		for (i = myRegion; i <= myRegionEnd; ++i)
		{
			if (NULL == *(bitmap->Bitmap + i))
			{
				if (NULL == (*(bitmap->Bitmap + i) = (tBitmap*)DPBitmapAlloc(0, sizeof(tBitmap) * bitmap->regionSize)))
				{
					status = STATUS_INSUFFICIENT_RESOURCES;
					__leave;
				}
				else
				{
					memset(*(bitmap->Bitmap + i), 0, sizeof(tBitmap) * bitmap->regionSize);
				}
			}
		}

		//开始设置bitmap，首先我们需要将要设置的区域按照byte对齐，这样可以按byte设置而不需要按bit设置，加快设置速度
		//对于没有byte对齐的区域先手工设置掉他们
		for (i = offset.QuadPart; i < offset.QuadPart + (__int64)length; i += bitmap->sectorSize)
		{
			myRegion = (unsigned long)(i / (__int64)bitmap->regionReferSize);
			myRegionOffset = (unsigned long)(i % (__int64)bitmap->regionReferSize);
			myByteOffset = myRegionOffset / bitmap->byteSize / bitmap->sectorSize;
			myBitPos = (myRegionOffset / bitmap->sectorSize) % bitmap->byteSize;
			if (0 == myBitPos)
			{
				setBegin.QuadPart = i;
				break;
			}
			*(*(bitmap->Bitmap + myRegion) + myByteOffset) |= bitmapMask[myBitPos];
		}
		if (i >= offset.QuadPart + (__int64)length)
		{
			status = STATUS_SUCCESS;
			__leave;
		}

		for (i = offset.QuadPart + (__int64)length - bitmap->sectorSize; i >= offset.QuadPart; i -= bitmap->sectorSize)
		{
			myRegion = (unsigned long)(i / (__int64)bitmap->regionReferSize);
			myRegionOffset = (unsigned long)(i % (__int64)bitmap->regionReferSize);
			myByteOffset = myRegionOffset / bitmap->byteSize / bitmap->sectorSize;
			myBitPos = (myRegionOffset / bitmap->sectorSize) % bitmap->byteSize;
			if (7 == myBitPos)
			{
				setEnd.QuadPart = i;
				break;
			}
			*(*(bitmap->Bitmap + myRegion) + myByteOffset) |= bitmapMask[myBitPos];
		}

		if (i < offset.QuadPart || setEnd.QuadPart == setBegin.QuadPart)
		{
			status = STATUS_SUCCESS;
			__leave;
		}

		myRegionEnd = (unsigned long)(setEnd.QuadPart / (__int64)bitmap->regionReferSize);

		for (i = setBegin.QuadPart; i <= setEnd.QuadPart;)
		{
			myRegion = (unsigned long)(i / (__int64)bitmap->regionReferSize);
			myRegionOffset = (unsigned long)(i % (__int64)bitmap->regionReferSize);
			myByteOffset = myRegionOffset / bitmap->byteSize / bitmap->sectorSize;
			//如果我们设置的区域没有跨两个region，只需要使用memset去做按byte的设置然后跳出即可
			if (myRegion == myRegionEnd)
			{
				myRegionOffsetEnd = (unsigned long)(setEnd.QuadPart % (__int64)bitmap->regionReferSize);
				myByteOffsetEnd = myRegionOffsetEnd / bitmap->byteSize / bitmap->sectorSize;
				memset(*(bitmap->Bitmap + myRegion) + myByteOffset, 0xff, myByteOffsetEnd - myByteOffset + 1);
				break;
			}
			//如果我们设置的区域跨了两个region，需要设置完后递增
			else
			{
				myRegionOffsetEnd = bitmap->regionReferSize;
				myByteOffsetEnd = myRegionOffsetEnd / bitmap->byteSize / bitmap->sectorSize;
				memset(*(bitmap->Bitmap + myRegion) + myByteOffset, 0xff, myByteOffsetEnd - myByteOffset);
				i += (myByteOffsetEnd - myByteOffset) * bitmap->byteSize * bitmap->sectorSize;
			}
		}
		status = STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		status = STATUS_UNSUCCESSFUL;
	}

	if (!NT_SUCCESS(status))
	{
		
	}
	return status;
}

NTSTATUS DPBitmapGet(
	DP_BITMAP *    bitmap,
	LARGE_INTEGER     offset,
	unsigned long     length,
	void *            bufInOut,
	void *            bufIn
	)
{
	unsigned long i = 0;
	unsigned long myRegion = 0;
	unsigned long myRegionOffset = 0;
	unsigned long myByteOffset = 0;
	unsigned long myBitPos = 0;
	NTSTATUS status = STATUS_SUCCESS;

	__try
	{
		//检查参数
		if (NULL == bitmap || offset.QuadPart < 0 || NULL == bufInOut || NULL == bufIn)
		{
			status = STATUS_INVALID_PARAMETER;
			__leave;
		}
		if (0 != offset.QuadPart % bitmap->sectorSize || 0 != length % bitmap->sectorSize)
		{
			status = STATUS_INVALID_PARAMETER;
			__leave;
		}

		//遍历需要获取的位图范围，如果出现了位被设置为1，就需要用bufIn参数中指向的相应位置的数据拷贝到bufInOut中
		for (i = 0; i < length; i += bitmap->sectorSize)
		{
			myRegion = (unsigned long)((offset.QuadPart + (__int64)i) / (__int64)bitmap->regionReferSize);

			myRegionOffset = (unsigned long)((offset.QuadPart + (__int64)i) % (__int64)bitmap->regionReferSize);

			myByteOffset = myRegionOffset / bitmap->byteSize / bitmap->sectorSize;

			myBitPos = (myRegionOffset / bitmap->sectorSize) % bitmap->byteSize;

			if (NULL != *(bitmap->Bitmap + myRegion) && (*(*(bitmap->Bitmap + myRegion) + myByteOffset) &bitmapMask[myBitPos]))
			{
				memcpy((tBitmap*)bufInOut + i, (tBitmap*)bufIn + i, bitmap->sectorSize);
			}
		}

		status = STATUS_SUCCESS;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		status = STATUS_UNSUCCESSFUL;
	}

	return status;
}

long DPBitmapTest(
	DP_BITMAP *      bitmap,
	LARGE_INTEGER       offset,
	unsigned long       length
	)
{
	char flag = 0;
	unsigned long i = 0;
	unsigned long myRegion = 0;
	unsigned long myRegionOffset = 0;
	unsigned long myByteOffset = 0;
	unsigned long myBitPos = 0;
	long ret = BITMAP_BIT_UNKNOW;

	__try
	{
		//检查参数
		if (NULL == bitmap || offset.QuadPart < 0 || offset.QuadPart + length > bitmap->bitmapReferSize)
		{
			ret = BITMAP_BIT_UNKNOW;

			__leave;
		}

		for (i = 0; i < length; i += bitmap->sectorSize)
		{
			//针对需要测试的bitmap范围进行测试，如果全部为0则返回BITMAP_RANGE_CLEAR，如果全部为1，则返回BITMAP_RANGE_SET，如果为0，1混合则返回BITMAP_RANGE_BLEND
			myRegion = (unsigned long)((offset.QuadPart + (__int64)i) / (__int64)bitmap->regionReferSize);

			myRegionOffset = (unsigned long)((offset.QuadPart + (__int64)i) % (__int64)bitmap->regionReferSize);

			myByteOffset = myRegionOffset / bitmap->byteSize / bitmap->sectorSize;

			myBitPos = (myRegionOffset / bitmap->sectorSize) % bitmap->byteSize;

			if (NULL != *(bitmap->Bitmap + myRegion) && (*(*(bitmap->Bitmap + myRegion) + myByteOffset) &bitmapMask[myBitPos]))
			{
				flag |= 0x2;
			}
			else
			{
				flag |= 0x1;
			}

			if (flag == 0x3)
			{
				break;
			}
		}

		if (0x2 == flag)
		{
			ret = BITMAP_RANGE_SET;
		}
		else if (0x01 == flag)
		{
			ret = BITMAP_RANGE_CLEAR;
		}
		else if (0x03 == flag)
		{
			ret = BITMAP_RANGE_BLEND;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		ret = BITMAP_BIT_UNKNOW;
	}

	return ret;
}
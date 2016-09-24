//
//

#include "Spliter.h"

/**************************************************************************

    PAGEABLE CODE

**************************************************************************/

#ifdef ALLOC_PRAGMA
#pragma code_seg("PAGE")
#endif // ALLOC_PRAGMA


CFilter::CFilter (IN PKSFILTER Filter) :
    m_Filter (Filter)
{
}

/*************************************************/

// DispatchCreate是一个类静态函数，它注册给AVStream框架，
// 当框架要创建一个Filter对象的时候，将调用此函数。
NTSTATUS CFilter::DispatchCreate (
    IN PKSFILTER pKsFilter,
    IN PIRP Irp
    )
{
    PAGED_CODE();

    NTSTATUS Status = STATUS_SUCCESS;

    // 先创建一个CFilter类对象，在系统的非分页池中申请内容。
    CFilter *pFilterObj = new (NonPagedPool)
                            CFilter (pKsFilter);

    do{
        if (!pFilterObj) 
        {       
            Status = STATUS_INSUFFICIENT_RESOURCES;
            break;
        }

        // 在AVStream架构中，很重要的一点是要善于利用对象包。
        // 把申请到的所有资源都放到对象包中，可以避免资源泄漏。
        Status = KsAddItemToObjectBag (
            pKsFilter -> Bag,
            reinterpret_cast <PVOID> (pFilterObj),
            ClearObj    // 释放函数，此处设置为空亦可。
            );

        if (!NT_SUCCESS (Status)) 
        {
            delete pFilterObj;
            break;
        }

        // 下面一步非常重要:将Filter类对象指针作为环境参数，保存
        // 在KSFILTER框架对象中。由于框架对象是唯一且不变的，所以每
        // 次Filter回调被执行的时候，都可以从框架对象中取出类对象。
        pKsFilter -> Context = reinterpret_cast <PVOID> (pFilterObj);        
    }while(0);    

    return Status;
}

/**************************************************************************

    LOCKED CODE

**************************************************************************/

#ifdef ALLOC_PRAGMA
#pragma code_seg()
#endif // ALLOC_PRAGMA

// Filter对象的处理函数。
// 我们的处理步骤分下面几步:
// 1. 取输入和输出Pin指针
// 2. 处理Pin上的数据
// 3. 步进处理
NTSTATUS CFilter::Process (
    IN PKSPROCESSPIN_INDEXENTRY ProcessPinsIndex
    )
{
    KdPrint(("Process"));
    NTSTATUS Status = STATUS_SUCCESS;

    BOOLEAN PendingFrames = FALSE;
    ULONG    size;

    // 取输入/输出Pin对象。记住，在我们这个驱动中,
    // Filter对象包含一个输入Pin和两个目地Pin。
    PKSPROCESSPIN pPinInput = NULL;
    PKSPROCESSPIN pPinOutput[2] = {NULL, NULL};
    
    // 所有的Pin对象都由输入参数ProcessPinsIndex传入
    // 需要对这个参数进行解析，才能够取出其中包含的Pin对象
    ASSERT(ProcessPinsIndex[0].Count == 1);
    ASSERT(ProcessPinsIndex[1].Count > 1);

    pPinInput = ProcessPinsIndex[0].Pins[0];
    pPinOutput[0] = ProcessPinsIndex[1].Pins[0];

	if(ProcessPinsIndex[1].Count > 1)
		pPinOutput[1] = ProcessPinsIndex[1].Pins[1];

    // 下面一步求出可供处理的数据长度。
    // 我们应当选取输入/输出Pin中较小的那个长度来处理。
    size = min (pPinInput->BytesAvailable, pPinOutput[0]->BytesAvailable);

    //下面两步：
    //1. 输入Pin上的数据拷贝到输出Pin上，由于有两个输出Pin从而实现1对2的splitter。
    if (!pPinInput->InPlaceCounterpart)
    {
        RtlCopyMemory(pPinOutput[0]->Data, pPinInput->Data, size);
    }

	if(pPinOutput[1])
	{
		RtlCopyMemory(pPinOutput[1]->Data, pPinOutput[0]->Data, size);
	}

    // 2. 输出音量提高2倍
    for(int i = 0; i < size; i++)
    {
        ((char*)pPinOutput[0]->Data)[i] *= 2;

		if(pPinOutput[1])
		{
			((char*)pPinOutput[1]->Data)[i] *= 2;
		}
    }

    // 下面一步用来维护Pin对象的步进。
    // 只有设置了输出Pin的BytesUsed值，系统才会处理输出Pin上的有效输出数据。
    // 同理，只有对输入Pin进行步进设置，系统才会继续给他Render有效的音频数据。
    pPinOutput[0]->BytesUsed = pPinInput->BytesUsed = size;

	if(pPinOutput[1])
	{
		pPinOutput[1]->BytesUsed = size;
	}

    return STATUS_SUCCESS;
}

// 可以为Pin对象定义一个名字，并可以被系统显示
GUID gPinName_GUID = 
{0xba1184b9, 0x1fe6, 0x488a, 0xae, 0x78, 0x6e, 0x99, 0x7b, 0x2, 0xca, 0xe0};


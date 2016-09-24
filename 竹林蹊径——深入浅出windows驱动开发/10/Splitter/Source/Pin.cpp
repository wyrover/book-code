
#include "Spliter.h"

/**************************************************************************

    PAGED CODE

**************************************************************************/

#ifdef ALLOC_PRAGMA
#pragma code_seg("PAGE")
#endif // ALLOC_PRAGMA

// DispatchCreate是一个类静态函数，它注册给AVStream框架，当框架要
// 为根据Filter描述符中的设置为它创建Pin对象的时候，将调用此函数。
NTSTATUS CPin::DispatchCreate (
    IN PKSPIN pKsPin,
    IN PIRP Irp
    )
{
    PAGED_CODE();

    NTSTATUS Status = STATUS_SUCCESS;

    CPin *pPinObj = new (NonPagedPool) CPin (pKsPin);

    if (!pPinObj) 
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    Status = KsAddItemToObjectBag (
        pKsPin -> Bag,
        reinterpret_cast <PVOID> (pPinObj),
        reinterpret_cast <PFNKSFREE> (ClearObj));

    if (!NT_SUCCESS (Status))
    {
        delete pPinObj;
    }

    // 下面一步非常重要:将Pin类对象指针作为环境参数，保存
    // 在KSPIN框架对象中。由于框架对象是唯一且不变的，所以每
    // 次Pin回调被执行的时候，都可以从框架对象中取出类对象。
    pKsPin -> Context = reinterpret_cast <PVOID> (pPinObj);

    return Status;
}

// 当Pin对象状态迁移的时候，会调用此回调函数。
// 我们在这个驱动里面不用做任何多余的事情
NTSTATUS CPin::SetState (
                         IN KSSTATE ToState,
                         IN KSSTATE FromState
                         )
{
    PAGED_CODE();

    switch (ToState) 
    {
    case KSSTATE_RUN:
        KdPrint(("KSSTATE_RUN"));
        break;
    case KSSTATE_STOP:  
        KdPrint(("KSSTATE_STOP"));
        break;
    case KSSTATE_ACQUIRE:
        KdPrint(("KSSTATE_ACQUIRE"));
        break;
    case KSSTATE_PAUSE:
        KdPrint(("KSSTATE_PAUSE"));
        break;
    }

    m_State = ToState;
    return STATUS_SUCCESS;
}

NTSTATUS CPin::IntersectHandler (
    IN PKSFILTER pKsFilter,
    IN PIRP Irp,
    IN PKSP_PIN PinInstance,
    IN PKSDATARANGE CallerDataRange,
    IN PKSDATARANGE DescriptorDataRange,
    IN ULONG BufferSize,
    OUT PVOID Data OPTIONAL,
    OUT PULONG DataSize
    )
{    
    PAGED_CODE();
    PKSPIN pKsPin;

    PKSDATARANGE_AUDIO CallerAudioRange =
        reinterpret_cast <PKSDATARANGE_AUDIO> (CallerDataRange);

    PKSDATARANGE_AUDIO DescriptorAudioRange =
        reinterpret_cast <PKSDATARANGE_AUDIO> (DescriptorDataRange);

    // 因为输入/输出Pin的数据格式必须统一，
    // 所以下面要做的一件事情，是首先判断对应的输出/输入Pin对象是否存在
    // 如果已经存在，则Pin的数据格式一定已经设置好了，只要拷贝过来即可
    // 如果不存在，必须手动从头设置
    if (PinInstance->PinId == 0)
    {
        pKsPin = KsFilterGetFirstChildPin (pKsFilter, 1);
    }
    else
    {
        pKsPin = KsFilterGetFirstChildPin (pKsFilter, 0);
    }

    if(pKsPin)
    {
        // 专门用以获取长度，按规定应返回STATUS_BUFFER_OVERFLOW
        if(BufferSize == 0)
        {
            *DataSize = pKsPin->ConnectionFormat->FormatSize;
            return STATUS_BUFFER_OVERFLOW;
        }

        // 如果缓冲区长度太短，返回STATUS_BUFFER_TOO_SMALL
        if(BufferSize < pKsPin->ConnectionFormat->FormatSize)
        {
            return STATUS_BUFFER_TOO_SMALL;
        }

        // 在拷贝之前，先对pKsPin中的数据格式做简单的判断
        if(IsEqualGUIDAligned (CallerDataRange->Specifier, 
            KSDATAFORMAT_SPECIFIER_WAVEFORMATEX))
        {
            if(CallerDataRange->FormatSize < sizeof(KSDATARANGE_AUDIO))
            {
                return STATUS_INVALID_PARAMETER;
            }

            PWAVEFORMATEX pWaveFormatEx = 
                (PWAVEFORMATEX)(pKsPin->ConnectionFormat + 1);

            if (DescriptorAudioRange -> MaximumChannels < pWaveFormatEx -> nChannels ||
                DescriptorAudioRange -> MinimumBitsPerSample > pWaveFormatEx -> wBitsPerSample ||
                DescriptorAudioRange -> MaximumBitsPerSample < pWaveFormatEx -> wBitsPerSample ||
                DescriptorAudioRange -> MaximumSampleFrequency < pWaveFormatEx -> nSamplesPerSec ||
                DescriptorAudioRange -> MinimumSampleFrequency > pWaveFormatEx -> nSamplesPerSec) 
            {
                *DataSize = sizeof (KSDATAFORMAT_WAVEFORMATEX);
                return STATUS_NO_MATCH;
            }
        }
        else
        {
            return STATUS_NO_MATCH;
        }

        // 复制数据格式结构体
        *DataSize = pKsPin->ConnectionFormat->FormatSize;

        RtlCopyMemory (Data,
            pKsPin->ConnectionFormat, 
            *DataSize);

        return STATUS_SUCCESS;
    }

    // 下面手动设置格式结构体
    // 

    if(0 == BufferSize)
    {
        *DataSize = sizeof(KSDATAFORMAT_WAVEFORMATEX);
        return STATUS_BUFFER_OVERFLOW;
    }

    if (BufferSize < sizeof(KSDATAFORMAT_WAVEFORMATEX)) 
    {
        return STATUS_NO_MATCH;
    }

    if(CallerDataRange->FormatSize < sizeof(KSDATARANGE_AUDIO))
    {
        return STATUS_INVALID_PARAMETER;
    }

    if (DescriptorAudioRange -> MinimumBitsPerSample > CallerAudioRange -> MaximumBitsPerSample ||
        DescriptorAudioRange -> MaximumBitsPerSample < CallerAudioRange -> MinimumBitsPerSample ||
        DescriptorAudioRange -> MaximumSampleFrequency < CallerAudioRange -> MinimumSampleFrequency ||
        DescriptorAudioRange -> MinimumSampleFrequency > CallerAudioRange -> MaximumSampleFrequency) 
    {
        return STATUS_NO_MATCH;
    }

    if(IsEqualGUIDAligned (CallerDataRange->Specifier, 
        KSDATAFORMAT_SPECIFIER_WAVEFORMATEX))
    {
        *DataSize = sizeof (KSDATAFORMAT_WAVEFORMATEX);

        PWAVEFORMATEX WaveFormat = (PWAVEFORMATEX)((PKSDATAFORMAT)Data + 1);
        PKSDATARANGE_AUDIO DescriptorAudioRange = (PKSDATARANGE_AUDIO)DescriptorDataRange;

        // 下面手动设置格式的各个方面！
        *(PKSDATAFORMAT)Data = *DescriptorDataRange;

        WaveFormat -> wFormatTag = WAVE_FORMAT_PCM;
        WaveFormat -> nChannels = (WORD)DescriptorAudioRange -> MaximumChannels;
        WaveFormat -> nSamplesPerSec = DescriptorAudioRange -> MinimumSampleFrequency;
        WaveFormat -> wBitsPerSample = (WORD)DescriptorAudioRange -> MinimumBitsPerSample;
        WaveFormat -> nBlockAlign = (WaveFormat -> wBitsPerSample / 8) * WaveFormat -> nChannels;
        WaveFormat -> nAvgBytesPerSec = (WaveFormat -> nBlockAlign) * (WaveFormat -> nSamplesPerSec);
        WaveFormat -> cbSize = 0;

        ((PKSDATAFORMAT)Data)->SampleSize = WaveFormat -> nBlockAlign;
        ((PKSDATAFORMAT)Data)->FormatSize = sizeof (KSDATAFORMAT_WAVEFORMATEX);

        return STATUS_SUCCESS;
    }

    return STATUS_NO_MATCH;
}

NTSTATUS CPin::DispatchSetFormat (
    IN PKSPIN pKsPin,
    IN PKSDATAFORMAT OldFormat OPTIONAL,
    IN PKSMULTIPLE_ITEM OldAttributeList OPTIONAL,
    IN const KSDATARANGE *DataRange,
    IN const KSATTRIBUTE_LIST *AttributeRange OPTIONAL
    )
{
    PAGED_CODE();

    const KSDATARANGE_AUDIO *DataRangeAudio =
        reinterpret_cast <const KSDATARANGE_AUDIO *> (DataRange);

    // 格式应该是KSDATAFORMAT_WAVEFORMATEX类型的
    if (pKsPin -> ConnectionFormat -> FormatSize <
        sizeof (KSDATAFORMAT_WAVEFORMATEX)) 
    {
        return STATUS_NO_MATCH;
    }

    // 简单判断一下，格式是否正确。格式范围应由框架调用回调
    // 函数IntersectHandler返回，并作为输入参数DataRange传入
    PWAVEFORMATEX WaveFormat = (PWAVEFORMATEX)(pKsPin -> ConnectionFormat + 1);

    if (WaveFormat -> wFormatTag != WAVE_FORMAT_PCM ||
        WaveFormat -> nChannels != DataRangeAudio -> MaximumChannels ||
        WaveFormat -> nSamplesPerSec > DataRangeAudio -> MaximumSampleFrequency || 
		WaveFormat -> nSamplesPerSec < DataRangeAudio -> MinimumSampleFrequency ||
        WaveFormat -> wBitsPerSample > DataRangeAudio -> MaximumBitsPerSample ||
		WaveFormat -> wBitsPerSample < DataRangeAudio -> MinimumBitsPerSample) 
    {
        return STATUS_NO_MATCH;
    }

    // 下面要做的事情是判断输入、输出Pin的格式是否相同
    PKSPIN pKsOtherPin = 0;
    PKSFILTER pKsFilter = KsPinGetParentFilter (pKsPin);

    if(pKsPin->Id == 0)
    {
        pKsOtherPin = KsFilterGetFirstChildPin (pKsFilter, 1);
    }
    else
    {
        pKsOtherPin = KsFilterGetFirstChildPin (pKsFilter, 0);
    }

    if(!pKsOtherPin)
    {
        return STATUS_SUCCESS;
    }

    PWAVEFORMATEX   thisWaveFmt = (PWAVEFORMATEX)(pKsPin->ConnectionFormat + 1);
    PWAVEFORMATEX  otherWaveFmt = (PWAVEFORMATEX)(pKsOtherPin->ConnectionFormat + 1);

    if ((thisWaveFmt->nChannels == otherWaveFmt->nChannels) &&
        (thisWaveFmt->nSamplesPerSec == otherWaveFmt->nSamplesPerSec) &&
        (thisWaveFmt->wBitsPerSample == otherWaveFmt->wBitsPerSample))
    {
        // 二者格式相同，返回正确
        return STATUS_SUCCESS;
    }

    // 如果格式不完全相同，要尝试将二者进行统一。
    // 把当前设置的格式去设置另外一个Pin，如果成功则亦可。
    // 应注意下面这一步具体是如何实现的。
    KSPROPERTY      property;
    PIKSCONTROL     pIKsControl;
    ULONG           cbReturned;

    property.Set = KSPROPSETID_Connection;
    property.Id = KSPROPERTY_CONNECTION_DATAFORMAT;
    property.Flags = KSPROPERTY_TYPE_SET;
    
    // 获取连接到pKsOtherPin的Pin对象的控制接口
    // 下面会通过这个控制接口重新设置它的Format属性
    NTSTATUS status = KsPinGetConnectedPinInterface (
        pKsOtherPin,
        &IID_IKsControl, 
        (PVOID*)&pIKsControl);

    if (!NT_SUCCESS(status))
    {
        return status;
    }

    // 释放当前线程占有的框架Mutex
    // 如果不这样做，可能会导致死锁
    KsFilterReleaseControl (pKsFilter);

    // 发送命令
    status = pIKsControl->KsProperty (
        &property, 
        sizeof(property),
        pKsPin->ConnectionFormat, 
        pKsPin->ConnectionFormat->FormatSize,
        &cbReturned);

    // 重新获取Mutex，
    // 这样框架在外面释放的时候才不会出错
    KsFilterAcquireControl (pKsFilter);

    // 释放控制接口
    pIKsControl->Release();
    return status;
}

NTSTATUS CPin::AudioPosition(
                 IN     PIRP irp,
                 IN     PKSPROPERTY Request,
                 IN OUT PVOID Data)
{
	PAGED_CODE ();

	PKSFILTER       pKsFilter;
	PKSPIN          pKsPinOther;
	PKSPIN          pKsPin;
	ULONG           bytesReturned;
	PIKSCONTROL     pIKsControl;
	NTSTATUS        ntStatus;

	KdPrint(("[PropertyAudioPosition]"));

	// Pin对象指针保持在IRP中
	pKsPin = KsGetPinFromIrp (irp);
	if (!pKsPin)
	{
		KdPrint(("[PropertyAudioPosition] this property is for a filter node?"));
		return STATUS_UNSUCCESSFUL;
	}

	// 我们认为，此请求只应该发送给Pin 0
	if (pKsPin->Id != 0)
	{
		KdPrint (("[PropertyAudioPosition] this property was invoked on the source pin!"));
		return STATUS_INVALID_DEVICE_REQUEST;
	}

	// 取得另一个Pin 1对象
	pKsFilter = KsPinGetParentFilter (pKsPin);
	KsFilterAcquireControl (pKsFilter);
	pKsPinOther = KsFilterGetFirstChildPin (pKsFilter, 1);
	if (!pKsPinOther)
	{
		KsFilterReleaseControl (pKsFilter);
		KdPrint (("[PropertyAudioPosition] couldn't find the source pin."));
		return STATUS_UNSUCCESSFUL;
	}

	//取得连接可输出Pin的下一个Filter的输入Pin
	ntStatus = KsPinGetConnectedPinInterface (
		pKsPinOther, 
		&IID_IKsControl,
		(PVOID*)&pIKsControl);

	if (!NT_SUCCESS (ntStatus))
	{
		KsFilterReleaseControl (pKsFilter);
		KdPrint (("[PropertyAudioPosition] couldn't get IID_IKsControl interface."));
		return STATUS_UNSUCCESSFUL;
	}

	// 在调用其他Pin函数前，应释放当前Filter的互斥对象
	// 否则可能引起死锁
	KsFilterReleaseControl (pKsFilter);

	// 把当前请求原原本本地传递下去
	ntStatus = pIKsControl->KsProperty (
		Request, 
		sizeof (KSPROPERTY),
		Data, 
		sizeof (KSAUDIO_POSITION),
		&bytesReturned);

	pIKsControl->Release();
	irp->IoStatus.Information = bytesReturned;

	return(ntStatus);
}

#ifdef ALLOC_PRAGMA
#pragma code_seg()
#endif // ALLOC_PRAGMA

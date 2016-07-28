/// 
///
#include "Spliter.h"

void ClearObj (IN void* obj)
{
    delete obj;
}

/**************************************************************************

    INITIALIZATION CODE

**************************************************************************/


extern "C"
NTSTATUS
DriverEntry (
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath
    )
{

    // 入口函数里面只要做很少量的事情，
    // 但务必不要忘记调用KsInitializeDriver。
    // KsInitializeDriver是AVStream框架暴露的接口函数，
    // 用来初始化框架有关的结构体。作为最主要的参数，
    // 调用者应当详细并准确地设置设备描述符。
    return KsInitializeDriver (
            DriverObject,
            RegistryPath,
            &gKsSplitterDevice
            );

}

/**************************************************************************

    下面是描述符列表，包括设备、Filter描述符及描述符表，等多种类型

**************************************************************************/

//=====================首先是Pin相关的描述符

// 输入/输出Pin所支持的数据格式，这其实是一个格式范围（Range）
// 在这个范围里面，可以设置最大、最小采用，最大最小采样深度
//
// 下面的结构体所表示的格式范围是：
// 双声道、24位采样、44.1或者48K采样率
KSDATARANGE_AUDIO gPinDataFormatRange =
{
    {
        sizeof(KSDATARANGE_AUDIO),
        0, 6, 0,
        STATIC_KSDATAFORMAT_TYPE_AUDIO,
        STATICGUIDOF(KSDATAFORMAT_SUBTYPE_PCM),
        STATICGUIDOF(KSDATAFORMAT_SPECIFIER_WAVEFORMATEX) 
    },
    2,          // channels
    16,         // min. bits per sample
    24,         // max. bits per sample
    44100,      // min. sample rate
    48000       // max. sample rate
};

const PKSDATARANGE gPinDataFormatRanges[] =
{
    PKSDATARANGE(&gPinDataFormatRange)
};

const KSPIN_DISPATCH gPinDispatch = 
{
    CPin::DispatchCreate,                    // Pin Create
    NULL,                                   // Pin Close
    NULL,                                   // Pin Process
    NULL,                                   // Pin Reset
    CPin::DispatchSetFormat,                // Pin Set Data Format
    CPin::DispatchSetState,                    // Pin Set Device State
    NULL,                                   // Pin Connect
    NULL,                                   // Pin Disconnect
    NULL,                                   // Clock Dispatch
    NULL                                    // Allocator Dispatch
};

// 分配帧格式（Allocator Framing）
DECLARE_SIMPLE_FRAMING_EX (
    gAllocatorFraming,
    STATICGUIDOF (KSMEMORY_TYPE_KERNEL_NONPAGED),
    KSALLOCATOR_REQUIREMENTF_SYSTEM_MEMORY |
    KSALLOCATOR_REQUIREMENTF_PREFERENCES_ONLY,
    25,
    0,
    2 * PAGE_SIZE,
    2 * PAGE_SIZE
);

// 定义属性列表
// 下表中包含了两个属性：分别用来获取音频位置以及处理采样变换
DEFINE_KSPROPERTY_TABLE (gPinProperty)
{
    DEFINE_KSPROPERTY_ITEM
    (
        KSPROPERTY_AUDIO_POSITION,      // property item defined in ksmedia.h
        CPin::AudioPositionHandler,     // our "get" property handler
        sizeof(KSPROPERTY),             // minimum buffer length for property
        sizeof(KSAUDIO_POSITION),       // minimum buffer length for returned data
        CPin::AudioPositionHandler,     // our "set" property handler
        NULL,                           // default values
        0,                              // related properties
        NULL,
        NULL,                           // no raw serialization handler
        0                                // don't serialize        
    ), 
#if 0
    DEFINE_KSPROPERTY_ITEM
    (
        KSPROPERTY_AUDIO_SAMPLING_RATE,
        CPin::AudioSampleRateHandler,
        sizeof(KSPROPERTY),
        sizeof(KSAUDIO_POSITION),
        CPin::AudioSampleRateHandler,
        NULL,
        0,
        NULL,
        NULL,
        0
    )
#endif
};

// 定义属性集列表
// 唯一的Property Set是系统定义的标准的属性集
DEFINE_KSPROPERTY_SET_TABLE (gPinPropertySet)
{
    DEFINE_KSPROPERTY_SET
    (
        &KSPROPSETID_Audio,
        SIZEOF_ARRAY(gPinProperty),
        gPinProperty,
        0,
        NULL
    )
};

// 定义自控表
// 自控表由属性、方法、事件三者组成
// 但在没有必要的情况下，可以不定义它们
// 本驱动仅实现了属性
DEFINE_KSAUTOMATION_TABLE (gPinAutomation)
{
    DEFINE_KSAUTOMATION_PROPERTIES(gPinPropertySet),
    DEFINE_KSAUTOMATION_METHODS_NULL,
    DEFINE_KSAUTOMATION_EVENTS_NULL
};

// 定义Pin描述符
const KSPIN_DESCRIPTOR_EX gPins [2] = 
{
    // 输入Pin
    // 可实例化的Pin个数为1，即仅有一个输入Pin。
    {
        &gPinDispatch,
        &gPinAutomation,
        {
            DEFINE_KSPIN_DEFAULT_INTERFACES,
            DEFINE_KSPIN_DEFAULT_MEDIUMS,
            SIZEOF_ARRAY(gPinDataFormatRanges), 
            gPinDataFormatRanges,
            KSPIN_DATAFLOW_IN,
            KSPIN_COMMUNICATION_BOTH,
            0, 0, 0
        },
        0,
        1,
        1,
        &gAllocatorFraming,
        reinterpret_cast <PFNKSINTERSECTHANDLEREX> (CPin::IntersectHandler)
    },

    // 输出Pin
    // 把可实例化的Pin个数定义为2，即可有两个输出Pin
    {
        &gPinDispatch,
        &gPinAutomation,
        {
            DEFINE_KSPIN_DEFAULT_INTERFACES, 
            DEFINE_KSPIN_DEFAULT_MEDIUMS,
            SIZEOF_ARRAY(gPinDataFormatRanges),
            gPinDataFormatRanges,
            KSPIN_DATAFLOW_OUT,
            KSPIN_COMMUNICATION_BOTH,
            0, 0, 0
        },
        0,
        2,
        1,
        &gAllocatorFraming,
        reinterpret_cast <PFNKSINTERSECTHANDLEREX> (CPin::IntersectHandler)
    }
};

// =====================下面开始是Filter的定义

const KSFILTER_DISPATCH gFilterDispatch = 
{
    CFilter::DispatchCreate,         // Filter Create
    NULL,                            // Filter Close
    CFilter::DispatchProcess,        // Filter Process
    NULL                             // Filter Reset
};

// 下面是Node和Connection描述符
const KSNODE_DESCRIPTOR gNodes[] =
{
    DEFINE_NODE_DESCRIPTOR(NULL, &KSNODETYPE_DEMUX, NULL)
};

// Filter对象类型
// KS的类型都是用GUID来表示的，所以它是一组GUID值
// 下面包含了两个GUID：
// KSCATEGORY_AUDIO：表明这个Filter是用来处理音频数据的（而非视频数据）
// KSCATEGORY_DATATRANSFORM：表明这个Filter对音频数据进行转换操作（而非压缩、混音等其他操作）
const GUID gFilterCategories [2] = 
{
    STATICGUIDOF (KSCATEGORY_AUDIO),
    STATICGUIDOF (KSCATEGORY_DATATRANSFORM)
};

// 对于我们的示例Driver，其实并没有真的用上Node对象
// 如果把Node对象省去，下面的Connection描述符中将仅
// 剩下一项，用来连接输入和输出Pin：
// {KSFILTER_NODE,  0, KSFILTER_NODE,  1}
KSTOPOLOGY_CONNECTION gConnections[] = 
{
    { KSFILTER_NODE,  0,    0, 1},
    { 0, 0,   KSFILTER_NODE,   1}
};

// Filter描述符表。设备描述符中可以包含一组Filter描述符。
// 根据需要，描述符的个数可以是0个或者多个。下表仅包含唯一的Filter描述符。
const KSFILTER_DESCRIPTOR gSplitterFilter = 
{
    &gFilterDispatch,                        // Dispatch Table
    NULL,                                    // Automation Table
    KSFILTER_DESCRIPTOR_VERSION,            // Version
    KSFILTER_FLAG_DISPATCH_LEVEL_PROCESSING,// Flags
    &KSNAME_Filter,                            // Reference GUID
    DEFINE_KSFILTER_PIN_DESCRIPTORS (gPins),
    DEFINE_KSFILTER_CATEGORIES (gFilterCategories),
    DEFINE_KSFILTER_NODE_DESCRIPTORS(gNodes),
    DEFINE_KSFILTER_CONNECTIONS(gConnections),
    NULL                                    // Component ID
};

DEFINE_KSFILTER_DESCRIPTOR_TABLE (gKsFilters) 
{
    &gSplitterFilter
};

// ===============================设备描述符

const KSDEVICE_DESCRIPTOR gKsSplitterDevice = 
{
    // 第一个参数对于Pin-Centre类型的AVStream驱动是非常重要的，
    // 相反对于我们这种Filter-Centre类型的驱动，则一般可以省略。
    // 这是一组PNP相关的回调函数，包括Start/Stop/Power等等回调。
    NULL,

    // 下面是Filter描述符
    SIZEOF_ARRAY (gKsFilters),
    gKsFilters,

    // 下面两个参数可以设置为0
    0,
    0
};

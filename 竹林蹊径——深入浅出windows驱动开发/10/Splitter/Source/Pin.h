//
//

class CPin
{
public:

    CFilter *m_pFilter;
    PKSPIN m_Pin;
    KSSTATE m_State;

    CPin (IN PKSPIN Pin)
    {
        m_Pin = Pin;
        ASSERT(Pin != 0);

        PKSFILTER ParentFilter = KsPinGetParentFilter (Pin);

        m_pFilter = reinterpret_cast <CFilter *> 
            (ParentFilter -> Context);
    }

    ~CPin ()
    {
    }

    static NTSTATUS DispatchCreate (
        IN PKSPIN Pin,
        IN PIRP Irp
        );

    static NTSTATUS DispatchSetFormat (
        IN PKSPIN Pin,
        IN PKSDATAFORMAT OldFormat OPTIONAL,
        IN PKSMULTIPLE_ITEM OldAttributeList OPTIONAL,
        IN const KSDATARANGE *DataRange,
        IN const KSATTRIBUTE_LIST *AttributeRange OPTIONAL
        );

    static NTSTATUS IntersectHandler (
        IN PKSFILTER Filter,
        IN PIRP Irp,
        IN PKSP_PIN PinInstance,
        IN PKSDATARANGE CallerDataRange,
        IN PKSDATARANGE DescriptorDataRange,
        IN ULONG BufferSize,
        OUT PVOID Data OPTIONAL,
        OUT PULONG DataSize
        );

    static NTSTATUS DispatchSetState (
        IN PKSPIN Pin,
        IN KSSTATE ToState,
        IN KSSTATE FromState
        )
    {
        return (reinterpret_cast <CPin *> 
            (Pin -> Context))->SetState(ToState, FromState);
    }
    
    NTSTATUS SetState (
        IN KSSTATE ToState,
        IN KSSTATE FromState
        );

    static NTSTATUS AudioSampleRateHandler(
        IN PIRP Irp,
        IN PKSPROPERTY Request,
        IN OUT PVOID Data)
    {
        KSPIN* Pin = KsGetPinFromIrp(Irp);
        if(Pin == NULL)return STATUS_INVALID_HANDLE;

        return (reinterpret_cast <CPin *> 
            (Pin -> Context))->SampleRateHandler(Irp, Request, Data);
    }

    NTSTATUS SampleRateHandler(
        IN PIRP Irp,
        IN PKSPROPERTY Request,
        IN OUT PVOID Data);

    static NTSTATUS AudioPositionHandler(
        IN PIRP Irp,
        IN PKSPROPERTY Request,
        IN OUT PVOID Data)
    {
        KSPIN* Pin = KsGetPinFromIrp(Irp);
        if(Pin == NULL)return STATUS_INVALID_HANDLE;

        return (reinterpret_cast <CPin *> 
            (Pin -> Context))->AudioPosition(Irp, Request, Data);
    }

    NTSTATUS AudioPosition(
        IN PIRP Irp,
        IN PKSPROPERTY Request,
        IN OUT PVOID Data);    
};


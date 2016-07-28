//
//

class CFilter 
{
    PKSFILTER m_Filter;

    NTSTATUS Process (
        IN PKSPROCESSPIN_INDEXENTRY ProcessPinsIndex
        );
public:

    PKSFILTER GetKsFilter(){return m_Filter;}

    CFilter (
        IN PKSFILTER Filter
        );

    ~CFilter (
        )
    {
    }

    static NTSTATUS DispatchCreate (
        IN PKSFILTER Filter,
        IN PIRP Irp
        );

    static NTSTATUS DispatchProcess (
        IN PKSFILTER Filter,
        IN PKSPROCESSPIN_INDEXENTRY ProcessPinsIndex
        )
    {
        return (reinterpret_cast <CFilter *> (Filter -> Context)) ->
                Process (ProcessPinsIndex);
    }
};


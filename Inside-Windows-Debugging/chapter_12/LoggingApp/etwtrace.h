#ifndef __ETWTRACE_H__
#define __ETWTRACE_H__

#include <evntprov.h>

class CEtwProvider
{
public:
    CEtwProvider(
        VOID
        )
    {
        m_hProviderHandle = NULL;
    }

    CEtwProvider(
        __in REFGUID ProviderId
        )
    {
        (VOID) Register(ProviderId);
    }

    ~CEtwProvider(
        VOID
        )
    {
        Unregister();
    }

    COREDEFS_INLINE
    HRESULT
    Register(
        __in REFGUID ProviderId
        )
    {
        DWORD dwErr = ERROR_SUCCESS;

        Unregister();
        dwErr = EventRegister(&ProviderId, 0, 0, &m_hProviderHandle);

        return HRESULT_FROM_WIN32(dwErr);
    }

    COREDEFS_INLINE
    VOID
    Unregister(
        VOID
        )
    {
        if (m_hProviderHandle != 0)
        {
            EventUnregister(m_hProviderHandle);
            m_hProviderHandle = 0;
        }
    }

    COREDEFS_INLINE
    VOID
    Write(
        __in const EVENT_DESCRIPTOR& eventDesc
        )
    {
        if (ProviderEventEnabled(eventDesc))
        {
            (VOID) EventWrite(
                m_hProviderHandle,
                &eventDesc,
                0,
                NULL
                );
        }
    }

private:
    COREDEFS_INLINE
    BOOL
    ProviderEventEnabled(
        __in const EVENT_DESCRIPTOR& eventDesc
        )
    {
        return (m_hProviderHandle != 0 && 
            EventEnabled(m_hProviderHandle, &eventDesc));
    }

    REGHANDLE m_hProviderHandle;
};

#endif //__ETWTRACE_H__

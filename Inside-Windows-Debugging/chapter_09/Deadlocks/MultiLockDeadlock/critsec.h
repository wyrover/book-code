#ifndef __CRITSEC_H__
#define __CRITSEC_H__

class CWin32CriticalSection :
    CZeroInit<CWin32CriticalSection>
{
public:
    ~CWin32CriticalSection(
        VOID
        )
    {
        if (m_bInitialized)
        {
            DeleteCriticalSection(&m_cs);
            m_bInitialized = FALSE;
        }
    }

    operator
    LPCRITICAL_SECTION(
        VOID
        )
    {
        return &m_cs;
    }

    BOOL
    IsInitialized(
        VOID
        ) const
    {
        return m_bInitialized;
    }

    HRESULT
    Init(
        VOID
        )
    {
        ChkProlog();

        if (m_bInitialized)
        {
            ChkReturn(S_OK);
        }

        ChkWin32(InitializeCriticalSectionAndSpinCount(&m_cs, 0));
        m_bInitialized = TRUE;

        ChkNoCleanup();
    }

private:

    CRITICAL_SECTION m_cs;
    BOOL m_bInitialized;
};

class CWin32CriticalSectionHolder :
    CZeroInit<CWin32CriticalSectionHolder>
{
public:
    CWin32CriticalSectionHolder(
        __in_opt CRITICAL_SECTION* pCS = NULL
        )
    {
        m_pCS = pCS;
    }

    ~CWin32CriticalSectionHolder(
        VOID
        )
    {
        Unlock();
    }

    VOID
    Lock(
        __in_opt CRITICAL_SECTION* pCS = NULL
        )
    {
        if (!m_bLocked)
        {
            ASSERT((pCS && m_pCS == NULL) || (m_pCS && pCS == NULL));

            if (pCS)
            {
                m_pCS = pCS;
            }

            EnterCriticalSection(m_pCS);
            m_bLocked = TRUE;
        }
        else
        {
            ASSERT(pCS == NULL || pCS == m_pCS);
        }
    }

    BOOL
    TryLock(
        VOID
        )
    {
        if (!m_bLocked)
        {
            m_bLocked = (TryEnterCriticalSection(m_pCS) == 0 ? FALSE : TRUE);
        }
        return m_bLocked;
    }

    VOID
    Unlock(
        VOID
        )
    {
        if (m_bLocked)
        {
            ASSERT(m_pCS);

            LeaveCriticalSection(m_pCS);
            m_bLocked = FALSE;
        }
    }

private:
    CRITICAL_SECTION* m_pCS;
    BOOL m_bLocked;
};

#endif //__CRITSEC_H__

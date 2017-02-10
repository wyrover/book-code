#pragma once

class CRefCountImpl
{
public:
    //
    // Declare a virtual destructor to ensure that derived classes
    // will be destroyed properly
    //
    virtual
    ~CRefCountImpl(
        VOID
        )
    {}

    CRefCountImpl(
        VOID
        ) : m_nRefs(1)
    {}

    VOID
    AddRef(
        VOID
        )
    {
        InterlockedIncrement(&m_nRefs);
    }

    VOID
    Release(
        VOID
        )
    {
        //
        // InterlockedIncrement and InterlockedDecrement return the *new* value after
        // the atomic assignment by the current thread (In contrast, the Interlocked*Exchange*
        // operations return the *old* value at the time of the "exchange"). This means
        // only one thread will ever get to free the ref-counted object (as desired).
        //
        if (0 == InterlockedDecrement(&m_nRefs))
        {
            delete this;
        }
    }

private:
    LONG m_nRefs;
};

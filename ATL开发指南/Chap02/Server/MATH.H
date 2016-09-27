//
// math.h
//

#include "imath.h"

extern long g_lObjs;
extern long g_lLocks;


class Math : public IMath, public IAdvancedMath
{
protected:
    // Reference count
    long          m_lRef;

public:
    Math();
    ~Math();

public:
    // IUnknown
    STDMETHOD(QueryInterface(REFIID, void**));
    STDMETHOD_(ULONG, AddRef());
    STDMETHOD_(ULONG, Release());

    // IMath
    STDMETHOD(Add(long, long, long*));
    STDMETHOD(Subtract(long, long, long*));
    STDMETHOD(Multiply(long, long, long*));
    STDMETHOD(Divide(long, long, long*));

    // IAdvancedMath
    STDMETHOD(Factorial(short, long*));
    STDMETHOD(Fibonacci(short, long*));

};

class MathClassFactory : public IClassFactory
{
protected:
    long       m_lRef;

public:
    MathClassFactory();
    ~MathClassFactory();

    // IUnknown
    STDMETHOD(QueryInterface(REFIID, void**));
    STDMETHOD_(ULONG, AddRef());
    STDMETHOD_(ULONG, Release());

    // IClassFactory
    STDMETHOD(CreateInstance(LPUNKNOWN, REFIID, void**));
    STDMETHOD(LockServer(BOOL));
};


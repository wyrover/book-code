//
// imath.h
//

// {A888F560-58E4-11d0-A68A-0000837E3100}
DEFINE_GUID(CLSID_Math,
            0xa888f560, 0x58e4, 0x11d0, 0xa6, 0x8a, 0x0, 0x0, 0x83, 0x7e, 0x31, 0x0);
// {A888F561-58E4-11d0-A68A-0000837E3100}
DEFINE_GUID(IID_IMath,
            0xa888f561, 0x58e4, 0x11d0, 0xa6, 0x8a, 0x0, 0x0, 0x83, 0x7e, 0x31, 0x0);

// {A888F562-58E4-11d0-A68A-0000837E3100}
DEFINE_GUID(IID_IAdvancedMath,
            0xa888f562, 0x58e4, 0x11d0, 0xa6, 0x8a, 0x0, 0x0, 0x83, 0x7e, 0x31, 0x0);

class IMath : public IUnknown
{
public:
    STDMETHOD(Add(long, long, long*))      PURE;
    STDMETHOD(Subtract(long, long, long*)) PURE;
    STDMETHOD(Multiply(long, long, long*)) PURE;
    STDMETHOD(Divide(long, long, long*))   PURE;
};

class IAdvancedMath : public IUnknown
{
public:
    STDMETHOD(Factorial(short, long*)) PURE;
    STDMETHOD(Fibonacci(short, long*))  PURE;
};


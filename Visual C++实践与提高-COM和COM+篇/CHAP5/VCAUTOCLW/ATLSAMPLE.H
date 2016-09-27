// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// IAccount wrapper class

class IAccount : public COleDispatchDriver
{
public:
    IAccount() {}       // Calls COleDispatchDriver default constructor
    IAccount(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    IAccount(const IAccount& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    long GetBalance();
    void SetBalance(long nNewValue);
    CString Post(long lAmount);
};

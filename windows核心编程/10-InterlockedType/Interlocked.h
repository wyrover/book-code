/******************************************************************************
Module:  Interlocked.h
Notices: Copyright (c) 2000 Jeffrey Richter
******************************************************************************/


#pragma once


///////////////////////////////////////////////////////////////////////////////


// Instances of this class will be accessed by multiple threads. So, 
// all members of this class (except the constructor and destructor) 
// must be thread-safe.
class CResGuard {
public:
   CResGuard()  { m_lGrdCnt = 0; InitializeCriticalSection(&m_cs); }
   ~CResGuard() { DeleteCriticalSection(&m_cs); }

   // IsGuarded is used for debugging
   BOOL IsGuarded() const { return(m_lGrdCnt > 0); }

public:
   class CGuard {
   public:
      CGuard(CResGuard& rg) : m_rg(rg) { m_rg.Guard(); };
      ~CGuard() { m_rg.Unguard(); }

   private:
      CResGuard& m_rg;
   };

private:
   void Guard()   { EnterCriticalSection(&m_cs); m_lGrdCnt++; }
   void Unguard() { m_lGrdCnt--; LeaveCriticalSection(&m_cs); }

   // Guard/Unguard can only be accessed by the nested CGuard class.
   friend class CResGuard::CGuard;

private:
   CRITICAL_SECTION m_cs;
   long m_lGrdCnt;   // # of EnterCriticalSection calls
};


///////////////////////////////////////////////////////////////////////////////


// Instances of this class will be accessed by multiple threads. So, 
// all members of this class (except the constructor and destructor) 
// must be thread-safe.
template <class TYPE>
class CInterlockedType {

public:     // Public member functions
   // Note: Constructors & destructors are always thread safe
   CInterlockedType() { }
   CInterlockedType(const TYPE& TVal) { m_TVal = TVal; }
   virtual ~CInterlockedType()  { }

   // Cast operator to make writing code that uses 
   // thread-safe data type easier
   operator TYPE() const { 
      CResGuard::CGuard x(m_rg); 
      return(GetVal()); 
   }

protected:  // Protected function to be called by derived class
   TYPE& GetVal() { 
      chASSERT(m_rg.IsGuarded()); 
      return(m_TVal); 
   }

   const TYPE& GetVal() const { 
      assert(m_rg.IsGuarded()); 
      return(m_TVal); 
   }

   TYPE SetVal(const TYPE& TNewVal) { 
      chASSERT(m_rg.IsGuarded()); 
      TYPE& TVal = GetVal();
      if (TVal != TNewVal) {
         TYPE TPrevVal = TVal;
         TVal = TNewVal;
         OnValChanged(TNewVal, TPrevVal);
      }
      return(TVal); 
   }

protected:  // Overridable functions
   virtual void OnValChanged(
      const TYPE& TNewVal, const TYPE& TPrevVal) const { 
      // Nothing to do here
   }

protected:  
   // Protected guard for use by derived class functions
      mutable CResGuard m_rg;

private:    // Private data members
   TYPE m_TVal;
};


///////////////////////////////////////////////////////////////////////////////


// Instances of this class will be accessed by multiple threads. So, 
// all members of this class (except the constructor and destructor) 
// must be thread-safe.
template <class TYPE>
class CInterlockedScalar : protected CInterlockedType<TYPE> {

public:
   CInterlockedScalar(TYPE TVal = 0) : CInterlockedType<TYPE>(TVal) { 
   }

   ~CInterlockedScalar() { /* Nothing to do */ }

   // C++ does not allow operator cast to be inherited.
   operator TYPE() const { 
      return(CInterlockedType<TYPE>::operator TYPE()); 
   }

   TYPE operator=(TYPE TVal) { 
      CResGuard::CGuard x(m_rg); 
      return(SetVal(TVal)); 
   }

   TYPE operator++(int) {    // Postfix increment operator
      CResGuard::CGuard x(m_rg);
      TYPE TPrevVal = GetVal();
      SetVal((TYPE) (TPrevVal + 1));
      return(TPrevVal);      // Return value BEFORE increment
   }

   TYPE operator--(int) {    // Postfix decrement operator.
      CResGuard::CGuard x(m_rg);
      TYPE TPrevVal = GetVal();
      SetVal((TYPE) (TPrevVal - 1));
      return(TPrevVal);      // Return value BEFORE decrement
   }

   TYPE operator += (TYPE op)   
      { CResGuard::CGuard x(m_rg); return(SetVal(GetVal() +  op)); }
   TYPE operator++()            
      { CResGuard::CGuard x(m_rg); return(SetVal(GetVal() +   1)); }
   TYPE operator -= (TYPE op)   
      { CResGuard::CGuard x(m_rg); return(SetVal(GetVal() -  op)); }
   TYPE operator--()            
      { CResGuard::CGuard x(m_rg); return(SetVal(GetVal() -   1)); }
   TYPE operator *= (TYPE op)   
      { CResGuard::CGuard x(m_rg); return(SetVal(GetVal() *  op)); }
   TYPE operator /= (TYPE op)   
      { CResGuard::CGuard x(m_rg); return(SetVal(GetVal() /  op)); }
   TYPE operator %= (TYPE op)   
      { CResGuard::CGuard x(m_rg); return(SetVal(GetVal() %  op)); }
   TYPE operator ^= (TYPE op)   
      { CResGuard::CGuard x(m_rg); return(SetVal(GetVal() ^  op)); }
   TYPE operator &= (TYPE op)   
      { CResGuard::CGuard x(m_rg); return(SetVal(GetVal() &  op)); }
   TYPE operator |= (TYPE op)   
      { CResGuard::CGuard x(m_rg); return(SetVal(GetVal() |  op)); }
   TYPE operator <<=(TYPE op)   
      { CResGuard::CGuard x(m_rg); return(SetVal(GetVal() << op)); }
   TYPE operator >>=(TYPE op)   
      { CResGuard::CGuard x(m_rg); return(SetVal(GetVal() >> op)); }
};


///////////////////////////////////////////////////////////////////////////////

   
// Instances of this class will be accessed by multiple threads. So, 
// all members of this class (except the constructor and destructor) 
// must be thread-safe.
template <class TYPE> 
class CWhenZero : public CInterlockedScalar<TYPE> {
public:
   CWhenZero(TYPE TVal = 0, BOOL fManualReset = TRUE) 
      : CInterlockedScalar<TYPE>(TVal) {

      // The event should be signaled if TVal is 0
      m_hevtZero = CreateEvent(NULL, fManualReset, (TVal == 0), NULL);

      // The event should be signaled if TVal is NOT 0
      m_hevtNotZero = CreateEvent(NULL, fManualReset, (TVal != 0), NULL);
   }

   ~CWhenZero() {
      CloseHandle(m_hevtZero);
      CloseHandle(m_hevtNotZero);
   }

   // C++ does not allow operator= to be inherited.
   TYPE operator=(TYPE x) { 
      return(CInterlockedScalar<TYPE>::operator=(x)); 
   }

   // Return handle to event signaled when value is zero
   operator HANDLE() const { return(m_hevtZero); }

   // Return handle to event signaled when value is not zero
   HANDLE GetNotZeroHandle() const { return(m_hevtNotZero); }

   // C++ does not allow operator cast to be inherited.
   operator TYPE() const { 
      return(CInterlockedScalar<TYPE>::operator TYPE()); 
   }

protected:
   void OnValChanged(const TYPE& TNewVal, const TYPE& TPrevVal) const { 
      // For best performance, avoid jumping to 
      // kernel mode if we don't have to
      if ((TNewVal == 0) && (TPrevVal != 0)) {
         SetEvent(m_hevtZero);
         ResetEvent(m_hevtNotZero);
      }
      if ((TNewVal != 0) && (TPrevVal == 0)) {
         ResetEvent(m_hevtZero);
         SetEvent(m_hevtNotZero);
      }
   }

private:
   HANDLE m_hevtZero;      // Signaled when data value is 0
   HANDLE m_hevtNotZero;   // Signaled when data value is not 0
};


//////////////////////////////// End of File //////////////////////////////////

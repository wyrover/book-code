////////////////////////////////////////////////////////////////////////////////
//      
//      Kingsoft File for kiscomm
//      
//      File      : kis_lock.h
//      Version   : 1.0
//      Comment   : Ò»°ãËø¶¨²Ù×÷
//      
//      Create at : 2008-3-5
//      Create by : liupeng
//      
////////////////////////////////////////////////////////////////////////////////

#pragma once 

namespace kis
{
class KLock
{
private:
	CRITICAL_SECTION  sec;
private:
	KLock(const KLock& locker);

public:
	KLock()
	{
		memset(&sec, 0, sizeof(CRITICAL_SECTION));
		::InitializeCriticalSection(&sec);
	}

	~KLock()
	{
		::DeleteCriticalSection(&sec);
	}

	void	Lock()
	{
		EnterCriticalSection(&sec);
	}

	void	Unlock()
	{
		LeaveCriticalSection(&sec);
	}

	
	BOOL TryLock()
	{
		return ::TryEnterCriticalSection(&sec);
	}
};

template< class T>
class KLockerT
{
private:
	T&		m_lock;

public:
	KLockerT(T& lock): m_lock(lock)
	{
		lock.Lock();
	}
	~KLockerT()
	{
		m_lock.Unlock();
	}
};

typedef KLockerT< KLock >	KLocker;

template< class T>
class KPtrLockerT
{
private:
	T*		m_lock;

public:
	KPtrLockerT(T* lock): m_lock(lock)
	{
		if (m_lock)
			m_lock->Lock();
	}
	~KPtrLockerT()
	{
		if (m_lock)
			m_lock->Unlock();
	}
};

typedef KPtrLockerT< KLock >	KPtrLokcer;
}

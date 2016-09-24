#ifndef SafeLocker_h__
#define SafeLocker_h__

class OSafeLocker
{
private:
	CRITICAL_SECTION m_csSect;
	int m_nBlocked;
public:
	OSafeLocker()
	{
		InitializeCriticalSection(&m_csSect);
		m_nBlocked = 0;
	}
	~OSafeLocker()
	{
		DeleteCriticalSection(&m_csSect);
	}

	int Lock()
	{
#ifdef _DEBUG
		InterlockedIncrement((long*)&m_nBlocked);
#endif // _DEBUG
		EnterCriticalSection(&m_csSect);
		return m_nBlocked;
	}
	int Unlock()
	{
		LeaveCriticalSection(&m_csSect);
#ifdef _DEBUG
		InterlockedDecrement((long*)&m_nBlocked);
#endif // _DEBUG
		return m_nBlocked;
	}
};

#endif // SafeLocker_h__


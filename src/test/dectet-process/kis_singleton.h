////////////////////////////////////////////////////////////////////////////////
//      
//      Kingsoft File for kiscomm
//      
//      File      : kis_singletem.h
//      Version   : 1.0
//      Comment   : 定义一个线程安全的单例，避免多个线程同时初始化
//      
//      Create at : 2008-3-5
//      Create by : liupeng
//      
////////////////////////////////////////////////////////////////////////////////
#pragma once

template<class T>
class KThreadSafeSigletem
{
public:
	_declspec(noinline) static T& Instance()
	{
		static T* _pInst = NULL;
		static volatile LONG _lock = 0;
		while (!_pInst)
		{
			if (::InterlockedIncrement(&_lock) == 1)
			{
				_pInst = CreateInstance();
				::InterlockedDecrement(&_lock);
			}
			else
			{
				::InterlockedDecrement(&_lock);
				::Sleep(10);
			}
		}
		return *_pInst;
	}

private:
	_declspec(noinline) static T* CreateInstance()
	{
		static T _inst;
		return &_inst;
	}
};
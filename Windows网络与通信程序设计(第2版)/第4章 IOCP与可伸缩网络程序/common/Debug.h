////////////////////////////////////////////
// Debug.h文件

// 调试支持


#ifndef __DEBUG_H__
#define __DEBUG_H__


#ifdef _DEBUG

	#define ODS(szOut)					\
	{									\
		OutputDebugString(szOut);		\
	}

	#define ODS1(szOut, var)			\
	{									\
		TCHAR sz[1024];				    \
		_stprintf(sz, szOut, var);		\
		OutputDebugString(sz);			\
	}

#else

	#define ODS(szOut)
	#define ODS1(szOut, var)	

#endif	// _DEBUG

#endif // __DEBUG_H__
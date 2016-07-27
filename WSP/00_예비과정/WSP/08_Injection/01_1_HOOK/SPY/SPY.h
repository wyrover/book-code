// SPY.h

#ifdef DLL_SOURCE
	#define DLLAPI extern "C" __declspec( dllexport)

#else
	#define DLLAPI extern "C" __declspec(dllimport)
#endif

// 2개의 함수를 export
DLLAPI void Install();
DLLAPI void Uninstall();



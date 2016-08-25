#ifdef DLL1_API
#else
#define DLL1_API extern "C" _declspec(dllimport)
#endif

DLL1_API int _stdcall add(int a,int b);
DLL1_API int _stdcall subtract(int a,int b);

/*class  Point
{
public:
	void DLL1_API output(int x,int y);
	void test();
};*/

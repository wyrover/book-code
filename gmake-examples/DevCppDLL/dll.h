#ifndef _DLL_H_
#define _DLL_H_

#if BUILDING_DLL
# define DLLIMPORT __declspec (dllexport)
#else /* Not BUILDING_DLL */
# define DLLIMPORT __declspec (dllimport)
#endif /* Not BUILDING_DLL */



class DLLIMPORT DllClass
{
  public:
    DllClass();
    virtual ~DllClass(void);

  private:

};

struct IXyz
{
    virtual int Foo(int n) = 0;
    virtual void Release() = 0;
};




#endif /* _DLL_H_ */

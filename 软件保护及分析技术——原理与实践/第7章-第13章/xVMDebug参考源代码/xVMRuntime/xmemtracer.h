#ifndef memtracer_h__
#define memtracer_h__

#include "../../../nCom/nstate.h"


class xmemtracer
{
	typedef nstate<int,int> memst;
public:
	xmemtracer();
	~xmemtracer();
	int set(int oft,int size,int state);
	int unset(int oft,int size);
	int hit(int& oft,int size,int& state);
private:
	memst	m_st_ctx;
};

#endif // memtracer_h__



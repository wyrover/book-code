#include "xmemtracer.h"


xmemtracer::xmemtracer()
{
}


xmemtracer::~xmemtracer()
{
}

int xmemtracer::set(int oft,int size,int state)
{
	memst::s_node* node = m_st_ctx.set(oft,size,state,false);
	if (node)
		return node->addr;
	return -1;
}

int xmemtracer::unset(int oft,int size)
{
	return m_st_ctx.unset(oft,size);
}

int xmemtracer::hit(int& oft,int size,int& state)
{
	memst::s_node* node = m_st_ctx.hit(oft,size);
	if (node)
	{
		oft = node->addr;
		state = node->state;
		return node->size;
	}
	return 0;
}




void* xst_allocstate()
{
	return new xmemtracer;
}

int xst_set(void* lpst,int oft,int size,int state)
{
	return ((xmemtracer*)lpst)->set(oft,size,state);
}


int xst_unset(void* lpst,int oft,int size)
{
	return ((xmemtracer*)lpst)->unset(oft,size);
}

int xst_hit(void* lpst,int* oft,int size,int* state)
{
	xmemtracer* mt = reinterpret_cast<xmemtracer*>(lpst);
	int oftv = *oft;
	int statev;
	int vsz = mt->hit(oftv,size,statev);
	if (vsz > 0)
	{
		*oft = oftv;
		if (state)*state = statev;
		return vsz;
	}
	return 0;
}

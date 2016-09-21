#ifdef _MSC_VER
#   if _MSC_VER > 7
#       define _CRT_SECURE_NO_DEPRECATE 1
#       define _SCL_SECURE_NO_DEPRECATE 1
#       define _CRT_NONSTDC_NO_DEPRECATE 1
#   endif
#endif

#include <string.h>
#include "luaargs.h"

//------------------------------------------------------------------------------
template<class T>
class Stack {
public:
	class Node {
		T el;
		Node *prev;
		Node();
	public:
		Node(T El, Node *Prev);
		Node *Prev();
		T Get();
	};

private:
	Node *current;

public:
	Stack();
	~Stack();
	T push(T el);
	T pop();
};

struct Refs {
	Stack<const char*> strings;
	Stack<int> indices;
};

//------------------------------------------------------------------------------
template<class T>
Stack<T>::Node::Node(T El, Node *Prev)
	: prev(Prev), el(El)
{ }

template<class T>
inline typename Stack<T>::Node *Stack<T>::Node::Prev()
{
	return prev;
}

template<typename T>
inline T Stack<T>::Node::Get()
{
	return el;
}

//------------------------------------------------------------------------------
template<class T>
Stack<T>::Stack()
	: current(0)
{ }

template<class T>
Stack<T>::~Stack()
{
	while (current) {
		typename Stack<T>::Node *p = current;
		current = current->Prev();
		delete p;
	}
}

template<typename T>
T Stack<T>::push(T el)
{
	current = new typename Stack<T>::Node(el, current);
	return el;
}

template<typename T>
T Stack<T>::pop()
{
	if (current) {
		typename Stack<T>::Node *p = current;
		current = current->Prev();
		T el = p->Get();
		delete p;
		return el;
	} else {
		return 0;
	}
}

//------------------------------------------------------------------------------
LuaArgs::LuaArgs(lua_State *ls)
	:
	L      (ls),
	tbl    (lua_istable(L, 1)? 1 : 0),
	origin (lua_gettop(L)),
	refs   (new Refs)
{ }

LuaArgs::~LuaArgs()
{
	while (const char *p = refs->strings.pop())
		delete[] p;
	while (int ref = refs->indices.pop())
		releaseF(L, ref);
	delete refs;
	refs = 0;
}

int LuaArgs::count()
{
	int ds = lua_gettop(L) - origin;
	if (ds < 0) {
		char err[64];
		sprintf(err, "Stack exhaustion : diff = %i\n", ds);
		lua_pushstring(L, err);
		lua_error(L);
	}
	return ds;
}

const char *LuaArgs::scopy(int idx)
{
	size_t len;
	const char *src = lua_tolstring(L, idx, &len);
	char *ret = new char[len+1];
	ret[len] = '\0';
	return strncpy(ret, src, len);
}

const char *LuaArgs::scopy(const char *src)
{
	size_t len = strlen(src);
	char *ret = new char[len+1];
	ret[len] = '\0';
	return strncpy(ret, src, len);
}

const char *LuaArgs::sref(int idx)
{
	return refs->strings.push(scopy(idx));
}

const char *LuaArgs::sref(const char *src)
{
	return refs->strings.push(scopy(src));
}

const char *LuaArgs::gets(int idx, const char *Default)
{
	return lua_isstring(L, idx)?
		sref(idx) :
		sref(Default);
}

const char *LuaArgs::getS(int idx, const char *Default)
{
	return lua_isstring(L, idx)?
		scopy(idx) :
		scopy(Default);
}

const char *LuaArgs::gets(int idx, const char *key, const char *Default)
{
	if (tbl > 0 && lua_checkstack(L, 1)) {
		lua_getfield(L, tbl, key);
		bool success = lua_isstring(L, -1) != 0;
		if (!success && idx > 0) {
			lua_pop(L, 1);
			lua_pushinteger(L, idx);
			lua_gettable(L, tbl);
			success = lua_isstring(L, -1) != 0;
		}
		if (success)
			Default = sref(-1);
		lua_pop(L, 1);
		return Default;
	} else {
		return gets(idx, Default);
	}
}

const char *LuaArgs::getS(int idx, const char *key, const char *Default)
{
	if (tbl > 0 && lua_checkstack(L, 1)) {
		lua_getfield(L, tbl, key);
		bool success = lua_isstring(L, -1) != 0;
		if (!success && idx > 0) {
			lua_pop(L, 1);
			lua_pushinteger(L, idx);
			lua_gettable(L, tbl);
			success = lua_isstring(L, -1) != 0;
		}
		Default = success? scopy(-1) : scopy(Default);
		lua_pop(L, 1);
		return Default;
	} else {
		return getS(idx, Default);
	}
}

int LuaArgs::geti(int idx, int Default)
{
	return lua_isnumber(L, idx)?
		lua_tointeger(L, idx):
		Default;
}

int LuaArgs::geti(int idx, const char *key, int Default)
{
	int ret;
	if (tbl > 0 && lua_checkstack(L, 1)) {
		lua_getfield(L, tbl, key);
		bool success = lua_isnumber(L, -1) != 0;
		if (!success && idx > 0) {
			lua_pop(L, 1);
			lua_pushinteger(L, idx);
			lua_gettable(L, tbl);
			success = lua_isnumber(L, -1) != 0;
		}
		ret = success? lua_tointeger(L, -1) : Default;
		lua_pop(L, 1);
	} else {
		ret = geti(idx, Default);
	}
	return ret;
}

int findopt(const char *opt, const char **list, const char *Default)
{
	int i = 0;
	for (const char **p = list; *p; p++, i++)
		if (strcmp(*p, opt) == 0)
			return i;
	i = 0;
	for (const char **p = list; *p; p++, i++)
		if (strcmp(*p, Default) == 0)
			return i;
	return -1;
}

int LuaArgs::getp(int idx, const char **list, const char *Default)
{
	const char *opt = getS(idx, "");
	int ret = findopt(opt, list, Default);
	delete[] opt;
	return ret;
}

int LuaArgs::getp(int idx, const char *key, const char **list, const char *Default)
{
	const char *opt = getS(idx, key, "");
	int ret = findopt(opt, list, Default);
	delete[] opt;
	return ret;
}

double LuaArgs::getd(int idx, double Default)
{
	return lua_isnumber(L, idx)?
		lua_tonumber(L, idx) :
		Default;
}

double LuaArgs::getd(int idx, const char *key, double Default)
{
	double ret;
	if (tbl > 0 && lua_checkstack(L, 1)) {
		lua_getfield(L, tbl, key);
		bool success = lua_isnumber(L, -1) != 0;
		if (!success && idx > 0) {
			lua_pop(L, 1);
			lua_pushinteger(L, idx);
			lua_gettable(L, tbl);
			success = lua_isnumber(L, -1) != 0;
		}
		ret = success? lua_tonumber(L, -1) : Default;
		lua_pop(L, 1);
	} else {
		ret = getd(idx, Default);
	}
	return ret;
}

bool LuaArgs::getb(int idx, bool Default)
{
	return lua_isboolean(L, idx)?
		lua_toboolean(L, idx) != 0 :
		Default;
}

bool LuaArgs::getb(int idx, const char *key, bool Default)
{
	bool ret;
	if (tbl > 0 && lua_checkstack(L, 1)) {
		lua_getfield(L, tbl, key);
		bool success = lua_isboolean(L, -1) != 0;
		if (!success && idx > 0) {
			lua_pop(L, 1);
			lua_pushinteger(L, idx);
			lua_gettable(L, tbl);
			success = lua_isboolean(L, -1) != 0;
		}
		ret = success? lua_toboolean(L, -1) != 0 : Default;
		lua_pop(L, 1);
	} else {
		ret = getb(idx, Default);
	}
	return ret;
}

int LuaArgs::fcopy(int idx)
{
	int ret = 0;
	if (lua_isfunction(L, idx)) {
		lua_pushvalue(L, idx);
		ret = luaL_ref(L, LUA_REGISTRYINDEX);
	}
	return ret;
}

int LuaArgs::fref(int idx)
{
	return refs->indices.push(fcopy(idx));
}

int LuaArgs::getf(int idx)
{
	return fref(idx);
}

int LuaArgs::getF(int idx)
{
	return fcopy(idx);
}

int LuaArgs::getf(int idx, const char *key)
{
	if (tbl > 0 && lua_checkstack(L, 1)) {
		lua_getfield(L, tbl, key);
		if (!lua_isfunction(L, -1) && idx > 0) {
			lua_pop(L, 1);
			lua_pushinteger(L, idx);
			lua_gettable(L, tbl);
		}
		int ret = fref(-1);
		lua_pop(L, 1);
		return ret;
	} else {
		return getf(idx);
	}
}

int LuaArgs::getF(int idx, const char *key)
{
	if (tbl > 0 && lua_checkstack(L, 1)) {
		lua_getfield(L, tbl, key);
		if (!lua_isfunction(L, -1) && idx > 0) {
			lua_pop(L, 1);
			lua_pushinteger(L, idx);
			lua_gettable(L, tbl);
		}
		int ret = fcopy(-1);
		lua_pop(L, 1);
		return ret;
	} else {
		return getF(idx);
	}
}

void LuaArgs::releaseF(lua_State *L, int ref)
{
	luaL_unref(L, LUA_REGISTRYINDEX, ref);
}

#ifndef __CACTUS_UTILS_H__
#define __CACTUS_UTILS_H__

#include <Windows.h>
#include <tchar.h>
#include <WinCrypt.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include <boost/unordered_map.hpp>  

#define ARRAY_SIZE(A) (sizeof(A)/sizeof(*A))
typedef std::basic_string<TCHAR> tstring;



#include "basic.inl"

void hello1();

LPWSTR strdupW(LPCWSTR str);

BOOL XCOPY_CreateDirectory(const WCHAR* path);

LPWSTR GetRegValue(HKEY service_key, const WCHAR *value_name);

DWORD load_reg_string(HKEY hKey, LPCWSTR szValue, BOOL bExpand, LPWSTR *output);
DWORD load_reg_multisz(HKEY hKey, LPCWSTR szValue, BOOL bAllowSingle, LPWSTR *output);
DWORD load_reg_dword(HKEY hKey, LPCWSTR szValue, DWORD *output);

WCHAR *reg_data_to_wchar(DWORD type, const BYTE *src, DWORD size_bytes);

LPWSTR ExpandEnv(LPWSTR string);

LPWSTR get_parent_dir(WCHAR* path);

void output(const WCHAR *message);



////////////////////////////////////////////////////////////////////////
template<typename key_t, typename value_t>
class LruCache
{
public:
    LruCache(size_t maxSize)
        : maxSize(maxSize)
    {
    }

    void insert_or_assign(const key_t & key, const value_t & val)
    {
        ItemsMap::iterator mapIt = itemsMap.find(key);
        if (mapIt == itemsMap.end())
        {
            evict(maxSize - 1);

            ItemsList::iterator listIt = itemsList.insert(itemsList.cend(), ListItem(key, val));
            itemsMap.insert(std::make_pair(key, listIt));
        }
        else
        {
            mapIt->second->val = val;
        }
    }

    const value_t * try_get(const key_t & key)
    {
        ItemsMap::const_iterator it = itemsMap.find(key);
        if (it == itemsMap.end())
            return nullptr;

        // Move last recently accessed item to the end.
        if (it->second != itemsList.end())
        {
            itemsList.splice(itemsList.end(), itemsList, it->second);
        }

        return &it->second->val;
    }

    void reserve(size_t size)
    {
        itemsMap.reserve(min(maxSize, size));
    }

    void clear()
    {
        itemsMap.clear();
        itemsList.clear();
    }
protected:
    void evict(size_t itemsToKeep)
    {
        for(ItemsList::iterator it = itemsList.begin();
            itemsList.size() > itemsToKeep && it != itemsList.end();)
        {
            itemsMap.erase(it->key);
            it = itemsList.erase(it);
        }
    }
private:
    struct ListItem
    {
        ListItem(const key_t & key, const value_t & val)
            : key(key), val(val)
        {
        }

        key_t key;
        value_t val;
    };

    typedef std::list<ListItem> ItemsList;
    typedef boost::unordered_map<key_t, typename ItemsList::iterator> ItemsMap;

    size_t maxSize;
    ItemsMap itemsMap;
    ItemsList itemsList;
};
////////////////////////////////////////////////////////////////////////

#endif // __CACTUS_UTILS_H__

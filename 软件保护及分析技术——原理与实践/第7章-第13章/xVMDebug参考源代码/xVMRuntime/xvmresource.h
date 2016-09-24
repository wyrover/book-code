#ifndef XVMRESOURCE_H
#define XVMRESOURCE_H

#include <windows.h>
#include <vector>
#include <list>


static long mem_hash(const char *s, int len)
{
    // Copied from <xhash>
    long _Val = 2166136261U;
    for (int i=0;i<len;i++)
        _Val = 16777619U * _Val ^ (long)s[i];
    return _Val;
}


struct ResourceNodeName
{
    bool                m_NameIsString;     //资源节点是否为名称
    int                 m_id;               //资源节点ID
    std::wstring        m_sName;            //资源节点名称
    bool                selected;
    bool operator ==(const ResourceNodeName& rsname) const
    {
        if (m_NameIsString == rsname.m_NameIsString)
        {
            if (m_NameIsString)
            {
                if (wcsicmp(m_sName.c_str(),rsname.m_sName.c_str()) == 0)
                    return true;
            }else
                if (m_id == rsname.m_id) return true;
        }
        return false;
    }

    DWORD   hashVal() const
    {
        DWORD hv = 0;
        if (m_NameIsString)
        {
            hv +=  mem_hash((const char*)m_sName.data(),m_sName.size()*sizeof(wchar_t));
        }else
            hv = 1 + m_id;
        return hv;
    }

    bool operator <(const ResourceNodeName& rsname) const
    {
        return hashVal() < rsname.hashVal();
    }
};
//用list来描述资源路径
typedef std::list<ResourceNodeName>     ResourcePath;
//资源数据
struct ResourceLeaf
{
    ResourcePath        m_ResPath;      //资源路径
    DWORD               m_CodePage;     //资源代码页
    std::vector<char>   m_data;         //资源数据
    //bool operator ==(const ResourceLeaf& leaf) const;
    DWORD   hashVal() const
    {
        DWORD hv = m_ResPath.size()+m_CodePage+m_data.size();
        for (ResourcePath::const_iterator itr = m_ResPath.begin();itr != m_ResPath.end();itr++)
        {
            const ResourceNodeName* rn = &*itr;
            hv += rn->hashVal();
        }
        if (m_data.size() > 0)
            hv += mem_hash(&m_data[0],m_data.size());
        return hv;
    }

    bool operator <(const ResourceLeaf& leaf) const
    {
        return (hashVal() < leaf.hashVal());
    }
};
typedef std::list<ResourceLeaf>     ResourceNodes;


void FollowResourceLeaf(ResourcePath& pResPath,
                        ResourceNodes& resNodes,
                        const char* lpImage,
                        const char *pResPtr,
                        IMAGE_RESOURCE_DIRECTORY* pResHdr);



#endif // XVMRESOURCE_H

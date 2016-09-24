#include "xvmresource.h"



void FollowResourceLeaf(ResourcePath& pResPath,ResourceNodes& resNodes,const char* lpImage,const char *pResPtr,IMAGE_RESOURCE_DIRECTORY* pResHdr)
{
    IMAGE_RESOURCE_DIRECTORY_ENTRY* pDirEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)((const char*)pResHdr + sizeof(IMAGE_RESOURCE_DIRECTORY));
    int pnEntry = pResHdr->NumberOfNamedEntries + pResHdr->NumberOfIdEntries;
    for (int i=0;i<pnEntry;i++,pDirEntry++)
    {
        ResourceNodeName pResName;

        pResName.m_NameIsString = pDirEntry->NameIsString;
        if (pDirEntry->NameIsString)
        {
            IMAGE_RESOURCE_DIRECTORY_STRING* pdStr = (IMAGE_RESOURCE_DIRECTORY_STRING*)(pResPtr+pDirEntry->NameOffset);
            pResName.m_sName.clear();
            pResName.m_sName.append((const wchar_t*)(pdStr->NameString),pdStr->Length);
        }
        else
            pResName.m_id = pDirEntry->Id;
        pResPath.push_back(pResName);

        if (pDirEntry->DataIsDirectory)
        {
            IMAGE_RESOURCE_DIRECTORY* pSubDir = (IMAGE_RESOURCE_DIRECTORY*)(pResPtr + pDirEntry->OffsetToDirectory);
            FollowResourceLeaf(pResPath,resNodes,lpImage,pResPtr,pSubDir);
        }else
        {
            ResourceLeaf resLeaf;
            IMAGE_RESOURCE_DATA_ENTRY* pDataEntry = (IMAGE_RESOURCE_DATA_ENTRY*)(pResPtr + pDirEntry->OffsetToData);
            resLeaf.m_ResPath = pResPath;
            resLeaf.m_CodePage = pDataEntry->CodePage;
            resLeaf.m_data.resize(pDataEntry->Size);
            memcpy(&resLeaf.m_data[0],lpImage+ pDataEntry->OffsetToData,pDataEntry->Size);
            resNodes.push_back(resLeaf);
        }
        pResPath.pop_back();
    }
}



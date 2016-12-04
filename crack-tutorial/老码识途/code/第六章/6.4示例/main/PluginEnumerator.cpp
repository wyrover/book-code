#include "PluginEnumerator.h"
#include <windows.h>

const std::string PLUGIN_PATH = "Plugin";

CPluginEnumerator::CPluginEnumerator(void)
{
}

CPluginEnumerator::~CPluginEnumerator(void)
{
}

bool CPluginEnumerator::GetPluginNames(std::vector<std::string>& vstrPluginNames)
{
	WIN32_FIND_DATA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

	std::string strDirSpec = PLUGIN_PATH;
	strDirSpec += "//*";

	hFind = ::FindFirstFile(strDirSpec.c_str(), &FindFileData);

    if(hFind == INVALID_HANDLE_VALUE)
    {
		return false;
    }

	do 
	{
		int length = strlen(FindFileData.cFileName);

		if((FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) || (length < 4))
		{
			continue;
		}

		if(!((FindFileData.cFileName[length-1] == 'l') || (FindFileData.cFileName[length-1] == 'L')))
			continue;

		if(!((FindFileData.cFileName[length-2] == 'l') || (FindFileData.cFileName[length-2] == 'L')))
			continue;

		if(!((FindFileData.cFileName[length-3] == 'd') || (FindFileData.cFileName[length-3] == 'D')))
			continue;

		if(!(FindFileData.cFileName[length-4] == '.'))
			continue;

		std::string strName = PLUGIN_PATH;
		strName += "\\";

		strName += FindFileData.cFileName;

		vstrPluginNames.push_back(strName);

	} while(::FindNextFile(hFind, &FindFileData));

	::FindClose(hFind);

	return true;
}

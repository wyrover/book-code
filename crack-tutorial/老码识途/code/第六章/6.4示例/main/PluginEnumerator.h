#pragma once

#include <vector>
#include <string>

class CPluginEnumerator
{
public:
	CPluginEnumerator(void);
	virtual ~CPluginEnumerator(void);

	bool GetPluginNames(std::vector<std::string>& vstrPluginNames);
};

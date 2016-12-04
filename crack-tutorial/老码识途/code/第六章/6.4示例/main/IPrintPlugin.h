#pragma once

class IPrintPlugin
{
public:
	IPrintPlugin();
	virtual ~IPrintPlugin();

	virtual void Help() = 0;
	virtual void Print() = 0;
	virtual int GetID() = 0;
};

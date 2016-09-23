#pragma once

class CDriverManager
{
public:
	CDriverManager(void);
	~CDriverManager(void);
	static BOOL LoadNTDriver(PTCHAR lpszDriverName, PTCHAR lpszDriverPath);  // 装载NT驱动程序
	static BOOL UnloadNTDriver(PTCHAR szSvrName);  // 卸载NT驱动程序    
};

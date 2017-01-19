/*!
* \file CProcessDetect.h
* \brief 游戏进程退出检测
*
* 游戏进程退出检测, 初始化类后设置回调函数
*
* example:
*	CProcessDetect::Instance().Init(Notify);
*
* \author wangyang
* \date 2016/09/06
* \version 1.0
*/


#ifndef __CPROCESSDETECT_H__
#define __CPROCESSDETECT_H__

#include <windows.h>
#include <vector>
#include <map>
#include "kis_singleton.h"
#include "kis_lock.h"
#include "KSysProcess.h"

typedef VOID (WINAPI *NotifyExitProcess)(const char* process_name);

namespace pnpsub
{

class CProcessDetect : public KThreadSafeSigletem<CProcessDetect>,
	public IKSysProcessObserver
{
public:
	CProcessDetect();
	virtual ~CProcessDetect();

	void Init(NotifyExitProcess proc);

private:

	HRESULT __stdcall NotifyProcessChange(
        const MAP_PROCID_2_INFO&    mapNewProc,
        const MAP_PROCID_2_INFO&    mapEndProc
        );	
	
	void FastGetProcesses();		
	NotifyExitProcess callback_proc_;

	kis::KLock read_lck_;
	MAP_PROCID_2_INFO process_list_;
	KSysProcess sysprocess_;
};
 
} // namespace pnpsub

#endif // __CPROCESSDETECT_H__
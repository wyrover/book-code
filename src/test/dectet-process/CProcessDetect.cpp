#include "stdafx.h"
#include "CProcessDetect.h"
//#include "GamerUtils.h"

namespace pnpsub
{


CStringA convert_to(const CStringW& from)
{
	int len = from.GetLength();
	int required_size = WideCharToMultiByte(CP_ACP, 0, LPCWSTR(from), (DWORD)len, NULL, 0, NULL, NULL);

	if (required_size == 0)
		return "";

	std::vector<char> buf(++required_size);
	int ret = WideCharToMultiByte(CP_ACP, 0, LPCWSTR(from), len, &buf[0], required_size, NULL, NULL);

	if (ret == 0)
		return "";
	else
		return CStringA(&buf[0]);
}




CProcessDetect::CProcessDetect()
{
    FastGetProcesses();
    KSysProcessMonitor::Instance().RegisterObserver(this);
}

CProcessDetect::~CProcessDetect()
{
    KSysProcessMonitor::Instance().UnregisterObserver(this);

    if (process_list_.size() > 0) {
        process_list_.clear();
    }
}

void CProcessDetect::Init( NotifyExitProcess proc )
{
	if (proc) {
		callback_proc_ = proc;
	}
}

void CProcessDetect::FastGetProcesses()
{
    std::vector<int> vecCurrPID;
    BOOL bRet = sysprocess_.GetAllProcess(vecCurrPID);

    for (int i = 0; i != vecCurrPID.size(); ++i) {
        S_PROC_INFO procInfo;
        procInfo.nProcessId = vecCurrPID[i];
        procInfo.wstrExePath = static_cast<LPCWSTR>(sysprocess_.GetProcessPath(vecCurrPID[i]));
        process_list_.insert(std::make_pair(vecCurrPID[i], procInfo));
    }
}


HRESULT CProcessDetect::NotifyProcessChange(
    const MAP_PROCID_2_INFO&    mapNewProc,
    const MAP_PROCID_2_INFO&    mapEndProc
)
{
    kis::KLocker _locker(read_lck_);
    MAP_PROCID_2_INFO::iterator itfind;

    for (MAP_PROCID_2_INFO::const_iterator iter = mapNewProc.begin();
         iter != mapNewProc.end();
         ++iter) {
        process_list_.insert(std::make_pair(iter->first, iter->second));
    }

    if ((mapEndProc.size() > 0) && (process_list_.size() > 0)) {
        for (MAP_PROCID_2_INFO::const_iterator iter = mapEndProc.begin();
             iter != mapEndProc.end();
             ++iter) {
            S_PROC_INFO info = iter->second;

			CStringA exe_path = convert_to(CStringW(info.wstrExePath.c_str()));            

            itfind = process_list_.find(iter->first);

            if (process_list_.end() != itfind) {
                process_list_.erase(itfind);
            }

			//if (CmGamerHelper::Instance().IsGameProcess(exe_path)) {
				if (callback_proc_) {
					callback_proc_((LPCSTR)exe_path);
				}
			//}
        }
    }

    return S_OK;
}

}











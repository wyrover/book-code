
// 开启关闭还原点
// http://www.sevenforums.com/tutorials/81500-system-restore-enable-disable.html

// 改变还原点大小空间
// http://www.sevenforums.com/tutorials/335-system-protection-change-disk-space-usage.html
// 删除还原点
// http://www.sevenforums.com/tutorials/166102-shadow-copies-delete.html

// http://nicbedford.co.uk/software/systemrestoreexplorer/

// power-shell 枚举还原点
// https://superuser.com/questions/461592/how-can-i-list-all-system-restore-points

// api 创建还原点
// https://msdn.microsoft.com/en-us/library/aa378987


// https://github.com/MangoDango/Volume-Shadow-Copy-Comparison-Tool

// https://github.com/sblosser/pyshadowcopy

// https://github.com/itjc/vscsc

// https://github.com/nicbedford/SystemRestoreExplorer

// 自动还原点创建

// http://www.eightforums.com/tutorials/33910-restore-point-automatic-creation-disable-windows.html

#ifndef restore_point_h__
#define restore_point_h__

#include <Windows.h>
#include <comdef.h>
#include <wbemidl.h>
#include <string>

namespace cactus
{
typedef struct _VOLUME_INFO {
    std::wstring driveLetter;
    std::wstring driveType;
    std::wstring capacity;
    std::wstring freeSpace;
    std::wstring label;
} VOLUME_INFO, *PVOLUME_INFO;


class CSnapshotManager
{
public:
    CSnapshotManager();
    virtual ~CSnapshotManager();
public:
    BOOL init();
    BOOL listSnapshotList();
    BOOL listVolumes();
    BOOL createSnapshot(std::wstring volume);
    BOOL deleteSnapShot(std::wstring shadow_id);

private:
    HRESULT result_;
    IWbemLocator* loc_;
    IWbemServices* svc_;
    IEnumWbemClassObject* pEnum_;
    IWbemClassObject* pls_;

};
}


#endif // restore_point_h__
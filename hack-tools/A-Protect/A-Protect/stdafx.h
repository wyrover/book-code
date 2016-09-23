
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#define NOHOOK 0;
#define SSDTHOOK 1
#define SSDTINLINEHOOK 2
#define INLINEHOOK 3
#define WM_SHOWTASK (WM_USER +1)

/*
* 鹰眼分析
*/
#define EYEOPEN 1
#define EYECLOSE 2
#define EYEUPDATE 3
#define EYEUNKNOWN 0

/*
 * 初始化
 */
#define SAFE_SYSTEM 0x18881111


#define LIST_SSDT   0x18881112                                //列举被挂钩的SSDT函数
#define LIST_SSDT_ALL    0x18881113                           //列举所有SSDT函数  
#define SET_ALL_SSDT 0x18881114                               //恢复所有SSDT函数HOOK
#define SET_ONE_SSDT 0x18881115                               //恢复单个SSDT函数hook
#define SET_INLINE_HOOK 0x18881116                            //恢复单个SSDT inline hook


#define LIST_TCPUDP 0x18881117                                //列举所有网络连接



#define KILL_PROCESS_BY_PID 0x18881118                        //从进程pid结束一个进程
#define LIST_PROCESS 0x18881119                               //列举所有进程
#define DELETE_FILE 0x18881120                                //删除一个文件


#define LIST_PROCESS_MODULE 0x18881121                        //列举进程DLL模块
#define INIT_PROCESS_LIST_PROCESS_MODULE 0x18881122           //在列举进程DLL模块的时候，必须要初始化某些参数


#define LIST_INLINEHOOK 0x18881123                            //枚举ntos*的hook，包括inline hook，eat hook
#define ANTI_INLINEHOOK 0x18881124                            //绕过ntos* inline hook


#define KERNEL_SAFE_MODULE 0x18881125                         //开启内核安全模式
#define NO_KERNEL_SAFE_MODULE 0x18881126                      //关闭内核安全模式


#define LIST_SYS_MODULE 0x18881127                           //列举内核模块

#define EXIT_PROCESS 0x18881128                              //退出AProtect的时候，必须要对内核做清理的命令


#define LIST_SERVICES 0x18881129                             //列举系统服务
#define CHANG_SERVICES_TYPE_1 0x18881130                     //服务手动
#define CHANG_SERVICES_TYPE_2 0x18881131                     //服务自动
#define CHANG_SERVICES_TYPE_3 0x18881132                     //服务禁用
#define LIST_DEPTH_SERVICES 0x18881133                       //深度服务扫描

#define LIST_LOG 0x18881134                                  //列举防御日志

#define INIT_DUMP_KERNEL_MODULE_MEMORY 0x18881135            //初始化dump内核模块
#define DUMP_KERNEL_MODULE_MEMORY 0x18881136                 //dump内核模块内存到文件


#define DIS_CREATE_PROCESS      0x18881137                   //禁止创建进程
#define CREATE_PROCESS          0x18881138                   //允许创建进程

#define DIS_WRITE_FILE      0x18881139                       //禁止创建文件
#define WRITE_FILE      0x18881140                           //允许创建文件 


#define DIS_LOAD_DRIVER      0x18881141                      //禁止加载驱动
#define LOAD_DRIVER      0x18881142                          //允许加载驱动


#define SHUT_DOWN_SYSTEM      0x18881143                     //强制环保重启系统


#define LIST_SHADOWSSDT       0x18881144                     //列举被挂钩的shadowSSDT函数
#define LIST_SHADOWSSDT_ALL       0x18881145                 //列举所有shadowSSDT函数

#define SET_ALL_SHADOWSSDT 0x18881146                        //恢复所有shadowSSDT hook
#define SET_ONE_SHADOWSSDT  0x18881147                       //恢复单个shadowSSDT hook  

#define SET_SHADOWSSDT_INLINE_HOOK 0x18881148                //恢复单个shadowSSDT inline hook

#define LIST_OBJECT_HOOK     0x18881149                      //李傕object hook

#define PROTECT_360SAFE 0x18881150                           //是否保护360  （已去掉此功能）
#define UNPROTECT_360SAFE 0x18881151                         //

#define LIST_FSD_HOOK     0x18881152                         //列举所有NTFS FSD HOOK
#define SET_FSD_HOOK      0x18881153                         //恢复NTFS FSD hook
#define INIT_SET_FSD_HOOK  0x18881154                        //恢复NTFS FSD inline hook之前的初始化工作

#define CLEAR_LIST_LOG  0x18881155                           //清空防御日志

#define LIST_KERNEL_FILTER_DRIVER   0x18881156               //列举过滤驱动
#define DELETE_KERNEL_FILTER_DRIVER   0x18881157             //摘除过滤驱动
#define INIT_KERNEL_FILTER_DRIVER     0x18881158             //摘除过滤驱动之前的初始化工作

#define ONLY_DELETE_FILE  0x18881159                         //直接删除文件，不Reload

#define LIST_TCPIP_HOOK         0x18881160                   //列举所有Tcpip.sys模块hook
#define SET_TCPIP_HOOK      0x18881161                       //恢复Tcpip.sys模块 hook
#define INIT_SET_TCPIP_HOOK  0x18881162                      //恢复Tcpip.sys模块 inline hook之前的初始化工作

#define LIST_NSIPROXY_HOOK         0x18881163                //列举所有Nsiproxy.sys模块hook
#define SET_NSIPROXY_HOOK      0x18881164                    //恢复Nsiproxy.sys模块 hook
#define INIT_SET_NSIPROXY_HOOK  0x18881165                   //恢复Nsiproxy.sys模块 inline hook之前的初始化工作

#define LIST_SYSTEM_THREAD   0x18881166                      //列举系统线程
#define KILL_SYSTEM_THREAD   0x18881167                      //结束系统线程

#define PROTECT_DRIVER_FILE  0x18881168                      //保护驱动文件(已去掉)
#define UNPROTECT_DRIVER_FILE  0x18881169

#define LIST_KERNEL_THREAD   0x18881170                      //列举内核线程
#define CLEAR_KERNEL_THREAD   0x18881171                     //清除内核线程日志

#define SET_EAT_HOOK          0x18881172                     //恢复eat hook

#define PROTECT_PROCESS       0x18881173                     //保护AProtect自身进程
#define UNPROTECT_PROCESS       0x18881174                   //不保护

#define DIS_RESET_SRV 0x18881175                             //禁止服务回写
#define RESET_SRV 0x18881176                                 //允许服务回写

#define KERNEL_THREAD 0x18881177                             //允许创建内核线程
#define DIS_KERNEL_THREAD 0x18881178                         //禁止创建内核线程 

#define RESUME_THREAD    0x18881179                          //恢复线程运行
#define SUSPEND_THREAD    0x18881180                         //暂停线程运行

#define LIST_KBDCLASS_HOOK         0x18881181                //列举所有kbdclass.sys模块hook
#define SET_KBDCLASS_HOOK      0x18881182                    //恢复kbdclass.sys模块 hook
#define INIT_SET_KBDCLASS_HOOK  0x18881183                   //恢复kbdclass.sys模块 inline hook之前的初始化工作

#define LIST_MOUCLASS_HOOK         0x18881184                //列举所有Mouclass.sys模块hook
#define SET_MOUCLASS_HOOK      0x18881185                    //恢复Mouclass.sys模块 hook
#define INIT_SET_MOUCLASS_HOOK  0x18881186                   //恢复Mouclass.sys模块 inline hook之前的初始化工作

#define LIST_ATAPI_HOOK         0x18881187                   //列举所有Atapi.sys模块hook
#define SET_ATAPI_HOOK      0x18881188                       //恢复Atapi.sys模块 hook
#define INIT_SET_ATAPI_HOOK  0x18881189                      //恢复Atapi.sys模块 inline hook之前的初始化工作

#define LIST_DPC_TIMER    0x18881190                         //枚举DPC定时器
#define KILL_DPC_TIMER    0x18881191                         //摘除DPC定时器

#define LIST_SYSTEM_NOTIFY    0x18881192                     //枚举系统回调
#define KILL_SYSTEM_NOTIFY    0x18881193                     //摘除系统回调
#define INIT_KILL_SYSTEM_NOTIFY 0x18881194                   //初始化摘除

#define INIT_PROCESS_THREAD   0x18881195                     //初始化进程EPROCESS
#define LIST_PROCESS_THREAD   0x18881196                     //获取进程线程

#define LIST_START_UP         0x18881197                     //启动项


#define LIST_WORKQUEUE        0x18881198                     //工作队列线程

#define INIT_PDB_KERNEL_INFO  0x18881199                     //从pdb里面读到的所有内核函数和名字

#define SUSPEND_PROCESS       0x18881200                     //暂停进程
#define RESUME_PROCESS        0x18881201                     //恢复进程运行

#define INIT_THREAD_STACK     0x18881202                     //初始化线程堆栈
#define LIST_THREAD_STACK     0x18881203                     //获取线程堆栈

#define INIT_KERNEL_DATA_BASE     0x18881204                     //初始要查看的起始地址
#define INIT_KERNEL_DATA_SIZE     0x18881205                     //初始化要查看的大小
#define LIST_KERNEL_DATA          0x18881206                     //开始查看咯

#define SUSPEND_PROTECT        0x18881207                    //暂停保护   
#define RESUME_PROTECT         0x18881208                    //恢复保护 

#define KERNEL_BSOD            0x18881209                   //手动蓝屏  

#define INIT_SELECT_MODULE_INLINE_HOOK  0x18881210          //初始化所选模块的inlinehook扫描 
#define LIST_SELECT_MODULE_INLINE_HOOK  0x18881211          //扫描所选模块的inlinehook扫描 

#define INIT_SET_SELECT_INLINE_HOOK 0x18881212              //初始化恢复inlinehook (原始地址)
#define INIT_SET_SELECT_INLINE_HOOK_1 0x18881213            //初始化恢复inlinehook (原始模块地址)
#define SET_SELECT_INLINE_HOOK      0x18881214              //开始恢复inlinehook
#define ANTI_SELECT_INLINE_HOOK     0x18881215              //绕过所选的inlinehook

#define SET_WINDOWS_HOOK            0x18881216             //允许全局钩子
#define DIS_SET_WINDOWS_HOOK        0x18881217             //拒绝全局钩子


#define INIT_DUMP_KERNEL_MODULE_MEMORY_1   0x18881218      //初始化dump大小

#define LIST_IO_TIMER               0x18881219             //枚举IO定时器
#define START_IO_TIMER               0x18881220            //启动IO定时器
#define STOP_IO_TIMER               0x18881221             //停止IO定时器

#define INIT_EAT_NUMBER          0x18881222                //初始化EAT的导入函数位置
#define INIT_EAT_REAL_ADDRESS          0x18881223          //初始化EAT的原始地址

#define DIS_DLL_FUCK                0x18881224             //关闭DLL挟持防护
#define DLL_FUCK                    0x18881225             //开启DLL挟持防护

#define LIST_MESSAGE_HOOK            0x18881226            //消息钩子

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif



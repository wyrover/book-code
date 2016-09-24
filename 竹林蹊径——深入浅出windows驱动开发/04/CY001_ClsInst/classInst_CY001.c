
#include <DriverSpecs.h>
__user_code  

#include <windows.h>
#include <setupapi.h>
#include <stdio.h>
#include "resource.h"
#include <dontuse.h>


#if DBG
#define DbgOut(Text) OutputDebugString(TEXT("ClassInstaller: " Text "\n"))
#else
#define DbgOut(Text) 
#endif 

INT_PTR CY001PageProc(__in HWND   hDlg,
               __in UINT   uMessage,
               __in WPARAM wParam,
               __in LPARAM lParam);

DWORD PropPageCY001(
    __in  HDEVINFO            DeviceInfoSet,
    __in  PSP_DEVINFO_DATA    DeviceInfoData OPTIONAL
);

HMODULE ModuleInstance;

BOOL WINAPI DllMain(
    HINSTANCE DllInstance,
    DWORD Reason,
    PVOID Reserved)
{

    UNREFERENCED_PARAMETER( Reserved );

    switch(Reason) 
    {
        case DLL_PROCESS_ATTACH: 
        {

            ModuleInstance = DllInstance;
            DisableThreadLibraryCalls(DllInstance);
            InitCommonControls();
            break;
        }

        case DLL_PROCESS_DETACH: 
        {
            ModuleInstance = NULL;
            break;
        }

        default: 
            break;
    }

    return TRUE;
}

void DbgPrintDIFCode(DI_FUNCTION InstallFunction)
{
    switch (InstallFunction)
    {
        case DIF_INSTALLDEVICE: 
            DbgOut("DIF_INSTALLDEVICE");
            break;
            
        case DIF_ADDPROPERTYPAGE_ADVANCED:
            DbgOut("DIF_ADDPROPERTYPAGE_ADVANCED");
        	break;
        	
        case DIF_POWERMESSAGEWAKE:
            DbgOut("DIF_POWERMESSAGEWAKE");
            break;

        case DIF_PROPERTYCHANGE:
            DbgOut("DIF_PROPERTYCHANGE");
            break;
            
        case DIF_REMOVE: 
             DbgOut("DIF_REMOVE");
             break;
             
        case DIF_NEWDEVICEWIZARD_FINISHINSTALL:
            DbgOut("DIF_NEWDEVICEWIZARD_FINISHINSTALL");
            break;
            
        case DIF_SELECTDEVICE:
            DbgOut("DIF_SELECTDEVICE");
            break;
            
        case DIF_DESTROYPRIVATEDATA:
            DbgOut("DIF_DESTROYPRIVATEDATA");
            break;
        case DIF_INSTALLDEVICEFILES:
            DbgOut("DIF_INSTALLDEVICEFILES");
            break;
            
        case DIF_ALLOW_INSTALL:
            DbgOut("DIF_ALLOW_INSTALL");
            break;
        case DIF_SELECTBESTCOMPATDRV:
            DbgOut("DIF_SELECTBESTCOMPATDRV");
            break;

        case DIF_INSTALLINTERFACES:
            DbgOut("DIF_INSTALLINTERFACES");
            break;
            
        case DIF_REGISTER_COINSTALLERS:
            DbgOut("DIF_REGISTER_COINSTALLERS");
            break;
        default:
            DbgOut("DIF_UNKNOWN");
            break;
    }
}

DWORD CALLBACK CY001ClassInstaller(
    __in  DI_FUNCTION         InstallFunction,
    __in  HDEVINFO            DeviceInfoSet,
    __in  PSP_DEVINFO_DATA    DeviceInfoData OPTIONAL
    )
{
    switch (InstallFunction)
    {    
    	// 创建新的属性页，我将在这里面写上Hello CY001的字样    
        case DIF_ADDPROPERTYPAGE_ADVANCED:
            DbgOut("DIF_ADDPROPERTYPAGE_ADVANCED");
            return PropPageCY001(DeviceInfoSet, DeviceInfoData);      
        
        default:
            DbgPrintDIFCode(InstallFunction);
            break;
    }   
    return ERROR_DI_DO_DEFAULT;    
}

// 创建自定义的属性页
DWORD PropPageCY001(
    __in  HDEVINFO            DeviceInfoSet,
    __in  PSP_DEVINFO_DATA    DeviceInfoData OPTIONAL
)
{
    HPROPSHEETPAGE  pageHandle;
    PROPSHEETPAGE   page;
    SP_ADDPROPERTYPAGE_DATA AddPropertyPageData = {0};

    if (DeviceInfoData==NULL) {
        return ERROR_DI_DO_DEFAULT;
    }

    AddPropertyPageData.ClassInstallHeader.cbSize = 
         sizeof(SP_CLASSINSTALL_HEADER);

    // 属性表单的详细定义，保存在类安装参数结构体中。
    // 下面的做法主要分成三步：
    // 1. 取结构体
    // 2. 创建新页面，并修改结构体
    // 3. 存结构体
    if (SetupDiGetClassInstallParams(DeviceInfoSet, DeviceInfoData,
         (PSP_CLASSINSTALL_HEADER)&AddPropertyPageData,
         sizeof(SP_ADDPROPERTYPAGE_DATA), NULL )) 
    {
        // 存在一个最大页数，不能超过
        if(AddPropertyPageData.NumDynamicPages >= MAX_INSTALLWIZARD_DYNAPAGES)
        {
            return NO_ERROR;
        }
        
        // 创建一个新页面
        memset(&page, 0, sizeof(PROPSHEETPAGE));

        page.dwSize = sizeof(PROPSHEETPAGE);
        page.dwFlags = 0;
        page.hInstance = ModuleInstance;
        page.pszTemplate = MAKEINTRESOURCE(DLG_TOASTER_PORTSETTINGS);
        page.pfnDlgProc = CY001PageProc;
        page.pfnCallback = NULL;
        page.lParam = 0;

        pageHandle = CreatePropertySheetPage(&page);
        if(!pageHandle)
        {
            return NO_ERROR;
        }

        // 添加新的属性页，并重新设置给类设备
        AddPropertyPageData.DynamicPages[
            AddPropertyPageData.NumDynamicPages++]=pageHandle;

        SetupDiSetClassInstallParams(DeviceInfoSet,
                    DeviceInfoData,
                    (PSP_CLASSINSTALL_HEADER)&AddPropertyPageData,
                    sizeof(SP_ADDPROPERTYPAGE_DATA));
    }
    
    return NO_ERROR;
} 

// Page页面的消息处理函数
INT_PTR CY001PageProc(__in HWND   hDlg,
                   __in UINT   uMessage,
                   __in WPARAM wParam,
                   __in LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	UNREFERENCED_PARAMETER(hDlg);
	
    switch(uMessage) 
    {
    case WM_COMMAND:
    case WM_CONTEXTMENU:
    case WM_HELP:
    case WM_NOTIFY:
    case WM_INITDIALOG:
    	return TRUE;

    default: 
        return FALSE;
    }
} 
#ifndef __CACTUS_SERVICE_H__
#define __CACTUS_SERVICE_H__

#include <cactus/cactus.h>
#include <cactus/cactus_types.h>
#include <cactus/cactus_string.h>

namespace cactus
{
namespace service
{


BOOL CACTUS_API IsServiceExist(const string16& service_name);

BOOL CACTUS_API IsInstalled(const string16& service_name);

BOOL CACTUS_API IsStarted(const string16& service_name);

BOOL CACTUS_API Start(const string16& service_name);

void CACTUS_API Stop(const string16& service_name);

BOOL CACTUS_API Install(const string16& service_name, const string16& display_name, const string16& dependencies, DWORD dwServiceType = SERVICE_WIN32_OWN_PROCESS, string16& args = string16(), string16& exe = string16());

BOOL CACTUS_API Uninstall(const string16& service_name);

void CACTUS_API SetDescription(const string16& service_name, const string16& desc);

BOOL CACTUS_API GetServiceType(const string16& service_name, DWORD& service_type);

BOOL CACTUS_API CheckServiceStatus(const wchar_t* pszServiceName,
                        BOOL* pbServiceExist,
                        DWORD* pdwStatus,
                        DWORD* pdwError);

BOOL CACTUS_API StartService(const wchar_t* pszServiceName, DWORD* pdwError);

BOOL CACTUS_API ChangeServiceConfig(const wchar_t* pszServiceName,
                         const wchar_t* pszImagePath,
                         const wchar_t* pszSvcOrderGroup,
                         const wchar_t* pszDisplayName,
                         const wchar_t* pszDescription,
                         DWORD dwStartType,
                         DWORD* pdwError);

BOOL CACTUS_API CreateService(const wchar_t* pszServiceName,
                   const wchar_t* pszImagePath,
                   const wchar_t* pszSvcOrderGroup,
                   const wchar_t* pszDisplayName,
                   const wchar_t* pszDescription,
                   DWORD dwStartType,
                   DWORD* pdwError,
                   const wchar_t* pDepend = NULL,
                   DWORD nServicesType = SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS);

BOOL CACTUS_API StopService(const wchar_t* pszServiceName, DWORD* pdwError);

BOOL CACTUS_API DeleteService(const wchar_t* pszServiceName, DWORD* pdwError);

BOOL CACTUS_API CheckAndFixService(const wchar_t* pszServiceName,
                        const wchar_t* pszImagePath,
                        const wchar_t* pszSvcOrderGroup,
                        const wchar_t* pszDisplayName,
                        const wchar_t* pszDescription,
                        int* pnFixAction,
                        DWORD* pdwError);

int CACTUS_API KxEInstallService(TCHAR* pPath,
                      TCHAR* pInternalName,
                      TCHAR* pszServiceName,
                      TCHAR* pDescriptiveName,
                      TCHAR* pSvcGroupOrder);

int CACTUS_API KxEInstallServiceInteractive(TCHAR* pPath,
                                 TCHAR* pInternalName,
                                 TCHAR* pszServiceName,
                                 TCHAR* pDescriptiveName,
                                 TCHAR* pSvcGroupOrder);

int CACTUS_API KxEUninstallService(TCHAR *pInternalName);

int CACTUS_API KxEStartService(const TCHAR *pInternalName,
                    bool bWaitRunning);

int CACTUS_API KxEStopService(const TCHAR *pInternalName,
                   bool bWaitUntilStopped);

BOOL CACTUS_API ModifyServiceType(const string16& service_name, DWORD service_type);

string16 CACTUS_API get_service_exe_path(const string16& service_name);

} // namespace service
} // namespace cactus

namespace cactus
{
class CACTUS_API WinService
/// This class provides an object-oriented interface to
/// the Windows Service Control Manager for registering,
/// unregistering, configuring, starting and stopping
/// services.
///
/// This class is only available on Windows platforms.
{
public:
    enum Startup {
        SVC_AUTO_START,
        SVC_MANUAL_START,
        SVC_DISABLED
    };

    WinService(const string16& name);
    /// Creates the WinService, using the given service name.

    ~WinService();
    /// Destroys the WinService.

    const string16& name() const;
    /// Returns the service name.

    string16 displayName() const;
    /// Returns the service's display name.

    string16 path() const;
    /// Returns the path to the service executable.
    ///
    /// Throws a NotFoundException if the service has not been registered.

    void registerService(const string16& path, const string16& displayName);
    /// Creates a Windows service with the executable specified by path
    /// and the given displayName.
    ///
    /// Throws a ExistsException if the service has already been registered.

    void registerService(const string16& path);
    /// Creates a Windows service with the executable specified by path
    /// and the given displayName. The service name is used as display name.
    ///
    /// Throws a ExistsException if the service has already been registered.

    void unregisterService();
    /// Deletes the Windows service.
    ///
    /// Throws a NotFoundException if the service has not been registered.

    bool isRegistered() const;
    /// Returns true if the service has been registered with the Service Control Manager.

    bool isRunning() const;
    /// Returns true if the service is currently running.

    void start();
    /// Starts the service.
    /// Does nothing if the service is already running.
    ///
    /// Throws a NotFoundException if the service has not been registered.

    void stop();
    /// Stops the service.
    /// Does nothing if the service is not running.
    ///
    /// Throws a NotFoundException if the service has not been registered.

    void setStartup(Startup startup);
    /// Sets the startup mode for the service.

    Startup getStartup() const;
    /// Returns the startup mode for the service.

    void setDescription(const string16& description);
    /// Sets the service description in the registry.

    string16 getDescription() const;
    /// Returns the service description from the registry.

    static const int STARTUP_TIMEOUT;

protected:
    static const string16 REGISTRY_KEY;
    static const string16 REGISTRY_DESCRIPTION;

private:
    void open() const;
    bool tryOpen() const;
    void close() const;
    LPQUERY_SERVICE_CONFIGW config() const;

    WinService();
    WinService(const WinService&);
    WinService& operator = (const WinService&);

    string16           _name;
    SC_HANDLE         _scmHandle;
    mutable SC_HANDLE _svcHandle;
};
} // namespace cactus

#endif // __CACTUS_SERVICE_H__

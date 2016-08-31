/*!
* \file cactus_environment.h
* \brief 操作系统环境声明
*
* 操作系统环境声明
*
* \author wangyang
* \date 2015/01/30
* \version 1.0
*/

#ifndef __CACTUS_CACTUS_ENVIRONMENT_H__
#define __CACTUS_CACTUS_ENVIRONMENT_H__

#include <cactus/cactus.h>
#include <cactus/cactus_types.h>

namespace cactus
{
string16 CACTUS_API GetCurrentSystemVersion();
}

namespace cactus
{
class CACTUS_API Environment
/// This class provides access to environment variables
/// and some general system information.
{
public:
    typedef unsigned char NodeId[6]; /// Ethernet address.

    static string16 get(const string16& name);
    /// Returns the value of the environment variable
    /// with the given name. Throws a NotFoundException
    /// if the variable does not exist.

    static string16 get(const string16& name, const string16& defaultValue);
    /// Returns the value of the environment variable
    /// with the given name. If the environment variable
    /// is undefined, returns defaultValue instead.

    static bool has(const string16& name);
    /// Returns true iff an environment variable
    /// with the given name is defined.

    static void set(const string16& name, const string16& value);
    /// Sets the environment variable with the given name
    /// to the given value.

    static string16 osName();
    /// Returns the operating system name.

    static string16 osDisplayName();
    /// Returns the operating system name in a
    /// "user-friendly" way.
    ///
    /// Currently this is only implemented for
    /// Windows. There it will return names like
    /// "Windows XP" or "Windows 7/Server 2008 SP2".
    /// On other platforms, returns the same as
    /// osName().

    static string16 osVersion();
    /// Returns the operating system version.

    static string16 osArchitecture();
    /// Returns the operating system architecture.

    static string16 nodeName();
    /// Returns the node (or host) name.

    static void nodeId(NodeId& id);
    /// Returns the Ethernet address of the first Ethernet
    /// adapter found on the system.
    ///
    /// Throws a SystemException if no Ethernet adapter is available.

    static string16 nodeId();
    /// Returns the Ethernet address (format "xx:xx:xx:xx:xx:xx")
    /// of the first Ethernet adapter found on the system.
    ///
    /// Throws a SystemException if no Ethernet adapter is available.

    static unsigned processorCount();
    /// Returns the number of processors installed in the system.
    ///
    /// If the number of processors cannot be determined, returns 1.

    static string16 desktop_dir();

    static string16 program_files_dir();

    static string16 appdata_dir();

    static string16 common_appdata_dir();

    static string16 user_home_dir();

    static string16 startmenu_dir();

    static string16 common_startmenu_dir();

    static string16 common_desktop_dir();

    static string16 system_dir();

    static string16 windows_dir();

    static string16 temp_dir();

    /*! @brief 操作系统所在盘符
     *
     * @return 返回操作系统所在盘符
     */
    static string16 sys_drv();

    static bool is_admin_user();

    static LANGID lang_id();

    static HRESULT GetDirectXVerionViaFileVersions(DWORD* pdwDirectXVersionMajor,
            DWORD* pdwDirectXVersionMinor,
            TCHAR* pcDirectXVersionLetter);
};

} // namespace cactus

namespace cactus
{
class CACTUS_API SysInfo
{
public:

    // Return the number of logical processors/cores on the current machine.
    // WARNING: On POSIX, this method uses static variables and is not threadsafe
    // until it's been initialized by being called once without a race.
    static int NumberOfProcessors();

    // Return the number of bytes of physical memory on the current machine.
    static int64_t AmountOfPhysicalMemory();

    // Return the number of megabytes of physical memory on the current machine.
    static int AmountOfPhysicalMemoryMB()
    {
        return static_cast < int >(AmountOfPhysicalMemory() / 1024 / 1024);
    }

    // Return the available disk space in bytes on the volume containing |path|,
    // or -1 on failure.
    static int64_t AmountOfFreeDiskSpace(const std::wstring & path);

    // Return true if the given environment variable is defined.
    // TODO: find a better place for HasEnvVar.
    static bool HasEnvVar(const wchar_t *var);

    // Return the value of the given environment variable
    // or an empty string if not defined.
    // TODO: find a better place for GetEnvVar.
    static std::wstring GetEnvVar(const wchar_t *var);

    // Returns the name of the host operating system.
    static std::string OperatingSystemName();

    // Returns the version of the host operating system.
    static std::string OperatingSystemVersion();

    // Retrieves detailed numeric values for the OS version.
    // WARNING: On OS X, this method uses static variables and is not threadsafe
    // until it's been initialized by being called once without a race.
    // TODO(port): Implement a Linux version of this method and enable the
    // corresponding unit test.
    static void OperatingSystemVersionNumbers(int32_t *major_version, int32_t *minor_version,
            int32_t *bugfix_version);

    // Returns the CPU architecture of the system. Exact return value may differ
    // across platforms.
    static std::string CPUArchitecture();

    // Returns the pixel dimensions of the primary display via the
    // width and height parameters.
    static void GetPrimaryDisplayDimensions(int * width, int * height);

    // Return the number of displays.
    static int DisplayCount();

    // Return the smallest amount of memory (in bytes) which the VM system will
    // allocate.
    static size_t VMAllocationGranularity();

#if defined(OS_MACOSX)

    // Under the OS X Sandbox, our access to the system is limited, this call
    // caches the system info on startup before we turn the Sandbox on.
    // The above functions are all wired up to return the cached value so the rest
    // of the code can call them in the Sandbox without worrying.

    static void CacheSysInfo();

#endif

};
} // namespace cactus


#endif // __CACTUS_CACTUS_ENVIRONMENT_H__

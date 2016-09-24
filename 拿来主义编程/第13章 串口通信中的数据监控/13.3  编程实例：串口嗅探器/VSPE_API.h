// Virtual Serial Port Emulator API header
// Author: Volodymyr Ter (http://www.eterlogic.com)

#ifndef _VSPE_API_H_
#define _VSPE_API_H_

#ifdef VSPE_API_EXPORTS
    #define VSPE_API extern "C" _declspec(dllexport)
#else
    #define VSPE_API extern "C" _declspec(dllimport)
#endif


/*
* Activate VSPE API using activation key
* \return result
*/
VSPE_API bool __cdecl vspe_activate(const char* key);

/*
 * Initialize VSPE core
 * \return result
 */
VSPE_API bool __cdecl vspe_initialize();

/*
 * Load configuration file
 * \param name
 * \return result
 */
VSPE_API bool __cdecl vspe_loadConfiguration(const char* name);


/*
* Save configuration
* \param name
* \return result
*/
VSPE_API bool __cdecl vspe_saveConfiguration(const char* name);


/*
* Create device
* \param name Device name. For example "Connector", "Splitter", "Pair" etc.
* \param initString device initialization string
* \return deviceId
*/
VSPE_API int __cdecl vspe_createDevice(const char* name, const char* initString);

/*
* Destroy device by deviceId
* \param deviceId
* \return result
*/
VSPE_API bool __cdecl vspe_destroyDevice(int deviceId);


/*
* Get VSPE devices count
* \return result
*/
VSPE_API int __cdecl vspe_getDevicesCount();


/*
* Get VSPE deviceId by device index
* \param idx device index
* \return deviceId
*/
VSPE_API int __cdecl vspe_getDeviceIdByIdx(int idx);

/*
* Get VSPE deviceId by COM port index
* \param ComPortIdx
* \return deviceId (-1 if not found).
*/
VSPE_API int __cdecl vspe_getDeviceIdByComPortIndex(int ComPortIdx);

/*
* Get device information
* \param deviceId
* \param name [out] device name
* \param initStirng [out] device initString
* \param ok [out] device state (1 = good)
* \param used [out] device clients count (0 - not used)
* \return result
*/
VSPE_API bool __cdecl vspe_getDeviceInfo(int deviceId, const char** name, const char** initString, int* ok, int* used);

/*
* Reinitialize device by deviceId
* \param deviceId
* \return result
*/
VSPE_API bool __cdecl vspe_reinitializeDevice(int deviceId);

/*
* Destroy all devices
* \return result
*/
VSPE_API bool __cdecl vspe_destroyAllDevices();


/*
 * Start emulation
 * \return result
 */
VSPE_API bool __cdecl vspe_startEmulation();

/*
 * Stop emulation
 * \return result
 */
VSPE_API bool __cdecl vspe_stopEmulation();

/*
 * Release VSPE core
 */
VSPE_API void __cdecl vspe_release();

/*
* Get VSPE API version information
* \return result
*/
VSPE_API const char* __cdecl vspe_getVersionInformation();

#endif


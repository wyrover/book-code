#include "sound_device.h"
#include <tchar.h>
#include <Mmdeviceapi.h>
#include <endpointvolume.h>
#include "PolicyConfig.h"
#include <Functiondiscoverykeys_devpkey.h>
#include <math.h>

namespace cactus
{

	HRESULT SetDefaultAudioPlaybackDevice(LPCWSTR devID)
	{
		IPolicyConfigVista *pPolicyConfig;
		ERole reserved = eConsole;

		HRESULT hr = CoCreateInstance(__uuidof(CPolicyConfigVistaClient),
			NULL, CLSCTX_ALL, __uuidof(IPolicyConfigVista), (LPVOID *)&pPolicyConfig);
		if (SUCCEEDED(hr))
		{
			hr = pPolicyConfig->SetDefaultEndpoint(devID, reserved);
			pPolicyConfig->Release();
		}
		return hr;
	}

	LPCWSTR GetDeviceID(int deviceCount)
	{
		LPWSTR wstrID = NULL;
		IMMDeviceEnumerator *pEnum = NULL;
		IMMDeviceCollection *pDevices;
		IMMDevice *pDevice;

		CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
		pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDevices);
		pDevices->Item(deviceCount, &pDevice);
		pDevice->GetId(&wstrID);
		return wstrID;
	}

	LPWSTR GetDeviceIDasLPWSTR(int deviceCount)
	{
		LPWSTR wstrID = NULL;
		IMMDeviceEnumerator *pEnum = NULL;
		IMMDeviceCollection *pDevices;
		IMMDevice *pDevice;

		CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
		pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDevices);
		pDevices->Item(deviceCount, &pDevice);
		pDevice->GetId(&wstrID);
		return wstrID;
	}

	LPWSTR GetDeviceFriendlyName(int deviceCount)
	{
		IMMDeviceEnumerator *pEnum = NULL;
		IMMDeviceCollection *pDevices;
		IMMDevice *pDevice;
		IPropertyStore *pStore;
		PROPVARIANT friendlyName;

		CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
		pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDevices);
		pDevices->Item(deviceCount, &pDevice);
		pDevice->OpenPropertyStore(STGM_READ, &pStore);
		pStore->GetValue(PKEY_Device_FriendlyName, &friendlyName);

		return friendlyName.pwszVal;
	}

	BOOL IsSoundPlaying(int deviceCount)
	{
		HRESULT hr;
		IAudioMeterInformation *pMeterInfo = NULL;
		IMMDeviceEnumerator *pEnum = NULL;
		IMMDeviceCollection *pDevices;
		IMMDevice *pDevice;

		CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
		pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDevices);
		pDevices->Item(deviceCount, &pDevice);

		hr = pDevice->Activate(__uuidof(IAudioMeterInformation), CLSCTX_ALL, NULL, (void**)&pMeterInfo);

		float peak;
		pMeterInfo->GetPeakValue(&peak);
		return peak > 0;	
	}

	int GetDeviceCount()
	{
		UINT count;
		IMMDeviceEnumerator *pEnum = NULL;
		IMMDeviceCollection *pDevices;

		CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
		pEnum->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &pDevices);
		pDevices->GetCount(&count);
		return count;
	}


	void SetMute (bool mute) 
	{
		HRESULT hr;
		IMMDeviceEnumerator *deviceEnumerator = NULL;
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
		IMMDevice *defaultDevice = NULL;

		hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
		deviceEnumerator->Release();
		deviceEnumerator = NULL;

		IAudioEndpointVolume *endpointVolume = NULL;
		hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
		defaultDevice->Release();
		defaultDevice = NULL; 

		hr = endpointVolume->SetMute(mute, NULL);

		endpointVolume->Release();
		
	}


	bool GetMute() 
	{

		HRESULT hr;

		
		IMMDeviceEnumerator *deviceEnumerator = NULL;
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
		IMMDevice *defaultDevice = NULL;

		hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
		deviceEnumerator->Release();
		deviceEnumerator = NULL;

		IAudioEndpointVolume *endpointVolume = NULL;
		hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
		defaultDevice->Release();
		defaultDevice = NULL; 

		BOOL mute;
		hr = endpointVolume->GetMute(&mute);

		endpointVolume->Release();
		

		return mute;
	}

	double round(double d)
	{
		return floor(d + 0.5);
	}

	/*void SetVolume (int volume_level) 
	{
		double new_volume = Math::Round ((double)volume_level/100, 2);
		HRESULT hr;

		
		IMMDeviceEnumerator *deviceEnumerator = NULL;
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
		IMMDevice *defaultDevice = NULL;

		hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
		deviceEnumerator->Release();
		deviceEnumerator = NULL;

		IAudioEndpointVolume *endpointVolume = NULL;
		hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
		defaultDevice->Release();
		defaultDevice = NULL; 

		hr = endpointVolume->SetMasterVolumeLevelScalar((float)new_volume, NULL);

		endpointVolume->Release();
		
	}*/


	int GetVolume()
	{
		HRESULT hr;

		
		IMMDeviceEnumerator *deviceEnumerator = NULL;
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
		IMMDevice *defaultDevice = NULL;

		hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
		deviceEnumerator->Release();
		deviceEnumerator = NULL;

		IAudioEndpointVolume *endpointVolume = NULL;
		hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
		defaultDevice->Release();
		defaultDevice = NULL; 

		float currentVolume = 0;
		hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);

		endpointVolume->Release();
		

		int ret = ceil(currentVolume * 100);
		return ret;
	}


	


	AudioDetector::AudioDetector()
	{
	}

	AudioDetector::~AudioDetector()
	{
	}

	IAudioMeterInformation* AudioDetector::aInfo = NULL;

	void AudioDetector::initializeAudioAnalyzer()
	{
		HRESULT hr;
		IMMDeviceEnumerator *pEnumerator = NULL;
		IMMDevice *pDevice = NULL;
		IAudioMeterInformation *pMeterInfo = NULL;
		//CoInitialize(NULL);
		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (void**)&pEnumerator);
		//checkError(hr);
		hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
		//checkError(hr);
		hr = pDevice->Activate(__uuidof(IAudioMeterInformation), CLSCTX_ALL, NULL, (void**)&pMeterInfo);
		//checkError(hr);
		aInfo = pMeterInfo;
	}

	bool AudioDetector::soundPlaying()
	{
		float peak;
		aInfo->GetPeakValue(&peak);
		return peak > 0;
	}

	

	
}

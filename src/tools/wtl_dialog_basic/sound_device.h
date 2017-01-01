#ifndef sound_device_h__
#define sound_device_h__

#include <Windows.h>
#include <endpointvolume.h>

namespace cactus
{
	HRESULT SetDefaultAudioPlaybackDevice(LPCWSTR devID);	

	LPCWSTR GetDeviceID(int deviceCount);	

	LPWSTR GetDeviceIDasLPWSTR(int deviceCount);	

	LPWSTR GetDeviceFriendlyName(int deviceCount);	

	BOOL IsSoundPlaying(int deviceCount);

	int GetDeviceCount();	

	void SetMute(bool mute);

	bool GetMute();


	class AudioDetector
	{
	public:

		AudioDetector();
		~AudioDetector();
	public:
		static IAudioMeterInformation* aInfo;
		static void initializeAudioAnalyzer();
		static bool soundPlaying();		
	};
}

#endif // sound_device_h__

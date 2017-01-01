// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"
#include "sound_device.h"
#include <MMSystem.h>
#include <iostream>

#define LOG(format, ...) wprintf(format L"\n", __VA_ARGS__)

typedef wchar_t char16;
typedef std::wstring string16;

extern "C" IMAGE_DOS_HEADER __ImageBase;

const char16* GetAllPathDelimiters()
{
	return L"\\/";
}

string16 ExtractFilePath(const string16& FileName)
{
	string16 Result = FileName;
	return Result.erase(Result.find_last_of(GetAllPathDelimiters()) + 1);
}

string16 GetAppFileName()
{
	char16 filename[MAX_PATH + 10] = {0};
	GetModuleFileNameW((HMODULE)&__ImageBase, filename, MAX_PATH);
	return string16(filename);
}

string16 GetAppDir()
{
	return ExtractFilePath(GetAppFileName());
}





LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // center the dialog on the screen
    CenterWindow();
    // set icons
    HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
                                     IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
                                          IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
    SetIcon(hIconSmall, FALSE);
    return TRUE;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
    dlg.DoModal();
    return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // TODO: Add validation code
    EndDialog(wID);
    return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EndDialog(wID);
    return 0;
}
LRESULT CMainDlg::OnBnClickedButton1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    //TRACE_MSGBOX_A("hello world!");

	

	using namespace cactus;

	string16 sound_filename = GetAppDir() + L"test.wav";
	PlaySound(sound_filename.c_str(), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

	int device_count = GetDeviceCount();


	std::cout << "音频设备数:" << device_count << std::endl;

	for (int i = 0; i < device_count; ++i) {
		std::wcout << GetDeviceFriendlyName(i) << std::endl;
		//std::wcout << GetDeviceID(i) << std::endl;
		std::wcout << GetDeviceIDasLPWSTR(i) << std::endl;

		std::wstring deviceID = GetDeviceID(i);

		::Sleep(2000);
		if (IsSoundPlaying(i)) {
			std::wcout << GetDeviceFriendlyName(i) << L"有声音......" << std::endl;
		}

		/*HRESULT hr = SetDefaultAudioPlaybackDevice(deviceID.c_str());
		if (SUCCEEDED(hr)) {
			std::cout << "默认音频设备设置成功." << std::endl;

			AudioDetector::initializeAudioAnalyzer();

			Sleep(2000);
			if (AudioDetector::soundPlaying()) {
				std::wcout << GetDeviceFriendlyName(i) << L"有声音......" << std::endl;				
			}
		}*/
	}

	//Sleep(5000);

	PlaySound(0,0,SND_FILENAME);

	



    return 0;
}

LRESULT CMainDlg::OnBnClickedButton2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{

	string16 sound_filename = GetAppDir() + L"test.wav";
	PlaySound(sound_filename.c_str(), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);



	return 0;
}

LRESULT CMainDlg::OnBnClickedButton3( WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled )
{
	UINT devs = waveInGetNumDevs();
	LOG(L"waveIn devices: %u", devs);
	for (UINT dev = 0; dev <devs; dev++) {
		WAVEINCAPS caps = {};
		MMRESULT mmr = waveInGetDevCaps(dev, &caps, sizeof(caps));

		if (MMSYSERR_NOERROR != mmr) {
			LOG(L"waveInGetDevCaps failed: mmr = 0x%08x", mmr);
			return mmr;
		}

		LOG(
			L"-- waveIn device #%u --\n"
			L"Manufacturer ID: %u\n"
			L"Product ID: %u\n"
			L"Version: %u.%u\n"
			L"Product Name: %s\n"
			L"Formats: 0x%x\n"
			L"Channels: %u\n"
			L"Reserved: %u\n"
			,
			dev,
			caps.wMid,
			caps.wPid,
			caps.vDriverVersion / 256, caps.vDriverVersion % 256,
			caps.szPname,
			caps.dwFormats,
			caps.wChannels,
			caps.wReserved1
			);
	}

	devs = waveOutGetNumDevs();
	LOG(L"waveOut devices: %u", devs);
	for (UINT dev = 0; dev <devs; dev++) {
		WAVEOUTCAPS caps = {};
		MMRESULT mmr = waveOutGetDevCaps(dev, &caps, sizeof(caps));

		if (MMSYSERR_NOERROR != mmr) {
			LOG(L"waveOutGetDevCaps failed: mmr = 0x%08x", mmr);
			return mmr;
		}

		LOG(
			L"-- waveOut device #%u --\n"
			L"Manufacturer ID: %u\n"
			L"Product ID: %u\n"
			L"Version: %u.%u\n"
			L"Product Name: %s\n"
			L"Formats: 0x%x\n"
			L"Channels: %u\n"
			L"Reserved: %u\n"
			L"Support: 0x%x\n"
			L"%s%s%s%s%s"
			,
			dev,
			caps.wMid,
			caps.wPid,
			caps.vDriverVersion / 256, caps.vDriverVersion % 256,
			caps.szPname,
			caps.dwFormats,
			caps.wChannels,
			caps.wReserved1,
			caps.dwSupport,
			((caps.dwSupport & WAVECAPS_LRVOLUME) ?       L"\tWAVECAPS_LRVOLUME\n" :       L""),
			((caps.dwSupport & WAVECAPS_PITCH) ?          L"\tWAVECAPS_PITCH\n" :          L""),
			((caps.dwSupport & WAVECAPS_PLAYBACKRATE) ?   L"\tWAVECAPS_PLAYBACKRATE\n" :   L""),
			((caps.dwSupport & WAVECAPS_VOLUME) ?         L"\tWAVECAPS_VOLUME\n" :         L""),
			((caps.dwSupport & WAVECAPS_SAMPLEACCURATE) ? L"\tWAVECAPS_SAMPLEACCURATE\n" : L"")
			);
	}

	return 0;
}

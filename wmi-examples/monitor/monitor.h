#ifndef monitor_h__
#define monitor_h__

#include <Windows.h>
#include <vector>

typedef struct tagDisplayMode
{
	int width;
	int height;
} DisplayMode;

namespace cactus
{	
	void GetCurrentDesktopRes(int* width, int* height);

	BOOL getCurrentUserDPI(DWORD* value);
	BOOL setCurrentUserDPI(DWORD value);

	BOOL IsWow64();
	

	//Reads and prints current DPI, only used indebugging
	//void ReadAndPrintDPI();

	DWORD ScalingProcentToRegNum(int num);

	//void UpdateDisplay(unsigned int width, unsigned int height, int zoom);

	void GetBestDevMode(unsigned int width, unsigned int height, DEVMODE* devOut);

	int SetResolution(unsigned int width, unsigned int height);

	bool setDisplayMode(int width, int height, int bpp, DEVMODE* oldMode);

	void Reset96DPI();

	void ListDisplayDevices();

	DISPLAY_DEVICE GetPrimaryDevice();

	void ListDisplaySettings(int index);

	bool SetDisplayResolution(long PelsWidth, long PelsHeight);

	void SetDisplayDefaults();

	void getDisplayModes(std::vector<DisplayMode>& modes);

	int GetBrightness();

	bool SetBrightness(int val);

	


	class Monitor
	{
	public:
		Monitor();
		Monitor(
			HMONITOR hMonitor,
			const std::wstring name,
			bool isPrimary,
			int left,
			int top,
			int right,
			int bottom
			);
		~Monitor();

		HMONITOR hMonitor; 
		bool isPrimary;

		int width;
		int height;

		int left;
		int top;
		int right;
		int bottom;

		void setName(std::wstring newName);
		std::wstring getName();		

		struct DisplaySettings
		{
			SIZE Resolution;
			int RefreshRate;
			int BitsPerPixel;
			void* DriverData;

			DisplaySettings(): DriverData(0) {}
		};

		bool GetCurrentSettings(DisplaySettings& settings);
		void GetSupportedSettings(std::vector<DisplaySettings>& settings);
		bool ChangeSettings(const DisplaySettings& settings);

	private:
		std::wstring name;
		
	};


	class MonitorList
	{
	public:
		MonitorList();
		~MonitorList();

		// Populate the list with monitor informations from the system
		void update();

		// Returns the monitor with the specified name
		// if not found, returns the primary monitor
		Monitor* getMonitorByName(const std::wstring name);		
		// Returns the pointer to the primary monitor
		Monitor* getPrimaryMonitor();

		// ONLY FOR READ ACCESS, please
		std::vector <Monitor>* monList;


	};

}


#endif // monitor_h__

#pragma once

#include <windows.h>
#include <HighLevelMonitorConfigurationAPI.h>
#include <PhysicalMonitorEnumerationAPI.h>
#include <string>

#include "ContinuousSetting.h"
#include "MonitorEnums.h"
#include "IOCTL.h"

class Monitor {

public:

	Monitor(HMONITOR nonPhysicalMonitor);

	~Monitor();

	bool isValid();

	std::string getName();

	void printCapabilities();


	//Single Actions
	bool degaussMonitor();

	bool restoreColourDefaults();

	bool restoreDefaults();

	bool saveSettings();

	//Get Setting Values (I won't cache these as they can change via external influences (Monitor controls))
	ContinuousSetting* getBrightness();

	ColourTemps* getColourTemperature();

	ContinuousSetting* getContrast();

	bool getDisplayAreaPosition();

	bool getDisplayAreaSize();

	bool getRGBDrive();

	bool getRGBGain();

	TechnologyType* getTechnologyType();

	//Set Setting
	bool setBrightness(unsigned long newValue);

	bool setColourTemperature(ColourTemps newValue);

	bool setContrast(unsigned long newValue);

	bool setDisplayAreaPosition();

	bool setDisplayAreaSize();

	bool setRGBDrive();

	bool setRGBGain();


protected:

	PHYSICAL_MONITOR monitorPointer;

	std::string name;

	std::string systemName;

	IOCTL* ioctl;


	bool valid;

	bool primaryMonitor;

	bool initialiseMonitor(HMONITOR nonPhysicalMonitor);


	//Capabilities
	bool getCapabilities();

	unsigned long capabilities;


	bool supportsBrightness();

	bool supportsColourTemperature();

	bool supportsContrast();

	//CRT Only, just keeping for completeness
	bool supportsDegauss();

	bool supportsDisplayAreaPosition();

	bool supportsDisplayAreaSize();

	bool supportsTechnologyType();

	bool supportsRGBDrive();

	bool supportsRGBGain();

	bool supportsRestoreColourDefaults();

	bool supportsRestoreDefaults();

	bool supportsRestoreDefaultEX();

	//Colour Capabilities
	unsigned long colourCapabilities;

	bool supportsSpecificColourTemperature(ColourTemps colourTemperature);

	unsigned long colourFlags[ColourTemps::ENUM_TEMP_COUNT];

	MC_COLOR_TEMPERATURE colourTempToRawColour(ColourTemps colour);

	ColourTemps rawColourToColourTemp(MC_COLOR_TEMPERATURE colour);


	MC_DISPLAY_TECHNOLOGY_TYPE techTypeToRawTech(TechnologyType tech);

	TechnologyType rawTechToTechType(MC_DISPLAY_TECHNOLOGY_TYPE tech);

	//Misc
	bool settingChangeCheck(ContinuousSetting setting, unsigned long newValue);
};
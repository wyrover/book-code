#include "Monitor.h"

#include <iostream>
#include <stdlib.h>

Monitor::Monitor(HMONITOR nonPhysicalMonitor)
{
	primaryMonitor = false;
	valid = false;
	name = "Undefined";
	capabilities = 0;
	colourCapabilities = 0;

	//Populate colourFlags array
	colourFlags[Temp4000k] = MC_SUPPORTED_COLOR_TEMPERATURE_4000K;
	colourFlags[Temp5000k] = MC_SUPPORTED_COLOR_TEMPERATURE_5000K;
	colourFlags[Temp6500k] = MC_SUPPORTED_COLOR_TEMPERATURE_6500K;
	colourFlags[Temp7500k] = MC_SUPPORTED_COLOR_TEMPERATURE_7500K;
	colourFlags[Temp8200k] = MC_SUPPORTED_COLOR_TEMPERATURE_8200K;
	colourFlags[Temp9300k] = MC_SUPPORTED_COLOR_TEMPERATURE_9300K;
	colourFlags[Temp10000k] = MC_SUPPORTED_COLOR_TEMPERATURE_10000K;
	colourFlags[Temp11500k] = MC_SUPPORTED_COLOR_TEMPERATURE_11500K;


	initialiseMonitor(nonPhysicalMonitor);


	//Fetch Monitor name
	//Would love some advice on how to deal with WCHAR properly
	char* nameRaw = new char[128];
	size_t length;

	///@TODO Find better method
	if (wcstombs_s(&length, nameRaw, 128, monitorPointer.szPhysicalMonitorDescription, 128) != 0)
	{
		std::cout << "Error in converting wide to char array for monitor name." << std::endl;
	}

	name = nameRaw;

	delete nameRaw;

	getCapabilities();

	if (!supportsBrightness())
	{
		//If standard method does not work try to use IOCTL
		ioctl = new IOCTL(systemName);
	}
}

Monitor::~Monitor()
{
	if (ioctl != nullptr)
		delete ioctl;
	
	//Release Handles
	DestroyPhysicalMonitor(monitorPointer.hPhysicalMonitor);
}


std::string Monitor::getName()
{
	return name;
}

void Monitor::printCapabilities()
{
	std::cout << "Capabilities:" << std::endl;
	std::cout << "	Brightness: " << supportsBrightness() << std::endl;
	std::cout << "	Contrast: " << supportsContrast() << std::endl;
	std::cout << "	Degauss: " << supportsDegauss() << std::endl;
	std::cout << "	Display Area Position: " << supportsDisplayAreaPosition() << std::endl;
	std::cout << "	Display Area Size: " << supportsDisplayAreaSize() << std::endl;
	std::cout << "	Technology Type: " << supportsTechnologyType() << std::endl;
	std::cout << "	RGB Drive: " << supportsRGBDrive() << std::endl;
	std::cout << "	RGB Gain: " << supportsRGBGain() << std::endl;
	std::cout << "	Restore Colour Defaults: " << supportsRestoreColourDefaults() << std::endl;
	std::cout << "	Restore Defaults: " << supportsRestoreDefaults() << std::endl;
	std::cout << "	Restore Defaults Extended: " << supportsRestoreDefaultEX() << std::endl;

	std::cout << "	Colour Temperature: " << supportsColourTemperature() << std::endl;
	if (supportsColourTemperature())
	{
		std::cout << "		4000k: " << supportsSpecificColourTemperature(Temp4000k) << std::endl;
		std::cout << "		5000k: " << supportsSpecificColourTemperature(Temp5000k) << std::endl;
		std::cout << "		6500k: " << supportsSpecificColourTemperature(Temp6500k) << std::endl;
		std::cout << "		7500k: " << supportsSpecificColourTemperature(Temp7500k) << std::endl;
		std::cout << "		8200k: " << supportsSpecificColourTemperature(Temp8200k) << std::endl;
		std::cout << "		9300k: " << supportsSpecificColourTemperature(Temp9300k) << std::endl;
		std::cout << "		10000k: " << supportsSpecificColourTemperature(Temp10000k) << std::endl;
		std::cout << "		11500k: " << supportsSpecificColourTemperature(Temp11500k) << std::endl;
	}
}

bool Monitor::degaussMonitor()
{
	if (supportsDegauss())
	{
		if (DegaussMonitor(monitorPointer.hPhysicalMonitor) == FALSE)
		{
			std::cout << getName() << " could not complete 'Degauss' function. Win32 Error Code: " << GetLastError();
			return false;
		}
	}
	else 
	{
		std::cout << getName() << " does not support the 'Degauss' function" << std::endl;
		return false;
	}

	return true;
}

bool Monitor::restoreColourDefaults()
{
	if (supportsRestoreColourDefaults())
	{
		if (RestoreMonitorFactoryColorDefaults(monitorPointer.hPhysicalMonitor) == FALSE)
		{
			std::cout << getName() << " could not Restore Factory Colours. Win32 Error Code: " << GetLastError();
			return false;
		}
	}
	else
	{
		std::cout << getName() << " does not support the 'Restore Factory Colours' function" << std::endl;
		return false;
	}

	return true;
}

bool Monitor::restoreDefaults()
{
	if (supportsRestoreDefaults())
	{
		if (RestoreMonitorFactoryDefaults(monitorPointer.hPhysicalMonitor) == FALSE)
		{
			std::cout << getName() << " could not Restore Factory Settings. Win32 Error Code: " << GetLastError();
			return false;
		}
	}
	else
	{
		std::cout << getName() << " does not support the 'Restore Factory Defaults' function" << std::endl;
		return false;
	}

	return true;
}

bool Monitor::saveSettings()
{
	if (SaveCurrentMonitorSettings(monitorPointer.hPhysicalMonitor) == FALSE)
	{
		std::cout << getName() << " could not Save Monitor Settings. Win32 Error Code: " << GetLastError();
		return false;
	}
	
	return true;
}

ContinuousSetting* Monitor::getBrightness()
{
	if (supportsBrightness())
	{
		ContinuousSetting* brightness = new ContinuousSetting;

		if (GetMonitorBrightness(monitorPointer.hPhysicalMonitor, &brightness->min, &brightness->current, &brightness->max) == TRUE)
		{
			return brightness;
		} 
		else 
		{
			delete brightness;
			std::cout << getName() << " could not fetch brightness info. Win32 Error Code: " << GetLastError();
			return nullptr;
		}
	}
	else
	{
		std::cout << getName() << " does not support Brightness functions" << std::endl;
		return nullptr;
	}
}

ColourTemps* Monitor::getColourTemperature()
{
	if (supportsColourTemperature())
	{
		MC_COLOR_TEMPERATURE colour;

		if (GetMonitorColorTemperature(monitorPointer.hPhysicalMonitor, &colour) == TRUE)
		{
			return new ColourTemps(rawColourToColourTemp(colour));
		} 
		else
		{
			std::cout << getName() << " could not Get Colour Temperature. Win32 Error Code: " << GetLastError();
			return nullptr;
		}
	}
	else
	{
		std::cout << getName() << " does not support 'Colour Temperature' functions" << std::endl;
		return nullptr;
	}
}

ContinuousSetting* Monitor::getContrast()
{
	if (supportsContrast())
	{
		ContinuousSetting* contrast = new ContinuousSetting;

		if (GetMonitorContrast(monitorPointer.hPhysicalMonitor, &contrast->min, &contrast->current, &contrast->max) == TRUE)
		{
			return contrast;
		}
		else
		{
			delete contrast;
			std::cout << getName() << " could not fetch contrast info. Win32 Error Code: " << GetLastError();
			return nullptr;
		}
	}
	else
	{
		std::cout << getName() << " does not support Contrast functions" << std::endl;
		return nullptr;
	}
}

bool Monitor::getDisplayAreaPosition()
{
	return false;
}

bool Monitor::getDisplayAreaSize()
{
	return false;
}

bool Monitor::getRGBDrive()
{
	return false;
}

bool Monitor::getRGBGain()
{
	return false;
}

TechnologyType* Monitor::getTechnologyType()
{
	if (supportsTechnologyType())
	{
		MC_DISPLAY_TECHNOLOGY_TYPE tech;

		if (GetMonitorTechnologyType(monitorPointer.hPhysicalMonitor, &tech) == TRUE)
		{
			return new TechnologyType(rawTechToTechType(tech));
		}
		else
		{
			std::cout << getName() << " could not Get Technology Type. Win32 Error Code: " << GetLastError();
			return nullptr;
		}
	}
	else
	{
		std::cout << getName() << " does not support 'Technology Type' functions" << std::endl;
		return nullptr;
	}
}

bool Monitor::setBrightness(unsigned long newValue)
{
	ContinuousSetting* bright = getBrightness();
	if (bright == nullptr)
	{
		return false;
	}

	if (!settingChangeCheck(*bright, newValue))
	{
		return false;
	}

	if (SetMonitorBrightness(monitorPointer.hPhysicalMonitor, newValue) == TRUE)
	{
		return true;
	}
	else
	{
		std::cout << getName() << " could not set brightness. Win32 Error Code: " << GetLastError();
		return false;
	}
}

bool Monitor::setColourTemperature(ColourTemps newValue)
{
	return false;
}

bool Monitor::setContrast(unsigned long newValue)
{
	return false;
}

bool Monitor::setDisplayAreaPosition()
{
	return false;
}

bool Monitor::setDisplayAreaSize()
{
	return false;
}

bool Monitor::setRGBDrive()
{
	return false;
}

bool Monitor::setRGBGain()
{
	return false;
}

bool Monitor::initialiseMonitor(HMONITOR nonPhysicalMonitor)
{
	if (nonPhysicalMonitor == NULL)
	{
		std::cout << "No monitor handle passed to Monitor Class";
		valid = false;
		return false;
	}

	MONITORINFOEX monitorInfo;
	monitorInfo.cbSize = sizeof(MONITORINFOEX); //Need to set this to allow next function to identify Structure type

	if (GetMonitorInfo(nonPhysicalMonitor, &monitorInfo) == 0)
	{
		std::cout << "Monitor is not providing info" << std::endl;
	}
	else
	{
		if (monitorInfo.dwFlags == MONITORINFOF_PRIMARY)
		{
			std::cout << "Is Primary Monitor" << std::endl;
			primaryMonitor = true;
		}

		systemName = monitorInfo.szDevice;
		std::cout << "Sys Name: " << systemName << std::endl;
	}

	DWORD monCount = 0;
	GetNumberOfPhysicalMonitorsFromHMONITOR(nonPhysicalMonitor, &monCount);

	LPPHYSICAL_MONITOR pMonitor = (LPPHYSICAL_MONITOR)malloc(monCount * sizeof(PHYSICAL_MONITOR));

	GetPhysicalMonitorsFromHMONITOR(nonPhysicalMonitor, monCount, pMonitor);

	monitorPointer = pMonitor[0];

	valid = true;
	return true;
}

bool Monitor::isValid()
{
	return valid;
}

bool Monitor::getCapabilities()
{
	if (!GetMonitorCapabilities(monitorPointer.hPhysicalMonitor, &capabilities, &colourCapabilities))
	{
		std::cout << "Monitor does not seem to support DDC/CI and is thus unusable with this program." << std::endl;
		std::cout << "Extended error info: " << GetLastError() << std::endl;
		return false;
	}

	return true;
}

bool Monitor::supportsBrightness()
{
	return (capabilities & MC_CAPS_BRIGHTNESS) != 0;
}

bool Monitor::supportsColourTemperature()
{
	return (capabilities & MC_CAPS_COLOR_TEMPERATURE) != 0;
}

bool Monitor::supportsContrast()
{
	return (capabilities & MC_CAPS_CONTRAST) != 0;
}

bool Monitor::supportsDegauss()
{
	return (capabilities & MC_CAPS_DEGAUSS) != 0;
}

bool Monitor::supportsDisplayAreaPosition()
{
	return (capabilities & MC_CAPS_DISPLAY_AREA_POSITION) != 0;
}

bool Monitor::supportsDisplayAreaSize()
{
	return (capabilities & MC_CAPS_DISPLAY_AREA_SIZE) != 0;
}

bool Monitor::supportsTechnologyType()
{
	return (capabilities & MC_CAPS_MONITOR_TECHNOLOGY_TYPE) != 0;
}

bool Monitor::supportsRGBDrive()
{
	return (capabilities & MC_CAPS_RED_GREEN_BLUE_DRIVE) != 0;
}

bool Monitor::supportsRGBGain()
{
	return (capabilities & MC_CAPS_RED_GREEN_BLUE_GAIN) != 0;
}

bool Monitor::supportsRestoreColourDefaults()
{
	return (capabilities & MC_CAPS_RESTORE_FACTORY_COLOR_DEFAULTS) != 0;
}

bool Monitor::supportsRestoreDefaults()
{
	return (capabilities & MC_CAPS_RESTORE_FACTORY_DEFAULTS) != 0;
}

bool Monitor::supportsRestoreDefaultEX()
{
	return (capabilities & MC_RESTORE_FACTORY_DEFAULTS_ENABLES_MONITOR_SETTINGS) != 0;
}

bool Monitor::supportsSpecificColourTemperature(ColourTemps colourTemperature)
{
	return (colourCapabilities & colourFlags[colourTemperature]) != 0;	
}

MC_COLOR_TEMPERATURE Monitor::colourTempToRawColour(ColourTemps colour)
{
	switch (colour)
	{
	case Temp4000k:
		return MC_COLOR_TEMPERATURE_4000K;
		break;
	case Temp5000k:
		return MC_COLOR_TEMPERATURE_5000K;
		break;
	case Temp6500k:
		return MC_COLOR_TEMPERATURE_6500K;
		break;
	case Temp7500k:
		return MC_COLOR_TEMPERATURE_7500K;
		break;
	case Temp8200k:
		return MC_COLOR_TEMPERATURE_8200K;
		break;
	case Temp9300k:
		return MC_COLOR_TEMPERATURE_9300K;
		break;
	case Temp10000k:
		return MC_COLOR_TEMPERATURE_10000K;
		break;
	case Temp11500k:
		return MC_COLOR_TEMPERATURE_11500K;
		break;
	
	case ENUM_TEMP_COUNT:
	case UnknownTemp:
	default:
		std::cout << "Invalid colour temp conversion was attempted" << std::endl;
		return MC_COLOR_TEMPERATURE_UNKNOWN;
		break;

	}
}

ColourTemps Monitor::rawColourToColourTemp(MC_COLOR_TEMPERATURE colour)
{
	switch (colour)
	{
	case MC_COLOR_TEMPERATURE_4000K:
		return Temp4000k;
		break;
	case MC_COLOR_TEMPERATURE_5000K:
		return Temp5000k;
		break;
	case MC_COLOR_TEMPERATURE_6500K:
		return Temp6500k;
		break;
	case MC_COLOR_TEMPERATURE_7500K:
		return Temp7500k;
		break;
	case MC_COLOR_TEMPERATURE_8200K:
		return Temp8200k;
		break;
	case MC_COLOR_TEMPERATURE_9300K:
		return Temp9300k;
		break;
	case MC_COLOR_TEMPERATURE_10000K:
		return Temp10000k;
		break;
	case MC_COLOR_TEMPERATURE_11500K:
		return Temp11500k;
		break;
	
	case MC_COLOR_TEMPERATURE_UNKNOWN:
	default:
		std::cout << "Invalid colour temp conversion was attempted" << std::endl;
		return UnknownTemp;
		break;
	}
}

MC_DISPLAY_TECHNOLOGY_TYPE Monitor::techTypeToRawTech(TechnologyType tech)
{
	switch (tech)
	{
	case ShadowMaskCRT:
		return MC_SHADOW_MASK_CATHODE_RAY_TUBE;
		break;
	case ApertureGrillCRT:
		return MC_APERTURE_GRILL_CATHODE_RAY_TUBE;
		break;
	case ThinFilmTransistor:
		return MC_THIN_FILM_TRANSISTOR;
		break;
	case LiquidCrystalOnSilicon:
		return MC_LIQUID_CRYSTAL_ON_SILICON;
		break;
	case Plasma:
		return MC_PLASMA;
		break;
	case OLED:
		return MC_ORGANIC_LIGHT_EMITTING_DIODE;
		break;
	case Electroluminescent:
		return MC_ELECTROLUMINESCENT;
		break;
	case Microelectromechanical:
		return MC_MICROELECTROMECHANICAL;
		break;
	case FieldEmissionDevice:
		return MC_FIELD_EMISSION_DEVICE;
		break;

	case ENUM_TYPE_COUNT:
	case UnknownType:
	default:
		std::cout << "Invalid tech type conversion was attempted" << std::endl;
		return MC_SHADOW_MASK_CATHODE_RAY_TUBE; // Have to return something (Maybe put assert here)
		break;

	}
}

TechnologyType Monitor::rawTechToTechType(MC_DISPLAY_TECHNOLOGY_TYPE tech)
{
	switch (tech)
	{
	case MC_SHADOW_MASK_CATHODE_RAY_TUBE:
		return ShadowMaskCRT;
		break;
	case MC_APERTURE_GRILL_CATHODE_RAY_TUBE:
		return ApertureGrillCRT;
		break;
	case MC_THIN_FILM_TRANSISTOR:
		return ThinFilmTransistor;
		break;
	case MC_LIQUID_CRYSTAL_ON_SILICON:
		return LiquidCrystalOnSilicon;
		break;
	case MC_PLASMA:
		return Plasma;
		break;
	case MC_ORGANIC_LIGHT_EMITTING_DIODE:
		return OLED;
		break;
	case MC_ELECTROLUMINESCENT:
		return Electroluminescent;
		break;
	case MC_MICROELECTROMECHANICAL:
		return Microelectromechanical;
		break;
	case MC_FIELD_EMISSION_DEVICE:
		return FieldEmissionDevice;
		break;

	default:
		std::cout << "Invalid tech type conversion was attempted" << std::endl;
		return UnknownType;
		break;
	}
}

bool Monitor::settingChangeCheck(ContinuousSetting setting, unsigned long newValue)
{
	//Check if new value is between the min/max values
	if (newValue > setting.max && newValue < setting.min)
	{
		std::cout << getName() << ": new value provided is outside of setting boundries";
		return false;
	}

	//Check if new value is the same as the current value
	if (newValue == setting.current)
	{
		std::cout << getName() << ": new value is the same as the current setting";
		return false;
	}

	return true;
}

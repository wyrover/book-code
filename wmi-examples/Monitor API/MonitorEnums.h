#pragma once

enum ColourTemps
{
	Temp4000k,
	Temp5000k,
	Temp6500k,
	Temp7500k,
	Temp8200k,
	Temp9300k,
	Temp10000k,
	Temp11500k,
	ENUM_TEMP_COUNT = 8,
	UnknownTemp = -1
};

enum TechnologyType
{
	ShadowMaskCRT,
	ApertureGrillCRT,
	ThinFilmTransistor,
	LiquidCrystalOnSilicon,
	Plasma,
	OLED,
	Electroluminescent,
	Microelectromechanical,
	FieldEmissionDevice,
	ENUM_TYPE_COUNT = 9,
	UnknownType = -1
};
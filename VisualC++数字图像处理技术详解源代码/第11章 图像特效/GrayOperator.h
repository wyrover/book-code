#pragma once



void LineTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, 
			  double slope, double inter);

void LogTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, 
			  double a, double b);

void PowerTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, 
				double b, double c, double r);

void ExpTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, 
			  double a, double b, double c);

void GrayStretch(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, 
				 float X1, float Y1, float X2, float Y2);

void ThresholdTrans(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height, int thr);

void GrayEqualize(BYTE* pixel, BYTE* tempPixel, UINT width, UINT height);
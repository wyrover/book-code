#pragma once

bool MoveTransNormal(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
					 int xx, int yy);

void MoveTransSize(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
				   int xx, int yy, UINT& outWidth, UINT& outHeight);

void VMirrorTrans(BYTE* pixel, BYTE*& tempPixel, int width, int height);

void HMirrorTrans(BYTE* pixel, BYTE*& tempPixel, int width, int height);

void Transpose(BYTE* pixel, BYTE*& tempPixel, int width, int height, UINT& outWidth, UINT& outHeight);

void ZoomNormal(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
				UINT& outWidth, UINT& outHeight, double fx, double fy);

void ZoomInterpolation(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
					   UINT& outWidth, UINT& outHeight, double fx, double fy);

void RotateNormal(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
				  UINT& outWidth, UINT& outHeight, double angle);

void RotateInterpolation(BYTE* pixel, BYTE*& tempPixel, int width, int height, 
						 UINT& outWidth, UINT& outHeight, double angle);
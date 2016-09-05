#pragma once

void Negative(BYTE* pixel, BYTE* tempPixel, int width, int height);
void Emboss(BYTE* pixel, BYTE* tempPixel, int width, int height);
void ColorToBW(BYTE* pixel, BYTE* tempPixel, int width, int height);
void HorFog(BYTE* pixel, BYTE* tempPixel, int width, int height, int f);
void VerFog(BYTE* pixel, BYTE* tempPixel, int width, int height, int f);
void ComFog(BYTE* pixel, BYTE* tempPixel, int width, int height, int f);
void Mosaic(BYTE* pixel, BYTE* tempPixel, unsigned int w, unsigned int h, unsigned int size);
void LaplacianB(BYTE* pixel, BYTE* tempPixel, int w, int h);
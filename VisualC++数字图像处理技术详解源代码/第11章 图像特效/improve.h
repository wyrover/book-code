#pragma once

int GetAsh(BYTE** imageBuf, int x, int y);
int TempltExcuteCl(BYTE** imageBuf0, int w, int h, int* templt, int tw, int x, int y, int cn);
void SmoothAsh(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, unsigned int size);
void SmoothCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, unsigned int size);
void SmoothGaussCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h);
void SmoothMedianAsh(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, unsigned int size);
void SmoothMedianCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, unsigned int size);
void SharpLaplacianAsh(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h);
void SharpLaplacianCl(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h);
void SideSobel(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, bool type);
void ToTwoValue(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h, int K);
void Subtract(BYTE* image0, BYTE* image1, unsigned int w, unsigned int h);
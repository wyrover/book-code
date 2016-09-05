#pragma once
#include "Dib.h"

class CColorSeg
{
public:
    CColorSeg(CDib *pDib);
    void Template24bit(LPBYTE lpData, int nLineByte, int nWidth, int nHeight, int nTemH, int nTemW, int nTemCX, int nTemCY, float *fpArray, float fCoef);    
    void Prewitt();
public:
    ~CColorSeg(void);

private:
    CDib * m_pDib;
};

#pragma once
#include "Dib.h"

class CEdgeDetection
{
public:
    CEdgeDetection(CDib *pDib);
    void Roberts(void);
    void Template(LPBYTE lpData, int nLineByte, int nWidth, int nHeight, int nTemH, int nTemW, int nTemCX, int nTemCY, float *fpArray, float fCoef);
    void Sobel(void);
    void Prewitt(void);
    void Laplacian(void);
    void GuassLaplacian(void);
    void Krisch(void);
public:
    ~CEdgeDetection(void);
private:
    CDib *m_pDib;
};

#pragma once

#include "Dib.h"

class CThreshold
{
public:
    CThreshold();
    CThreshold(CDib *pDib);
public:
    ~CThreshold(void);
public:
    void AdaptiveThreshold(void);
    void OtusThreshold(void);
private:
    CDib * m_pDib; 
};

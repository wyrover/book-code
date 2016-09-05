#pragma once

#include "Dib.h"

class CRegionSegment
{
public:
    CRegionSegment(CDib *pDib);
    void RegionGrow(CPoint point);
public:
    ~CRegionSegment(void);
private:
    CDib *m_pDib;
};

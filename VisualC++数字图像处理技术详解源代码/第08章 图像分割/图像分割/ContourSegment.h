#pragma once

#include "Dib.h"

class CContourSegment
{
public:
    CContourSegment(CDib *pDib);
    void ContourExtract(void);
    void ContourTrack(void);
public:
    ~CContourSegment(void);
private:
    CDib *m_pDib;
    
};

#pragma once
#include "highgui.h"

class myimg :
    public CvvImage
{
public:
    myimg();
    void mSetImg(IplImage *pImg) ;
public:
    ~myimg();
};

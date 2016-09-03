#include "StdAfx.h"
#include "myimg.h"

myimg::myimg()
{
}

void myimg::mSetImg(IplImage* pImg)
{
 m_img=cvCloneImage(pImg);  //这句很关键 ,调试也曾换成CImage类中的CopeOf方法,但是总有类型转换错误
}
myimg::~myimg(void)
{
}

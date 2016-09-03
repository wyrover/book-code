// FaceDetection.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>

static CvHaarClassifierCascade* cascade = 0;
static CvMemStorage* storage = 0;

void detect_and_draw( IplImage* image );
const char* cascade_name ="haarcascade_frontalface_alt.xml";                //人脸检测要用到的分类器

int _tmain(int argc, _TCHAR* argv[])
{
     cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );   //加载人脸检测所用的分类器
     if( !cascade )
     {
        fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
        return -1;
     }
     storage = cvCreateMemStorage(0);                                       //动态存储结构，用来存储人脸在图像中的位置
     cvNamedWindow( "result", 1 );
     const char* filename = "lena.jpg";                                     //待检测图像
     IplImage* image = cvLoadImage( filename, 1 );                          //加载图像
     detect_and_draw( image );                                              //对加载的图像进行检测
     cvWaitKey(0);
     cvReleaseImage( &image );
     cvDestroyWindow("result");
     return 0;
}

void detect_and_draw( IplImage* img )
{
    static CvScalar colors[] = 
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}
    };
 
    double scale = 1.3;
    IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
    IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
                         cvRound (img->height/scale)), 8, 1 );
 
    cvCvtColor( img, gray, CV_BGR2GRAY );
    cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img );
    cvClearMemStorage( storage );
 
    if( cascade )
    {
        /*函数cvHaarDetectObjects检测图像中的目标，由OpenCV提供。*/
        CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage, 1.1, 2, 0 ,
                                            cvSize(30, 30) );
        for( int i = 0; i < (faces ? faces->total : 0); i++ )
        {
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
            CvPoint center;
            int radius;
            center.x = cvRound((r->x + r->width*0.5)*scale);
            center.y = cvRound((r->y + r->height*0.5)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale);
            cvCircle( img, center, radius, colors[i%8], 3, 8, 0 );
        }
    }
 
    cvShowImage( "result", img );
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
}
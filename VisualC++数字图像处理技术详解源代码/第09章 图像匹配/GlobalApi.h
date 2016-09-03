#ifndef _GLOBAL_API
#define _GLOBAL_API


#include <math.h>
#include "cdib.h"
#include "ColorTable.h"
#include <complex>
using namespace std;

//----------------------------------------------------------------------
// DIB处理函数
BOOL DIBShow(CFile* pFile);
BOOL ConvertDDBToDIB(HBITMAP hBitmap, CDib* pDibDest, HPALETTE hPal);
BOOL CopyDIB(CDib* pDibSrc, CDib* pDibDst);
BOOL CopyScreenToDIB(LPRECT lpRect, CDib* pDibDest);
HPALETTE GetSystemPalette() ;
HPALETTE CopyPalette(HPALETTE hPalSrc);
BOOL ReadDIB(CFile* pFile, LPBITMAPINFOHEADER lpBMIH, LPBYTE lpImage);
HPALETTE MakeDIBPalette(LPVOID lpvColorTable,  LPBITMAPINFOHEADER lpBMIH);
BOOL PaintDIB(CDC* pDC, LPBITMAPINFOHEADER lpBMIH, LPBYTE lpImage, CPoint origin, CSize size, HPALETTE hPalette);

//----------------------------------------------------------------------
// 运动检测
BOOL GetBackground(CString strFilePath, int nTotalFrameNum, int nImageWidth, 
		   int nImageHeight, unsigned char* pUnchBackGround);
BOOL LoadDibSeq(CString strFilePath, int nCurFrameNum, int nTotalFrameNum, CDib* pDib);
CString GetFileName(CString strFilePathName, int nCurFrameNum);
void BinaFrameDiff(unsigned char *pUnchImg1, unsigned char *pUnchImg2, 
		  int nWidth, int nHeight, unsigned char * pUnchResult,
		  int nThreshold=10);
void ErodeFrameDiff(unsigned char *pUnchImg, int nWidth, int nHeight, int nErodeHalfWin, 
	   int nErodeThreshold, unsigned char *pUnchResult);

//----------------------------------------------------------------------
// 图象分割
void RegionSegFixThreshold(CDib * pDib, int nThreshold=100);
void RobertsOperator(CDib * pDib, double * pdGrad);
void LaplacianOperator(CDib * pDib, double * pdGrad);
void EdgeTrack(CDib * pDib, unsigned char * pUnEdgeTrack);
void RegionGrow(CDib * pDib, unsigned char * pUnRegion, int nThreshold = 10);
void RegionSegAdaptive(CDib * pDib) ;
void SobelOperator(CDib * pDib, double * pdGrad);
void PrewittOperator(CDib * pDib, double * pdGrad);
	// canny 算子
void GaussianSmooth(unsigned char *pUnchImg, int nWidth, int nHeight, double sigma, unsigned char * pUnchSmthdImg);
void MakeGauss(double sigma, double **pdKernel, int *pnWindowSize) ; 
void DirGrad(unsigned char *pUnchSmthdImg, int nWidth, int nHeight,
						 int *pnGradX , int *pnGradY);
void GradMagnitude(int *pnGradX, int *pnGradY, int nWidth, int nHeight, int *pnMag) ; 
void NonmaxSuppress(int *pnMag, int *pnGradX, int *pnGradY, int nWidth, int nHeight,	unsigned char *pUnchRst);
void EstimateThreshold(int *pnMag, int nWidth, int nHeight, int *pnThdHigh,int *pnThdLow, 
											 unsigned char * pUnchEdge, double dRatioHigh, double dRationLow) ;
void Canny(unsigned char *pUnchImage, int nWidth, int nHeight, double sigma,
					 double dRatioLow, double dRatioHigh, unsigned char *pUnchEdge) ;
void Hysteresis(int *pnMag, int nWidth, int nHeight, double dRatioLow, 
								double dRatioHigh, unsigned char *pUnchEdge) ;
void TraceEdge (int y, int x, int nLowThd, unsigned char *pUnchEdge, int *pnMag, int nWidth) ;


// 小波变换函数原型
// Daubechies紧致正交小波基
// 不同支撑区间长度下的滤波器系数如下
const double hCoef[10][20] =
{
	{ .707106781187,  .707106781187},

	{ .482962913145,  .836516303738,  .224143868042, -.129409522551 },

	{ .332670552950,  .806891509311,  .459877502118, -.135011020010, -.085441273882,  .035226291882 },

	{ .230377813309,  .714846570553,  .630880767930, -.027983769417,
	 -.187034811719,  .030841381836,  .032883011667, -.010597401785 },

	{ .160102397974,  .603829269797,  .724308528438,  .138428145901, -.242294887066,
	 -.032244869585,  .077571493840, -.006241490213, -.012580751999,  .003335725285 },

	{ .111540743350,  .494623890398,  .751133908021,  .315250351709, -.226264693965,
	 -.129766867567,  .097501605587,  .027522865530, -.031582039318,  .000553842201,
	  .004777257511, -.001077301085 },

	{ .077852054085,  .396539319482,  .729132090846,  .469782287405, -.143906003929,
	 -.224036184994,  .071309219267,  .080612609151, -.038029936935, -.016574541631,
	  .012550998556,  .000429577973, -.001801640704,  .000353713800 },

	{ .054415842243,  .312871590914,  .675630736297,  .585354683654, -.015829105256,
	 -.284015542962,  .000472484574,  .128747426620, -.017369301002, -.044088253931,
	  .013981027917,  .008746094047, -.004870352993, -.000391740373,  .000675449406,
	 -.000117476784 },

	{ .038077947364,  .243834674613,  .604823123690,  .657288078051,  .133197385825,
	 -.293273783279, -.096840783223,  .148540749338,  .030725681479, -.067632829061,
	  .000250947115,  .022361662124, -.004723204758, -.004281503682,  .001847646883,
	  .000230385764, -.000251963189,  .000039347320 },

	{ .026670057901,  .188176800078,  .527201188932,  .688459039454,  .281172343661,
	 -.249846424327, -.195946274377,  .127369340336,  .093057364604, -.071394147166,
	 -.029457536822,  .033212674059,  .003606553567, -.010733175483,  .001395351747,
	  .001992405295, -.000685856695, -.000116466855,  .000093588670, -.000013264203 }
};

BOOL DWT_1D(double* pDbSrc, int nMaxLevel,int nDWTSteps, int nInv, int nStep, int nSupp);
BOOL DWTStep_1D(double* pDbSrc, int nCurLevel,int nInv, int nStep, int nSupp);
BOOL DWT_2D(double* pDbSrc, int nMaxWLevel, int nMaxHLevel, int nDWTSteps, int nInv, int nStep, int nSupp);
BOOL DWTStep_2D(double* pDbSrc, int nCurWLevel, int nCurHLevel,	int nMaxWLevel, int nMaxHLevel, int nInv, int nStep, int nSupp);	
BOOL ImageDWT(LPBYTE lpImage, int nMaxWLevel, int nMaxHLevel,int nDWTSteps, int nInv, int nStep, int nSupp);
int Log2(int n);
BYTE FloatToByte(double f);
char FloatToChar(double f);


//----------------------------------------------------------------------
// 图象增强函数
BOOL GraySegLinTrans(CDib* pDib, int nX1, int nY1, 
					 int nX2, int nY2);					// 分段线性变换
BOOL HistogramEqualize(CDib* pDib);						// 直方图均衡化
BOOL  GeneralTemplate(CDib* pDib,  int nTempWidth, int nTempHeight,	
					 int nTempCenX, int nTempCenY,
					 double* pdbTemp, double dbCoef);	// 通用模板操作
BOOL MedianFilter(CDib* pDib,  int nTempWidth, int nTempHeight, 
				 int nTempCenX, int nTempCenY);			// 中值滤波
unsigned char  GetMedianValue(unsigned char * pUnchFltValue, int iFilterLen);	// 获取中值
BOOL  ReplaceDIBColorTable(CDib* pDib, LPBYTE  pColorsTable);			// 替换DIB颜色表

void LowPassFilterEnhance(LPBYTE lpImage, int nWidth, int nHeight, int nRadius) ;// 低通滤波
void HighPassFilterEnhance(LPBYTE lpImage, int nWidth, int nHeight, int nRadius);// 高通滤波
void ButterWorthLowPass(LPBYTE lpImage, int nWidth, int nHeight, int nRadius);	//  ButterWorth低通滤波
void ButterWorthHighPass(LPBYTE lpImage, int nWidth, int nHeight, int nRadius);	//  ButterWorth高通滤波
void LinearSharpen (LPBYTE lpImage, int nWidth, int nHeight);			//  线性锐化增强

//-----------------------------------------------------------------------
// imageview.cpp函数原型
BOOL LimbPatternBayer(CDib *pDib);
BOOL DitherFloydSteinberg(CDib *pDib);

//--------------------------------------------------------------------------
// 图象分析函数原型
BOOL DIBHOLENUMBER(CDib *pDib);
BOOL DIBMOMENT(CDib *pDib);
BOOL DIBBARYCENTERMOMENT(CDib *pDib);
BOOL DIBFREAMEWORK(CDib *pDib);
BOOL DIBCHESSBOARDDISRESTORE(CDib *pDib);
BOOL DIBSTREETDIS(CDib *pDib);
BOOL DIBOUTLINE(CDib *pDib);
BOOL WINAPI DIBTrace(CDib *pDib);

//-------------------------------------------------------------------------------------
// 图象变换函数原型
VOID WINAPI FFT_1D(complex<double> * pCTData, complex<double> * pCFData, int nLevel);
VOID WINAPI IFFT_1D(complex<double> * pCFData, complex<double> * pCTData, int nLevel);
VOID WINAPI IFFT_2D(complex<double> * pCFData, complex<double> * pCTData, int nWidth, int nHeight);
VOID WINAPI IDFT_2D(CDib * pDib,double * pTrRstRpart, double * pTrRstIpart);
VOID WINAPI WINAPI DCT(double *f, double *F, int power);
VOID WINAPI WALSH(double *f, double *F, int r);
VOID WINAPI IWALSH(double *F, double *f, int r);
BOOL WINAPI THREECROSS(double *Matrix, int Rank, double *QMatrix, double *MainCross, double *HypoCross);
BOOL WINAPI BSTQ(int Rank, double *MainCross, double *HypoCross, double *Matrix, double Eqs, int MaxT);

BOOL WINAPI DIBDct(CDib *pDib);
BOOL WINAPI DIBWalsh(CDib *pDib);
BOOL WINAPI DIBHOTELLING(CDib *pDib);
VOID WINAPI DIBDFT_2D(CDib * pDib,double * pTrRstRpart, double * pTrRstIpart);
VOID WINAPI DIBFFT_2D(complex<double> * pCTData, int nWidth, int nHeight, complex<double> * pCFData);

//------------------------------------------------------------------------------------
// 图象复原的函数原型
BOOL WINAPI DIBNoRestriction(CDib *pDib );
BOOL WINAPI DIBInverseFilter (CDib *pDib);
BOOL WINAPI DIBNoiseDegeneration (CDib *pDib);
BOOL WINAPI DIBWinnerFilter (CDib *pDib);
BOOL WINAPI DIBMotionDegeneration(CDib *pDib);
BOOL WINAPI DIBMotionRestore(CDib *pDib);

//-------------------------------------------------------------------------------------
// 图象编码函数原型
BOOL WINAPI WRITE2IMG(CDib * pDib, CFile& file);
BOOL WINAPI LOADIMG(CDib * pDib, CFile& file);
BOOL WINAPI DIBBITPLANE(CDib *pDib,BYTE bBitNum);
#endif